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

#include "vm_test_toolkit.hpp"

#include "gtest/gtest.h"

namespace {

class VMTests : public ::testing::Test
{
public:
  VmTestToolkit toolkit;
};

// Test we can compile and run a fairly inoffensive smart contract
TEST_F(VMTests, CheckCompileAndExecute)
{
  static char const *TEXT = R"(
    function main()
      print("Hello, world");
    endfunction
  )";

  ASSERT_TRUE(toolkit.Compile(TEXT));
  ASSERT_TRUE(toolkit.Run());
}

TEST_F(VMTests, CheckRandom)
{
  static char const *TEXT = R"(
    function main()
      print('rnd = ' + toString(Rand(0u64, 1000u64)));
      print('rnd = ' + toString(Rand(0u64, 1000u64)));
      print('rnd = ' + toString(Rand(0u64, 1000u64)));
      print('rnd = ' + toString(Rand(0u64, 1000u64)));
      print('rnd = ' + toString(Rand(0u64, 1000u64)));
      print('rnd = ' + toString(Rand(0.0f, 1000.0f)));
      print('rnd = ' + toString(Rand(0.0, 1000.0)));
    endfunction
  )";

  ASSERT_TRUE(toolkit.Compile(TEXT));
  ASSERT_TRUE(toolkit.Run());
}

}  // namespace
