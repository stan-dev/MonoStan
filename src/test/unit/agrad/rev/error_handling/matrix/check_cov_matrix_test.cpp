#include <gtest/gtest.h>
#include <stan/error_handling/matrix.hpp>
#include <stan/agrad/rev/matrix.hpp>

TEST(AgradRevErrorHandlingMatrix,CheckCovMatrix) {
  using stan::agrad::var;
  using Eigen::Dynamic;
  using Eigen::Matrix;
  
  using stan::math::check_cov_matrix;
  
  const char* function = "check_cov_matrix";
  Matrix<var,Dynamic,Dynamic> Sigma;
  Sigma.resize(1,1);
  Sigma << 1;

  EXPECT_NO_THROW(check_cov_matrix(function, "Sigma", Sigma))
    << "check_cov_matrix should not throw exception with Sigma";
}
