#include <stan/math/fwd/mat.hpp>
#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <gtest/gtest.h>

using namespace Eigen;
using stan::math::fvar;

TEST(AgradFwd, append_array_double_fvar) {
  std::vector<double> x(3);
  std::vector<fvar<double> > y(2), result;

  x[0] = 1.0;
  x[1] = 2.0;
  x[2] = 3.0;
  y[0] = 0.5;
  y[0].d_ = 5.0;
  y[1] = 4.0;
  y[1].d_ = 6.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());

  EXPECT_FLOAT_EQ(1.0, result[0].val());
  EXPECT_FLOAT_EQ(2.0, result[1].val());
  EXPECT_FLOAT_EQ(3.0, result[2].val());
  EXPECT_FLOAT_EQ(0.5, result[3].val());
  EXPECT_FLOAT_EQ(4.0, result[4].val());

  EXPECT_FLOAT_EQ(0.0, result[0].tangent());
  EXPECT_FLOAT_EQ(0.0, result[1].tangent());
  EXPECT_FLOAT_EQ(0.0, result[2].tangent());
  EXPECT_FLOAT_EQ(5.0, result[3].tangent());
  EXPECT_FLOAT_EQ(6.0, result[4].tangent());
}

TEST(AgradFwd, append_array_fvar_double) {
  std::vector<double> x(2);
  std::vector<fvar<double> > y(3), result;

  x[0] = 1.0;
  x[1] = 2.0;
  y[0] = 5.0;
  y[0].d_ = 1.5;
  y[1] = 6.0;
  y[1].d_ = 2.5;
  y[2] = 7.0;
  y[2].d_ = 3.5;

  EXPECT_NO_THROW(result = stan::math::append_array(y, x));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(5.0, result[0].val());
  EXPECT_FLOAT_EQ(6.0, result[1].val());
  EXPECT_FLOAT_EQ(7.0, result[2].val());
  EXPECT_FLOAT_EQ(1.0, result[3].val());
  EXPECT_FLOAT_EQ(2.0, result[4].val());

  EXPECT_FLOAT_EQ(1.5, result[0].tangent());
  EXPECT_FLOAT_EQ(2.5, result[1].tangent());
  EXPECT_FLOAT_EQ(3.5, result[2].tangent());
  EXPECT_FLOAT_EQ(0.0, result[3].tangent());
  EXPECT_FLOAT_EQ(0.0, result[4].tangent());
}

TEST(AgradFwd, append_array_double_fvar_fvar) {
  std::vector<double> x(3);
  std::vector<fvar<fvar<double> > > y(2), result;

  x[0] = 1.0;
  x[1] = 2.0;
  x[2] = 3.0;
  y[0] = 0.5;
  y[0].val_.d_ = 1.5;
  y[0].d_ = 5.0;
  y[0].d_.d_ = 2.5;
  y[1] = 4.0;
  y[1].val_.d_ = 3.5;
  y[1].d_ = 6.0;
  y[1].d_.d_ = 4.5;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());

  EXPECT_FLOAT_EQ(1.0, result[0].val().val());
  EXPECT_FLOAT_EQ(2.0, result[1].val().val());
  EXPECT_FLOAT_EQ(3.0, result[2].val().val());
  EXPECT_FLOAT_EQ(0.5, result[3].val().val());
  EXPECT_FLOAT_EQ(4.0, result[4].val().val());

  EXPECT_FLOAT_EQ(0.0, result[0].val().tangent());
  EXPECT_FLOAT_EQ(0.0, result[1].val().tangent());
  EXPECT_FLOAT_EQ(0.0, result[2].val().tangent());
  EXPECT_FLOAT_EQ(1.5, result[3].val().tangent());
  EXPECT_FLOAT_EQ(3.5, result[4].val().tangent());

  EXPECT_FLOAT_EQ(0.0, result[0].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[1].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[2].tangent().val());
  EXPECT_FLOAT_EQ(5.0, result[3].tangent().val());
  EXPECT_FLOAT_EQ(6.0, result[4].tangent().val());

  EXPECT_FLOAT_EQ(0.0, result[0].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[1].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[2].tangent().tangent());
  EXPECT_FLOAT_EQ(2.5, result[3].tangent().tangent());
  EXPECT_FLOAT_EQ(4.5, result[4].tangent().tangent());
}

