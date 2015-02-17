#ifndef STAN__MATH__REV__SCAL__FUN__INC_BETA_HPP
#define STAN__MATH__REV__SCAL__FUN__INC_BETA_HPP

#include <valarray>
#include <stan/math/rev/core/var.hpp>
#include <stan/math/rev/core/vvv_vari.hpp>
#include <stan/math/rev/core/vvd_vari.hpp>
#include <stan/math/rev/core/vdv_vari.hpp>
#include <stan/math/rev/core/dvv_vari.hpp>
#include <stan/math/rev/core/vdd_vari.hpp>
#include <stan/math/rev/core/dvd_vari.hpp>
#include <stan/math/rev/core/ddv_vari.hpp>
#include <stan/math/prim/scal/fun/constants.hpp>

#include <stan/math/prim/scal/fun/grad_reg_inc_beta.hpp>

#include <stan/math/rev/scal/fun/pow.hpp>
#include <stan/math/prim/scal/fun/lbeta.hpp>
#include <stan/math/prim/scal/fun/digamma.hpp>

namespace stan {
  namespace agrad {

    namespace {

      class inc_beta_vvv_vari : public op_vvv_vari {
      public:
        inc_beta_vvv_vari(vari* avi, vari* bvi, vari* cvi) :
          op_vvv_vari(stan::math::inc_beta(avi->val_, bvi->val_, cvi->val_),
                      avi,bvi,cvi) {
        }
        void chain() {
          using stan::math::digamma;
          using stan::math::lbeta;

          double d_a; double d_b;
          stan::math::grad_reg_inc_beta(d_a,d_b,avi_->val_,bvi_->val_,
                                        cvi_->val_,digamma(avi_->val_),
                                        digamma(bvi_->val_),
                                        digamma(avi_->val_ + bvi_->val_),
                                        std::exp(lbeta(avi_->val_, bvi_->val_)));

          avi_->adj_ += adj_ * d_a;
          bvi_->adj_ += adj_ * d_b;
          cvi_->adj_ += adj_ * std::pow((1-cvi_->val_),bvi_->val_-1)
            * std::pow(cvi_->val_,avi_->val_-1)
            / std::exp(stan::math::lbeta(avi_->val_,bvi_->val_));
        }
      };

    }

    inline var inc_beta(const stan::agrad::var& a,
                        const stan::agrad::var& b,
                        const stan::agrad::var& c) {
      return var(new inc_beta_vvv_vari(a.vi_,b.vi_,c.vi_));
    }

  }
}
#endif
