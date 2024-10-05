#ifndef _FLEXFLOW_LIB_UTILS_INCLUDE_UTILS_GRAPH_SERIES_PARALLEL_BINARY_SP_DECOMPOSITION_TREE_BINARY_PARALLEL_SPLIT_H
#define _FLEXFLOW_LIB_UTILS_INCLUDE_UTILS_GRAPH_SERIES_PARALLEL_BINARY_SP_DECOMPOSITION_TREE_BINARY_PARALLEL_SPLIT_H

#include "utils/graph/series_parallel/binary_sp_decomposition_tree/binary_parallel_split.dtg.h"
#include "utils/graph/series_parallel/binary_sp_decomposition_tree/binary_sp_decomposition_tree.dtg.h"

namespace FlexFlow {

BinarySPDecompositionTree get_left_child(BinaryParallelSplit const &);
BinarySPDecompositionTree get_right_child(BinaryParallelSplit const &);

} // namespace FlexFlow

#endif
