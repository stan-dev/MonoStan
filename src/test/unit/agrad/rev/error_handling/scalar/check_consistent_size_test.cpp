#include <stan/error_handling/scalar/check_consistent_size.hpp>
#include <stan/agrad/rev.hpp>
#include <gtest/gtest.h>

TEST(AgradRevErrorHandlingScalar, checkConsistentSize) {
  using Eigen::Matrix;
  using Eigen::Dynamic;
  using stan::math::check_consistent_size;
  using stan::size_of;
  using stan::agrad::var;

  const char* function = "check_consistent_size";
  const char* name1 = "name1";
  

  Matrix<var,Dynamic,1> v1(4);
  v1 << 4.0,5.0,6.0,7.0;
  EXPECT_EQ(4U, size_of(v1));
  EXPECT_TRUE(check_consistent_size(function, name1, v1, 4U));
  EXPECT_THROW(check_consistent_size(function, name1, v1, 2U), std::invalid_argument);
  stan::agrad::recover_memory();
}

TEST(AgradRevErrorHandlingScalar, checkConsistentSize_nan) {
  using Eigen::Matrix;
  using Eigen::Dynamic;
  using stan::math::check_consistent_size;
  using stan::size_of;
  using stan::agrad::var;

  const char* function = "check_consistent_size";
  const char* name1 = "name1";
  
  double nan = std::numeric_limits<double>::quiet_NaN();

  Matrix<var,Dynamic,1> v1(4);
  v1 << nan,nan,4,nan;
  EXPECT_EQ(4U, size_of(v1));
  EXPECT_TRUE(check_consistent_size(function, name1, v1, 4U));
  EXPECT_THROW(check_consistent_size(function, name1, v1, 2U), std::invalid_argument);
  stan::agrad::recover_memory();
}

TEST(AgradRevErrorHandlingScalar, CheckConsistentSizeVarCheckVectorized) {
  using stan::agrad::var;
  using std::vector;
  using stan::math::check_consistent_size;

  int N = 5;
  const char* function = "check_consistent_size";
  vector<var> a;

  for (int i = 0; i < N; ++i)
   a.push_back(var(i));

  size_t stack_size = stan::agrad::ChainableStack::var_stack_.size();

  EXPECT_EQ(5U,stack_size);
  EXPECT_TRUE(check_consistent_size(function,"a",a,5U));

  size_t stack_size_after_call = stan::agrad::ChainableStack::var_stack_.size();
  EXPECT_EQ(5U,stack_size_after_call);
  stan::agrad::recover_memory();
}
