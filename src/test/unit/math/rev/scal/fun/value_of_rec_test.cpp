#include <stan/math/fwd/scal/fun/value_of_rec.hpp>
#include <stan/math/rev/scal/fun/value_of_rec.hpp>
#include <stan/math/rev/core/var.hpp>
#include <stan/math/fwd/core/fvar.hpp>
#include <test/unit/math/rev/mat/fun/util.hpp>
#include <gtest/gtest.h>

TEST(AgradRev,value_of_rec) {
  using stan::agrad::var;
  using stan::agrad::fvar;
  using stan::math::value_of_rec;

  double a(5.0);

  var v_a(5.0);
  fvar<var> fv_a(5.0);
  fvar<double> fd_a(5.0);
  fvar<fvar<double> > ffd_a(5.0);

  fvar<fvar<var> > ffv_a(5.0);
  fvar<fvar<fvar<fvar<fvar<var> > > > > fffffv_a(5.0);

  EXPECT_FLOAT_EQ(5.0,value_of_rec(a));
  EXPECT_FLOAT_EQ(5.0,value_of_rec(v_a));
  EXPECT_FLOAT_EQ(5.0,value_of_rec(fd_a));
  EXPECT_FLOAT_EQ(5.0,value_of_rec(fv_a));
  EXPECT_FLOAT_EQ(5.0,value_of_rec(ffd_a));
  EXPECT_FLOAT_EQ(5.0,value_of_rec(ffv_a));
  EXPECT_FLOAT_EQ(5.0,value_of_rec(fffffv_a));
}
