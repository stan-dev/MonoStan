#include <gtest/gtest.h>
#include <stan/agrad/fwd.hpp>
#include <boost/math/special_functions/digamma.hpp>
#include <stan/agrad/rev.hpp>
#include <test/agrad/util.hpp>

TEST(AgradFwdDigamma,Fvar) {
  using stan::agrad::fvar;
  using boost::math::digamma;
  using boost::math::zeta;

  fvar<double> x(0.5,1.0);
  fvar<double> a = digamma(x);
  EXPECT_FLOAT_EQ(digamma(0.5), a.val_);
  EXPECT_FLOAT_EQ(4.9348022005446793094, a.d_);
}

TEST(AgradFwdDigamma,FvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using boost::math::digamma;  
  using boost::math::zeta;
  
  fvar<var> x(0.5,1.3);
  fvar<var> a = digamma(x);

  EXPECT_FLOAT_EQ(digamma(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(1.3 * 4.9348022005446793094, a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(4.9348022005446793094, g[0]);
}

TEST(AgradFwdDigamma,FvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using boost::math::digamma;  
  using boost::math::zeta;
  
  fvar<var> x(0.5,1.3);
  fvar<var> a = digamma(x);

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.d_.grad(y,g);
  EXPECT_FLOAT_EQ(1.3 * -16.8288, g[0]);
}
TEST(AgradFwdDigamma,FvarFvarDouble) {
  using stan::agrad::fvar;
  using boost::math::digamma;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = digamma(x);

  EXPECT_FLOAT_EQ(digamma(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(4.9348022005446793094, a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);

  fvar<fvar<double> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  a = digamma(y);
  EXPECT_FLOAT_EQ(digamma(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(0, a.val_.d_);
  EXPECT_FLOAT_EQ(4.9348022005446793094, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}

TEST(AgradFwdDigamma,FvarFvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using boost::math::digamma;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = digamma(x);

  EXPECT_FLOAT_EQ(digamma(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(4.9348022005446793094, a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.val_.grad(p,g);
  EXPECT_FLOAT_EQ(4.9348022005446793094, g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = digamma(y);
  EXPECT_FLOAT_EQ(digamma(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(4.9348022005446793094, b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.val_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(4.9348022005446793094, r[0]);
}

TEST(AgradFwdDigamma,FvarFvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using boost::math::digamma;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = digamma(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(-16.8288, g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = digamma(y);

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.d_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(-16.8288, r[0]);
}
TEST(AgradFwdDigamma,FvarFvarVar_3rdDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using boost::math::digamma;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = digamma(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.d_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(0, g[0]);
}

