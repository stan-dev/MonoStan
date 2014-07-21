#ifndef STAN__MATH__MATRIX__RBIND_HPP
#define STAN__MATH__MATRIX__RBIND_HPP

#include <stan/math/matrix/Eigen.hpp>
#include <stan/meta/traits.hpp> //stan::return_type
#include <stan/math/error_handling/matrix/check_size_match.hpp>
#include <vector>

namespace stan {
  namespace math {
    
    using Eigen::Dynamic;
    using Eigen::Matrix; 
    using std::vector;
    using stan::math::check_size_match;
       
    //matrix rbind(matrix, matrix)
    //matrix rbind(matrix, row_vector)
    //matrix rbind(row_vector, matrix)
    //matrix rbind(row_vector, row_vector)
    template <typename T1, typename T2, int R1, int C1, int R2, int C2>
    inline Matrix<typename return_type<T1, T2>::type, Dynamic, Dynamic>
    rbind(const Matrix<T1, R1, C1> & A,
          const Matrix<T2, R2, C2> & B) {
      check_size_match("rbind(%1%)",
                       A.cols(), "columns of A",
                       B.cols(), "columns of B",
                       (double*)0);            
      
      Matrix<typename return_type<T1, T2>::type, Dynamic, Dynamic>
        result(A.rows() + B.rows(), A.cols());
      result << A, B;
      return result;
    }
       
    //matrix rbind(vector, vector)
    template <typename T1, typename T2, int R1, int R2>
    inline Matrix<typename return_type<T1, T2>::type, Dynamic, 1>
    rbind(const Matrix<T1, R1, 1> & A,
          const Matrix<T2, R1, 1> & B) {          
      Matrix<typename return_type<T1, T2>::type, Dynamic, 1>
        result(A.size()+B.size());
      result << A, B;
      return result;
    }
    
  }
}
#endif
