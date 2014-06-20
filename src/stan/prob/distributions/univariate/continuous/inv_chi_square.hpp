#ifndef __STAN__PROB__DIST__UNI__CONTINUOUS__INV_CHI_SQUARE_HPP__
#define __STAN__PROB__DIST__UNI__CONTINUOUS__INV_CHI_SQUARE_HPP__

#include <boost/random/chi_squared_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <stan/agrad/partials_vari.hpp>
#include <stan/math/error_handling.hpp>
#include <stan/math/constants.hpp>
#include <stan/math/functions/multiply_log.hpp>
#include <stan/math/functions/value_of.hpp>
#include <stan/meta/traits.hpp>
#include <stan/prob/constants.hpp>
#include <stan/prob/traits.hpp>

#include <stan/prob/internal_math/math/grad_reg_inc_gamma.hpp>

#include <stan/agrad/fwd/functions/gamma_q.hpp>
#include <stan/agrad/fwd/functions/exp.hpp>
#include <stan/agrad/fwd/functions/pow.hpp>
#include <stan/agrad/fwd/functions/tgamma.hpp>
#include <stan/agrad/fwd/functions/digamma.hpp>
#include <stan/agrad/rev/functions/gamma_q.hpp>
#include <stan/agrad/rev/functions/exp.hpp>
#include <stan/agrad/rev/functions/pow.hpp>
#include <stan/agrad/rev/functions/tgamma.hpp>
#include <stan/agrad/rev/functions/digamma.hpp>
#include <stan/math/functions/gamma_q.hpp>
#include <stan/math/functions/digamma.hpp>

namespace stan {

  namespace prob {

