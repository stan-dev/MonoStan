#ifndef STAN_MATH_REV_SCAL_FUN_ERFC_HPP
#define STAN_MATH_REV_SCAL_FUN_ERFC_HPP

#include <stan/math/rev/core.hpp>
#include <stan/math/prim/scal/fun/constants.hpp>
#include <boost/math/special_functions/erf.hpp>
#include <cmath>
#include <valarray>

namespace stan {
  namespace math {

    namespace {
      class erfc_vari : public op_v_vari {
      public:
        explicit erfc_vari(vari* avi) :
          op_v_vari(std::erfc(avi->val_), avi) {
        }
        void chain() {
          avi_->adj_ += adj_ * stan::math::NEG_TWO_OVER_SQRT_PI
            * std::exp(- avi_->val_ * avi_->val_);
        }
      };
    }

    /**
     * The complementary error function for variables (C99).
     *
     * For non-variable function, see std::erfc() from <cmath>.
     *
     * The derivative is
     *
     * \f$\frac{d}{dx} \mbox{erfc}(x) = - \frac{2}{\sqrt{\pi}} \exp(-x^2)\f$.
     *
     *
       \f[
       \mbox{erfc}(x) =
       \begin{cases}
         \operatorname{erfc}(x) & \mbox{if } -\infty\leq x \leq \infty \\[6pt]
         \textrm{NaN} & \mbox{if } x = \textrm{NaN}
       \end{cases}
       \f]

       \f[
       \frac{\partial\, \mbox{erfc}(x)}{\partial x} =
       \begin{cases}
         \frac{\partial\, \operatorname{erfc}(x)}{\partial x} & \mbox{if } -\infty\leq x\leq \infty \\[6pt]
         \textrm{NaN} & \mbox{if } x = \textrm{NaN}
       \end{cases}
       \f]

       \f[
       \operatorname{erfc}(x)=\frac{2}{\sqrt{\pi}}\int_x^\infty e^{-t^2}dt
       \f]

       \f[
       \frac{\partial \, \operatorname{erfc}(x)}{\partial x} = -\frac{2}{\sqrt{\pi}} e^{-x^2}
       \f]
     *
     * @param a The variable.
     * @return Complementary error function applied to the variable.
     */
    inline var erfc(const var& a) {
      return var(new erfc_vari(a.vi_));
    }

  }
}
#endif
