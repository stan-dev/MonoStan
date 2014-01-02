#include <gtest/gtest.h>
#include <stan/agrad/fwd.hpp>
#include <boost/math/special_functions/erf.hpp>
#include <stan/agrad/rev.hpp>
#include <test/unit/agrad/util.hpp>

TEST(AgradFwdErf,Fvar) {
  using stan::agrad::fvar;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<double> x(0.5,1.0);

  fvar<double> a = erf(x);
  EXPECT_FLOAT_EQ(erf(0.5), a.val_);
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), a.d_);

 fvar<double> b = erf(-x);
  EXPECT_FLOAT_EQ(erf(-0.5), b.val_);
  EXPECT_FLOAT_EQ(-2 * exp(-0.5 * 0.5) 
                  / sqrt(boost::math::constants::pi<double>()), b.d_);
}

TEST(AgradFwdErf,FvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<var> x(0.5,1.3);
  fvar<var> a = erf(x);

  EXPECT_FLOAT_EQ(erf(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(1.3 * 2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), g[0]);
}
TEST(AgradFwdErf,FvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<var> x(0.5,1.3);
  fvar<var> a = erf(x);

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.d_.grad(y,g);
  EXPECT_FLOAT_EQ(-1.3 * 2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), g[0]);
}

TEST(AgradFwdErf,FvarFvarDouble) {
  using stan::agrad::fvar;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = erf(x);

  EXPECT_FLOAT_EQ(erf(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);

  fvar<fvar<double> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  a = erf(y);
  EXPECT_FLOAT_EQ(erf(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(0, a.val_.d_);
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}

TEST(AgradFwdErf,FvarFvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = erf(x);

  EXPECT_FLOAT_EQ(erf(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.val_.grad(p,g);
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = erf(y);
  EXPECT_FLOAT_EQ(erf(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.val_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), r[0]);
}

TEST(AgradFwdErf,FvarFvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = erf(x);

  EXPECT_FLOAT_EQ(erf(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(-2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = erf(y);
  EXPECT_FLOAT_EQ(erf(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());

  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.d_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(-2 * exp(-0.5 * 0.5) / 
                  sqrt(boost::math::constants::pi<double>()), r[0]);
}
TEST(AgradFwdErf,FvarFvarVar_3rdDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;
  using std::sqrt;
  using boost::math::erf;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = erf(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.d_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(0, g[0]);
}