    /**
     * The log of an inverse chi-squared density for y with the specified
     * degrees of freedom parameter.
     * The degrees of freedom prarameter must be greater than 0.
     * y must be greater than 0.
     * 
     \f{eqnarray*}{
     y &\sim& \mbox{\sf{Inv-}}\chi^2_\nu \\
     \log (p (y \,|\, \nu)) &=& \log \left( \frac{2^{-\nu / 2}}{\Gamma (\nu / 2)} y^{- (\nu / 2 + 1)} \exp^{-1 / (2y)} \right) \\
     &=& - \frac{\nu}{2} \log(2) - \log (\Gamma (\nu / 2)) - (\frac{\nu}{2} + 1) \log(y) - \frac{1}{2y} \\
     & & \mathrm{ where } \; y > 0
     \f}
     * @param y A scalar variable.
     * @param nu Degrees of freedom.
     * @throw std::domain_error if nu is not greater than or equal to 0
     * @throw std::domain_error if y is not greater than or equal to 0.
     * @tparam T_y Type of scalar.
     * @tparam T_dof Type of degrees of freedom.
     */
    template <bool propto,
              typename T_y, typename T_dof>
    typename return_type<T_y,T_dof>::type
    inv_chi_square_log(const T_y& y, const T_dof& nu) {
      static const char* function = "stan::prob::inv_chi_square_log(%1%)";
      typedef typename stan::partials_return_type<T_y,T_dof>::type 
        T_partials_return;

      // check if any vectors are zero length
      if (!(stan::length(y) 
            && stan::length(nu)))
        return 0.0;

      using stan::math::check_finite;      
      using stan::math::check_positive;
      using stan::math::check_not_nan;
      using stan::math::value_of;
      using stan::math::check_consistent_sizes;

      T_partials_return logp(0.0);
      check_finite(function, nu, "Degrees of freedom parameter", &logp);
      check_positive(function, nu, "Degrees of freedom parameter", &logp);
      check_not_nan(function, y, "Random variable", &logp);
      check_consistent_sizes(function,
                             y,nu,
                             "Random variable","Degrees of freedom parameter",
                             &logp);

       
      // set up template expressions wrapping scalars into vector views
      VectorView<const T_y> y_vec(y);
      VectorView<const T_dof> nu_vec(nu);
      size_t N = max_size(y, nu);
      
      for (size_t n = 0; n < length(y); n++) 
        if (value_of(y_vec[n]) <= 0)
          return LOG_ZERO;

      using boost::math::digamma;
      using boost::math::lgamma;
      using stan::math::multiply_log;

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_y,T_dof>::value,
                    is_vector<T_y>::value> log_y(length(y));
      for (size_t i = 0; i < length(y); i++)
        if (include_summand<propto,T_y,T_dof>::value)
          log_y[i] = log(value_of(y_vec[i]));

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_y>::value,
                    is_vector<T_y>::value> inv_y(length(y));
      for (size_t i = 0; i < length(y); i++)
        if (include_summand<propto,T_y>::value)
          inv_y[i] = 1.0 / value_of(y_vec[i]);

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_dof>::value,
                    is_vector<T_dof>::value> lgamma_half_nu(length(nu));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value,
                    is_vector<T_dof>::value> 
        digamma_half_nu_over_two(length(nu));
      for (size_t i = 0; i < length(nu); i++) {
        T_partials_return half_nu = 0.5 * value_of(nu_vec[i]);
        if (include_summand<propto,T_dof>::value)
          lgamma_half_nu[i] = lgamma(half_nu);
        if (!is_constant_struct<T_dof>::value)
          digamma_half_nu_over_two[i] = digamma(half_nu) * 0.5;
      }

      agrad::OperandsAndPartials<T_y, T_dof> operands_and_partials(y, nu);
      for (size_t n = 0; n < N; n++) {
        const T_partials_return nu_dbl = value_of(nu_vec[n]);
        const T_partials_return half_nu = 0.5 * nu_dbl;
  
        if (include_summand<propto,T_dof>::value)
          logp += nu_dbl * NEG_LOG_TWO_OVER_TWO - lgamma_half_nu[n];
        if (include_summand<propto,T_y,T_dof>::value)
          logp -= (half_nu+1.0) * log_y[n];
        if (include_summand<propto,T_y>::value)
          logp -= 0.5 * inv_y[n];

        if (!is_constant_struct<T_y>::value) {
          operands_and_partials.d_x1[n] 
            += -(half_nu+1.0) * inv_y[n] + 0.5 * inv_y[n] * inv_y[n];
        }
        if (!is_constant_struct<T_dof>::value) {
          operands_and_partials.d_x2[n]
            += NEG_LOG_TWO_OVER_TWO - digamma_half_nu_over_two[n]
            - 0.5*log_y[n];
        }
      }
      return operands_and_partials.to_var(logp,y,nu);
    }

    template <typename T_y, typename T_dof>
    inline
    typename return_type<T_y,T_dof>::type
    inv_chi_square_log(const T_y& y, const T_dof& nu) {
      return inv_chi_square_log<false>(y,nu);
    }
      
