#ifndef __STAN__MATH__MATRIX__DETERMINANT_HPP__
#define __STAN__MATH__MATRIX__DETERMINANT_HPP__

#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/matrix/check_square.hpp>

namespace stan {
  namespace math {
    
    /**
     * Returns the determinant of the specified square matrix.
     *
     * @param m Specified matrix.
     * @return Determinant of the matrix.
     * @throw std::domain_error if matrix is not square.
     */
    template <typename T,int R, int C>
    inline T determinant(const Eigen::Matrix<T,R,C>& m) {
      stan::math::check_square("determinant(%1%)",m,"m",(double*)0);
      return m.determinant();
    }    
    
  }
}
#endif