TEST(AgradFwd, append_array_fvar_fvar_double) {
  std::vector<double> x(2);
  std::vector<fvar<fvar<double> > > y(3), result;

  x[0] = 1.0;
  x[1] = 2.0;
  y[0] = 5.0;
  y[0].val_.d_ = 11.0;
  y[0].d_ = 1.5;
  y[0].d_.d_ = 15.0;
  y[1] = 6.0;
  y[1].val_.d_ = 12.0;
  y[1].d_ = 2.5;
  y[1].d_.d_ = 16.0;
  y[2] = 7.0;
  y[2].val_.d_ = 13.0;
  y[2].d_ = 3.5;
  y[2].d_.d_ = 17.0;

  EXPECT_NO_THROW(result = stan::math::append_array(y, x));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(5.0, result[0].val().val());
  EXPECT_FLOAT_EQ(6.0, result[1].val().val());
  EXPECT_FLOAT_EQ(7.0, result[2].val().val());
  EXPECT_FLOAT_EQ(1.0, result[3].val().val());
  EXPECT_FLOAT_EQ(2.0, result[4].val().val());

  EXPECT_FLOAT_EQ(11.0, result[0].val().tangent());
  EXPECT_FLOAT_EQ(12.0, result[1].val().tangent());
  EXPECT_FLOAT_EQ(13.0, result[2].val().tangent());
  EXPECT_FLOAT_EQ(0.0, result[3].val().tangent());
  EXPECT_FLOAT_EQ(0.0, result[4].val().tangent());

  EXPECT_FLOAT_EQ(1.5, result[0].tangent().val());
  EXPECT_FLOAT_EQ(2.5, result[1].tangent().val());
  EXPECT_FLOAT_EQ(3.5, result[2].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[3].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[4].tangent().val());

  EXPECT_FLOAT_EQ(15.0, result[0].tangent().tangent());
  EXPECT_FLOAT_EQ(16.0, result[1].tangent().tangent());
  EXPECT_FLOAT_EQ(17.0, result[2].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[3].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[4].tangent().tangent());
}

TEST(AgradFwd, append_array_fvar_fvar) {
  std::vector<fvar<double> > x(3), y(2), result;

  x[0] = 5.0;
  x[0].d_ = 1.5;
  x[1] = 6.0;
  x[1].d_ = 0.5;
  x[2] = 7.0;
  x[2].d_ = -1.5;
  y[0] = 0.5;
  y[0].d_ = 2.5;
  y[1] = 4.0;
  y[1].d_ = -5.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(5.0, result[0].val());
  EXPECT_FLOAT_EQ(6.0, result[1].val());
  EXPECT_FLOAT_EQ(7.0, result[2].val());
  EXPECT_FLOAT_EQ(0.5, result[3].val());
  EXPECT_FLOAT_EQ(4.0, result[4].val());

  EXPECT_FLOAT_EQ(1.5, result[0].tangent());
  EXPECT_FLOAT_EQ(0.5, result[1].tangent());
  EXPECT_FLOAT_EQ(-1.5, result[2].tangent());
  EXPECT_FLOAT_EQ(2.5, result[3].tangent());
  EXPECT_FLOAT_EQ(-5.0, result[4].tangent());
}

TEST(AgradFwd, append_array_fvar_fvar_fvar1) {
  std::vector<fvar<double> > x(3);
  std::vector<fvar<fvar<double> > > y(2), result;

  x[0] = 5.0;
  x[0].d_ = 1.5;
  x[1] = 6.0;
  x[1].d_ = 0.5;
  x[2] = 7.0;
  x[2].d_ = -1.5;
  y[0] = 0.5;
  y[0].val_.d_ = 11.0;
  y[0].d_ = 2.5;
  y[0].d_.d_ = 15.0;
  y[1] = 4.0;
  y[1].val_.d_ = 12.0;
  y[1].d_ = -5.0;
  y[1].d_.d_ = 16.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(5.0, result[0].val().val());
  EXPECT_FLOAT_EQ(6.0, result[1].val().val());
  EXPECT_FLOAT_EQ(7.0, result[2].val().val());
  EXPECT_FLOAT_EQ(0.5, result[3].val().val());
  EXPECT_FLOAT_EQ(4.0, result[4].val().val());

  EXPECT_FLOAT_EQ(1.5, result[0].val().tangent());
  EXPECT_FLOAT_EQ(0.5, result[1].val().tangent());
  EXPECT_FLOAT_EQ(-1.5, result[2].val().tangent());
  EXPECT_FLOAT_EQ(11.0, result[3].val().tangent());
  EXPECT_FLOAT_EQ(12.0, result[4].val().tangent());

  EXPECT_FLOAT_EQ(0.0, result[0].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[1].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[2].tangent().val());
  EXPECT_FLOAT_EQ(2.5, result[3].tangent().val());
  EXPECT_FLOAT_EQ(-5.0, result[4].tangent().val());

  EXPECT_FLOAT_EQ(0.0, result[0].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[1].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[2].tangent().tangent());
  EXPECT_FLOAT_EQ(15.0, result[3].tangent().tangent());
  EXPECT_FLOAT_EQ(16.0, result[4].tangent().tangent());
}

