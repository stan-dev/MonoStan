#ifndef STAN__MATH__FWD__SCAL__FUN__LOG1M_EXP_HPP
#define STAN__MATH__FWD__SCAL__FUN__LOG1M_EXP_HPP

#include <stan/math/fwd/core.hpp>

#include <stan/math/prim/scal/fun/log1m_exp.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <stan/math/prim/scal/fun/constants.hpp>

namespace stan {

  namespace agrad {

    template <typename T>
    inline
    fvar<T>
    log1m_exp(const fvar<T>& x) {
      using stan::math::log1m_exp;
      using boost::math::expm1;
      using stan::math::NOT_A_NUMBER;
      using std::exp;
      if (x.val_ >= 0)
        return fvar<T>(NOT_A_NUMBER);
      return fvar<T>(log1m_exp(x.val_), x.d_ / -expm1(-x.val_));
    }
  }
}
#endif
