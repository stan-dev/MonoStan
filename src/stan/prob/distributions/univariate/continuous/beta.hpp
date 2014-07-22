#ifndef STAN__PROB__DISTRIBUTIONS__UNIVARIATE__CONTINUOUS__BETA_HPP
#define STAN__PROB__DISTRIBUTIONS__UNIVARIATE__CONTINUOUS__BETA_HPP

#include <boost/math/special_functions/gamma.hpp>
#include <boost/random/gamma_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <stan/agrad/partials_vari.hpp>
#include <stan/math/error_handling.hpp>
#include <stan/math/functions/log1m.hpp>
#include <stan/math/functions/multiply_log.hpp>
#include <stan/math/functions/value_of.hpp>
#include <stan/math/functions/digamma.hpp>
#include <stan/math/functions/lgamma.hpp>
#include <stan/math/functions/lbeta.hpp>
#include <stan/meta/traits.hpp>
#include <stan/prob/constants.hpp>
#include <stan/prob/traits.hpp>

#include <stan/prob/internal_math/math/grad_reg_inc_beta.hpp>
#include <stan/prob/internal_math/fwd/inc_beta.hpp>
#include <stan/prob/internal_math/rev/inc_beta.hpp>
#include <stan/prob/internal_math/math/inc_beta.hpp>

namespace stan {

  namespace prob {

