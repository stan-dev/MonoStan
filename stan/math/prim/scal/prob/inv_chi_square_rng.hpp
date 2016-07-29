#ifndef STAN_MATH_PRIM_SCAL_PROB_INV_CHI_SQUARE_RNG_HPP
#define STAN_MATH_PRIM_SCAL_PROB_INV_CHI_SQUARE_RNG_HPP

#include <stan/math/prim/scal/err/check_consistent_sizes.hpp>
#include <stan/math/prim/scal/err/check_nonnegative.hpp>
#include <stan/math/prim/scal/err/check_not_nan.hpp>
#include <stan/math/prim/scal/err/check_positive_finite.hpp>
#include <stan/math/prim/scal/fun/constants.hpp>
#include <stan/math/prim/scal/fun/multiply_log.hpp>
#include <stan/math/prim/scal/fun/value_of.hpp>
#include <stan/math/prim/scal/fun/gamma_q.hpp>
#include <stan/math/prim/scal/fun/digamma.hpp>
#include <stan/math/prim/scal/meta/length.hpp>
#include <stan/math/prim/scal/meta/VectorView.hpp>
#include <stan/math/prim/scal/meta/VectorBuilder.hpp>
#include <stan/math/prim/scal/meta/return_type.hpp>
#include <stan/math/prim/scal/fun/grad_reg_inc_gamma.hpp>
#include <stan/math/prim/scal/meta/include_summand.hpp>
#include <boost/random/chi_squared_distribution.hpp>
#include <boost/random/variate_generator.hpp>

namespace stan {
  namespace math {

    template <class RNG>
    inline double
    inv_chi_square_rng(const double nu,
                       RNG& rng) {
      using boost::variate_generator;
      using boost::random::chi_squared_distribution;

      static const char* function("stan::math::inv_chi_square_rng");


      check_positive_finite(function, "Degrees of freedom parameter", nu);

      variate_generator<RNG&, chi_squared_distribution<> >
        chi_square_rng(rng, chi_squared_distribution<>(nu));
      return 1 / chi_square_rng();
    }
  }
}
#endif
