#ifndef __STAN__AGRAD__FWD__MATRIX__QR_Q_HPP__
#define __STAN__AGRAD__FWD__MATRIX__QR_Q_HPP__

#include <stan/math/matrix/Eigen.hpp>
#include <Eigen/QR>
#include <stan/math/error_handling/matrix/check_nonzero_size.hpp>
#include <stan/math/error_handling/check_greater_or_equal.hpp>
#include <stan/agrad/fwd/fvar.hpp>

namespace stan {
  namespace agrad {

    template <typename T>
    Eigen::Matrix<fvar<T>,Eigen::Dynamic,Eigen::Dynamic>
    qr_Q(const Eigen::Matrix<fvar<T>,Eigen::Dynamic,Eigen::Dynamic>& m) {
      typedef Eigen::Matrix<fvar<T>,Eigen::Dynamic,Eigen::Dynamic> matrix_fwd_t;
      stan::math::check_nonzero_size("qr_Q(%1%)",m,"m",(double*)0);
      stan::math::check_greater_or_equal("qr_Q(%1%)",m.rows(),m.cols(),"m.rows()",
                                         (double*)0);
      Eigen::HouseholderQR< matrix_fwd_t > qr(m.rows(), m.cols());
      qr.compute(m);
      matrix_fwd_t Q = qr.householderQ();
      for (int i=0; i<m.cols(); i++)
        if (qr.matrixQR()(i,i) < 0.0)
          Q.col(i) *= -1.0;
      return Q;
    }
  }
}
#endif