    /**
     * The log of the beta density for the specified scalar(s) given the specified
     * sample size(s). y, alpha, or beta can each either be scalar or std::vector.
     * Any vector inputs must be the same length.
     *
     * <p> The result log probability is defined to be the sum of
     * the log probabilities for each observation/alpha/beta triple.
     *
     * Prior sample sizes, alpha and beta, must be greater than 0.
     * 
     * @param y (Sequence of) scalar(s).
     * @param alpha (Sequence of) prior sample size(s).
     * @param beta (Sequence of) prior sample size(s).
     * @return The log of the product of densities.
     * @tparam T_y Type of scalar outcome.
     * @tparam T_scale_succ Type of prior scale for successes.
     * @tparam T_scale_fail Type of prior scale for failures.
     * @error_policy
     *    @li alpha must be positive and finite.
     *    @li beta must be positive and finite.
     */
    template <bool propto,
              typename T_y, typename T_scale_succ, typename T_scale_fail>
    typename return_type<T_y,T_scale_succ,T_scale_fail>::type
    beta_log(const T_y& y, 
             const T_scale_succ& alpha, const T_scale_fail& beta) {
      static const char* function = "stan::prob::beta_log(%1%)";
      typedef typename stan::partials_return_type<T_y,
                                                  T_scale_succ,
                                                  T_scale_fail>::type 
        T_partials_return;

      using stan::math::digamma;
      using stan::math::lgamma;

      using stan::is_constant_struct;
      using stan::is_vector;
      using stan::math::check_positive;
      using stan::math::check_finite;
      using stan::math::check_not_nan;
      using stan::math::check_consistent_sizes;
      using stan::prob::include_summand;
      using stan::math::log1m;
      using stan::math::multiply_log;
      using stan::math::value_of;
      using stan::math::check_nonnegative;
      using stan::math::check_less_or_equal;

      // check if any vectors are zero length
      if (!(stan::length(y) 
            && stan::length(alpha) 
            && stan::length(beta)))
        return 0.0;

      // set up return value accumulator
      T_partials_return logp(0.0);
      
      // validate args (here done over var, which should be OK)
      check_finite(function, alpha, "First shape parameter", &logp);
      check_positive(function, alpha, "First shape parameter", &logp);
      check_finite(function, beta, "Second shape parameter", &logp);
      check_positive(function, beta, "Second shape parameter", &logp);
      check_not_nan(function, y, "Random variable", &logp);
      check_consistent_sizes(function,
                             y,alpha,beta,
                             "Random variable","First shape parameter",
                             "Second shape parameter",
                             &logp);
      check_nonnegative(function, y, "Random variable", &logp);
      check_less_or_equal(function, y, 1,"Random variable", &logp);

      // check if no variables are involved and prop-to
      if (!include_summand<propto,T_y,T_scale_succ,T_scale_fail>::value)
        return 0.0;

      VectorView<const T_y> y_vec(y);
      VectorView<const T_scale_succ> alpha_vec(alpha);
      VectorView<const T_scale_fail> beta_vec(beta);
      size_t N = max_size(y, alpha, beta);

      for (size_t n = 0; n < N; n++) {
        const T_partials_return y_dbl = value_of(y_vec[n]);
        if (y_dbl < 0 || y_dbl > 1)
          return LOG_ZERO;
      }

      // set up template expressions wrapping scalars into vector views
      agrad::OperandsAndPartials<T_y, T_scale_succ, T_scale_fail>
        operands_and_partials(y, alpha, beta);

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_y,T_scale_succ>::value,
                    is_vector<T_y>::value> log_y(length(y));
      VectorBuilder<T_partials_return,
                    include_summand<propto,T_y,T_scale_fail>::value,
                    is_vector<T_y>::value> log1m_y(length(y));
      
      for (size_t n = 0; n < length(y); n++) {
        if (include_summand<propto,T_y,T_scale_succ>::value)
          log_y[n] = log(value_of(y_vec[n]));
        if (include_summand<propto,T_y,T_scale_fail>::value)
          log1m_y[n] = log1m(value_of(y_vec[n]));
      }

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_scale_succ>::value,
                    is_vector<T_scale_succ>::value> 
        lgamma_alpha(length(alpha));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value,
                    is_vector<T_scale_succ>::value> 
        digamma_alpha(length(alpha));
      for (size_t n = 0; n < length(alpha); n++) {
        if (include_summand<propto,T_scale_succ>::value) 
          lgamma_alpha[n] = lgamma(value_of(alpha_vec[n]));
        if (!is_constant_struct<T_scale_succ>::value)
          digamma_alpha[n] = digamma(value_of(alpha_vec[n]));
      }

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_scale_fail>::value,
                    is_vector<T_scale_fail>::value> 
        lgamma_beta(length(beta));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_fail>::value> 
        digamma_beta(length(beta));

      for (size_t n = 0; n < length(beta); n++) {
        if (include_summand<propto,T_scale_fail>::value) 
          lgamma_beta[n] = lgamma(value_of(beta_vec[n]));
        if (!is_constant_struct<T_scale_fail>::value)
          digamma_beta[n] = digamma(value_of(beta_vec[n]));
      }

      VectorBuilder<T_partials_return,
                    include_summand<propto,T_scale_succ,T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        lgamma_alpha_beta(max_size(alpha,beta));
    
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_alpha_beta(max_size(alpha,beta));
  
      for (size_t n = 0; n < max_size(alpha,beta); n++) {
        const T_partials_return alpha_beta = value_of(alpha_vec[n]) 
          + value_of(beta_vec[n]);
        if (include_summand<propto,T_scale_succ,T_scale_fail>::value)
          lgamma_alpha_beta[n] = lgamma(alpha_beta);
        if (!is_constant_struct<T_scale_succ>::value
            || !is_constant_struct<T_scale_fail>::value)
          digamma_alpha_beta[n] = digamma(alpha_beta);
      }

      for (size_t n = 0; n < N; n++) {
        // pull out values of arguments
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return alpha_dbl = value_of(alpha_vec[n]);
        const T_partials_return beta_dbl = value_of(beta_vec[n]);

        // log probability
        if (include_summand<propto,T_scale_succ,T_scale_fail>::value)
          logp += lgamma_alpha_beta[n];
        if (include_summand<propto,T_scale_succ>::value)
          logp -= lgamma_alpha[n];
        if (include_summand<propto,T_scale_fail>::value)
          logp -= lgamma_beta[n];
        if (include_summand<propto,T_y,T_scale_succ>::value)
          logp += (alpha_dbl-1.0) * log_y[n];
        if (include_summand<propto,T_y,T_scale_fail>::value)
          logp += (beta_dbl-1.0) * log1m_y[n];

        // gradients
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] += (alpha_dbl-1)/y_dbl 
            + (beta_dbl-1)/(y_dbl-1);
        if (!is_constant_struct<T_scale_succ>::value)
          operands_and_partials.d_x2[n]
            += log_y[n] + digamma_alpha_beta[n] - digamma_alpha[n];
        if (!is_constant_struct<T_scale_fail>::value)
          operands_and_partials.d_x3[n] 
            += log1m_y[n] + digamma_alpha_beta[n] - digamma_beta[n];
      }
      return operands_and_partials.to_var(logp,y,alpha,beta);
    }

