#ifndef _FLEXFLOW_AGGREGATE_H_
#define _FLEXFLOW_AGGREGATE_H_

#include "op_task_spec.h"
#include "sim_environment.h"
#include "op-attrs/ops/aggregate.h"

namespace FlexFlow {

template <> void register_task<AGGREGATE_INIT_TASK_ID>();
template <> void register_task<AGGREGATE_FWD_TASK_ID>();
template <> void register_task<AGGREGATE_BWD_TASK_ID>();

OpTaskInvocation init(AggregateAttrs const &);
OpTaskInvocation forward(AggregateAttrs const &);
OpTaskInvocation backward(AggregateAttrs const &);

CostMetrics measure_operator_cost(SimEnvironment const &sim,
                                  AggregateAttrs const &attrs,
                                  ParallelTensorShape const &gate_preds_shape,
                                  ParallelTensorShape const &gate_assign_shape,
                                  ParallelTensorShape const &true_gate_assign_shape,
                                  ParallelTensorShape const &full_gate_gradients_shape,
                                  std::vector<ParallelTensorShape> const &exp_preds_shapes,
                                  ProfilingSettings const &settings,
                                  MachineView const &mv);
}

#endif
