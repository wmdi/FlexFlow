#ifndef _FLEXFLOW_LIB_UTILS_INCLUDE_UTILS_GRAPH_SERIES_PARALLEL_BINARY_SP_DECOMPOSITION_TREE_LEAF_ONLY_BINARY_SP_DECOMPOSITION_TREE_IS_BINARY_SP_TREE_RIGHT_ASSOCIATIVE_H
#define _FLEXFLOW_LIB_UTILS_INCLUDE_UTILS_GRAPH_SERIES_PARALLEL_BINARY_SP_DECOMPOSITION_TREE_LEAF_ONLY_BINARY_SP_DECOMPOSITION_TREE_IS_BINARY_SP_TREE_RIGHT_ASSOCIATIVE_H

#include "utils/graph/series_parallel/binary_sp_decomposition_tree/leaf_only_binary_sp_decomposition_tree/leaf_only_binary_sp_decomposition_tree.dtg.h"
#include "utils/graph/series_parallel/binary_sp_decomposition_tree/generic_binary_sp_decomposition_tree/is_binary_sp_tree_right_associative.h"

namespace FlexFlow {

template <typename LeafLabel>
bool is_binary_sp_tree_right_associative(LeafOnlyBinarySPDecompositionTree<LeafLabel> const &t) {
  return is_binary_sp_tree_right_associative(t.raw_tree);
}

} // namespace FlexFlow

#endif
