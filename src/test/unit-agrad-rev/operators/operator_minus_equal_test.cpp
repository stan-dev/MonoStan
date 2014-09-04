#include <stan/agrad/rev/operators/operator_minus_equal.hpp>
#include <test/unit/agrad/util.hpp>
#include <gtest/gtest.h>
#include <test/unit-agrad-rev/nan_util.hpp>
#include <stan/meta/traits.hpp>

TEST(AgradRev,a_minuseq_b) {
  AVAR a(5.0);
  AVAR b(-1.0);
  AVEC x = createAVEC(a,b);
  AVAR f = (a -= b);
  EXPECT_FLOAT_EQ(6.0,f.val());
  EXPECT_FLOAT_EQ(6.0,a.val());
  EXPECT_FLOAT_EQ(-1.0,b.val());
  VEC g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(1.0,g[0]);
  EXPECT_FLOAT_EQ(-1.0,g[1]);
}

TEST(AgradRev,a_negeq_bd) {
  AVAR a(5.0);
  double b = -1.0;
  AVEC x = createAVEC(a);
  AVAR f = (a -= b);
  EXPECT_FLOAT_EQ(6.0,f.val());
  EXPECT_FLOAT_EQ(6.0,a.val());
  VEC g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(1.0,g[0]);
}

struct minus_eq_fun {
  template <typename T0, typename T1>
  inline 
  typename stan::return_type<T0,T1>::type
  operator()(T0 arg1,
             T1 arg2) const {
    return (arg1 -= arg2);
  }
};

TEST(AgradRev, minus_eq_nan) {
  minus_eq_fun minus_eq_;
  double nan = std::numeric_limits<double>::quiet_NaN();

  test_nan_vv(minus_eq_,3.0,nan,false, true);
  test_nan_vv(minus_eq_,nan,5.0,false, true);
  test_nan_vv(minus_eq_,nan,nan,false, true);
  test_nan_vd(minus_eq_,3.0,nan,false, true);
  test_nan_vd(minus_eq_,nan,5.0,false, true);
  test_nan_vd(minus_eq_,nan,nan,false, true);
}
