#ifndef __STAN__AGRAD__REV__FUNCTIONS__SINH_HPP__
#define __STAN__AGRAD__REV__FUNCTIONS__SINH_HPP__

#include <valarray>
#include <stan/agrad/rev/var.hpp>
#include <stan/agrad/rev/internal/v_vari.hpp>

namespace stan {
  namespace agrad {
    
    namespace {
      class sinh_vari : public op_v_vari {
      public:
        sinh_vari(vari* avi) :
          op_v_vari(std::sinh(avi->val_),avi) {
        }
        void chain() {
          avi_->adj_ += adj_ * std::cosh(avi_->val_);
        }
      };
    }

    /**
     * Return the hyperbolic sine of the specified variable (cmath).
     *
     * The derivative is defined by
     *
     * \f$\frac{d}{dx} \sinh x = \cosh x\f$.
     *
     * @param a Variable.
     * @return Hyperbolic sine of variable.
     */
    inline var sinh(const var& a) {
      return var(new sinh_vari(a.vi_));
    }
    
  }
}
#endif