    template <typename T_y, typename T_dof>
    typename return_type<T_y,T_dof>::type
    inv_chi_square_cdf(const T_y& y, const T_dof& nu) {
      typedef typename stan::partials_return_type<T_y,T_dof>::type 
        T_partials_return;

      // Size checks
      if ( !( stan::length(y) && stan::length(nu) ) ) return 1.0;
          
      // Error checks
      static const char* function = "stan::prob::inv_chi_square_cdf(%1%)";
          
      using stan::math::check_finite;      
      using stan::math::check_positive;
      using stan::math::check_not_nan;
      using stan::math::check_consistent_sizes;
      using stan::math::check_nonnegative;
      using boost::math::tools::promote_args;
      using stan::math::value_of;
          
      T_partials_return P(1.0);
          
      check_finite(function, nu, "Degrees of freedom parameter", &P);
      check_positive(function, nu, "Degrees of freedom parameter", &P);
      check_not_nan(function, y, "Random variable", &P);
      check_nonnegative(function, y, "Random variable", &P);
      check_consistent_sizes(function, y, nu,
                             "Random variable", 
                             "Degrees of freedom parameter",
                             &P);
          
      // Wrap arguments in vectors
      VectorView<const T_y> y_vec(y);
      VectorView<const T_dof> nu_vec(nu);
      size_t N = max_size(y, nu);
          
      agrad::OperandsAndPartials<T_y, T_dof> operands_and_partials(y, nu);
          
      // Explicit return for extreme values
      // The gradients are technically ill-defined, but treated as zero

      for (size_t i = 0; i < stan::length(y); i++)
        if (value_of(y_vec[i]) == 0) 
          return operands_and_partials.to_var(0.0,y,nu);
        
      // Compute CDF and its gradients
      using stan::math::gamma_q;
      using stan::math::digamma;
      using boost::math::tgamma;
      using stan::agrad::tgamma;
      using stan::agrad::gamma_q;
      using stan::agrad::exp;
      using stan::agrad::pow;
      using std::exp;
      using std::pow;
          
      // Cache a few expensive function calls if nu is a parameter
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value,
                    is_vector<T_dof>::value> gamma_vec(stan::length(nu));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value, 
                    is_vector<T_dof>::value> digamma_vec(stan::length(nu));
          
      if (!is_constant_struct<T_dof>::value)  {
        for (size_t i = 0; i < stan::length(nu); i++) {
          const T_partials_return nu_dbl = value_of(nu_vec[i]);
          gamma_vec[i] = tgamma(0.5 * nu_dbl);
          digamma_vec[i] = digamma(0.5 * nu_dbl);
        }
      }
          
      // Compute vectorized CDF and gradient
      for (size_t n = 0; n < N; n++) {
              
        // Explicit results for extreme values
        // The gradients are technically ill-defined, but treated as zero
        if (value_of(y_vec[n]) == std::numeric_limits<double>::infinity()) {
          continue;
        }

        // Pull out values
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return y_inv_dbl = 1.0 / y_dbl;
        const T_partials_return nu_dbl = value_of(nu_vec[n]);
                  
        // Compute
        const T_partials_return Pn = gamma_q(0.5 * nu_dbl, 0.5 * y_inv_dbl);
                  
        P *= Pn;
                  
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] += 0.5 * y_inv_dbl * y_inv_dbl
            * exp(-0.5*y_inv_dbl) * pow(0.5*y_inv_dbl,0.5*nu_dbl-1) 
            / tgamma(0.5*nu_dbl) / Pn;
        if (!is_constant_struct<T_dof>::value)
          operands_and_partials.d_x2[n] 
            += 0.5 * stan::math::grad_reg_inc_gamma(0.5 * nu_dbl, 
                                                    0.5 * y_inv_dbl, 
                                                    gamma_vec[n], 
                                                    digamma_vec[n]) / Pn;
      }
              
      if (!is_constant_struct<T_y>::value)
        for (size_t n = 0; n < stan::length(y); ++n) 
          operands_and_partials.d_x1[n] *= P;
      if (!is_constant_struct<T_dof>::value)
        for (size_t n = 0; n < stan::length(nu); ++n) 
          operands_and_partials.d_x2[n] *= P;
          
      return operands_and_partials.to_var(P,y,nu);
    }