    template <typename T_y, typename T_scale_succ, typename T_scale_fail>
    inline typename return_type<T_y,T_scale_succ,T_scale_fail>::type
    beta_log(const T_y& y, const T_scale_succ& alpha, 
             const T_scale_fail& beta) {
      return beta_log<false>(y,alpha,beta);
    }
    
    /**
     * Calculates the beta cumulative distribution function for the given
     * variate and scale variables.
     * 
     * @param y A scalar variate.
     * @param alpha Prior sample size.
     * @param beta Prior sample size.
     * @return The beta cdf evaluated at the specified arguments.
     * @tparam T_y Type of y.
     * @tparam T_scale_succ Type of alpha.
     * @tparam T_scale_fail Type of beta.
     */
    template <typename T_y, typename T_scale_succ, typename T_scale_fail>
    typename return_type<T_y,T_scale_succ,T_scale_fail>::type
    beta_cdf(const T_y& y, const T_scale_succ& alpha, 
             const T_scale_fail& beta) {
      typedef typename stan::partials_return_type<T_y,T_scale_succ,
                                                  T_scale_fail>::type 
        T_partials_return;


      // Size checks
      if ( !( stan::length(y) && stan::length(alpha) 
              && stan::length(beta) ) ) 
        return 1.0;
      
      // Error checks
      static const char* function = "stan::prob::beta_cdf(%1%)";

      using stan::math::check_positive;
      using stan::math::check_finite;
      using stan::math::check_not_nan;
      using boost::math::tools::promote_args;
      using stan::math::check_consistent_sizes;
      using stan::math::value_of;
      using stan::math::check_nonnegative;
      using stan::math::check_less_or_equal;
      
      T_partials_return P(1.0);
        
      check_finite(function, alpha, "First shape parameter", &P);
      check_positive(function, alpha, "First shape parameter", &P);
      check_finite(function, beta, "Second shape parameter", &P);
      check_positive(function, beta, "Second shape parameter", &P);
      check_not_nan(function, y, "Random variable", &P);
      check_consistent_sizes(function, y, alpha, beta,
                             "Random variable", "Shape parameter", 
                             "Scale Parameter", &P);
      check_nonnegative(function, y, "Random variable", &P);
      check_less_or_equal(function, y, 1,"Random variable", &P);

      // Wrap arguments in vectors
      VectorView<const T_y> y_vec(y);
      VectorView<const T_scale_succ> alpha_vec(alpha);
      VectorView<const T_scale_fail> beta_vec(beta);
      size_t N = max_size(y, alpha, beta);

      agrad::OperandsAndPartials<T_y, T_scale_succ, T_scale_fail> 
        operands_and_partials(y, alpha, beta);

      // Explicit return for extreme values
      // The gradients are technically ill-defined, but treated as zero
      for (size_t i = 0; i < stan::length(y); i++) {
        if (value_of(y_vec[i]) <= 0) 
          return operands_and_partials.to_var(0.0,y,alpha,beta);
      }
      
      // Compute CDF and its gradients
      using stan::math::inc_beta;
      using stan::math::digamma;
      using stan::math::lbeta;
      using std::pow;
      using std::exp;
      
      // Cache a few expensive function calls if alpha or beta is a parameter
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_alpha_vec(max_size(alpha, beta));
        
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_beta_vec(max_size(alpha, beta));
        
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_sum_vec(max_size(alpha, beta));
        
      if (!is_constant_struct<T_scale_succ>::value 
          || !is_constant_struct<T_scale_fail>::value) {
            
        for (size_t i = 0; i < N; i++) {

          const T_partials_return alpha_dbl = value_of(alpha_vec[i]);
          const T_partials_return beta_dbl = value_of(beta_vec[i]);
                
          digamma_alpha_vec[i] = digamma(alpha_dbl);
          digamma_beta_vec[i] = digamma(beta_dbl);
          digamma_sum_vec[i] = digamma(alpha_dbl + beta_dbl);
        }
            
      }
        
      // Compute vectorized CDF and gradient
      for (size_t n = 0; n < N; n++) {
            
        // Explicit results for extreme values
        // The gradients are technically ill-defined, but treated as zero
        if (value_of(y_vec[n]) >= 1.0) continue;
              
        // Pull out values
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return alpha_dbl = value_of(alpha_vec[n]);
        const T_partials_return beta_dbl = value_of(beta_vec[n]);
        const T_partials_return betafunc_dbl = exp(lbeta(alpha_dbl,beta_dbl));
                  
        // Compute
        const T_partials_return Pn = inc_beta(alpha_dbl, beta_dbl, y_dbl)
          / betafunc_dbl;

        P *= Pn;
                  
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] += pow(1-y_dbl,beta_dbl-1)
            * pow(y_dbl,alpha_dbl-1) / betafunc_dbl / Pn;

        T_partials_return g1 = 0;
        T_partials_return g2 = 0;
              
        if (!is_constant_struct<T_scale_succ>::value
            || !is_constant_struct<T_scale_fail>::value) {
          stan::math::grad_reg_inc_beta(g1, g2, alpha_dbl, beta_dbl, y_dbl, 
                                        digamma_alpha_vec[n], 
                                        digamma_beta_vec[n], digamma_sum_vec[n],
                                        betafunc_dbl);
        }

        if (!is_constant_struct<T_scale_succ>::value)
          operands_and_partials.d_x2[n] += g1 / Pn;
        if (!is_constant_struct<T_scale_fail>::value)
          operands_and_partials.d_x3[n] += g2 / Pn;
      }
            
