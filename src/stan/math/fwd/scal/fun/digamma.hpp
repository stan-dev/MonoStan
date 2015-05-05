#ifndef STAN_MATH_FWD_SCAL_FUN_DIGAMMA_HPP
#define STAN_MATH_FWD_SCAL_FUN_DIGAMMA_HPP

#include <stan/math/fwd/core.hpp>

#include <boost/math/special_functions/digamma.hpp>
#include <stan/math/prim/scal/fun/trigamma.hpp>

namespace stan {

  namespace agrad {

    template <typename T>
    inline
    fvar<T>
    digamma(const fvar<T>& x) {
      using boost::math::digamma;
      using stan::math::trigamma;
      return fvar<T>(digamma(x.val_), x.d_ * trigamma(x.val_));
    }
  }
}
#endif
