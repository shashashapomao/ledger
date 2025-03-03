//------------------------------------------------------------------------------
//
//   Copyright 2018-2019 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include "ml/ops/leaky_relu_op.hpp"

#include "math/tensor.hpp"
#include "vectorise/fixed_point/fixed_point.hpp"

#include "gtest/gtest.h"

template <typename T>
class LeakyReluOpTest : public ::testing::Test
{
};

using MyTypes = ::testing::Types<fetch::math::Tensor<float>, fetch::math::Tensor<double>,
                                 fetch::math::Tensor<fetch::fixed_point::FixedPoint<32, 32>>>;

TYPED_TEST_CASE(LeakyReluOpTest, MyTypes);

TYPED_TEST(LeakyReluOpTest, forward_test)
{
  using ArrayType = TypeParam;

  ArrayType data = ArrayType::FromString(R"(
  	 1, -2, 3,-4, 5,-6, 7,-8;
    -1,  2,-3, 4,-5, 6,-7, 8)")
                       .Transpose();

  ArrayType gt = ArrayType::FromString(R"(
  	   1,-0.4,   3,-1.6,   5,-3.6,   7,-6.4;
    -0.1,   2,-0.9,   4,-2.5,   6,-4.9,   8)")
                     .Transpose();

  ArrayType alpha = ArrayType::FromString(R"(
    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8)")
                        .Transpose();

  fetch::ml::ops::LeakyReluOp<ArrayType> op;

  TypeParam prediction(op.ComputeOutputShape({data, alpha}));
  op.Forward({data, alpha}, prediction);

  // test correct values
  ASSERT_TRUE(
      prediction.AllClose(gt, typename TypeParam::Type(1e-5), typename TypeParam::Type(1e-5)));
}

TYPED_TEST(LeakyReluOpTest, backward_test)
{
  using DataType  = typename TypeParam::Type;
  using ArrayType = TypeParam;

  ArrayType alpha = ArrayType::FromString(R"(
    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8)")
                        .Transpose();

  ArrayType data = ArrayType::FromString(R"(
  	 1, -2, 3,-4, 5,-6, 7,-8;
    -1,  2,-3, 4,-5, 6,-7, 8)")
                       .Transpose();

  ArrayType gt = ArrayType::FromString(R"(
    0, 0, 0, 0, 1, 0.6, 0, 0;
    0, 0, 0, 0, 0.5, 1, 0, 0)")
                     .Transpose();

  ArrayType error = ArrayType::FromString(R"(
  	0, 0, 0, 0, 1, 1, 0, 0;
    0, 0, 0, 0, 1, 1, 0, 0)")
                        .Transpose();

  fetch::ml::ops::LeakyReluOp<ArrayType> op;
  std::vector<ArrayType>                 prediction = op.Backward({data, alpha}, error);

  // test correct values
  ASSERT_TRUE(prediction[0].AllClose(gt, DataType(1e-5), DataType(1e-5)));
}
