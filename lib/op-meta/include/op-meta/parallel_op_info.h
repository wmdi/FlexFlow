#ifndef _FLEXFLOW_PARALLEL_OPS_PARALLEL_OP_INFO_H
#define _FLEXFLOW_PARALLEL_OPS_PARALLEL_OP_INFO_H

#include "op-meta/ffconst.h"
#include <tuple>
#include <functional>
#include "visit_struct/visit_struct.hpp"

namespace FlexFlow {

struct ParallelOpInfo {
  OperatorType op_type;
  int parallel_dim;
  int parallel_degree;
};

bool operator==(ParallelOpInfo const &, ParallelOpInfo const &);
bool operator<(ParallelOpInfo const &, ParallelOpInfo const &);

}

VISITABLE_STRUCT(::FlexFlow::ParallelOpInfo, op_type, parallel_dim, parallel_degree);

namespace std {
template <>
struct hash<::FlexFlow::ParallelOpInfo> {
  size_t operator()(::FlexFlow::ParallelOpInfo const &) const;
};
}

#endif /* _FLEXFLOW_PARALLEL_OPS_PARALLEL_OP_INFO_H */
