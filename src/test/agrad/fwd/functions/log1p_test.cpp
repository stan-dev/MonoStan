#include <gtest/gtest.h>
#include <stan/agrad/fwd.hpp>
#include <stan/math/functions/log1p.hpp>
#include <stan/agrad/rev.hpp>
#include <test/agrad/util.hpp>

TEST(AgradFwdLog1p,Fvar) {
  using stan::agrad::fvar;
  using stan::math::log1p;
  using std::isnan;

  fvar<double> x(0.5,1.0);
  fvar<double> y(-1.0,2.0);
  fvar<double> z(-2.0,3.0);

  fvar<double> a = log1p(x);
  EXPECT_FLOAT_EQ(log1p(0.5), a.val_);
  EXPECT_FLOAT_EQ(1 / (1 + 0.5), a.d_);

  fvar<double> b = log1p(y);
  isnan(b.val_);
  isnan(b.d_);

  fvar<double> c = log1p(z);
  isnan(c.val_);
  isnan(c.d_);
}
TEST(AgradFwdLog1p,FvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p;

  fvar<var> x(0.5,1.3);
  fvar<var> a = log1p(x);

  EXPECT_FLOAT_EQ(log1p(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(1.3 / (1 + 0.5), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(1 / (1.5), g[0]);
}
TEST(AgradFwdLog1p,FvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p;

  fvar<var> x(0.5,1.3);
  fvar<var> a = log1p(x);

  EXPECT_FLOAT_EQ(log1p(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(1.3 / (1 + 0.5), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.d_.grad(y,g);
  EXPECT_FLOAT_EQ(-1.3 / (1.5 * 1.5), g[0]);
}
TEST(AgradFwdLog1p,FvarFvarDouble) {
  using stan::agrad::fvar;
  using stan::math::log1p;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = log1p(x);

  EXPECT_FLOAT_EQ(log1p(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(1 / (1.5), a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);

  fvar<fvar<double> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  a = log1p(y);
  EXPECT_FLOAT_EQ(log1p(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(0, a.val_.d_);
  EXPECT_FLOAT_EQ(1 / (1.5), a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}
TEST(AgradFwdLog1p,FvarFvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = log1p(x);

  EXPECT_FLOAT_EQ(log1p(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(1 / (1.5), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.val_.grad(p,g);
  EXPECT_FLOAT_EQ(1.0 / 1.5, g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = log1p(y);
  EXPECT_FLOAT_EQ(log1p(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(1 / (1.5), b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.val_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(1.0 / 1.5, r[0]);
}
TEST(AgradFwdLog1p,FvarFvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::log1p;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = log1p(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(-1.0 / 2.25, g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = log1p(y);

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.d_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(-1.0 / 2.25, r[0]);
}
TEST(AgradFwdLog1p,FvarFvarVar_3rdDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = log1p(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.d_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(0, g[0]);
}
