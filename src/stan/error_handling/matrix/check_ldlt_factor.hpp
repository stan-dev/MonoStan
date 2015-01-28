#ifndef STAN__ERROR_HANDLING__MATRIX__CHECK_LDLT_FACTOR_HPP
#define STAN__ERROR_HANDLING__MATRIX__CHECK_LDLT_FACTOR_HPP

#include <sstream>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/error_handling/scalar/dom_err.hpp>
#include <stan/math/matrix/LDLT_factor.hpp>

namespace stan {
  namespace error_handling {

    /**
     * Return <code>true</code> if the underlying matrix is positive definite
     *
     * @param function
     * @param A 
     * @param name
     * @return <code>true</code> if the matrix is positive definite.
     * @return throws if any element in lower triangular of matrix is nan
     * @tparam T Type of scalar.
     */
    template <typename T, int R, int C>
    inline bool check_ldlt_factor(const char* function,
                                  const char* name,
                                  stan::math::LDLT_factor<T,R,C> &A) {
      if (!A.success()) {
        std::ostringstream msg;
        msg << "is not positive definite. "
            << "last conditional variance is ";
        std::string msg_str(msg.str());
        const T too_small = A.vectorD().tail(1)(0);
        dom_err(function, name, too_small,
                msg_str.c_str(), ".");
        return false;
      }
      return true;
    }

  }
}
#endif
