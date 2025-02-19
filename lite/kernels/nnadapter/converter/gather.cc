// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lite/kernels/nnadapter/converter/converter.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace nnadapter {

int ConvertGather(Converter* converter, OpInfo* op, Scope* scope) {
  auto x_name = op->Input("X").front();
  auto x_scale_name = "X0_scale";
  std::vector<float> x_scales;
  if (op->HasInputScale(x_scale_name, true)) {
    x_scales = op->GetInputScale(x_scale_name, true);
  }
  // Input operand
  auto input_operand = converter->AddInputOperand(scope, x_name, {}, x_scales);
  // Indices operand
  auto index_name = op->Input("Index").front();
  auto indices_operand = converter->AddInputOperand(scope, index_name);
  // Axis operand
  NNAdapterOperand* axis_operand = nullptr;
  if (HasInput(op, scope, "Axis")) {
    auto axis_name = op->Input("Axis").front();
    axis_operand = converter->AddInputOperand(scope, axis_name);
  } else {
    axis_operand = converter->AddConstantOperand<int32_t>(0);
  }
  // Output operand
  auto out_name = op->Output("Out").front();
  auto output_operand = converter->AddOutputOperand(out_name);
  converter->AddOperation(NNADAPTER_GATHER,
                          {input_operand, indices_operand, axis_operand},
                          {output_operand});
  return NO_ERROR;
}

}  // namespace nnadapter
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
