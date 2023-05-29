#ifndef _FLEXFLOW_COMPILER_GRAPH_UTILS_H
#define _FLEXFLOW_COMPILER_GRAPH_UTILS_H

#include "compiler/unity_algorithm.h"

namespace FlexFlow {

SerialParallelDecomposition
    get_serial_parallel_decomposition(OptimizerPCG const &pcg);

OptimizerPCG cg_to_pcg(OptimizerComputationGraph const &g);
SubParallelComputationGraph pcg_to_subpcg(OptimizerPCG const &g);


// NOTE(@wmdi): I think we should have the following interfaces in the graph library eventually.

std::unordered_set<Node> get_closed_sources(OpenMultiDiGraphView const &g);
std::unordered_set<Node> get_closed_sinks(OpenMultiDiGraphView const &g);
std::unordered_set<Node> get_open_sources(OpenMultiDiGraphView const &g);
std::unordered_set<Node> get_open_sinks(OpenMultiDiGraphView const &g);

std::unordered_set<MultiDiEdge> get_cut(OpenMultiDiGraphView const &g,
                                        GraphSplit const &split);

enum class InputSettings { INCLUDE_INPUTS, EXCLUDE_INPUTS };

enum class OutputSettings { INCLUDE_OUTPUTS, EXCLUDE_OUTPUTS };

template <typename NodeLabel,
          typename EdgeLabel,
          typename InputLabel = EdgeLabel,
          typename OutputLabel = EdgeLabel>
LabelledOpenMultiDiGraph<NodeLabel, EdgeLabel, InputLabel, OutputLabel>
    get_subgraph(LabelledOpenMultiDiGraph<NodeLabel,
                                          EdgeLabel,
                                          InputLabel,
                                          OutputLabel> const &g,
                 std::unordered_set<Node> const &nodes,
                 InputSettings input_settings,
                 OutputSettings output_settings);

template <typename NodeLabel,
          typename EdgeLabel,
          typename InputLabel = EdgeLabel,
          typename OutputLabel = EdgeLabel>
LabelledOpenMultiDiGraph<NodeLabel, EdgeLabel, InputLabel, OutputLabel>
    materialize_labelled_openmultidigraph_view(
        ILabelledOpenMultiDiGraphView<NodeLabel,
                                      EdgeLabel,
                                      InputLabel,
                                      OutputLabel> const &g);


template <class T>
void minimize(T &t, T const &v) {
  t = std::min(t, v);
}

}

namespace std {
template <>
struct hash<::FlexFlow::Serial> {
  size_t operator()(::FlexFlow::Serial const &) const; 
};

template <>
struct hash<::FlexFlow::Parallel> {
  size_t operator()(::FlexFlow::Parallel const &) const;
};

}; // namespace std

#endif