#ifndef STAN__ERROR_HANDLING__MATRIX__CHECK_MATCHING_DIMS_HPP
#define STAN__ERROR_HANDLING__MATRIX__CHECK_MATCHING_DIMS_HPP

#include <sstream>
#include <stan/error_handling/domain_error.hpp>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/error_handling/matrix/check_size_match.hpp>

namespace stan {
  namespace math {

    /**
     * Return <code>true</code> if the two matrices are of the same size.
     *
     * This function checks not only the runtime sizes, but the static 
     * sizes as well. For example, a 4x1 matrix is not the same as a vector
     * with 4 elements.
     * 
     * @tparam T1 Scalar type of the first matrix
     * @tparam T2 Scalar type of the second matrix
     * @tparam R1 Rows specified at compile time of the first matrix
     * @tparam C1 Columns specified at compile time of the first matrix
     * @tparam R2 Rows specified at compile time of the second matrix
     * @tparam C2 Columns specified at compile time of the second matrix
     *
     * @param function Function name (for error messages)
     * @param name1 Variable name for the first matrix (for error messages)
     * @param y1 First matrix
     * @param name2 Variable name for the second matrix (for error messages)
     * @param y2 Second matrix
     *
     * @return <code>true</code> if the dimensions of the two matrices match
     * @throw <code>std::invalid_argument</code> if the dimensions of the matrices
     *   do not match
     */
    template <typename T1, typename T2, int R1, int C1, int R2, int C2>
    inline bool check_matching_dims(const std::string& function,
                                    const std::string& name1,
                                    const Eigen::Matrix<T1,R1,C1>& y1,
                                    const std::string& name2,
                                    const Eigen::Matrix<T2,R2,C2>& y2) {
      check_size_match(function, 
                       "Templated rows of matrix 1", R1,
                       "Templated rows of matrix 2", R2);
      check_size_match(function, 
                       "Templated columns of matrix 1", C1,
                       "Templated columns of matrix 2", C2);
      check_size_match(function, 
                       "Rows of matrix 1", y1.rows(),
                       "Rows of matrix 2", y2.rows());
      check_size_match(function, 
                       "Columns of matrix 1", y1.cols(),
                       "Columns of matrix 2", y2.cols());
      return true;
    }

  }
}
#endif
