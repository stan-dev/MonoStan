#ifndef __STAN__AGRAD__FWD__FUNCTIONS__CBRT_HPP__
#define __STAN__AGRAD__FWD__FUNCTIONS__CBRT_HPP__

#include <stan/agrad/fwd/fvar.hpp>
#include <stan/meta/traits.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <stan/math/functions/square.hpp>

namespace stan {

  namespace agrad {

    template <typename T>
    inline
    fvar<T>
    cbrt(const fvar<T>& x) {
      using boost::math::cbrt;
      using stan::math::square;
      return fvar<T>(cbrt(x.val_),
                     x.d_ / (square(cbrt(x.val_)) * 3.0));
    }
  }
}
#endif
