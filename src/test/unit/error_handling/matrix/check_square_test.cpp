#include <stan/error_handling/matrix/check_square.hpp>
#include <gtest/gtest.h>

TEST(ErrorHandlingMatrix, checkSquareMatrix) {
  Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> y;
  double result;
  
  y.resize(3,3);
  EXPECT_TRUE(stan::error_handling::check_square("checkSquareMatrix(%1%)",
                                           y, "y", &result));

  y.resize(3, 2);
  EXPECT_THROW(stan::error_handling::check_square("checkSquareMatrix(%1%)", y, "y", &result), 
               std::domain_error);
}

TEST(ErrorHandlingMatrix, checkSquareMatrix_nan) {
  Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> y;
  double result;
  double nan = std::numeric_limits<double>::quiet_NaN();

  y.resize(3,3);
  y << nan, nan, nan,nan, nan, nan,nan, nan, nan;
  EXPECT_TRUE(stan::error_handling::check_square("checkSquareMatrix(%1%)",
                                           y, "y", &result));

  y.resize(3, 2);
  y << nan, nan, nan,nan, nan, nan;
  EXPECT_THROW(stan::error_handling::check_square("checkSquareMatrix(%1%)", y, "y", &result), 
               std::domain_error);
}
