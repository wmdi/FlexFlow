#ifndef _FLEXFLOW_UTILS_INCLUDE_UTILS_GRAPH_LABELLED_LABELLED_UPWARD_OPEN_INTERFACES_H
#define _FLEXFLOW_UTILS_INCLUDE_UTILS_GRAPH_LABELLED_LABELLED_UPWARD_OPEN_INTERFACES_H

#include "standard_labelled_interfaces.h"
#include "utils/graph/multidigraph.h"
#include "utils/graph/open_graph_interfaces.h"

namespace FlexFlow {

template <typename NodeLabel,
          typename EdgeLabel,
          typename InputLabel = EdgeLabel>
struct ILabelledUpwardOpenMultiDiGraphView
    : virtual public ILabelledMultiDiGraphView<NodeLabel, EdgeLabel>,
      virtual public IUpwardOpenMultiDiGraphView {
  virtual ~ILabelledUpwardOpenMultiDiGraphView() = default;

  std::unordered_set<MultiDiEdge>
      query_edges(MultiDiEdgeQuery const &q) const final {
    return this->query_edges(static_cast<UpwardOpenMultiDiEdgeQuery>(q));
  }
  virtual std::unordered_set<UpwardOpenMultiDiEdge>
      query_edges(UpwardOpenMultiDiEdgeQuery const &q) const = 0;

  virtual InputLabel const &at(InputMultiDiEdge const &) const = 0;
};
CHECK_RC_COPY_VIRTUAL_COMPLIANT(
    ILabelledUpwardOpenMultiDiGraphView<int, int, int>);

template <typename NodeLabel,
          typename EdgeLabel,
          typename InputLabel = EdgeLabel>
struct ILabelledUpwardOpenMultiDiGraph
    : virtual public ILabelledUpwardOpenMultiDiGraphView<NodeLabel,
                                                 EdgeLabel,
                                                 InputLabel> {

  virtual NodeLabel &at(NodeLabel const &) = 0;
  virtual EdgeLabel &at(EdgeLabel const &) = 0;
  virtual InputLabel &at(InputLabel const &) = 0;

  using ILabelledMultiDiGraph<NodeLabel, EdgeLabel>::add_edge;
  virtual void add_edge(InputMultiDiEdge const &, InputLabel const &) = 0;
};
CHECK_RC_COPY_VIRTUAL_COMPLIANT(ILabelledUpwardOpenMultiDiGraph<int, int, int>);

} // namespace FlexFlow

#endif
