#include <gtest/gtest.h>
#include <stan/math/fwd/core.hpp>
#include <stan/math/rev/core.hpp>
#include <sstream>

TEST(AgradFwdFvar, insertion_operator) {
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<var> a(5.0);
  std::stringstream ss;
  ss << a;
  EXPECT_EQ("5", ss.str());
}