    template <typename T_y, typename T_dof>
    typename return_type<T_y,T_dof>::type
    inv_chi_square_cdf_log(const T_y& y, const T_dof& nu) {
      typedef typename stan::partials_return_type<T_y,T_dof>::type
        T_partials_return;

      // Size checks
      if ( !( stan::length(y) && stan::length(nu) ) ) return 0.0;
          
      // Error checks
      static const char* function = "stan::prob::inv_chi_square_cdf_log(%1%)";
          
      using stan::math::check_finite;      
      using stan::math::check_positive;
      using stan::math::check_not_nan;
      using stan::math::check_consistent_sizes;
      using stan::math::check_nonnegative;
      using boost::math::tools::promote_args;
      using stan::math::value_of;
          
      T_partials_return P(0.0);
          
      check_finite(function, nu, "Degrees of freedom parameter", &P);
      check_positive(function, nu, "Degrees of freedom parameter", &P);
      check_not_nan(function, y, "Random variable", &P);
      check_nonnegative(function, y, "Random variable", &P);
      check_consistent_sizes(function, y, nu,
                             "Random variable", 
                             "Degrees of freedom parameter", &P);
          
      // Wrap arguments in vectors
      VectorView<const T_y> y_vec(y);
      VectorView<const T_dof> nu_vec(nu);
      size_t N = max_size(y, nu);
          
      agrad::OperandsAndPartials<T_y, T_dof> operands_and_partials(y, nu);
          
      // Explicit return for extreme values
      // The gradients are technically ill-defined, but treated as zero

      for (size_t i = 0; i < stan::length(y); i++)
        if (value_of(y_vec[i]) == 0) 
          return operands_and_partials.to_var(stan::math::negative_infinity(),
                                              y,nu);
        
      // Compute cdf_log and its gradients
      using stan::math::gamma_q;
      using stan::math::digamma;
      using boost::math::tgamma;
      using stan::agrad::tgamma;
      using stan::agrad::gamma_q;
      using stan::agrad::exp;
      using stan::agrad::pow;
      using std::exp;
      using std::pow;
          
      // Cache a few expensive function calls if nu is a parameter
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value,
                    is_vector<T_dof>::value> gamma_vec(stan::length(nu));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value, 
                    is_vector<T_dof>::value> digamma_vec(stan::length(nu));
          
      if (!is_constant_struct<T_dof>::value)  {
        for (size_t i = 0; i < stan::length(nu); i++) {
          const T_partials_return nu_dbl = value_of(nu_vec[i]);
          gamma_vec[i] = tgamma(0.5 * nu_dbl);
          digamma_vec[i] = digamma(0.5 * nu_dbl);
        }
      }
          
      // Compute vectorized cdf_log and gradient
      for (size_t n = 0; n < N; n++) {
              
        // Explicit results for extreme values
        // The gradients are technically ill-defined, but treated as zero
        if (value_of(y_vec[n]) == std::numeric_limits<double>::infinity()) {
          continue;
        }

        // Pull out values
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return y_inv_dbl = 1.0 / y_dbl;
        const T_partials_return nu_dbl = value_of(nu_vec[n]);
                  
        // Compute
        const T_partials_return Pn = gamma_q(0.5 * nu_dbl, 0.5 * y_inv_dbl);
                  
        P += log(Pn);
                  
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] += 0.5 * y_inv_dbl * y_inv_dbl
            * exp(-0.5*y_inv_dbl) * pow(0.5*y_inv_dbl,0.5*nu_dbl-1) 
            / tgamma(0.5*nu_dbl) / Pn;
        if (!is_constant_struct<T_dof>::value)
          operands_and_partials.d_x2[n] 
            += 0.5 * stan::math::grad_reg_inc_gamma(0.5 * nu_dbl, 
                                                    0.5 * y_inv_dbl, 
                                                    gamma_vec[n], 
                                                    digamma_vec[n]) / Pn;
      }
              
