#ifndef __STAN__MATH__MATRIX__COL_HPP__
#define __STAN__MATH__MATRIX__COL_HPP__

#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/error_handling/matrix/check_column_index.hpp>

namespace stan {
  namespace math {

    /**
     * Return the specified column of the specified matrix
     * using start-at-1 indexing.
     *
     * This is equivalent to calling <code>m.col(i - 1)</code> and
     * assigning the resulting template expression to a column vector.
     * 
     * @param m Matrix.
     * @param j Column index (count from 1).
     * @return Specified column of the matrix.
     */
    template <typename T>
    inline
    Eigen::Matrix<T,Eigen::Dynamic,1>
    col(const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>& m,
        size_t j) {
      stan::math::check_column_index("col(%1%)",j,m,"j",(double*)0);
      return m.col(j - 1);
    }

  }
}
#endif
