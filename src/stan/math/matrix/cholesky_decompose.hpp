#ifndef __STAN__MATH__MATRIX__CHOLESKY_DECOMPOSE_HPP__
#define __STAN__MATH__MATRIX__CHOLESKY_DECOMPOSE_HPP__

#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/matrix/check_square.hpp>
#include <stan/math/error_handling/matrix/check_symmetric.hpp>

namespace stan {
  namespace math {

    /**
     * Return the lower-triangular Cholesky factor (i.e., matrix
     * square root) of the specified square, symmetric matrix.  The return
     * value \f$L\f$ will be a lower-traingular matrix such that the
     * original matrix \f$A\f$ is given by
     * <p>\f$A = L \times L^T\f$.
     * @param m Symmetrix matrix.
     * @return Square root of matrix.
     * @throw std::domain_error if m is not a symmetric matrix.
     */
    template <typename T>
    Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>
    cholesky_decompose(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>& m) {
      stan::math::check_square("cholesky_decompose(%1%)",m,"m",(double*)0);
      stan::math::check_symmetric("cholesky_decompose(%1%)",m,"m",(double*)0);
      Eigen::LLT<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> >llt(m.rows());
      llt.compute(m);
      return llt.matrixL();
    }

  }
}
#endif
