#ifndef __STAN__MATH__FUNCTIONS__PHI_APPROX_HPP__
#define __STAN__MATH__FUNCTIONS__PHI_APPROX_HPP__

#include <boost/math/tools/promotion.hpp>
#include <stan/math/functions/inv_logit.hpp>
#include <stan/agrad/fwd/functions/pow.hpp>
#include <stan/agrad/rev/functions/pow.hpp>

namespace stan {
  namespace math {

    /**
     * Approximation of the unit normal CDF.
     *
     * http://www.jiem.org/index.php/jiem/article/download/60/27
     *
     * This function can be used to implement the inverse link function
     * for probit regression.
     *
     * @param x Argument.
     * @return Probability random sample is less than or equal to argument.
     */
    template <typename T>
    inline typename boost::math::tools::promote_args<T>::type
    Phi_approx(T x) {
      using std::pow;
      using stan::agrad::pow;
      return inv_logit(0.07056 * pow(x,3.0) + 1.5976 * x);
    }

  }
}

#endif
