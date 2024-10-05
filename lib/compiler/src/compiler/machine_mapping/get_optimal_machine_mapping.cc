#include "compiler/machine_mapping/get_optimal_machine_mapping.h"
#include "compiler/machine_mapping/abstracted_tensor_set_movement/abstracted_tensor_set_movement.h"
#include "compiler/machine_mapping/get_machine_resource_splits.h"
#include "compiler/machine_mapping/machine_mapping_cache.h"
#include "compiler/machine_mapping/machine_mapping_constraints.h"
#include "compiler/machine_mapping/machine_mapping_problem_tree/machine_mapping_problem_tree.h"
#include "compiler/machine_mapping/machine_mapping_problem_tree/mm_problem_tree_parallel_split.h"
#include "compiler/machine_mapping/machine_mapping_problem_tree/mm_problem_tree_series_split.h"
#include "compiler/machine_mapping/machine_mapping_problem_tree/unmapped_op_cost_estimate_key.h"
#include "compiler/machine_mapping/machine_mapping_result.h"
#include "compiler/machine_mapping/transitive_reduced_pcg.h"
#include "compiler/series_parallel/pcg_binary_sp_decomposition.dtg.h"
#include "compiler/series_parallel/pcg_binary_sp_decomposition.h"
#include "pcg/machine_specification.dtg.h"
#include "pcg/machine_specification.h"
#include "pcg/machine_view.dtg.h"
#include "pcg/machine_view.h"
#include "pcg/parallel_computation_graph/parallel_computation_graph.h"
#include "utils/containers/contains.h"
#include "utils/containers/flatmap.h"
#include "utils/containers/generate_map.h"
#include "utils/containers/get_all_assignments.h"
#include "utils/containers/unordered_set_of.h"
#include "utils/exception.h"
#include "utils/overload.h"

