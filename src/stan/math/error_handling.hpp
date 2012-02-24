#ifndef __STAN__MATH__ERROR_HANDLING_HPP__
#define __STAN__MATH__ERROR_HANDLING_HPP__

#include <stan/math/boost_error_handling.hpp>
#include <stan/math/special_functions.hpp>

#include <boost/math/policies/policy.hpp>
#include <cstddef>
#include <limits>


namespace stan { 

  namespace math {
    /**
     * This is the tolerance for checking arithmetic bounds
     * in rank and in simplexes.  The current value is <code>1E-8</code>.
     */
    const double CONSTRAINT_TOLERANCE = 1E-8;


    /**
     * Default error-handling policy from Boost.
     */
    typedef boost::math::policies::policy<> default_policy;
    

    /**
     * Checks if the variable y is nan.
     *
     * @param function Name of function being invoked.
     * @param y Reference to variable being tested.
     * @param name Name of variable being tested.
     * @param result Pointer to resulting value after test.
     * @tparam T_y Type of variable being tested.
     * @tparam T_result Type of result returned.
     * @tparam Policy Error handling policy.
     */
    template <typename T_y, typename T_result, class Policy = default_policy>
    inline bool check_not_nan(const char* function,
                              const T_y& y,
                              const char* name,
                              T_result* result,
                              const Policy& = Policy()) {
      if ((boost::math::isnan)(y)) {
        using stan::math::policies::raise_domain_error;
        std::string msg_str(name);
        msg_str += " is %1%, but must not be nan!";
        *result = raise_domain_error<T_result,T_y>(function,
                                                   msg_str.c_str(),
                                                   y,
                                                   Policy());
        return false;
      }
      return true;
    }

    template <typename T_y, typename T_result, class Policy = default_policy>
    inline bool check_not_nan(const char* function,
                              const std::vector<T_y>& y,
                              const char* name,
                              T_result* result,
                              const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      for (size_t i = 0; i < y.size(); i++) {
        if ((boost::math::isnan)(y[i])) {
          std::ostringstream msg_o;
          msg_o << name << "[" << i << "] is %1%, but must not be nan!";
          *result = raise_domain_error<T_result,T_y>(function,
                                                     msg_o.str().c_str(),
                                                     y[i],
                                                     Policy());
          return false;
        }
      }
      return true;
    }

    /**
     * Checks if the variable y is finite.
     */
    template <typename T_y, typename T_result, class Policy = default_policy>
    inline bool check_finite(const char* function,
                             const T_y& y,
                             const char* name,
                             T_result* result,
                             const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      if (!(boost::math::isfinite)(y)) {
        std::string message(name);
        message += " is %1%, but must be finite!";
        *result = raise_domain_error<T_result,T_y>(function,
                                                   message.c_str(), 
                                                   y, Policy());
        return false;
      }
      return true;
    }

    template <typename T_y, typename T_result, class Policy = default_policy>
    inline bool check_finite(const char* function,
                             const std::vector<T_y>& y,
                             const char* name,
                             T_result* result,
                             const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      for (size_t i = 0; i < y.size(); i++) {
        if (!(boost::math::isfinite)(y[i])) {
          std::ostringstream message;
          message << name << "[" << i << "] is %1%, but must be finite!";
          *result = raise_domain_error<T_result,T_y>(function,
                                            message.str().c_str(),
                                            y[i], Policy());
          return false;
        }
      }
      return true;
    }


    template <typename T_x, typename T_low, typename T_result, class Policy = default_policy>
    inline bool check_greater(const char* function,
                              const T_x& x,
                              const T_low& low,
                              const char* name,  
                              T_result* result,
                              const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      using boost::math::tools::promote_args;
      if (!(x > low)) {
        std::ostringstream msg;
        msg << name 
            << " is %1%, but must be greater than "
            << low;
        *result = raise_domain_error<T_result,T_x>(function, 
                                                   msg.str().c_str(), 
                                                   x, 
                                                   Policy());
        return false;
      }
      return true;
    }

    template <typename T_x, typename T_low, typename T_result, class Policy = default_policy>
    inline bool check_greater_or_equal(const char* function,
                                       const T_x& x,
                                       const T_low& low,
                                       const char* name,  
                                       T_result* result,
                                       const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      using boost::math::tools::promote_args;
      if (!(x >= low)) {
        std::ostringstream msg;
        msg << name 
            << " is %1%, but must be greater than or equal to "
            << low;
        *result = raise_domain_error<T_result,T_x>(function, 
                                                   msg.str().c_str(), 
                                                   x, 
                                                   Policy());
        return false;
      }
      return true;
    }


    template <typename T_x, typename T_low, typename T_high, typename T_result,
              class Policy = default_policy>
    inline bool check_bounded(const char* function,
                              const T_x& x,
                              const T_low& low,
                              const T_high& high,
                              const char* name,  
                              T_result* result,
                              const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      using boost::math::tools::promote_args;
      if (!(low <= x && x <= high)) {
        std::ostringstream msg;
        msg << name 
            << " is %1%, but must be between "
            << low
            << " and "
            << high;
        *result = raise_domain_error<T_result,T_x>(function,
                                                   msg.str().c_str(),
                                                   x, 
                                                   Policy());
        return false;
      }
      return true;
    }


