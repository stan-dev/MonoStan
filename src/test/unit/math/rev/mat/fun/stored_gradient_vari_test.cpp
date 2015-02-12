#include <gtest/gtest.h>

#include <vector>
#include <stan/math/rev/arr/meta/var.hpp>
#include <stan/math/rev/arr/meta/vari.hpp>
#include <stan/math/rev/mat/fun/stored_gradient_vari.hpp>
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

TEST(StoredGradientVari, propagate3) {
  using stan::agrad::var;
  using stan::agrad::vari;
  vari** xs
    = (vari**) stan::agrad::ChainableStack::memalloc_.alloc(3 * sizeof(vari*));
  var xs1 = 1; // value not used here
  var xs2 = 4; // value not used here
  var xs3 = 9; // value not used here
  xs[0] = xs1.vi_;
  xs[1] = xs2.vi_;
  xs[2] = xs3.vi_;
  double* partials = (double*) stan::agrad::ChainableStack::memalloc_.alloc(3 * sizeof(double));
  partials[0] = 10;
  partials[1] = 100;
  partials[2] = 1000;

  var sum = var(new stan::agrad::stored_gradient_vari(-14.7, 3, xs, partials));
  EXPECT_FLOAT_EQ(-14.7, sum.val());

  std::vector<var> in(3);
  in[0] = var(xs1);
  in[1] = var(xs2);
  in[2] = var(xs3);

  var f = 132.7 * sum;

  std::vector<double> g;
  f.grad(in, g);
  EXPECT_EQ(3U, g.size());
  EXPECT_EQ(10 * 132.7, g[0]);
  EXPECT_EQ(100 * 132.7, g[1]);
  EXPECT_EQ(1000 * 132.7, g[2]);
}

TEST(StoredGradientVari, propagate0) {
  using stan::agrad::var;
  using stan::agrad::vari;
  vari** xs = 0;
  double* partials = (double*) 0;

  var sum = var(new stan::agrad::stored_gradient_vari(-14.7, 0, xs, partials));
  EXPECT_FLOAT_EQ(-14.7, sum.val());

  std::vector<var> dummy(3);
  dummy[0] = 1;
  dummy[1] = 2;
  dummy[2] = 3;

  var f = 132.7 * sum;

  std::vector<double> g;
  f.grad(dummy, g);
  EXPECT_EQ(3U, g.size());
  for (int i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(0, g[i]);
}