      return operands_and_partials.to_var(P,y,nu);
    }
      
    template <typename T_y, typename T_dof>
    typename return_type<T_y,T_dof>::type
    inv_chi_square_ccdf_log(const T_y& y, const T_dof& nu) {
      typedef typename stan::partials_return_type<T_y,T_dof>::type
        T_partials_return;

      // Size checks
      if ( !( stan::length(y) && stan::length(nu) ) ) return 0.0;
          
      // Error checks
      static const char* function = "stan::prob::inv_chi_square_ccdf_log(%1%)";
          
      using stan::math::check_finite;      
      using stan::math::check_positive;
      using stan::math::check_not_nan;
      using stan::math::check_consistent_sizes;
      using stan::math::check_nonnegative;
      using boost::math::tools::promote_args;
      using stan::math::value_of;
          
      T_partials_return P(0.0);
          
      check_finite(function, nu, "Degrees of freedom parameter", &P);
      check_positive(function, nu, "Degrees of freedom parameter", &P);
      check_not_nan(function, y, "Random variable", &P);
      check_nonnegative(function, y, "Random variable", &P);
      check_consistent_sizes(function, y, nu,
                             "Random variable", 
                             "Degrees of freedom parameter", &P);
          
      // Wrap arguments in vectors
      VectorView<const T_y> y_vec(y);
      VectorView<const T_dof> nu_vec(nu);
      size_t N = max_size(y, nu);
          
      agrad::OperandsAndPartials<T_y, T_dof> operands_and_partials(y, nu);
          
      // Explicit return for extreme values
      // The gradients are technically ill-defined, but treated as zero

      for (size_t i = 0; i < stan::length(y); i++)
        if (value_of(y_vec[i]) == 0) 
          return operands_and_partials.to_var(0.0,y,nu);
        
      // Compute ccdf_log and its gradients
      using stan::math::gamma_q;
      using stan::math::digamma;
      using boost::math::tgamma;
      using stan::agrad::tgamma;
      using stan::agrad::gamma_q;
      using stan::agrad::exp;
      using stan::agrad::pow;
      using std::exp;
      using std::pow;

      // Cache a few expensive function calls if nu is a parameter
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value,
                    is_vector<T_dof>::value> gamma_vec(stan::length(nu));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_dof>::value, 
                    is_vector<T_dof>::value> digamma_vec(stan::length(nu));
          
      if (!is_constant_struct<T_dof>::value)  {
        for (size_t i = 0; i < stan::length(nu); i++) {
          const T_partials_return nu_dbl = value_of(nu_vec[i]);
          gamma_vec[i] = tgamma(0.5 * nu_dbl);
          digamma_vec[i] = digamma(0.5 * nu_dbl);
        }
      }
          
      // Compute vectorized ccdf_log and gradient
      for (size_t n = 0; n < N; n++) {
              
        // Explicit results for extreme values
        // The gradients are technically ill-defined, but treated as zero
        if (value_of(y_vec[n]) == std::numeric_limits<double>::infinity()) {
          return operands_and_partials.to_var(stan::math::negative_infinity(),
                                              y,nu);
        }

        // Pull out values
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return y_inv_dbl = 1.0 / y_dbl;
        const T_partials_return nu_dbl = value_of(nu_vec[n]);
                  
        // Compute
        const T_partials_return Pn = 1.0 - gamma_q(0.5 * nu_dbl, 0.5 
                                                   * y_inv_dbl);
                  
        P += log(Pn);
                  
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] -= 0.5 * y_inv_dbl * y_inv_dbl
            * exp(-0.5*y_inv_dbl) * pow(0.5*y_inv_dbl,0.5*nu_dbl-1) 
            / tgamma(0.5*nu_dbl) / Pn;      
        if (!is_constant_struct<T_dof>::value)
          operands_and_partials.d_x2[n] 
            -= 0.5 * stan::math::grad_reg_inc_gamma(0.5 * nu_dbl, 
                                                    0.5 * y_inv_dbl, 
                                                    gamma_vec[n], 
                                                    digamma_vec[n]) / Pn;
      }
              
      return operands_and_partials.to_var(P,y,nu);
    }

    template <class RNG>
    inline double
    inv_chi_square_rng(const double nu,
                       RNG& rng) {
      using boost::variate_generator;
      using boost::random::chi_squared_distribution;

      static const char* function = "stan::prob::inv_chi_square_rng(%1%)";

      using stan::math::check_finite;      
      using stan::math::check_positive;

      check_finite(function, nu, "Degrees of freedom parameter", (double*)0);
      check_positive(function, nu, "Degrees of freedom parameter", (double*)0);

      variate_generator<RNG&, chi_squared_distribution<> >
        chi_square_rng(rng, chi_squared_distribution<>(nu));
      return 1 / chi_square_rng();
    }
    
  }
}

#endif

