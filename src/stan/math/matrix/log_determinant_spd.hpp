#ifndef __STAN__MATH__MATRIX__LOG_DETERMINANT_SPD_HPP__
#define __STAN__MATH__MATRIX__LOG_DETERMINANT_SPD_HPP__


#include <cmath>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/matrix/check_square.hpp>

namespace stan {
  namespace math {
    
    /**
     * Returns the log absolute determinant of the specified square matrix.
     *
     * @param m Specified matrix.
     * @return log absolute determinant of the matrix.
     * @throw std::domain_error if matrix is not square.
     */
    template <typename T,int R, int C>
    inline T log_determinant_spd(const Eigen::Matrix<T,R,C>& m) {
      using std::log;
      stan::math::check_square("log_determinant_spd(%1%)",m,"m",(double*)0);
//      Eigen::TriangularView< Eigen::Matrix<T,R,C>, Eigen::Lower > L(m.llt().matrixL());
//      T ret(0.0);
//      for (size_t i = 0; i < L.rows(); i++)
//        ret += log(L(i,i));
//      return 2*ret;
      return m.ldlt().vectorD().array().log().sum();
    }
    
  }
}
#endif
