#include <gtest/gtest.h>
#include <stan/math/prim/scal/fun/grad_reg_inc_gamma.hpp>
#include <test/unit/math/rev/mat/fun/util.hpp>
#include <stan/math/fwd/core/operator_addition.hpp>
#include <stan/math/fwd/core/operator_division.hpp>
#include <stan/math/fwd/core/operator_equal.hpp>
#include <stan/math/fwd/core/operator_greater_than.hpp>
#include <stan/math/fwd/core/operator_greater_than_or_equal.hpp>
#include <stan/math/fwd/core/operator_less_than.hpp>
#include <stan/math/fwd/core/operator_less_than_or_equal.hpp>
#include <stan/math/fwd/core/operator_multiplication.hpp>
#include <stan/math/fwd/core/operator_not_equal.hpp>
#include <stan/math/fwd/core/operator_subtraction.hpp>
#include <stan/math/fwd/core/operator_unary_minus.hpp>
#include <stan/math/rev/core/operator_addition.hpp>
#include <stan/math/rev/core/operator_divide_equal.hpp>
#include <stan/math/rev/core/operator_division.hpp>
#include <stan/math/rev/core/operator_equal.hpp>
#include <stan/math/rev/core/operator_greater_than.hpp>
#include <stan/math/rev/core/operator_greater_than_or_equal.hpp>
#include <stan/math/rev/core/operator_less_than.hpp>
#include <stan/math/rev/core/operator_less_than_or_equal.hpp>
#include <stan/math/rev/core/operator_minus_equal.hpp>
#include <stan/math/rev/core/operator_multiplication.hpp>
#include <stan/math/rev/core/operator_multiply_equal.hpp>
#include <stan/math/rev/core/operator_not_equal.hpp>
#include <stan/math/rev/core/operator_plus_equal.hpp>
#include <stan/math/rev/core/operator_subtraction.hpp>
#include <stan/math/rev/core/operator_unary_decrement.hpp>
#include <stan/math/rev/core/operator_unary_increment.hpp>
#include <stan/math/rev/core/operator_unary_negative.hpp>
#include <stan/math/rev/core/operator_unary_not.hpp>
#include <stan/math/rev/core/operator_unary_plus.hpp>
#include <stan/math/fwd/scal/fun/digamma.hpp>
#include <stan/math/rev/scal/fun/digamma.hpp>
#include <stan/math/fwd/scal/fun/exp.hpp>
#include <stan/math/rev/scal/fun/exp.hpp>
#include <stan/math/fwd/scal/fun/fabs.hpp>
#include <stan/math/rev/scal/fun/fabs.hpp>
#include <stan/math/fwd/scal/fun/floor.hpp>
#include <stan/math/rev/scal/fun/floor.hpp>
#include <stan/math/fwd/scal/fun/gamma_p.hpp>
#include <stan/math/rev/scal/fun/gamma_p.hpp>
#include <stan/math/fwd/scal/fun/log.hpp>
#include <stan/math/rev/scal/fun/log.hpp>
#include <stan/math/fwd/scal/fun/pow.hpp>
#include <stan/math/rev/scal/fun/pow.hpp>
#include <stan/math/fwd/scal/fun/sin.hpp>
#include <stan/math/rev/scal/fun/sin.hpp>
#include <stan/math/fwd/scal/fun/tgamma.hpp>
#include <stan/math/rev/scal/fun/tgamma.hpp>
#include <stan/math/fwd/scal/fun/value_of.hpp>
#include <stan/math/rev/scal/fun/value_of.hpp>

TEST(ProbInternalMath, gradRegIncGamma_typical) {
  double a = 0.5;
  double b = 1.0;
  double g = 1.77245;
  double dig = -1.96351;
  
  EXPECT_FLOAT_EQ(0.38984156, stan::math::grad_reg_inc_gamma(a, b, g, dig));
}

TEST(ProbInternalMath, gradRegIncGamma_infLoopInVersion2_0_1) {
  double a = 8.01006;
  double b = 2.47579e+215;
  double g = 5143.28;
  double dig = 2.01698;
  
  EXPECT_THROW(stan::math::grad_reg_inc_gamma(a, b, g, dig),
               std::domain_error);
}
TEST(ProbInternalMath, gradRegIncGamma_fd) {
  using stan::agrad::fvar;

  fvar<double> a = 0.5;
  fvar<double> b = 1.0;
  fvar<double> g = 1.77245;
  fvar<double> dig = -1.96351;
  
  EXPECT_FLOAT_EQ(0.38984156, stan::math::grad_reg_inc_gamma(a, b, g, dig).val());
}
TEST(ProbInternalMath, gradRegIncGamma_ffd) {
  using stan::agrad::fvar;

  fvar<fvar<double> > a = 0.5;
  fvar<fvar<double> > b = 1.0;
  fvar<fvar<double> > g = 1.77245;
  fvar<fvar<double> > dig = -1.96351;
  
  EXPECT_FLOAT_EQ(0.38984156, stan::math::grad_reg_inc_gamma(a, b, g, dig).val_.val_);
}

TEST(ProbInternalMath, gradRegIncGamma_fv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::agrad::digamma;

  fvar<var> a = 0.5;
  fvar<var> b = 1.0;
  fvar<var> g = 1.77245;
  fvar<var> dig = digamma(a);
  
  EXPECT_FLOAT_EQ(0.38984156, stan::math::grad_reg_inc_gamma(a, b, g, dig).val_.val());
}

TEST(ProbInternalMath, gradRegIncGamma_fv_1stderiv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::agrad::digamma;
  using stan::agrad::tgamma;

  fvar<var> a = 0.5;
  a.d_ = 1.0;
  fvar<var> b = 1.0;
  fvar<var> g = tgamma(a);
  fvar<var> dig = digamma(a);
  a.d_ = 1.0;
  
  fvar<var> z = stan::math::grad_reg_inc_gamma(a, b, g, dig);

  AVEC y1 = createAVEC(a.val_);
  VEC grad1;
  z.val_.grad(y1,grad1);
  EXPECT_NEAR(0.2134999674954450667,grad1[0],1e-6);
}

TEST(ProbInternalMath, gradRegIncGamma_fv_2ndderiv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::agrad::digamma;
  using stan::agrad::tgamma;

  fvar<var> a = 0.5;
  a.d_ = 1.0;
  fvar<var> b = 1.0;
  fvar<var> g = tgamma(a);
  fvar<var> dig = digamma(a);
  a.d_ = 1.0;
  
  fvar<var> z = stan::math::grad_reg_inc_gamma(a, b, g, dig);

  AVEC y1 = createAVEC(a.val_);
  VEC grad1;
  z.d_.grad(y1,grad1);
  EXPECT_NEAR(-0.546236927878295422,grad1[0],1e-6);
}
