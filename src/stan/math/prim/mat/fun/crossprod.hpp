#ifndef STAN__MATH__PRIM__MAT__FUN__CROSSPROD_HPP
#define STAN__MATH__PRIM__MAT__FUN__CROSSPROD_HPP

#include <stan/math/prim/mat/fun/typedefs.hpp>
#include <stan/math/prim/mat/fun/tcrossprod.hpp>

namespace stan {
  namespace math {

    /**
     * Returns the result of pre-multiplying a matrix by its
     * own transpose.
     * @param M Matrix to multiply.
     * @return Transpose of M times M
     */
    inline matrix_d
    crossprod(const matrix_d& M) {
        return tcrossprod(M.transpose());
    }

  }
}
#endif