TEST(AgradFwd, append_array_fvar_fvar_fvar2) {
  std::vector<fvar<double> > y(2);
  std::vector<fvar<fvar<double> > > x(3), result;

  x[0] = 5.0;
  x[0].val_.d_ = 11.0;
  x[0].d_ = 1.5;
  x[0].d_.d_ = 15.0;
  x[1] = 6.0;
  x[1].val_.d_ = 12.0;
  x[1].d_ = 0.5;
  x[1].d_.d_ = 16.0;
  x[2] = 7.0;
  x[2].val_.d_ = 13.0;
  x[2].d_ = -1.5;
  x[2].d_.d_ = 17.0;
  y[0] = 0.5;
  y[0].d_ = 2.5;
  y[1] = 4.0;
  y[1].d_ = -5.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(5.0, result[0].val().val());
  EXPECT_FLOAT_EQ(6.0, result[1].val().val());
  EXPECT_FLOAT_EQ(7.0, result[2].val().val());
  EXPECT_FLOAT_EQ(0.5, result[3].val().val());
  EXPECT_FLOAT_EQ(4.0, result[4].val().val());

  EXPECT_FLOAT_EQ(11.0, result[0].val().tangent());
  EXPECT_FLOAT_EQ(12.0, result[1].val().tangent());
  EXPECT_FLOAT_EQ(13.0, result[2].val().tangent());
  EXPECT_FLOAT_EQ(2.5, result[3].val().tangent());
  EXPECT_FLOAT_EQ(-5.0, result[4].val().tangent());

  EXPECT_FLOAT_EQ(1.5, result[0].tangent().val());
  EXPECT_FLOAT_EQ(0.5, result[1].tangent().val());
  EXPECT_FLOAT_EQ(-1.5, result[2].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[3].tangent().val());
  EXPECT_FLOAT_EQ(0.0, result[4].tangent().val());

  EXPECT_FLOAT_EQ(15.0, result[0].tangent().tangent());
  EXPECT_FLOAT_EQ(16.0, result[1].tangent().tangent());
  EXPECT_FLOAT_EQ(17.0, result[2].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[3].tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[4].tangent().tangent());
}

TEST(AgradFwd, append_array_fvar_fvar_fvar_fvar) {
  std::vector<fvar<fvar<double> > > x(3), y(2), result;

  x[0] = 5.0;
  x[0].val_.d_ = 11.0;
  x[0].d_ = 1.5;
  x[0].d_.d_ = 16.0;
  x[1] = 6.0;
  x[1].val_.d_ = 12.0;
  x[1].d_ = 0.5;
  x[1].d_.d_ = 17.0;
  x[2] = 7.0;
  x[2].val_.d_ = 13.0;
  x[2].d_ = -1.5;
  x[2].d_.d_ = 18.0;
  y[0] = 0.5;
  y[0].val_.d_ = 14.0;
  y[0].d_ = 2.5;
  y[0].d_.d_ = 19.0;
  y[1] = 4.0;
  y[1].val_.d_ = 15.0;
  y[1].d_ = -5.0;
  y[1].d_.d_ = 20.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(5.0, result[0].val().val());
  EXPECT_FLOAT_EQ(6.0, result[1].val().val());
  EXPECT_FLOAT_EQ(7.0, result[2].val().val());
  EXPECT_FLOAT_EQ(0.5, result[3].val().val());
  EXPECT_FLOAT_EQ(4.0, result[4].val().val());

  EXPECT_FLOAT_EQ(1.5, result[0].tangent().val());
  EXPECT_FLOAT_EQ(0.5, result[1].tangent().val());
  EXPECT_FLOAT_EQ(-1.5, result[2].tangent().val());
  EXPECT_FLOAT_EQ(2.5, result[3].tangent().val());
  EXPECT_FLOAT_EQ(-5.0, result[4].tangent().val());

  EXPECT_FLOAT_EQ(11.0, result[0].val().tangent());
  EXPECT_FLOAT_EQ(12.0, result[1].val().tangent());
  EXPECT_FLOAT_EQ(13.0, result[2].val().tangent());
  EXPECT_FLOAT_EQ(14.0, result[3].val().tangent());
  EXPECT_FLOAT_EQ(15.0, result[4].val().tangent());

  EXPECT_FLOAT_EQ(16.0, result[0].tangent().tangent());
  EXPECT_FLOAT_EQ(17.0, result[1].tangent().tangent());
  EXPECT_FLOAT_EQ(18.0, result[2].tangent().tangent());
  EXPECT_FLOAT_EQ(19.0, result[3].tangent().tangent());
  EXPECT_FLOAT_EQ(20.0, result[4].tangent().tangent());
}


