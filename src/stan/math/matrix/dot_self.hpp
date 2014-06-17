#ifndef __STAN__MATH__MATRIX__DOT_SELF_HPP__
#define __STAN__MATH__MATRIX__DOT_SELF_HPP__

#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/matrix/check_vector.hpp>

namespace stan {
  namespace math {
    
    /**
     * Returns the dot product of the specified vector with itself.
     * @param v Vector.
     * @tparam R number of rows or <code>Eigen::Dynamic</code> for dynamic
     * @tparam C number of rows or <code>Eigen::Dyanmic</code> for dynamic
     * @throw std::domain_error If v is not vector dimensioned.
     */
    template <int R, int C>
    inline double dot_self(const Eigen::Matrix<double, R, C>& v) {
      stan::math::check_vector("dot_self(%1%)",v,"v",(double*)0);
      return v.squaredNorm();
    }    
    
  }
}
#endif
