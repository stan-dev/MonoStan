#ifndef STAN_MATH_REV_MAT_FUN_CHOLESKY_DECOMPOSE_HPP
#define STAN_MATH_REV_MAT_FUN_CHOLESKY_DECOMPOSE_HPP

#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <stan/math/prim/mat/err/check_pos_definite.hpp>
#include <stan/math/prim/mat/err/check_square.hpp>
#include <stan/math/prim/mat/err/check_symmetric.hpp>

namespace stan {
  namespace math {

    inline size_t vech_indexer(int i, int j, int N, size_t accum) {
      return static_cast<size_t>(j * N + i - accum);
    }

    class cholesky_decompose_v_vari : public vari {
    public:
      int M_;  // A.rows() = A.cols()
      double* A_;
      vari** _variRefA;
      vari** _variRefL;
      vari** _dummy;
      Eigen::Matrix<double, -1, -1> C;
      size_t accum_j;

      cholesky_decompose_v_vari(const Eigen::Matrix<var, -1, -1>& A)
        : vari(0.0),
          M_(A.rows()),
          A_(reinterpret_cast<double*>
             (stan::math::ChainableStack::memalloc_
              .alloc(sizeof(double) * A.rows() * A.rows()))),
          _variRefA(reinterpret_cast<vari**>
                    (stan::math::ChainableStack::memalloc_
                     .alloc(sizeof(vari*) * A.rows() * (A.cols() + 1) / 2))),
          _variRefL(reinterpret_cast<vari**>
                    (stan::math::ChainableStack::memalloc_
                     .alloc(sizeof(vari*) * A.rows() * (A.rows() + 1) / 2))),
          _dummy(reinterpret_cast<vari**>
                    (stan::math::ChainableStack::memalloc_
                     .alloc(sizeof(vari*)))),
          C(M_,M_),
          accum_j(0) {
          using Eigen::Map;
          using Eigen::Matrix;

        size_t posA = 0;
        for (size_type j = 0; j < M_; ++j) {
          for (size_type k = 0; k < M_; ++k)  
            A_[posA++] = A.coeffRef(k, j).vi_->val_;
          accum_j += j;
        }

        C = Map<Matrix<double, -1, -1> > (A_,M_,M_);
        Eigen::LLT<Eigen::MatrixXd> D = C.selfadjointView<Eigen::Lower>().llt();
        check_pos_definite("cholesky_decompose", "m", D);
        C = D.matrixL();

        size_t pos = 0;
        for (size_type j = 0; j < M_; ++j) {
          for (size_type i = j; i < M_; ++i) {
            _variRefA[pos] = A.coeffRef(i, j).vi_;
            _variRefL[pos] = new vari(C.coeffRef(i,j), false);
            ++pos;
          }
        }
        _dummy[0] = new vari(0.0, false);
      }

      virtual void chain() {
        size_t sum_j = accum_j;
        for (int i = M_ - 1; i >= 0; --i) {
          for (int j = i; j >= 0; --j) {
            size_t ij = vech_indexer(i, j, M_, sum_j);
            size_t jj = vech_indexer(j, j, M_, sum_j);
            if (i == j) 
             _variRefA[ij]->adj_ += 0.5 * _variRefL[ij]->adj_ / C.coeffRef(i, j);
            else {
              _variRefA[ij]->adj_ += _variRefL[ij]->adj_ / C.coeffRef(j, j);
              _variRefL[jj]->adj_ = _variRefL[jj]->adj_ - _variRefL[ij]->adj_ * C.coeffRef(i, j) / C.coeffRef(j, j);
            }
            size_t sum_k = sum_j - j;
            for (int k = j - 1; k >=0; --k) {
              size_t ik = vech_indexer(i, k, M_, sum_k);
              size_t jk = vech_indexer(j, k, M_, sum_k);
              _variRefL[ik]->adj_ = _variRefL[ik]->adj_ - _variRefA[ij]->adj_ * C.coeffRef(j, k);
              _variRefL[jk]->adj_ = _variRefL[jk]->adj_ - _variRefA[ij]->adj_ * C.coeffRef(i, k);
              sum_k -= k;
            }
            sum_j -= j;
          }
          accum_j -= i;
          sum_j = accum_j;
        }
      }
    };

    Eigen::Matrix<var, Eigen::Dynamic, Eigen::Dynamic>
    cholesky_decompose(const Eigen::Matrix<var, -1, -1> &A) {
      stan::math::check_square("cholesky_decompose", "A", A);
      stan::math::check_symmetric("cholesky_decompose", "A", A);
      
      // NOTE: this is not a memory leak, this vari is used in the
      // expression graph to evaluate the adjoint, but is not needed
      // for the returned matrix.  Memory will be cleaned up with the
      // arena allocator.
      cholesky_decompose_v_vari *baseVari
        = new cholesky_decompose_v_vari(A);

      Eigen::Matrix<var, -1, -1> L(A.rows(),A.rows());
      size_t pos = 0;
      for (size_type j = 0; j < L.cols(); ++j) {
        for (size_type i = j; i < L.rows(); ++i) 
          L.coeffRef(i, j).vi_ = baseVari->_variRefL[pos++];
        for (size_type k = 0; k < j; ++k) 
          L.coeffRef(k, j).vi_ = baseVari->_dummy[0];
      }
      return L;
    }
  }
}
#endif
