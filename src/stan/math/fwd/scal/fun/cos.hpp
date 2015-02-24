#ifndef STAN__MATH__FWD__SCAL__FUN__COS_HPP
#define STAN__MATH__FWD__SCAL__FUN__COS_HPP

#include <stan/math/fwd/core.hpp>

#include <math.h>

namespace stan {

  namespace agrad {

    template <typename T>
    inline
    fvar<T>
    cos(const fvar<T>& x) {
      using ::sin;
      using ::cos;
      return fvar<T>(cos(x.val_), x.d_ * -sin(x.val_));
    }
  }
}
#endif