    template <typename T_x, typename T_result, class Policy = default_policy>
    inline bool check_nonnegative(const char* function,
                                  const T_x& x,
                                  const char* name,
                                  T_result* result,
                                  const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      using boost::math::tools::promote_args;
      // have to use not is_unsigned. is_signed will be false
      // floating point types that have no unsigned versions.
      if (!boost::is_unsigned<T_x>::value && !(x >= 0)) {
        std::string message(name);
        message += " is %1%, but must be >= 0!";
        *result = raise_domain_error<T_result,T_x>(function,
                                                   message.c_str(), 
                                                   x, 
                                                   Policy());
        return false;
      }
      return true;
    }

    template <typename T_x, typename T_result, class Policy = default_policy>
    inline bool check_positive(const char* function,
                               const T_x& x,
                               const char* name,
                               T_result* result,
                               const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      if (!(x > 0)) {
        std::string message(name);
        message += " is %1%, but must be > 0";
        *result = raise_domain_error<T_result,T_x>(function,
                                                   message.c_str(), 
                                                   x, Policy());
        
        return false;
      }
      return true;
    }
    
    template <typename T_y, typename T_result, class Policy = default_policy>
    inline bool check_positive(const char* function,
                               const std::vector<T_y>& y,
                               const char* name,
                               T_result* result,
                               const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      for (size_t i = 0; i < y.size(); i++) {
        if (!(y[i] > 0)) {
          std::ostringstream message;
          message << name << "[" << i << "] is %1%, but must be > 0";
          *result = raise_domain_error<T_result,T_y>(function,
                                                     message.str().c_str(),
                                                     y[i], 
                                                     Policy());
          return false;
        }
      }
      return true;
    }

    template <typename T_prob_vector, typename T_result, class Policy = default_policy>
    inline bool check_simplex(const char* function,
                              const T_prob_vector& theta,
                              const char* name,
                              T_result* result,
                              const Policy& = Policy()) {
      using stan::math::policies::raise_domain_error;
      typename T_prob_vector::value_type T_prob;
      if (theta.size() == 0) {
        std::string message(name);
        message += " is not a valid simplex. %1% elements in the vector.";
        *result = raise_domain_error<T_result,T_prob>(function,
                                                      message.c_str(),
                                                      theta.size(),
                                                      Policy());
      }
      if (fabs(1.0 - theta.sum()) > CONSTRAINT_TOLERANCE) {
        std::string message(name);
        message += " is not a valid simplex.";
        message += " The sum of the elements is %1%, but should be 1.0";
        *result = raise_domain_error<T_result,T_prob>(function, 
                                                      message.c_str(), 
                                                      theta.sum(), 
                                                      Policy());
        return false;
      }
      for (size_t n = 0; n < theta.size(); n++) {
        if ((boost::math::isnan)(theta[n]) || !(theta[n] >= 0)) {
          std::ostringstream stream;
          stream << name << " is not a valid simplex."
                 << " The element at " << n 
                 << " is %1%, but should be greater than or equal to 0";
          *result = raise_domain_error<T_result,T_prob>(function, 
                                                        stream.str().c_str(), 
                                                        theta[n], 
                                                        Policy());
          return false;
        }
      }
      return true;
    }                         

    /**
     * Returns true if the specified value meets the constraint.
     *
     * <p>Because the identity mapping imposes no constraints, thi
     * method always returns <code>true</code>.
     *
     * @param x Value to validate.
     */
    template <typename T>
    inline
    bool identity_validate(const T x) {
      return true;
    }

    /**
     * Return <code>true</code> if the specified scalar is positive.
     *
     * @param y Scalar input.
     * @return <code>true</code> if the input is positive.
     * @tparam T Type of scalar.
     */
    template <typename T>
    inline
    bool positive_validate(const T y) {
      return y >= 0.0;
    }

    /**
     * Return <code>true</code> if the specified scalar is greater than
     * or equal to the specified lower bound.
     *
     * @param y Scalar to test.
     * @param lb Lower bound.
     * @return <code>true</code> if the scalar is greater than or
     * equal to the lower bound.
     * @tparam T Type of scalar.
     */
    template <typename T>
    inline
    bool lb_validate(const T y, const double lb) {
      return y >= lb;
    }

    /**
     * Return <code>true</code> if the specified scalar is less
     * than or equal to the specified upper bound.
     *
     * @param y Scalar to test.
     * @param ub Upper bound.
     * @return <code>true</code> if the specified scalar is less
     * than or equal to the specified upper bound.
     */
    template <typename T>
    bool ub_validate(const T y, const double ub) {
      return y <= ub;
    }

    /**
     * Return <code>true</code> if the specified scalar is between the
     * specified lower and upper bounds (inclusive).
     *
     * @param y Scalar to test.
     * @param lb Lower bound.
     * @param ub Upper bound.
     * @tparam T Type of scalar.
     */
    template <typename T>
    inline
    bool lub_validate(const T y, const double lb, const double ub) {
      return lb <= y && y <= ub;
    }

    /**
     * Return <code>true</code> if the specified scalar is
     * between 0 and 1 (inclusive).
     *
     * @param y Scalar to test.
     * @return <code>true</code> if the specified scalar is
     * between 0 and 1.
     * @tparam T Type of scalar.
     */
    template <typename T>
    inline
    bool prob_validate(const T y) {
      return 0.0 <= y && y <= 1.0;
    }

    /**
     * Return <code>true</code> if the specified scalar is
     * a valid correlation value between -1 and 1 (inclusive).
     *
     * @param y Scalar to test.
     * @return <code>true</code> if the specified scalar is
     * between -1 and 1.
     * @tparam T Type of scalar.
     */
    template <typename T>
    inline
    bool corr_validate(const T y) {
      return -1.0 <= y && y <= 1.0;
    }

  }
}

#endif


