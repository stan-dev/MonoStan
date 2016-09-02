#ifndef STAN_MATH_PRIM_SCAL_FUN_FDIM_HPP
#define STAN_MATH_PRIM_SCAL_FUN_FDIM_HPP

#include <stan/math/prim/scal/fun/is_nan.hpp>
#include <boost/math/tools/promotion.hpp>
#include <math.h>
#include <cmath>
#include <limits>

namespace stan {
  namespace math {

    /**
     * The positive difference function (C99).
     *
     * The function is defined by
     *
     * <code>fdim(a, b) = (a > b) ? (a - b) : 0.0</code>.
     *
     * @param a First value.
     * @param b Second value.
     * @return Returns min(a - b, 0.0).
     */
    template <typename T1, typename T2>
    inline typename boost::math::tools::promote_args<T1, T2>::type
    fdim(T1 a, T2 b) {
      using ::fdim;
      using std::numeric_limits;
      using boost::math::tools::promote_args;
      if (is_nan(a) || is_nan(b))
        return numeric_limits<typename promote_args<T1, T2>::type>::quiet_NaN();
      return fdim(a, b);
    }
  }
}

#endif
