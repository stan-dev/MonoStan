#include <stan/math/prim/mat.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/prim/mat/fun/expect_matrix_eq.hpp>

using Eigen::Matrix;
using Eigen::Dynamic;
using stan::math::matrix_exp;

TEST(MathMatrix, matrix_exp) {
    
    Matrix<double, Dynamic, Dynamic> m1(1,1), m2(1,1);
    m1 << 0;
    m2 << 1;
        
    expect_matrix_eq(m2, matrix_exp(m1));
}

TEST(MathMatrix, matrix_exp2) {
    
    // example from Moler & Van Loan, 2003
    Matrix<double, Dynamic, Dynamic> m1(2,2), m2(2,2);
    
    m1 << -49, 24,
          -64, 31;
    
    m2 << -.735759, .551819,
          -1.471518, 1.103638;
    
    expect_matrix_eq(m2, matrix_exp(m1));
}