#include <gtest/gtest.h>
#include <test/unit/math/rev/mat/fun/util.hpp>
#include <stan/math/fwd/core/fvar.hpp>
#include <stan/math/rev/core/var.hpp>
#include <stan/math/fwd/scal/fun/operator_addition.hpp>
#include <stan/math/fwd/scal/fun/operator_division.hpp>
#include <stan/math/fwd/scal/fun/operator_equal.hpp>
#include <stan/math/fwd/scal/fun/operator_greater_than.hpp>
#include <stan/math/fwd/scal/fun/operator_greater_than_or_equal.hpp>
#include <stan/math/fwd/scal/fun/operator_less_than.hpp>
#include <stan/math/fwd/scal/fun/operator_less_than_or_equal.hpp>
#include <stan/math/fwd/scal/fun/operator_multiplication.hpp>
#include <stan/math/fwd/scal/fun/operator_not_equal.hpp>
#include <stan/math/fwd/scal/fun/operator_subtraction.hpp>
#include <stan/math/fwd/scal/fun/operator_unary_minus.hpp>
#include <stan/math/rev/scal/fun/operator_addition.hpp>
#include <stan/math/rev/scal/fun/operator_divide_equal.hpp>
#include <stan/math/rev/scal/fun/operator_division.hpp>
#include <stan/math/rev/scal/fun/operator_equal.hpp>
#include <stan/math/rev/scal/fun/operator_greater_than.hpp>
#include <stan/math/rev/scal/fun/operator_greater_than_or_equal.hpp>
#include <stan/math/rev/scal/fun/operator_less_than.hpp>
#include <stan/math/rev/scal/fun/operator_less_than_or_equal.hpp>
#include <stan/math/rev/scal/fun/operator_minus_equal.hpp>
#include <stan/math/rev/scal/fun/operator_multiplication.hpp>
#include <stan/math/rev/scal/fun/operator_multiply_equal.hpp>
#include <stan/math/rev/scal/fun/operator_not_equal.hpp>
#include <stan/math/rev/scal/fun/operator_plus_equal.hpp>
#include <stan/math/rev/scal/fun/operator_subtraction.hpp>
#include <stan/math/rev/scal/fun/operator_unary_decrement.hpp>
#include <stan/math/rev/scal/fun/operator_unary_increment.hpp>
#include <stan/math/rev/scal/fun/operator_unary_negative.hpp>
#include <stan/math/rev/scal/fun/operator_unary_not.hpp>
#include <stan/math/rev/scal/fun/operator_unary_plus.hpp>
#include <stan/math/rev/scal/fun/operator_addition.hpp>
#include <stan/math/rev/scal/fun/operator_divide_equal.hpp>
#include <stan/math/rev/scal/fun/operator_division.hpp>
#include <stan/math/rev/scal/fun/operator_equal.hpp>
#include <stan/math/rev/scal/fun/operator_greater_than.hpp>
#include <stan/math/rev/scal/fun/operator_greater_than_or_equal.hpp>
#include <stan/math/rev/scal/fun/operator_less_than.hpp>
#include <stan/math/rev/scal/fun/operator_less_than_or_equal.hpp>
#include <stan/math/rev/scal/fun/operator_minus_equal.hpp>
#include <stan/math/rev/scal/fun/operator_multiplication.hpp>
#include <stan/math/rev/scal/fun/operator_multiply_equal.hpp>
#include <stan/math/rev/scal/fun/operator_not_equal.hpp>
#include <stan/math/rev/scal/fun/operator_plus_equal.hpp>
#include <stan/math/rev/scal/fun/operator_subtraction.hpp>
#include <stan/math/rev/scal/fun/operator_unary_decrement.hpp>
#include <stan/math/rev/scal/fun/operator_unary_increment.hpp>
#include <stan/math/rev/scal/fun/operator_unary_negative.hpp>
#include <stan/math/rev/scal/fun/operator_unary_not.hpp>
#include <stan/math/rev/scal/fun/operator_unary_plus.hpp>

