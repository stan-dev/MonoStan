#include <iostream>
#include <stan/math/matrix/append_col.hpp>
#include <gtest/gtest.h>
#include <test/unit/agrad/util.hpp>
#include <stan/agrad/rev.hpp>
#include <stan/agrad/rev/matrix.hpp>
#include <stan/math/matrix/sum.hpp>
#include <stan/agrad/rev/functions/exp.hpp>

using stan::math::sum;
using stan::math::append_col;
using stan::agrad::matrix_v;
using stan::agrad::row_vector_v;
using Eigen::MatrixXd;
using Eigen::RowVectorXd;

TEST(AgradRevMatrix, append_col_matrix) {
  matrix_v a(2,2);
  matrix_v a_exp(2,2);
  MatrixXd b(2,2);
  
  a << 2.0, 3.0,
       9.0, -1.0;
       
  b << 4.0, 3.0,
       0.0, 1.0;

  AVEC x;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      x.push_back(a(i,j));
      a_exp(i, j) = stan::agrad::exp(a(i, j));
    }
  }
  
  AVAR append_col_ab = sum(append_col(a_exp, b));

  VEC g = cgradvec(append_col_ab, x);
  
  size_t idx = 0;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      EXPECT_FLOAT_EQ(std::exp(a(i, j).val()), g[idx++]);
  stan::agrad::recover_memory();
}

TEST(AgradRevMatrix, append_col_row_vector) {
  row_vector_v a(3);
  row_vector_v a_exp(3);
  RowVectorXd b(3);
  
  a << 2.0, 3.0, 9.0;
       
  b << 4.0, 3.0, 0.0;

  AVEC x;
  for (int i = 0; i < 3; ++i) {
    x.push_back(a(i));
    a_exp(i) = stan::agrad::exp(a(i));
  }
  
  AVAR append_col_ab = sum(append_col(a_exp, b));

  VEC g = cgradvec(append_col_ab, x);
  
  size_t idx = 0;
  for (int i = 0; i < 3; i++)
    EXPECT_FLOAT_EQ(std::exp(a(i).val()), g[idx++]);
  stan::agrad::recover_memory();
}

template <typename T, int R, int C>
void correct_type_row_vector(const Eigen::Matrix<T,R,C>& x) {
  EXPECT_EQ(Eigen::Dynamic, C);
  EXPECT_EQ(1, R);
}

template <typename T, int R, int C>
void correct_type_matrix(const Eigen::Matrix<T,R,C>& x) {
  EXPECT_EQ(Eigen::Dynamic, C);
  EXPECT_EQ(Eigen::Dynamic, R);
}

TEST(MathMatrix, append_col_different_types) {
  using Eigen::Dynamic;
  using Eigen::Matrix;
  using Eigen::MatrixXd;
  using Eigen::VectorXd;
  using Eigen::RowVectorXd;
  using stan::agrad::matrix_v;
  using stan::agrad::row_vector_v;
  using stan::agrad::vector_v;
  using std::vector;  
  
  MatrixXd m33(3, 3);
  m33 << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
        
  MatrixXd m32(3, 2);
  m32 << 11, 12,
         13, 14,
         15, 16;

  MatrixXd m23(2, 3);
  m23 << 21, 22, 23,
         24, 25, 26;
         
  MatrixXd m23b(2, 3);        
  m23b = m23*1.101; //ensure some different values
         
  VectorXd v3(3);
  v3 << 31, 
        32,
        33;
        
  VectorXd v3b(3);
  v3b << 34, 
         35,
         36;

  RowVectorXd rv3(3);
  rv3 << 41, 42, 43;
  
  RowVectorXd rv3b(3);
  rv3b << 44, 45, 46;
  
  MatrixXd vm33(3, 3);
  vm33 << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
        
  MatrixXd vm32(3, 2);
  vm32 << 11, 12,
         13, 14,
         15, 16;

  MatrixXd vm23(2, 3);
  vm23 << 21, 22, 23,
         24, 25, 26;
         
  MatrixXd vm23b(2, 3);        
  vm23b = m23*1.101; //ensure some different values
         
  VectorXd vv3(3);
  vv3 << 31, 
        32,
        33;
        
  VectorXd vv3b(3);
  vv3b << 34, 
         35,
         36;

  RowVectorXd vrv3(3);
  vrv3 << 41, 42, 43;
  
  RowVectorXd vrv3b(3);
  vrv3b << 44, 45, 46;

  correct_type_matrix(append_col(m32, vm33));
  correct_type_matrix(append_col(m33, vm32));
  correct_type_matrix(append_col(m23, vm23b));
  correct_type_matrix(append_col(m23b, vm23));
  correct_type_matrix(append_col(m33, vv3));
  correct_type_matrix(append_col(v3, vm33));
  correct_type_matrix(append_col(m32, vv3));
  correct_type_matrix(append_col(v3, vm32));
  correct_type_matrix(append_col(v3, vv3b));
  correct_type_matrix(append_col(v3b, vv3));
  correct_type_row_vector(append_col(rv3, vrv3b));
  correct_type_row_vector(append_col(rv3b, vrv3));

  correct_type_matrix(append_col(vm32, m33));
  correct_type_matrix(append_col(vm33, m32));
  correct_type_matrix(append_col(vm23, m23b));
  correct_type_matrix(append_col(vm23b, m23));
  correct_type_matrix(append_col(vm33, v3));
  correct_type_matrix(append_col(vv3, m33));
  correct_type_matrix(append_col(vm32, v3));
  correct_type_matrix(append_col(vv3, m32));
  correct_type_matrix(append_col(vv3, v3b));
  correct_type_matrix(append_col(vv3b, v3));
  correct_type_row_vector(append_col(vrv3, rv3b));
  correct_type_row_vector(append_col(vrv3b, rv3));

  correct_type_matrix(append_col(vm32, vm33));
  correct_type_matrix(append_col(vm33, vm32));
  correct_type_matrix(append_col(vm23, vm23b));
  correct_type_matrix(append_col(vm23b, vm23));
  correct_type_matrix(append_col(vm33, vv3));
  correct_type_matrix(append_col(vv3, vm33));
  correct_type_matrix(append_col(vm32, vv3));
  correct_type_matrix(append_col(vv3, vm32));
  correct_type_matrix(append_col(vv3, vv3b));
  correct_type_matrix(append_col(vv3b, vv3));
  correct_type_row_vector(append_col(vrv3, vrv3b));
  correct_type_row_vector(append_col(vrv3b, vrv3));
}
