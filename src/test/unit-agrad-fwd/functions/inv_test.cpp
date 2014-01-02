#include <gtest/gtest.h>
#include <stan/agrad/fwd.hpp>
#include <stan/math/functions/inv.hpp>
#include <stan/math/constants.hpp>
#include <stan/agrad/rev.hpp>
#include <test/unit/agrad/util.hpp>

TEST(AgradFwdInv,Fvar) {
  using stan::agrad::fvar;
  using stan::math::inv;

  fvar<double> x(0.5);
  x.d_ = 1.0;   // Derivatives w.r.t. x
  fvar<double> a = inv(x);

  EXPECT_FLOAT_EQ(inv(0.5), a.val_);
  EXPECT_FLOAT_EQ(-1 / 0.25, a.d_);

  fvar<double> b = 3 * inv(x) + x;
  EXPECT_FLOAT_EQ(3 * inv(0.5) + 0.5, b.val_);
  EXPECT_FLOAT_EQ(-3 / 0.25 + 1, b.d_);

  fvar<double> c = -inv(x) + 5;
  EXPECT_FLOAT_EQ(-inv(0.5) + 5, c.val_);
  EXPECT_FLOAT_EQ(1 / 0.25, c.d_);

  fvar<double> d = -3 * inv(x) + 5 * x;
  EXPECT_FLOAT_EQ(-3 * inv(0.5) + 5 * 0.5, d.val_);
  EXPECT_FLOAT_EQ(3 / 0.25 + 5, d.d_);

  fvar<double> e = -3 * inv(-x) + 5 * x;
  EXPECT_FLOAT_EQ(-3 * inv(-0.5) + 5 * 0.5, e.val_);
  EXPECT_FLOAT_EQ(-3 / 0.25 + 5, e.d_);

  fvar<double> y(-0.5);
  y.d_ = 1.0;
  fvar<double> f = inv(y);
  EXPECT_FLOAT_EQ(inv(-0.5), f.val_);
  EXPECT_FLOAT_EQ(-1 / 0.25, f.d_);

  fvar<double> z(0.0);
  z.d_ = 1.0;
  fvar<double> g = inv(z);
  EXPECT_FLOAT_EQ(stan::math::positive_infinity(), g.val_);
  EXPECT_FLOAT_EQ(stan::math::negative_infinity(), g.d_);
}   

TEST(AgradFwdInv,FvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::inv;

  fvar<var> x(0.5,1.0);
  fvar<var> a = inv(x);

  EXPECT_FLOAT_EQ(inv(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(-inv(0.5) * inv(0.5), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(-1.0 / (0.5 * 0.5), g[0]);
}
TEST(AgradFwdInv,FvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::inv;

  fvar<var> x(0.5,1.0);
  fvar<var> a = inv(x);

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.d_.grad(y,g);
  EXPECT_FLOAT_EQ(-1.0 * -2.0 / (0.5 * 0.5 * 0.5), g[0]);
}
TEST(AgradFwdInv,FvarFvarDouble) {
  using stan::agrad::fvar;
  using stan::math::inv;
  using std::log;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = inv(x);

  EXPECT_FLOAT_EQ(inv(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(-inv(0.5) * inv(0.5), a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}
TEST(AgradFwdInv,FvarFvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::inv;
  using std::log;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = inv(x);

  EXPECT_FLOAT_EQ(inv(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(-inv(0.5) * inv(0.5), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.val_.grad(p,g);
  EXPECT_FLOAT_EQ(-inv(0.5) * inv(0.5), g[0]);
}
TEST(AgradFwdInv,FvarFvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::inv;
  using std::log;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = inv(x);

  EXPECT_FLOAT_EQ(inv(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(-inv(0.5) * inv(0.5), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(-1.0 * -2.0 / (0.5 * 0.5 * 0.5), g[0]);
}
TEST(AgradFwdInv,FvarFvarVar_3rdDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = inv(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.d_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(0, g[0]);
}

