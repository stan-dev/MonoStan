#ifndef __STAN__PROB__DISTRIBUTIONS__UNIVARIATE__DISCRETE__BETA_BINOMIAL_HPP__
#define __STAN__PROB__DISTRIBUTIONS__UNIVARIATE__DISCRETE__BETA_BINOMIAL_HPP__

#include <stan/agrad.hpp>
#include <stan/math/error_handling.hpp>
#include <stan/math/special_functions.hpp>
#include <stan/meta/traits.hpp>
#include <stan/prob/traits.hpp>
#include <stan/prob/constants.hpp>

namespace stan {
  
  namespace prob {

    // BetaBinomial(n|alpha,beta) [alpha > 0;  beta > 0;  n >= 0]
    template <bool propto,
	      typename T_n,
	      typename T_N,
              typename T_size1, 
	      typename T_size2, 
              class Policy>
    typename return_type<T_size1,T_size2>::type
    beta_binomial_log(const T_n& n, 
                      const T_N& N, 
                      const T_size1& alpha, 
                      const T_size2& beta, 
                      const Policy&) {
      static const char* function = "stan::prob::beta_binomial_log(%1%)";

      using stan::math::check_finite;
      using stan::math::check_nonnegative;
      using stan::math::check_positive;
      using stan::math::value_of;
      using stan::math::check_consistent_sizes;
      using stan::prob::include_summand;

      // check if any vectors are zero length
      if (!(stan::length(n)
	    && stan::length(N)
	    && stan::length(alpha)
	    && stan::length(beta)))
	return 0.0;
      
      typename return_type<T_size1,T_size2>::type logp(0.0);
      if (!check_nonnegative(function, N, "Population size parameter", &logp, Policy()))
        return logp;
      if (!check_finite(function, alpha, "First prior sample size parameter", &logp, 
                        Policy()))
        return logp;
      if (!check_positive(function, alpha, "First prior sample size parameter", 
                          &logp, Policy()))
        return logp;
      if (!check_finite(function, beta, "Second prior sample size parameter",
                        &logp, Policy()))
        return logp;
      if (!check_positive(function, beta, "Second prior sample size parameter", 
                          &logp, Policy()))
        return logp;
      if (!(check_consistent_sizes(function,
                                   n,N,alpha,beta,
				   "Successes variable","Population size parameter","First prior sample size parameter","Second prior sample size parameter",
                                   &logp, Policy())))
        return logp;

      // check if no variables are involved and prop-to
      if (!include_summand<propto,T_size1,T_size2>::value)
	return 0.0;

      VectorView<const T_n> n_vec(n);
      VectorView<const T_N> N_vec(N);
      VectorView<const T_size1> alpha_vec(alpha);
      VectorView<const T_size2> beta_vec(beta);
      size_t size = max_size(n, N, alpha, beta);
      
      for (size_t i = 0; i < size; i++) {
	if (n_vec[i] < 0 || n_vec[i] > N_vec[i])
	  return LOG_ZERO;
      }

      using stan::math::lbeta;
      using stan::math::binomial_coefficient_log;

      for (size_t i = 0; i < size; i++) {
	if (include_summand<propto>::value)
	  logp += binomial_coefficient_log(N_vec[i],n_vec[i]);
	if (include_summand<propto,T_size1,T_size2>::value)
	  logp += lbeta(n_vec[i] + alpha_vec[i], N_vec[i] - n_vec[i] + beta_vec[i]) 
	    - lbeta(alpha_vec[i],beta_vec[i]);
      }
      return logp;
    }

    template <bool propto,
	      typename T_n,
	      typename T_N,
              typename T_size1,
	      typename T_size2>
    typename return_type<T_size1,T_size2>::type
    beta_binomial_log(const T_n& n, const T_N& N, 
                      const T_size1& alpha, const T_size2& beta) {
      return beta_binomial_log<propto>(n,N,alpha,beta,
                                       stan::math::default_policy());
    }

    template <typename T_n,
	      typename T_N,
	      typename T_size1,
	      typename T_size2, 
              class Policy>
    typename return_type<T_size1,T_size2>::type
    inline
    beta_binomial_log(const T_n& n, const T_N& N, 
                      const T_size1& alpha, const T_size2& beta, 
                      const Policy&) {
      return beta_binomial_log<false>(n,N,alpha,beta,Policy());
    }

    template <typename T_n,
	      typename T_N,
	      typename T_size1,
	      typename T_size2>
    typename return_type<T_size1,T_size2>::type
    beta_binomial_log(const T_n& n, const T_N& N, 
                      const T_size1& alpha, const T_size2& beta) {
      return beta_binomial_log<false>(n,N,alpha,beta,
                                      stan::math::default_policy());
    }

