#include <stan/math/error_handling/matrix/check_pos_semidefinite.hpp>
#include <gtest/gtest.h>


TEST(MathErrorHandlingMatrix, checkPosSemidefiniteMatrix_nan) {
  Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> y;
  double result;
  double nan = std::numeric_limits<double>::quiet_NaN();
  using stan::math::check_pos_semidefinite;

  y.resize(1,1);
  y << nan;
  EXPECT_THROW(check_pos_semidefinite("checkPosSemidefiniteMatrix(%1%)", 
                                              y, "y", &result), 
               std::domain_error);
  
  y.resize(3,3);
  y << 2, -1, 0,
    -1, 2, -1,
    0, -1, 2;
  EXPECT_TRUE(check_pos_semidefinite("checkPosSemidefiniteMatrix(%1%)", 
                                              y, "y", &result));
  for (int i = 0; i < y.rows(); i++)
    for (int j = 0; j < y.cols(); j++) {
      y << 2, -1, 0, -1, 2, -1, 0, -1, 2;
      y(i,j) = nan;
      if (i >= j)
        EXPECT_THROW(check_pos_semidefinite("checkPosSemidefiniteMatrix(%1%)", 
                                        y, "y", &result), 
                     std::domain_error);
    }

  y << 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0;
  EXPECT_TRUE(check_pos_semidefinite("checkPosSemidefiniteMatrix(%1%)", 
                                              y, "y", &result));
  for (int i = 0; i < y.size(); i++) {
    y(i) = nan;
    EXPECT_THROW(check_pos_semidefinite("checkPosSemidefiniteMatrix(%1%)", 
                                        y, "y", &result), 
                 std::domain_error);
    y(i) = 0.0;
  }
}