namespace FlexFlow {

MachineMappingResult
    get_optimal_machine_mapping(MachineMappingCache &result_cache,
                                MachineMappingContext const &context,
                                MachineMappingProblemTree const &problem_tree,
                                MachineSpecification const &resources,
                                MachineMappingConstraints const &constraints) {

  MachineMappingState state = MachineMappingState{
      problem_tree,
      resources,
      constraints,
  };

  {
    std::optional<MachineMappingResult> cached_result =
        machine_mapping_cache_load(result_cache, state);
    if (cached_result) {
      return cached_result.value();
    }
  }

  MachineMappingResult result = visit<MachineMappingResult>(
      problem_tree,
      overload{
          [&](MMProblemTreeSeriesSplit const &series_split) {
            return get_optimal_machine_mapping(
                result_cache,
                context,
                series_split,
                resources,
                constraints,
                /*parallel_split_transformation=*/std::nullopt);
          },
          [&](auto const &decomp_tree_node) {
            return get_optimal_machine_mapping(result_cache,
                                               context,
                                               decomp_tree_node,
                                               resources,
                                               constraints);
          },
      });

  machine_mapping_cache_save(result_cache, state, result);
  return result;
}

MachineMappingResult
    get_optimal_machine_mapping(MachineMappingCache &result_cache,
                                MachineMappingContext const &context,
                                MMProblemTreeSeriesSplit const &series_split,
                                MachineSpecification const &resources,
                                MachineMappingConstraints const &constraints,
                                std::optional<ParallelSplitTransformation> const
                                    &parallel_split_transformation) {

  auto get_boundary_machine_view_assignments =
      [&](std::unordered_set<BinaryTreePath> const &boundary_layers)
      -> std::unordered_set<ParallelLayerGuidObliviousMachineMapping> {
    std::unordered_map<BinaryTreePath, std::unordered_set<MachineView>>
        allowed = generate_map(
            boundary_layers,
            [&](BinaryTreePath const &l) -> std::unordered_set<MachineView> {
              UnmappedOpCostEstimateKey leaf =
                  require_leaf(mm_problem_tree_get_subtree_at_path(
                                   wrap_series_split(series_split), l)
                                   .value());
              return context.allowed_machine_views(leaf, resources);
            });
    return transform(
        get_all_assignments(allowed),
        [](std::unordered_map<BinaryTreePath, MachineView> const &m) {
          return ParallelLayerGuidObliviousMachineMapping{m};
        });
  };

  auto eval_pre_boundary_mapping =
      [&](ParallelLayerGuidObliviousMachineMapping const
              &assigned_pre_machine_views) {
        MachineMappingConstraints pre_candidate = with_additional_constraints(
            restrict_to_left_child(constraints), assigned_pre_machine_views);

        MachineMappingResult pre_result =
            get_optimal_machine_mapping(result_cache,
                                        context,
                                        get_pre_child(series_split),
                                        resources,
                                        pre_candidate);

        return pre_result;
      };

  auto eval_post_boundary_mapping =
      [&](ParallelLayerGuidObliviousMachineMapping const
              &assigned_post_machine_views) {
        MachineMappingConstraints post_candidate = with_additional_constraints(
            restrict_to_right_child(constraints), assigned_post_machine_views);

        MachineMappingResult post_result =
            get_optimal_machine_mapping(result_cache,
                                        context,
                                        get_post_child(series_split),
                                        resources,
                                        post_candidate);

        return post_result;
      };

  MachineMappingResult result = infeasible_machine_mapping_result();
  AbstractedTensorSetMovement tensor_movement =
      get_abstracted_tensor_movement(series_split);

  for (ParallelLayerGuidObliviousMachineMapping const
           &assigned_pre_machine_views :
       get_boundary_machine_view_assignments(get_src_layers(tensor_movement))) {

    MachineMappingResult pre_result =
        eval_pre_boundary_mapping(assigned_pre_machine_views);

    for (ParallelLayerGuidObliviousMachineMapping const
             &assigned_post_machine_views :
         get_boundary_machine_view_assignments(
             get_dst_layers(tensor_movement))) {

      MachineMappingResult post_result =
          eval_post_boundary_mapping(assigned_post_machine_views);

      TensorSetMovement comm_across_split =
          concretize_abstracted_tensor_set_movement(
              tensor_movement,
              /*pre_mapping=*/assigned_pre_machine_views,
              /*post_mapping=*/assigned_post_machine_views);
      float cost_across_split =
          context.cost_estimator.estimate_cost(comm_across_split);

      result = minimize_runtime(result,
                                series_combine(cost_across_split,
                                               pre_result,
                                               post_result,
                                               parallel_split_transformation));
    }
  }

  return result;
}

MachineMappingResult get_optimal_machine_mapping(
    MachineMappingCache &result_cache,
    MachineMappingContext const &context,
    MMProblemTreeParallelSplit const &parallel_split,
    MachineSpecification const &resources,
    MachineMappingConstraints const &constraints) {

  MachineMappingProblemTree lhs = get_lhs_child(parallel_split);
  MachineMappingProblemTree rhs = get_rhs_child(parallel_split);

  MachineMappingResult series_result = [&] {
    MMProblemTreeSeriesSplit series_split =
        require_series_split(mm_problem_tree_make_series_split(
            /*tensor_set_movement=*/empty_abstracted_tensor_set_movement(),
            /*pre=*/lhs,
            /*post=*/rhs));

    return get_optimal_machine_mapping(result_cache,
                                       context,
                                       series_split,
                                       resources,
                                       constraints,
                                       ParallelSplitTransformation::LthenR);
  }();

  MachineMappingConstraints left_constraints =
      restrict_to_left_child(constraints);
  MachineMappingConstraints right_constraints =
      restrict_to_right_child(constraints);

  auto evaluate_resource_split =
      [&](std::pair<MachineSpecification, MachineSpecification> const
              &resource_split) {
        MachineMappingResult left_result = get_optimal_machine_mapping(
            result_cache, context, lhs, resource_split.first, left_constraints);
        MachineMappingResult right_result =
            get_optimal_machine_mapping(result_cache,
                                        context,
                                        rhs,
                                        resource_split.second,
                                        right_constraints);

        return parallel_combine(left_result, right_result);
      };

  std::unordered_set<MachineMappingResult> parallel_results = transform(
      get_machine_resource_splits(resources), evaluate_resource_split);

  return minimize_runtime(series_result,
                          get_mapping_with_minimal_runtime(parallel_results));
}

MachineMappingResult
    get_optimal_machine_mapping(MachineMappingCache &result_cache,
                                MachineMappingContext const &context,
                                UnmappedOpCostEstimateKey const &leaf,
                                MachineSpecification const &resource,
                                MachineMappingConstraints const &constraints) {

  std::unordered_set<MachineView> candidates = [&] {
    std::optional<MachineView> machine_view = require_only_root(constraints);
    if (machine_view.has_value()) {
      return std::unordered_set{machine_view.value()};
    } else {
      return context.allowed_machine_views(leaf, resource);
    }
  }();

  auto get_mapping_result = [&](MachineView const &machine_view) {
    OpCostEstimateKey mapped =
        map_unmapped_op_cost_estimate_key(leaf, machine_view);
    float cost = context.cost_estimator.estimate_cost(mapped);

    return make_singleton_machine_mapping_result(cost, machine_view);
  };

  std::unordered_set<MachineMappingResult> candidate_results =
      transform(candidates, get_mapping_result);

  return get_mapping_with_minimal_runtime(candidate_results);
}

} // namespace FlexFlow
