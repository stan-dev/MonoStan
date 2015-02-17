#ifndef STAN__MATH__FWD__MAT__FUN__QR_R_HPP
#define STAN__MATH__FWD__MAT__FUN__QR_R_HPP

#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <Eigen/QR>
#include <stan/math/prim/mat/err/check_nonzero_size.hpp>
#include <stan/math/prim/scal/err/check_greater_or_equal.hpp>
#include <stan/math/fwd/core/fvar.hpp>

namespace stan {
  namespace agrad {

    template <typename T>
    Eigen::Matrix<fvar<T>,Eigen::Dynamic,Eigen::Dynamic>
    qr_R(const Eigen::Matrix<fvar<T>,Eigen::Dynamic,Eigen::Dynamic>& m) {
      typedef Eigen::Matrix<fvar<T>,Eigen::Dynamic,Eigen::Dynamic> matrix_fwd_t;
      stan::math::check_nonzero_size("qr_R", "m", m);
      stan::math::check_greater_or_equal("qr_R", "m.rows()", m.rows(), m.cols());
      Eigen::HouseholderQR< matrix_fwd_t > qr(m.rows(), m.cols());
      qr.compute(m);
      matrix_fwd_t R = qr.matrixQR().topLeftCorner(m.rows(),m.cols());
      for (int i=0; i<R.rows(); i++) {
        for (int j=0; j<i; j++)
          R(i,j) = 0.0;
        if (i < R.cols() && R(i,i) < 0.0)
          R.row(i) *= -1.0;
      }
      return R;
    }
  }
}
#endif