      // Beta-Binomial CDF
      template <bool propto,
      typename T_n,
      typename T_N,
      typename T_size1, 
      typename T_size2, 
      class Policy>
      typename return_type<T_size1,T_size2>::type
      beta_binomial_cdf(const T_n& n, 
                        const T_N& N, 
                        const T_size1& alpha, 
                        const T_size2& beta, 
                        const Policy&) {
          
          
          static const char* function = "stan::prob::beta_binomial_cdf(%1%)";
          
          using stan::math::check_finite;
          using stan::math::check_nonnegative;
          using stan::math::check_positive;
          using stan::math::check_consistent_sizes;
          using stan::prob::include_summand;
          
          // Ensure non-zero argument lenghts
          if (!(stan::length(n)
                && stan::length(N)
                && stan::length(alpha)
                && stan::length(beta)))
              return 0.0;
          
          double P(1.0);
          
          // Validate arguments
          if (!check_nonnegative(function, N, "Population size parameter", &logp, Policy()))
              return logp;
          
          if (!check_finite(function, alpha, "First prior sample size parameter", &logp, 
                            Policy()))
              return logp;
          
          if (!check_positive(function, alpha, "First prior sample size parameter", 
                              &logp, Policy()))
              return logp;
          
          if (!check_finite(function, beta, "Second prior sample size parameter",
                            &logp, Policy()))
              return logp;
          
          if (!check_positive(function, beta, "Second prior sample size parameter", 
                              &logp, Policy()))
              return logp;
          
          if (!(check_consistent_sizes(function,
                                       n, N, alpha, beta,
                                       "Successes variable", "Population size parameter", "First prior sample size parameter", "Second prior sample size parameter",
                                       &logp, Policy())))
              return logp;
          
          // Return if everything is constant and only proportionality is required
          if (!include_summand<propto,T_size1,T_size2>::value)
              return 0.0;
          
          // Wrap everythign in vector views
          VectorView<const T_n> n_vec(n);
          VectorView<const T_N> N_vec(N);
          VectorView<const T_size1> alpha_vec(alpha);
          VectorView<const T_size2> beta_vec(beta);
          size_t size = max_size(n, N, alpha, beta);
          
          // Compute vectorized CDF and gradient
          using stan::math::value_of;
          
          agrad::OperandsAndPartials<T_size1, T_size2> operands_and_partials(alpha, beta);
          
          std::fill(operands_and_partials.all_partials,
                    operands_and_partials.all_partials + operands_and_partials.nvaris, 0.0);
          
          for (size_t i = 0; i < size; i++) {
              
              const double n_dbl = value_of(n_vec[i]);
              const double N_dbl = value_of(N_vec[i]);
              const double alpha_dbl = value_of(alpha_vec[i]);
              const double beta_dbl  value_of(beta_vec[i]));
              
              const double Pi = 0; // ???
              
              P *= Pi;
              
              if (!is_constant_struct<T_size1>::value)
                  operands_and_partials.d_x1[i] += 0 / Pi; // ???
              
              if (!is_constant_struct<T_size2>::value)
                  operands_and_partials.d_x2[i] += 0 / Pi; // ???
              
          }
          
          for (size_t i = 0; i < size; i++) {
              
              if (!is_constant_struct<T_prob>::value)
                  operands_and_partials.d_x1[i] *= P;
              
              if (!is_constant_struct<T_prob>::value)
                  operands_and_partials.d_x2[i] *= P;
              
          }
          
          return P;

      }
      
      template <bool propto, typename T_n, typename T_N, typename T_size1, typename T_size2>
      inline typename return_type<T_size1,T_size2>::type
      beta_binomial_cdf(const T_n& n, const T_N& N, const T_size1& alpha, const T_size2& beta) {
          return beta_binomial_cdf<propto>(n, N, alpha, beta, stan::math::default_policy());
      }
      
      template <typename T_n, typename T_N, typename T_size1, typename T_size2, class Policy>
      inline typename return_type<T_size1,T_size2>::type
      beta_binomial_cdf(const T_n& n, const T_N& N, const T_size1& alpha, const T_size2& beta, const Policy&) {
          return beta_binomial_cdf<false>(n, N, alpha, beta, Policy());
      }
      
      template <typename T_n, typename T_N, typename T_size1, typename T_size2>
      typename return_type<T_size1,T_size2>::type
      beta_binomial_cdf(const T_n& n, const T_N& N, const T_size1& alpha, const T_size2& beta) {
          return beta_binomial_cdf<false>(n, N, alpha, beta, stan::math::default_policy());
      }

      
  }
}
#endif
