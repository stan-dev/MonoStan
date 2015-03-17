#ifndef STAN__MATH__MATRIX_SPARSE_EXTRACTORS_HPP
#define STAN__MATH__MATRIX_SPARSE_EXTRACTORS_HPP

#include <Eigen/Sparse>
#include <vector>
#include <numeric>

namespace stan {

  namespace math {
    // FIXME: The implementations are the same, only the interpretation
    // differs in CSC vs. CSR.  In the time I had I couldn't get one
    // implementation that swallowed both matrix types.

    /** @defgroup sparse_csc CSC Sparse Extractors.
     *  This group of functions extracts the components of a
     *  Compressed Sparse Column (CSC) sparse matrix.  The components
     *  are:
     *    - w: the non-zero values in the sparse matrix.
     *    - v: one-based row index for each value in w, as a result this
     *      is the same length as w.
     *    - u: one-based index of where each column starts in w, length
     *      is equal to the number of columns plus one.  Last entry is
     *      one-past-the-end in w (one-based...)
     *    - z: number of non-zero entries in each column of w, length is
     *      equal to the number of columns.
     *  @{
     */

    template <typename _Scalar> 
    const Eigen::Matrix<_Scalar, Eigen::Dynamic,1> extract_w(Eigen::SparseMatrix<_Scalar> A) {
      Eigen::Matrix<_Scalar,Eigen::Dynamic,1> w(A.nonZeros());
      for ( int j = 0; j < A.nonZeros(); ++j)
        w[j] = *(A.valuePtr()+j);
      return w;
    }
    
    template <typename _Scalar> 
    const std::vector<int> extract_v(Eigen::SparseMatrix<_Scalar> A) {
      std::vector<int> v(A.nonZeros());
      for ( int j = 0; j < A.nonZeros(); ++j)
        v[j] = *(A.innerIndexPtr()+j) + 1;   // make 1-indexed
      return v;
    }
    
    template <typename _Scalar> 
    const std::vector<int> extract_u(Eigen::SparseMatrix<_Scalar> A) {
      std::vector<int> u(A.outerSize()+1);
      for ( int j = 0; j <= A.outerSize(); ++j)
        u[j] = *(A.outerIndexPtr()+j) + 1; // make 1-indexed
      return u;
    }
    
    template <typename _Scalar> 
    const std::vector<int> extract_z(Eigen::SparseMatrix<_Scalar> A) {
      std::vector<int> u(A.outerSize()+1);
      std::vector<int> z(A.outerSize()+1);
      u = extract_u(A);
      std::adjacent_difference(u.begin(), u.end(), z.begin());
      z.erase(z.begin());
      return z;
    }
    /** @} */ // end of sparse_csc group. 
    
    /** @defgroup sparse_csr CSR Sparse Extractors.
     *  This group of functions extracts the components of a
     *  Compressed Sparse Row (CSR) sparse matrix.  The components
     *  are:
     *    - w: the non-zero values in the sparse matrix.
     *    - v: one-based column index for each value in w, as a result this
     *      is the same length as w.
     *    - u: one-based index of where each row starts in w, length
     *      is equal to the number of rows plus one.  Last entry is
     *      one-past-the-end in w (one-based...)
     *    - z: number of non-zero entries in each row of w, length is
     *      equal to the number of rows.
     *  @{
     */
    template <typename _Scalar> 
    const Eigen::Matrix<_Scalar,Eigen::Dynamic,1> extract_w(Eigen::SparseMatrix<_Scalar, Eigen::RowMajor> A) {
      Eigen::Matrix<_Scalar,Eigen::Dynamic,1> w(A.nonZeros());
      for ( int j = 0; j < A.nonZeros(); ++j)
        w[j] = *(A.valuePtr()+j);
      return w;
    }
    
    template <typename _Scalar> 
    const std::vector<int> extract_v(Eigen::SparseMatrix<_Scalar, Eigen::RowMajor> A) {
      std::vector<int> v(A.nonZeros());
      for ( int j = 0; j < A.nonZeros(); ++j)
        v[j] = *(A.innerIndexPtr()+j) + 1;  // make 1-indexed
      return v;
    }
    
    template <typename _Scalar> 
    const std::vector<int> extract_u(Eigen::SparseMatrix<_Scalar, Eigen::RowMajor> A) {
      std::vector<int> u(A.outerSize()+1);
      for ( int j = 0; j <= A.outerSize(); ++j)
        u[j] = *(A.outerIndexPtr()+j) + 1;  // make 1-indexed
      return u;
    }
    
    template <typename _Scalar> 
    const std::vector<int> extract_z(Eigen::SparseMatrix<_Scalar, Eigen::RowMajor> A) {
      std::vector<int> u(A.outerSize()+1);
      std::vector<int> z(A.outerSize()+1);
      u = extract_u(A);
      std::adjacent_difference(u.begin(), u.end(), z.begin());
      z.erase(z.begin());
      return z;
    }
    
    /** @} */ // end of sparse_csr group. 
  }
}

#endif