      if (!is_constant_struct<T_y>::value) {
        for(size_t n = 0; n < stan::length(y); ++n) 
          operands_and_partials.d_x1[n] *= P;
      }
      if (!is_constant_struct<T_scale_succ>::value) {
        for(size_t n = 0; n < stan::length(alpha); ++n) 
          operands_and_partials.d_x2[n] *= P;
      }
      if (!is_constant_struct<T_scale_fail>::value) {
        for(size_t n = 0; n < stan::length(beta); ++n) 
          operands_and_partials.d_x3[n] *= P;
      }
        
      return operands_and_partials.to_var(P,y,alpha,beta);
    }

    template <typename T_y, typename T_scale_succ, typename T_scale_fail>
    typename return_type<T_y,T_scale_succ,T_scale_fail>::type
    beta_cdf_log(const T_y& y, const T_scale_succ& alpha, 
                 const T_scale_fail& beta) {
      typedef typename stan::partials_return_type<T_y,T_scale_succ,
                                                  T_scale_fail>::type
        T_partials_return;

      // Size checks
      if ( !( stan::length(y) && stan::length(alpha) 
              && stan::length(beta) ) ) 
        return 0.0;
      
      // Error checks
      static const char* function = "stan::prob::beta_cdf(%1%)";

      using stan::math::check_positive;
      using stan::math::check_finite;
      using stan::math::check_not_nan;
      using stan::math::check_nonnegative;
      using stan::math::check_less_or_equal;
      using boost::math::tools::promote_args;
      using stan::math::check_consistent_sizes;
      using stan::math::value_of;
      
      T_partials_return cdf_log(0.0);
        
      check_finite(function, alpha, "First shape parameter", &cdf_log);
      check_positive(function, alpha, "First shape parameter", &cdf_log);
      check_finite(function, beta, "Second shape parameter", &cdf_log);
      check_positive(function, beta, "Second shape parameter", &cdf_log);
      check_not_nan(function, y, "Random variable", &cdf_log);
      check_nonnegative(function, y, "Random variable", &cdf_log);
      check_less_or_equal(function, y, 1,"Random variable", &cdf_log);
      check_consistent_sizes(function, y, alpha, beta,
                             "Random variable", "Shape parameter", 
                             "Scale Parameter", &cdf_log);
      
      // Wrap arguments in vectors
      VectorView<const T_y> y_vec(y);
      VectorView<const T_scale_succ> alpha_vec(alpha);
      VectorView<const T_scale_fail> beta_vec(beta);
      size_t N = max_size(y, alpha, beta);

      agrad::OperandsAndPartials<T_y, T_scale_succ, T_scale_fail> 
        operands_and_partials(y, alpha, beta);

      // Compute CDF and its gradients
      using stan::math::inc_beta;
      using stan::math::digamma;
      using stan::math::lbeta;
      using std::pow;
      using std::exp;
        
      // Cache a few expensive function calls if alpha or beta is a parameter
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_alpha_vec(max_size(alpha, beta));
        
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_beta_vec(max_size(alpha, beta));
        
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value
                    || is_vector<T_scale_fail>::value>
        digamma_sum_vec(max_size(alpha, beta));
        
      if (!is_constant_struct<T_scale_succ>::value 
          || !is_constant_struct<T_scale_fail>::value) {
            
        for (size_t i = 0; i < N; i++) {

          const T_partials_return alpha_dbl = value_of(alpha_vec[i]);
          const T_partials_return beta_dbl = value_of(beta_vec[i]);
                
          digamma_alpha_vec[i] = digamma(alpha_dbl);
          digamma_beta_vec[i] = digamma(beta_dbl);
          digamma_sum_vec[i] = digamma(alpha_dbl + beta_dbl);
        }
      }
        
      // Compute vectorized CDFLog and gradient
      for (size_t n = 0; n < N; n++) {
              
        // Pull out values
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return alpha_dbl = value_of(alpha_vec[n]);
        const T_partials_return beta_dbl = value_of(beta_vec[n]);
        const T_partials_return betafunc_dbl = exp(lbeta(alpha_dbl,beta_dbl));
        // Compute
        const T_partials_return Pn = inc_beta(alpha_dbl, beta_dbl, y_dbl) 
          / betafunc_dbl;

        cdf_log += log(Pn);
                  
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] += pow(1-y_dbl,beta_dbl-1)
            * pow(y_dbl,alpha_dbl-1) / betafunc_dbl / Pn;

        T_partials_return g1 = 0;
        T_partials_return g2 = 0;
              
        if (!is_constant_struct<T_scale_succ>::value
            || !is_constant_struct<T_scale_fail>::value) {
          stan::math::grad_reg_inc_beta(g1, g2, alpha_dbl, beta_dbl, y_dbl, 
                                        digamma_alpha_vec[n], 
                                        digamma_beta_vec[n], digamma_sum_vec[n],
                                        betafunc_dbl);
        }
        if (!is_constant_struct<T_scale_succ>::value)
          operands_and_partials.d_x2[n] += g1 / Pn;
        if (!is_constant_struct<T_scale_fail>::value)
          operands_and_partials.d_x3[n]  += g2 / Pn;
      }
        
      return operands_and_partials.to_var(cdf_log,y,alpha,beta);
    }

    template <typename T_y, typename T_scale_succ, typename T_scale_fail>
    typename return_type<T_y,T_scale_succ,T_scale_fail>::type
    beta_ccdf_log(const T_y& y, const T_scale_succ& alpha, 
                  const T_scale_fail& beta) {
      typedef typename stan::partials_return_type<T_y,T_scale_succ,
                                                  T_scale_fail>::type
        T_partials_return;

      // Size checks
      if ( !( stan::length(y) && stan::length(alpha) 
              && stan::length(beta) ) ) 
        return 0.0;
      
      // Error checks
      static const char* function = "stan::prob::beta_cdf(%1%)";

      using stan::math::check_positive;
      using stan::math::check_finite;
      using stan::math::check_not_nan;
      using stan::math::check_nonnegative;
      using stan::math::check_less_or_equal;
      using boost::math::tools::promote_args;
      using stan::math::check_consistent_sizes;
      using stan::math::value_of;
      
      T_partials_return ccdf_log(0.0);
        
      check_finite(function, alpha, "First shape parameter", &ccdf_log);
      check_positive(function, alpha, "First shape parameter", &ccdf_log);
      check_finite(function, beta, "Second shape parameter", &ccdf_log);
      check_positive(function, beta, "Second shape parameter", &ccdf_log);
      check_not_nan(function, y, "Random variable", &ccdf_log);
      check_nonnegative(function, y, "Random variable", &ccdf_log);
      check_less_or_equal(function, y, 1,"Random variable", &ccdf_log);
      check_consistent_sizes(function, y, alpha, beta,
                             "Random variable", "Shape parameter", 
                             "Scale Parameter", &ccdf_log);
      
      // Wrap arguments in vectors
      VectorView<const T_y> y_vec(y);
      VectorView<const T_scale_succ> alpha_vec(alpha);
      VectorView<const T_scale_fail> beta_vec(beta);
      size_t N = max_size(y, alpha, beta);

      agrad::OperandsAndPartials<T_y, T_scale_succ, T_scale_fail> 
        operands_and_partials(y, alpha, beta);

      // Compute CDF and its gradients
      using stan::math::inc_beta;
      using stan::math::digamma;
      using stan::math::lbeta;
      using std::pow;
      using std::exp;
        
      // Cache a few expensive function calls if alpha or beta is a parameter
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_alpha_vec(max_size(alpha, beta));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value 
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_beta_vec(max_size(alpha, beta));
      VectorBuilder<T_partials_return,
                    !is_constant_struct<T_scale_succ>::value
                    || !is_constant_struct<T_scale_fail>::value,
                    is_vector<T_scale_succ>::value 
                    || is_vector<T_scale_fail>::value>
        digamma_sum_vec(max_size(alpha, beta));        
        
      if (!is_constant_struct<T_scale_succ>::value 
          || !is_constant_struct<T_scale_fail>::value) {
            
        for (size_t i = 0; i < N; i++) {

          const T_partials_return alpha_dbl = value_of(alpha_vec[i]);
          const T_partials_return beta_dbl = value_of(beta_vec[i]);
                
          digamma_alpha_vec[i] = digamma(alpha_dbl);
          digamma_beta_vec[i] = digamma(beta_dbl);
          digamma_sum_vec[i] = digamma(alpha_dbl + beta_dbl);
        }
      }
        
      // Compute vectorized CDFLog and gradient
      for (size_t n = 0; n < N; n++) {
              
        // Pull out values
        const T_partials_return y_dbl = value_of(y_vec[n]);
        const T_partials_return alpha_dbl = value_of(alpha_vec[n]);
        const T_partials_return beta_dbl = value_of(beta_vec[n]);
        const T_partials_return betafunc_dbl = exp(lbeta(alpha_dbl,beta_dbl));

        // Compute
        const T_partials_return Pn = 1.0 - inc_beta(alpha_dbl, beta_dbl, y_dbl) 
          / betafunc_dbl;

        ccdf_log += log(Pn);
                  
        if (!is_constant_struct<T_y>::value)
          operands_and_partials.d_x1[n] -= pow(1-y_dbl,beta_dbl-1)
            * pow(y_dbl,alpha_dbl-1) / betafunc_dbl / Pn;

        T_partials_return g1 = 0;
        T_partials_return g2 = 0;
              
        if (!is_constant_struct<T_scale_succ>::value
            || !is_constant_struct<T_scale_fail>::value) {
          stan::math::grad_reg_inc_beta(g1, g2, alpha_dbl, beta_dbl, y_dbl, 
                                        digamma_alpha_vec[n], 
                                        digamma_beta_vec[n], 
                                        digamma_sum_vec[n], 
                                        betafunc_dbl);
        }
        if (!is_constant_struct<T_scale_succ>::value)
          operands_and_partials.d_x2[n] -= g1 / Pn;
        if (!is_constant_struct<T_scale_fail>::value)
          operands_and_partials.d_x3[n] -= g2 / Pn;
      }
        
      return operands_and_partials.to_var(ccdf_log,y,alpha,beta);
    }

    template <class RNG>
    inline double
    beta_rng(const double alpha,
             const double beta,
             RNG& rng) {
      using boost::variate_generator;
      using boost::random::gamma_distribution;
      // Error checks
      static const char* function = "stan::prob::beta_rng(%1%)";

      using stan::math::check_positive;
      using stan::math::check_finite;
        
      check_finite(function, alpha, "First shape parameter", (double*)0);
      check_positive(function, alpha, "First shape parameter", (double*)0);
      check_finite(function, beta, "Second shape parameter", (double*)0);
      check_positive(function, beta, "Second shape parameter", (double*)0);

      variate_generator<RNG&, gamma_distribution<> >
        rng_gamma_alpha(rng, gamma_distribution<>(alpha, 1.0));
      variate_generator<RNG&, gamma_distribution<> >
        rng_gamma_beta(rng, gamma_distribution<>(beta, 1.0));
      double a = rng_gamma_alpha();
      double b = rng_gamma_beta();
      return a / (a + b);
    }

  }
}
#endif