TEST(AgradFwdOperatorNotEqual,Fvar) {
  using stan::agrad::fvar;
  fvar<double> v4 = 4;
  fvar<double> v5 = 5;
  double d4 = 4;
  double d5 = 5;
  
  EXPECT_TRUE(v5 != v4);
  EXPECT_TRUE(d5 != v4);
  EXPECT_TRUE(v5 != d4);
  EXPECT_TRUE(d5 != d4);

  int i4 = 4;
  int i5 = 5;
  int i6 = 5;

  EXPECT_TRUE(i5 != v4);
  EXPECT_TRUE(v5 != i4);
  EXPECT_TRUE(i5 != i4);
  EXPECT_TRUE(d5 != i4);
  EXPECT_TRUE(i5 != d4);
  EXPECT_FALSE(i6 != i5);
  EXPECT_FALSE(i6 != v5);
}

TEST(AgradFwdOperatorNotEqual, FvarVar) {
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<var> x(0.5,1.3);
  fvar<var> y(1.5,1.0);
  fvar<var> z(0.5,1.3);

  EXPECT_FALSE(x != z);
  EXPECT_TRUE(x != y);
  EXPECT_TRUE(z != y);
}

TEST(AgradFwdOperatorNotEqual, FvarFvarDouble) {
  using stan::agrad::fvar;

  fvar<fvar<double> > x;
  x.val_.val_ = 1.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<double> > z;
  z.val_.val_ = 0.5;
  z.d_.val_ = 1.0;

  EXPECT_TRUE(x != y);
  EXPECT_TRUE(x != z);
  EXPECT_FALSE(z != y);
}
TEST(AgradFwdOperatorNotEqual, FvarFvarVar) {
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x;
  x.val_.val_ = 1.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > z;
  z.val_.val_ = 0.5;
  z.d_.val_ = 1.0;

  EXPECT_TRUE(x != y);
  EXPECT_TRUE(x != z);
  EXPECT_FALSE(z != y);
}


TEST(AgradFwdOperatorNotEqual, ne_nan) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  double nan = std::numeric_limits<double>::quiet_NaN();
  double a = 3.0;
  fvar<double> nan_fd = std::numeric_limits<double>::quiet_NaN();
  fvar<double> a_fd = 3.0;
  fvar<var> nan_fv = std::numeric_limits<double>::quiet_NaN();
  fvar<var> a_fv = 3.0;
  fvar<fvar<double> > nan_ffd = std::numeric_limits<double>::quiet_NaN();
  fvar<fvar<double> > a_ffd = 3.0;
  fvar<fvar<var> > nan_ffv = std::numeric_limits<double>::quiet_NaN();
  fvar<fvar<var> > a_ffv = 3.0;

  EXPECT_TRUE(a != nan_fd);
  EXPECT_TRUE(a_fd != nan_fd);
  EXPECT_TRUE(nan != nan_fd);
  EXPECT_TRUE(nan_fd != nan_fd);
  EXPECT_TRUE(a_fd != nan);
  EXPECT_TRUE(nan_fd != nan);
  EXPECT_TRUE(nan_fd != a);
  EXPECT_TRUE(nan_fd != a_fd);
  EXPECT_TRUE(nan != a_fd);

  EXPECT_TRUE(a != nan_fv);
  EXPECT_TRUE(a_fv != nan_fv);
  EXPECT_TRUE(nan != nan_fv);
  EXPECT_TRUE(nan_fv != nan_fv);
  EXPECT_TRUE(a_fv != nan);
  EXPECT_TRUE(nan_fv != nan);
  EXPECT_TRUE(nan_fv != a);
  EXPECT_TRUE(nan_fv != a_fv);
  EXPECT_TRUE(nan != a_fv);

  EXPECT_TRUE(a != nan_ffd);
  EXPECT_TRUE(a_ffd != nan_ffd);
  EXPECT_TRUE(nan != nan_ffd);
  EXPECT_TRUE(nan_ffd != nan_ffd);
  EXPECT_TRUE(a_ffd != nan);
  EXPECT_TRUE(nan_ffd != nan);
  EXPECT_TRUE(nan_ffd != a);
  EXPECT_TRUE(nan_ffd != a_ffd);
  EXPECT_TRUE(nan != a_ffd);

  EXPECT_TRUE(a != nan_ffv);
  EXPECT_TRUE(a_ffv != nan_ffv);
  EXPECT_TRUE(nan != nan_ffv);
  EXPECT_TRUE(nan_ffv != nan_ffv);
  EXPECT_TRUE(a_ffv != nan);
  EXPECT_TRUE(nan_ffv != nan);
  EXPECT_TRUE(nan_ffv != a);
  EXPECT_TRUE(nan_ffv != a_ffv);
  EXPECT_TRUE(nan != a_ffv);
}
