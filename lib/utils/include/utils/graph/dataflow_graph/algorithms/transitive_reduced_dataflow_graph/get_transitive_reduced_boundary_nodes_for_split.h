#ifndef _FLEXFLOW_LIB_UTILS_INCLUDE_UTILS_GRAPH_DATAFLOW_GRAPH_ALGORITHMS_TRANSITIVE_REDUCED_DATAFLOW_GRAPH_GET_TRANSITIVE_REDUCED_BOUNDARY_NODES_FOR_SPLIT_H
#define _FLEXFLOW_LIB_UTILS_INCLUDE_UTILS_GRAPH_DATAFLOW_GRAPH_ALGORITHMS_TRANSITIVE_REDUCED_DATAFLOW_GRAPH_GET_TRANSITIVE_REDUCED_BOUNDARY_NODES_FOR_SPLIT_H

#include "utils/graph/dataflow_graph/algorithms/transitive_reduced_dataflow_graph/transitive_reduced_dataflow_graph.dtg.h"
#include "utils/graph/dataflow_graph/algorithms/transitive_reduced_dataflow_graph/split_boundary_nodes.dtg.h"
#include "utils/graph/series_parallel/binary_sp_decomposition_tree/binary_series_split.dtg.h"

namespace FlexFlow {

SplitBoundaryNodes get_transitive_reduced_boundary_nodes_for_split(TransitiveReducedDataflowGraphView const &,
                                                                   BinarySeriesSplit const &);

} // namespace FlexFlow

#endif
