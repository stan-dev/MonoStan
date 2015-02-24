#ifndef STAN__MATH__REV__SCAL__FUN__AS_BOOL_HPP
#define STAN__MATH__REV__SCAL__FUN__AS_BOOL_HPP

#include <stan/math/rev/core.hpp>

namespace stan {
  namespace agrad {

    /**
     * Return 1 if the argument is unequal to zero and 0 otherwise.
     *
     * @param v Value.
     * @return 1 if argument is equal to zero (or NaN) and 0 otherwise.
     */
    inline int as_bool(const agrad::var& v) {
      return 0.0 != v.vi_->val_;
    }

  }
}
#endif
