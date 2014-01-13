#include "stan/math/functions/as_bool.hpp"
#include <gtest/gtest.h>

TEST(MathFunctions,as_bool) {
  using stan::math::as_bool;
  EXPECT_TRUE(as_bool(1));
  EXPECT_TRUE(as_bool(1.7));
  EXPECT_TRUE(as_bool(-1.7));
  EXPECT_TRUE(as_bool(std::numeric_limits<double>::infinity()));
  EXPECT_TRUE(as_bool(-std::numeric_limits<double>::infinity()));
  // don't like this behavior, but it's what C++ does
  EXPECT_TRUE(as_bool(std::numeric_limits<double>::quiet_NaN()));

  EXPECT_FALSE(as_bool(0));
  EXPECT_FALSE(as_bool(0.0));
  EXPECT_FALSE(as_bool(0.0f));
}
