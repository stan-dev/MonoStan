#include <stan/error_handling/scalar/check_not_nan.hpp>
#include <gtest/gtest.h>
#include <stan/math/matrix/typedefs.hpp>
#include <stdexcept>

TEST(ErrorHandlingMatrix, checkNotNanEigenRow) {
  stan::math::vector_d y;
  double result;
  y.resize(3);
  
  EXPECT_TRUE(stan::error_handling::check_not_nan("checkNotNanEigenRow(%1)",
                                        y, "y", &result));
  EXPECT_TRUE(stan::error_handling::check_not_nan("checkNotNanEigenRow(%1)",
                                        y, "y", &result));
  
  y(1) = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(stan::error_handling::check_not_nan("checkNotNanEigenRow(%1%)", y, "y", &result), 
               std::domain_error);
  EXPECT_THROW(stan::error_handling::check_not_nan("checkNotNanEigenRow(%1%)", y, "y", &result), 
               std::domain_error);
}
