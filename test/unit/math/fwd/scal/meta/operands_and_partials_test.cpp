#include <stan/math/fwd/scal.hpp>
#include <gtest/gtest.h>

TEST(AgradPartialsVari, OperandsAndPartialsFvar) {
  using stan::math::operands_and_partials;
  using stan::math::fvar;

  fvar<double> x1 = 2.0;
  fvar<double> x2 = 3.0;
  fvar<double> x3 = 5.0;
  x1.d_ = 2.0;
  x2.d_ = -1.0;
  x3.d_ = 4.0;

  operands_and_partials<fvar<double>,fvar<double>,fvar<double> > o(x1, x2, x3);
  o.d_x1[0] += 17.0;
  o.d_x2[0] += 19.0;
  o.d_x3[0] += 23.0;

  fvar<double> y = o.value(-1.0);
  EXPECT_FLOAT_EQ(107,y.d_);
  EXPECT_FLOAT_EQ(-1,y.val_);
}
