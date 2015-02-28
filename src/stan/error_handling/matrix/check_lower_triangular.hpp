#ifndef STAN__ERROR_HANDLING__MATRIX__CHECK_LOWER_TRIANGULAR_HPP
#define STAN__ERROR_HANDLING__MATRIX__CHECK_LOWER_TRIANGULAR_HPP

#include <stan/math/matrix/Eigen.hpp>
#include <stan/meta/traits.hpp>
#include <stan/error_handling/domain_error.hpp>
#include <sstream>
#include <string>

namespace stan {

  namespace math {
    using Eigen::Dynamic;
    /**
     * Return <code>true</code> if the specified matrix is lower
     * triangular.
     *
     * A matrix x is not lower triangular if there is a non-zero entry
     * x[m,n] with m &lt; n. This function only inspects the upper
     * triangular portion of the matrix, not including the diagonal.
     *
     * @tparam T Type of scalar of the matrix
     *
     * @param function Function name (for error messages)
     * @param name Variable name (for error messages)
     * @param y Matrix to test
     *
     * @return <code>true</code> if the matrix is lower triangular.
     * @throw <code>std::domain_error</code> if the matrix is not
     *   lower triangular or if any element in the upper triangular
     *   portion is NaN
     */
    template <typename T_y>
    inline bool
    check_lower_triangular(const char* function,
                           const char* name,
                           const Eigen::Matrix<T_y, Dynamic, Dynamic>& y) {
      for (int n = 1; n < y.cols(); ++n) {
        for (int m = 0; m < n && m < y.rows(); ++m) {
          if (y(m, n) != 0) {
            std::stringstream msg;
            msg << "is not lower triangular;"
                << " " << name << "[" << stan::error_index::value + m << ","
                << stan::error_index::value + n << "]=";
            std::string msg_str(msg.str());
            domain_error(function, name, y(m, n),
                         msg_str.c_str());
            return false;
          }
        }
      }
      return true;
    }

  }
}
#endif
