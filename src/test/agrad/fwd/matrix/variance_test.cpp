#include <stan/math/matrix/variance.hpp>
#include <gtest/gtest.h>
#include <stan/math/matrix/typedefs.hpp>
#include <stan/agrad/fwd/matrix/typedefs.hpp>
#include <stan/agrad/fwd.hpp>
#include <stan/agrad/rev.hpp>
#include <test/agrad/util.hpp>

TEST(AgradFwdMatrixVariance, fd_vector) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_fd;

  vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  vector_d d1(6);
  vector_fd v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_);
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_);

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_);  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_);  
}
TEST(AgradFwdMatrixVariance, fd_vector_exception) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_fd;

  vector_d d1;
  vector_fd v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, fd_rowvector) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_fd;

  row_vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  row_vector_d d1(6);
  row_vector_fd v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_);
  EXPECT_FLOAT_EQ(-9.0, variance(v1).d_);

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_);  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_);  
}
TEST(AgradFwdMatrixVariance, fd_rowvector_exception) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_fd;

  row_vector_d d1;
  row_vector_fd v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, fd_matrix) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_fd;
  
  matrix_d m(1,1);
  m << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(m));

  matrix_d d1(2, 3);
  matrix_fd v1(2, 3);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_);
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_);

  d1.resize(1,1);
  v1.resize(1,1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_);  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_);  
}
TEST(AgradFwdMatrixVariance, fd_matrix_exception) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_fd;

  matrix_d d1;
  matrix_fd v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(0,1);
  v1.resize(0,1);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(1,0);
  v1.resize(1,0);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, fd_StdVector) {
  using stan::math::variance;
  using stan::agrad::fvar;

  fvar<double> x1 = 0.5;
  x1.d_ = 1.0;
  fvar<double> x2 = 2.0;
  x2.d_ = 2.0;
  fvar<double> x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<double> > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<double> f1 = variance(y1);

  EXPECT_FLOAT_EQ(2.25, f1.val_);
  EXPECT_FLOAT_EQ(1.5, f1.d_);
}
TEST(AgradFwdMatrixVariance, fv_vector_1stDeriv) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_fv;

  vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  vector_d d1(6);
  vector_fv v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val());
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_.val());

  AVEC q = createAVEC(v1(0).val(),v1(1).val(),v1(2).val(),v1(3).val(),v1(4).val(),v1(5).val());
  VEC h;
  variance(v1).val_.grad(q,h);
  EXPECT_FLOAT_EQ(-1,h[0]);
  EXPECT_FLOAT_EQ(-0.6,h[1]);
  EXPECT_FLOAT_EQ(-0.2,h[2]);
  EXPECT_FLOAT_EQ(0.2,h[3]);
  EXPECT_FLOAT_EQ(0.6,h[4]);
  EXPECT_FLOAT_EQ(1,h[5]);

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val());  
}
TEST(AgradFwdMatrixVariance, fv_vector_2ndDeriv) {
  using stan::math::variance;
  using stan::agrad::vector_fv;

  vector_fv v1(6);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val(),v1(1).val(),v1(2).val(),v1(3).val(),v1(4).val(),v1(5).val());
  VEC h;
  variance(v1).d_.grad(q,h);
  EXPECT_FLOAT_EQ(-1.0/3.0,h[0]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[1]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[2]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[3]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[4]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[5]);
}
TEST(AgradFwdMatrixVariance, fv_vector_exception) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_fv;

  vector_d d1;
  vector_fv v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, fv_rowvector_1stDeriv) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_fv;

  row_vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  row_vector_d d1(6);
  row_vector_fv v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val());
  EXPECT_FLOAT_EQ(-9.0, variance(v1).d_.val());

  AVEC q = createAVEC(v1(0).val(),v1(1).val(),v1(2).val(),v1(3).val(),v1(4).val(),v1(5).val());
  VEC h;
  variance(v1).val_.grad(q,h);
  EXPECT_FLOAT_EQ(-1,h[0]);
  EXPECT_FLOAT_EQ(-0.6,h[1]);
  EXPECT_FLOAT_EQ(-0.2,h[2]);
  EXPECT_FLOAT_EQ(0.2,h[3]);
  EXPECT_FLOAT_EQ(0.6,h[4]);
  EXPECT_FLOAT_EQ(1,h[5]);

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val());  
}
TEST(AgradFwdMatrixVariance, fv_rowvector_2ndDeriv) {
  using stan::math::variance;
  using stan::agrad::row_vector_fv;
  row_vector_fv v1(6);
  
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val(),v1(1).val(),v1(2).val(),v1(3).val(),v1(4).val(),v1(5).val());
  VEC h;
  variance(v1).d_.grad(q,h);
  EXPECT_FLOAT_EQ(3,h[0]);
  EXPECT_FLOAT_EQ(-.6,h[1]);
  EXPECT_FLOAT_EQ(-.6,h[2]);
  EXPECT_FLOAT_EQ(-.6,h[3]);
  EXPECT_FLOAT_EQ(-.6,h[4]);
  EXPECT_FLOAT_EQ(-.6,h[5]);
}
TEST(AgradFwdMatrixVariance, fv_rowvector_exception) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_fv;

  row_vector_d d1;
  row_vector_fv v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, fv_matrix_1stDeriv) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_fv;
  
  matrix_d m(1,1);
  m << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(m));

  matrix_d d1(2, 3);
  matrix_fv v1(2, 3);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val());
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_.val());

   AVEC q = createAVEC(v1(0,0).val(),v1(0,1).val(),v1(0,2).val(),v1(1,0).val(),v1(1,1).val(),v1(1,2).val());
  VEC h;
  variance(v1).val_.grad(q,h);
  EXPECT_FLOAT_EQ(-1,h[0]);
  EXPECT_FLOAT_EQ(-.60,h[1]);
  EXPECT_FLOAT_EQ(-0.2,h[2]);
  EXPECT_FLOAT_EQ(0.2,h[3]);
  EXPECT_FLOAT_EQ(0.6,h[4]);
  EXPECT_FLOAT_EQ(1,h[5]);

  d1.resize(1,1);
  v1.resize(1,1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val());  
}
TEST(AgradFwdMatrixVariance, fv_matrix_2ndDeriv) {
  using stan::math::variance;
  using stan::agrad::matrix_fv;
  matrix_fv v1(2, 3);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;

   AVEC q = createAVEC(v1(0,0).val(),v1(0,1).val(),v1(0,2).val(),v1(1,0).val(),v1(1,1).val(),v1(1,2).val());
  VEC h;
  variance(v1).d_.grad(q,h);
  EXPECT_FLOAT_EQ(-1.0/3.0,h[0]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[1]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[2]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[3]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[4]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[5]);
}
TEST(AgradFwdMatrixVariance, fv_matrix_exception) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_fv;

  matrix_d d1;
  matrix_fv v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(0,1);
  v1.resize(0,1);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(1,0);
  v1.resize(1,0);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, fv_StdVector_1stDeriv) {
  using stan::math::variance;
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<var> x1 = 0.5;
  x1.d_ = 1.0;
  fvar<var> x2 = 2.0;
  x2.d_ = 2.0;
  fvar<var> x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<var> > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<var> f1 = variance(y1);

  EXPECT_FLOAT_EQ(2.25, f1.val_.val());
  EXPECT_FLOAT_EQ(1.5, f1.d_.val());

  AVEC q = createAVEC(x1.val(),x2.val(),x3.val());
  VEC h;
  f1.val_.grad(q,h);
  EXPECT_FLOAT_EQ(-1.5,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(1.5,h[2]);
}
TEST(AgradFwdMatrixVariance, fv_StdVector_2ndDeriv) {
  using stan::math::variance;
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<var> x1 = 0.5;
  x1.d_ = 1.0;
  fvar<var> x2 = 2.0;
  x2.d_ = 2.0;
  fvar<var> x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<var> > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<var> f1 = variance(y1);

  AVEC q = createAVEC(x1.val(),x2.val(),x3.val());
  VEC h;
  f1.d_.grad(q,h);
  EXPECT_FLOAT_EQ(-2.0/3.0,h[0]);
  EXPECT_FLOAT_EQ(1.0/3.0,h[1]);
  EXPECT_FLOAT_EQ(1.0/3.0,h[2]);
}
TEST(AgradFwdMatrixVariance, ffd_vector) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_ffd;

  vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  vector_d d1(6);
  vector_ffd v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val());
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_.val());

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val());  
}
TEST(AgradFwdMatrixVariance, ffd_vector_exception) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_ffd;

  vector_d d1;
  vector_ffd v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, ffd_rowvector) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_ffd;

  row_vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  row_vector_d d1(6);
  row_vector_ffd v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val());
  EXPECT_FLOAT_EQ(-9.0, variance(v1).d_.val());

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val());  
}
TEST(AgradFwdMatrixVariance, ffd_rowvector_exception) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_ffd;

  row_vector_d d1;
  row_vector_ffd v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, ffd_matrix) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_ffd;
  
  matrix_d m(1,1);
  m << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(m));

  matrix_d d1(2, 3);
  matrix_ffd v1(2, 3);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val());
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_.val());

  d1.resize(1,1);
  v1.resize(1,1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val());  
}
TEST(AgradFwdMatrixVariance, ffd_matrix_exception) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_ffd;

  matrix_d d1;
  matrix_ffd v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(0,1);
  v1.resize(0,1);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(1,0);
  v1.resize(1,0);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, ffd_StdVector) {
  using stan::math::variance;
  using stan::agrad::fvar;

  fvar<fvar<double> > x1 = 0.5;
  x1.d_ = 1.0;
  fvar<fvar<double> > x2 = 2.0;
  x2.d_ = 2.0;
  fvar<fvar<double> > x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<fvar<double> > > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<fvar<double> > f1 = variance(y1);

  EXPECT_FLOAT_EQ(2.25, f1.val_.val());
  EXPECT_FLOAT_EQ(1.5, f1.d_.val());
}
TEST(AgradFwdMatrixVariance, ffv_vector_1stDeriv) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_ffv;

  vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  vector_d d1(6);
  vector_ffv v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val().val());
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_.val().val());

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).val_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-1,h[0]);
  EXPECT_FLOAT_EQ(-0.6,h[1]);
  EXPECT_FLOAT_EQ(-0.2,h[2]);
  EXPECT_FLOAT_EQ(0.2,h[3]);
  EXPECT_FLOAT_EQ(0.6,h[4]);
  EXPECT_FLOAT_EQ(1,h[5]);

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val().val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val().val());  
}
TEST(AgradFwdMatrixVariance, ffv_vector_2ndDeriv_1) {
  using stan::math::variance;
  using stan::agrad::vector_ffv;

  vector_ffv v1(6);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).val().d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
  EXPECT_FLOAT_EQ(0,h[3]);
  EXPECT_FLOAT_EQ(0,h[4]);
  EXPECT_FLOAT_EQ(0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_vector_2ndDeriv_2) {
  using stan::math::variance;
  using stan::agrad::vector_ffv;

  vector_ffv v1(6);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).d_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-1.0/3.0,h[0]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[1]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[2]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[3]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[4]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_vector_3rdDeriv) {
  using stan::math::variance;
  using stan::agrad::vector_ffv;

  vector_ffv v1(6);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 1.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).d_.d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
  EXPECT_FLOAT_EQ(0,h[3]);
  EXPECT_FLOAT_EQ(0,h[4]);
  EXPECT_FLOAT_EQ(0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_vector_exception) {
  using stan::math::variance;
  using stan::math::vector_d;
  using stan::agrad::vector_ffv;

  vector_d d1;
  vector_ffv v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, ffv_rowvector_1stDeriv) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_ffv;

  row_vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  row_vector_d d1(6);
  row_vector_ffv v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val().val());
  EXPECT_FLOAT_EQ(-9.0, variance(v1).d_.val().val());

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).val_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-1,h[0]);
  EXPECT_FLOAT_EQ(-0.6,h[1]);
  EXPECT_FLOAT_EQ(-0.2,h[2]);
  EXPECT_FLOAT_EQ(0.2,h[3]);
  EXPECT_FLOAT_EQ(0.6,h[4]);
  EXPECT_FLOAT_EQ(1,h[5]);

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val().val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val().val());  
}
TEST(AgradFwdMatrixVariance, ffv_rowvector_2ndDeriv_1) {
  using stan::math::variance;
  using stan::agrad::row_vector_ffv;
  row_vector_ffv v1(6);
  
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).val().d_.grad(q,h);  
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
  EXPECT_FLOAT_EQ(0,h[3]);
  EXPECT_FLOAT_EQ(0,h[4]);
  EXPECT_FLOAT_EQ(0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_rowvector_2ndDeriv_2) {
  using stan::math::variance;
  using stan::agrad::row_vector_ffv;
  row_vector_ffv v1(6);
  
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).d_.val().grad(q,h);
  EXPECT_FLOAT_EQ(3,h[0]);
  EXPECT_FLOAT_EQ(-.6,h[1]);
  EXPECT_FLOAT_EQ(-.6,h[2]);
  EXPECT_FLOAT_EQ(-.6,h[3]);
  EXPECT_FLOAT_EQ(-.6,h[4]);
  EXPECT_FLOAT_EQ(-.6,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_rowvector_3rdDeriv) {
  using stan::math::variance;
  using stan::agrad::row_vector_ffv;
  row_vector_ffv v1(6);
  
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0).d_ = 11.0;
   v1(1).d_ = 2.0;
   v1(2).d_ = 2.0;
   v1(3).d_ = 2.0;
   v1(4).d_ = 2.0;
   v1(5).d_ = 2.0;

  AVEC q = createAVEC(v1(0).val().val(),v1(1).val().val(),v1(2).val().val(),v1(3).val().val(),v1(4).val().val(),v1(5).val().val());
  VEC h;
  variance(v1).d_.d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
  EXPECT_FLOAT_EQ(0,h[3]);
  EXPECT_FLOAT_EQ(0,h[4]);
  EXPECT_FLOAT_EQ(0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_rowvector_exception) {
  using stan::math::variance;
  using stan::math::row_vector_d;
  using stan::agrad::row_vector_ffv;

  row_vector_d d1;
  row_vector_ffv v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, ffv_matrix_1stDeriv) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_ffv;
  
  matrix_d m(1,1);
  m << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(m));

  matrix_d d1(2, 3);
  matrix_ffv v1(2, 3);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val_.val().val());
  EXPECT_FLOAT_EQ(1.0, variance(v1).d_.val().val());

   AVEC q = createAVEC(v1(0,0).val().val(),v1(0,1).val().val(),v1(0,2).val().val(),v1(1,0).val().val(),v1(1,1).val().val(),v1(1,2).val().val());
  VEC h;
  variance(v1).val_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-1,h[0]);
  EXPECT_FLOAT_EQ(-.60,h[1]);
  EXPECT_FLOAT_EQ(-0.2,h[2]);
  EXPECT_FLOAT_EQ(0.2,h[3]);
  EXPECT_FLOAT_EQ(0.6,h[4]);
  EXPECT_FLOAT_EQ(1,h[5]);

  d1.resize(1,1);
  v1.resize(1,1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val_.val().val());  
  EXPECT_FLOAT_EQ(0.0, variance(v1).d_.val().val());  
}
TEST(AgradFwdMatrixVariance, ffv_matrix_2ndDeriv_1) {
  using stan::math::variance;
  using stan::agrad::matrix_ffv;
  matrix_ffv v1(2, 3);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;

   AVEC q = createAVEC(v1(0,0).val().val(),v1(0,1).val().val(),v1(0,2).val().val(),v1(1,0).val().val(),v1(1,1).val().val(),v1(1,2).val().val());
  VEC h;
  variance(v1).val().d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
  EXPECT_FLOAT_EQ(0,h[3]);
  EXPECT_FLOAT_EQ(0,h[4]);
  EXPECT_FLOAT_EQ(0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_matrix_2ndDeriv_2) {
  using stan::math::variance;
  using stan::agrad::matrix_ffv;
  matrix_ffv v1(2, 3);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;

   AVEC q = createAVEC(v1(0,0).val().val(),v1(0,1).val().val(),v1(0,2).val().val(),v1(1,0).val().val(),v1(1,1).val().val(),v1(1,2).val().val());
  VEC h;
  variance(v1).d_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-1.0/3.0,h[0]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[1]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[2]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[3]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[4]);
  EXPECT_FLOAT_EQ(1.0/15.0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_matrix_3rdDeriv) {
  using stan::math::variance;
  using stan::agrad::matrix_ffv;
  matrix_ffv v1(2, 3);
  v1 << 1, 2, 3, 4, 5, 6;
   v1(0,0).d_ = 1.0;
   v1(0,1).d_ = 2.0;
   v1(0,2).d_ = 2.0;
   v1(1,0).d_ = 2.0;
   v1(1,1).d_ = 2.0;
   v1(1,2).d_ = 2.0;

   AVEC q = createAVEC(v1(0,0).val().val(),v1(0,1).val().val(),v1(0,2).val().val(),v1(1,0).val().val(),v1(1,1).val().val(),v1(1,2).val().val());
  VEC h;
  variance(v1).d_.d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
  EXPECT_FLOAT_EQ(0,h[3]);
  EXPECT_FLOAT_EQ(0,h[4]);
  EXPECT_FLOAT_EQ(0,h[5]);
}
TEST(AgradFwdMatrixVariance, ffv_matrix_exception) {
  using stan::math::variance;
  using stan::math::matrix_d;
  using stan::agrad::matrix_ffv;

  matrix_d d1;
  matrix_ffv v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(0,1);
  v1.resize(0,1);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(1,0);
  v1.resize(1,0);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(AgradFwdMatrixVariance, ffv_StdVector_1stDeriv) {
  using stan::math::variance;
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x1 = 0.5;
  x1.d_ = 1.0;
  fvar<fvar<var> > x2 = 2.0;
  x2.d_ = 2.0;
  fvar<fvar<var> > x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<fvar<var> > > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<fvar<var> > f1 = variance(y1);

  EXPECT_FLOAT_EQ(2.25, f1.val_.val().val());
  EXPECT_FLOAT_EQ(1.5, f1.d_.val().val());

  AVEC q = createAVEC(x1.val().val(),x2.val().val(),x3.val().val());
  VEC h;
  f1.val_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-1.5,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(1.5,h[2]);
}
TEST(AgradFwdMatrixVariance, ffv_StdVector_2ndDeriv_1) {
  using stan::math::variance;
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x1 = 0.5;
  x1.d_ = 1.0;
  fvar<fvar<var> > x2 = 2.0;
  x2.d_ = 2.0;
  fvar<fvar<var> > x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<fvar<var> > > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<fvar<var> > f1 = variance(y1);

  AVEC q = createAVEC(x1.val().val(),x2.val().val(),x3.val().val());
  VEC h;
  f1.val().d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
}

TEST(AgradFwdMatrixVariance, ffv_StdVector_2ndDeriv_2) {
  using stan::math::variance;
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x1 = 0.5;
  x1.d_ = 1.0;
  fvar<fvar<var> > x2 = 2.0;
  x2.d_ = 2.0;
  fvar<fvar<var> > x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<fvar<var> > > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<fvar<var> > f1 = variance(y1);

  AVEC q = createAVEC(x1.val().val(),x2.val().val(),x3.val().val());
  VEC h;
  f1.d_.val().grad(q,h);
  EXPECT_FLOAT_EQ(-2.0/3.0,h[0]);
  EXPECT_FLOAT_EQ(1.0/3.0,h[1]);
  EXPECT_FLOAT_EQ(1.0/3.0,h[2]);
}

TEST(AgradFwdMatrixVariance, ffv_StdVector_3rdDeriv) {
  using stan::math::variance;
  using stan::agrad::fvar;
  using stan::agrad::var;

  fvar<fvar<var> > x1 = 0.5;
  x1.d_ = 1.0;
  fvar<fvar<var> > x2 = 2.0;
  x2.d_ = 2.0;
  fvar<fvar<var> > x3 = 3.5;
  x3.d_ = 2.0;
  std::vector<fvar<fvar<var> > > y1;
  y1.push_back(x1);
  y1.push_back(x2);
  y1.push_back(x3);
  fvar<fvar<var> > f1 = variance(y1);

  AVEC q = createAVEC(x1.val().val(),x2.val().val(),x3.val().val());
  VEC h;
  f1.d_.d_.grad(q,h);
  EXPECT_FLOAT_EQ(0,h[0]);
  EXPECT_FLOAT_EQ(0,h[1]);
  EXPECT_FLOAT_EQ(0,h[2]);
}
