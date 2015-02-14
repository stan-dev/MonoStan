#ifndef STAN__MATH__REV__SCAL__FUN__LOG1P_HPP
#define STAN__MATH__REV__SCAL__FUN__LOG1P_HPP

#include <valarray>
#include <stan/math/rev/arr/meta/var.hpp>
#include <stan/math/rev/scal/fun/v_vari.hpp>
#include <stan/math/prim/scal/fun/constants.hpp>
#include <stan/math/prim/scal/fun/log1p.hpp>

namespace stan {
  namespace agrad {

    namespace {
      class log1p_vari : public op_v_vari {
      public:
        log1p_vari(vari* avi) :
          op_v_vari(stan::math::log1p(avi->val_),avi) {
        }
        void chain() {
          avi_->adj_ += adj_ / (1 + avi_->val_);
        }
      };
    }

    /**
     * The log (1 + x) function for variables (C99).
     *
     * The derivative is given by
     *
     * \f$\frac{d}{dx} \log (1 + x) = \frac{1}{1 + x}\f$.
     *
     * @param a The variable.
     * @return The log of 1 plus the variable.
     */
    inline var log1p(const stan::agrad::var& a) {
      return var(new log1p_vari(a.vi_));
    }

  }
}
#endif