TEST(AgradFwd, append_array_matrix_double_matrix_fvar) {
  std::vector<Matrix<double, Dynamic, Dynamic> > x;
  std::vector<Matrix<fvar<double>, Dynamic, Dynamic> > y, result;

  for (int i = 0; i < 3; i++)
    x.push_back(Matrix<double, Dynamic, Dynamic>::Zero(3, 3));
  for (int i = 0; i < 2; i++)
    y.push_back(Matrix<fvar<double>, Dynamic, Dynamic>::Zero(3, 3));

  x[0](0, 0) = 1.0;
  y[1](2, 1) = 2.0;
  y[1](2, 1).d_ = 3.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(1.0, result[0](0, 0).val());
  EXPECT_FLOAT_EQ(2.0, result[4](2, 1).val());
  EXPECT_FLOAT_EQ(3.0, result[4](2, 1).tangent());
  EXPECT_FLOAT_EQ(0.0, result[4](2, 2).val());

  for (int i = 0; i < 2; i++)
    y[i] = Matrix<fvar<double>, Dynamic, Dynamic>::Zero(2, 2);

  EXPECT_THROW(result = stan::math::append_array(x, y), std::invalid_argument);
}

TEST(AgradFwd, append_array_matrix_double_matrix_fvar_fvar) {
  std::vector<Matrix<double, Dynamic, Dynamic> > x;
  std::vector<Matrix<fvar<fvar<double> >, Dynamic, Dynamic> > y, result;

  for (int i = 0; i < 3; i++)
    x.push_back(Matrix<double, Dynamic, Dynamic>::Zero(3, 3));
  for (int i = 0; i < 2; i++)
    y.push_back(Matrix<fvar<fvar<double> >, Dynamic, Dynamic>::Zero(3, 3));

  x[0](0, 0) = 1.0;
  y[1](2, 1) = 2.0;
  y[1](2, 1).d_ = 3.0;
  y[1](2, 1).val_.d_ = 4.0;
  y[1](2, 1).d_.d_ = 5.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(1.0, result[0](0, 0).val().val());
  EXPECT_FLOAT_EQ(2.0, result[4](2, 1).val().val());
  EXPECT_FLOAT_EQ(3.0, result[4](2, 1).tangent().val());
  EXPECT_FLOAT_EQ(4.0, result[4](2, 1).val().tangent());
  EXPECT_FLOAT_EQ(5.0, result[4](2, 1).tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[4](2, 2).val().val());

  for (int i = 0; i < 2; i++)
    y[i] = Matrix<fvar<fvar<double> >, Dynamic, Dynamic>::Zero(2, 2);

  EXPECT_THROW(result = stan::math::append_array(x, y), std::invalid_argument);
}

TEST(AgradFwd, append_array_matrix_fvar_fvar_matrix_fvar_fvar) {
  std::vector<Matrix<fvar<fvar<double> >, Dynamic, Dynamic> > x, y, result;

  for (int i = 0; i < 3; i++)
    x.push_back(Matrix<fvar<fvar<double> >, Dynamic, Dynamic>::Zero(3, 3));

  for (int i = 0; i < 2; i++)
    y.push_back(Matrix<fvar<fvar<double> >, Dynamic, Dynamic>::Zero(3, 3));

  x[0](0, 0) = 1.0;
  x[0](0, 0).d_ = 6.0;
  x[0](0, 0).val_.d_ = 7.0;
  x[0](0, 0).d_.d_ = 8.0;
  y[1](2, 1) = 2.0;
  y[1](2, 1).d_ = 3.0;
  y[1](2, 1).val_.d_ = 4.0;
  y[1](2, 1).d_.d_ = 5.0;

  EXPECT_NO_THROW(result = stan::math::append_array(x, y));
  EXPECT_EQ(5, result.size());
  EXPECT_FLOAT_EQ(1.0, result[0](0, 0).val().val());
  EXPECT_FLOAT_EQ(6.0, result[0](0, 0).tangent().val());
  EXPECT_FLOAT_EQ(7.0, result[0](0, 0).val().tangent());
  EXPECT_FLOAT_EQ(8.0, result[0](0, 0).tangent().tangent());
  EXPECT_FLOAT_EQ(2.0, result[4](2, 1).val().val());
  EXPECT_FLOAT_EQ(3.0, result[4](2, 1).tangent().val());
  EXPECT_FLOAT_EQ(4.0, result[4](2, 1).val().tangent());
  EXPECT_FLOAT_EQ(5.0, result[4](2, 1).tangent().tangent());
  EXPECT_FLOAT_EQ(0.0, result[4](2, 2).val().val());

  for (int i = 0; i < 2; i++)
    y[i] = Matrix<fvar<fvar<double> >, Dynamic, Dynamic>::Zero(2, 2);

  EXPECT_THROW(result = stan::math::append_array(x, y), std::invalid_argument);
}
