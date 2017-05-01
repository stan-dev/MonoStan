#include <stan/math/rev/core.hpp>
#include <stan/math/rev/mat/functor/algebra_solver.hpp>
#include <test/unit/math/rev/mat/fun/util.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <test/unit/util.hpp>

// Every test exists in duplicate to test the case
// where y (the auxiliary parameters) are passed as
// data (double type) or parameters (var types).

template <typename T0, typename T1>
inline
Eigen::Matrix<typename boost::math::tools::promote_args<T0, T1>::type,
              Eigen::Dynamic, 1>
simpleEq(const Eigen::Matrix<T0, Eigen::Dynamic, 1>& x,
         const Eigen::Matrix<T1, Eigen::Dynamic, 1>& y,
         const std::vector<double>& dat,
         const std::vector<int>& dat_int,
         std::ostream* pstream__) {
  typedef typename boost::math::tools::promote_args<T0, T1>::type scalar;
  Eigen::Matrix<scalar, Eigen::Dynamic, 1> z(2);
  z(0) = x(0) - y(0) * y(1);
  z(1) = x(1) - y(2);
  return z;
}

struct simpleEq_functor {
  template <typename T0, typename T1>
  inline
  Eigen::Matrix<typename boost::math::tools::promote_args<T0, T1>::type,
                Eigen::Dynamic, 1>
  operator()(const Eigen::Matrix<T0, Eigen::Dynamic, 1>& x,
             const Eigen::Matrix<T1, Eigen::Dynamic, 1>& y,
             const std::vector<double>& dat,
             const std::vector<int>& dat_int,
             std::ostream* pstream__) const {
    return simpleEq(x, y, dat, dat_int, pstream__);
  }
};

TEST(MathMatrix, simple_Eq) {
  using stan::math::algebra_solver;
  using stan::math::sum;
  using stan::math::var;

  int n_x = 2, n_y = 3;
  for (int k = 0; k < n_x; k++) {

    Eigen::VectorXd x(n_x);
    x << 1, 1;  // initial guess
    Eigen::Matrix<var, Eigen::Dynamic, 1> y(n_y);
    y << 5, 4, 2;
    std::vector<double> dummy_dat(0);
    std::vector<int> dummy_dat_int(0);
    Eigen::Matrix<var, Eigen::Dynamic, 1> theta;

    theta = algebra_solver(simpleEq_functor(),
                           x, y, dummy_dat, dummy_dat_int);

    EXPECT_EQ(20, theta(0));
    EXPECT_EQ(2, theta(1));

    Eigen::MatrixXd J(n_x, n_y);
    J << 4, 5, 0, 0, 0, 1;

    AVEC y_vec = createAVEC(y(0), y(1), y(2));
    VEC g;
    theta(k).grad(y_vec, g);

    for (int i = 0; i < n_y; i++)
      EXPECT_EQ(J(k, i), g[i]);
  }
}

TEST(MathMatrix, simple_Eq_dbl) {
  using stan::math::algebra_solver;

 Eigen::VectorXd x(2);
 x << 1, 1;  // initial guess
 Eigen::VectorXd y(3);
 y << 5, 4, 2;
 std::vector<double> dummy_dat(0);
 std::vector<int> dummy_dat_int(0);
 Eigen::VectorXd theta;

 theta = algebra_solver(simpleEq_functor(),
                        x, y, dummy_dat, dummy_dat_int);

 EXPECT_EQ(20, theta(0));
 EXPECT_EQ(2, theta(1));
}

TEST(MathMatrix, simple_Eq_tuned) {
  using stan::math::algebra_solver;
  using stan::math::sum;
  using stan::math::var;

  int n_x = 2, n_y = 3;
  for (int k = 0; k < n_x; k++) {

    Eigen::VectorXd x(n_x);
    x << 1, 1;  // initial guess
    Eigen::Matrix<var, Eigen::Dynamic, 1> y(n_y);
    y << 5, 4, 2;
    std::vector<double> dummy_dat(0);
    std::vector<int> dummy_dat_int(0);

    Eigen::Matrix<var, Eigen::Dynamic, 1> theta;
    double xtol = 1e-6, ftol = 1e-6;
    int maxfev = 1e+4;

    theta = algebra_solver(simpleEq_functor(),
                           x, y, dummy_dat, dummy_dat_int,
                           0, xtol, ftol, maxfev);

    EXPECT_EQ(20, theta(0));
    EXPECT_EQ(2, theta(1));

    Eigen::MatrixXd J(n_x, n_y);
    J << 4, 5, 0, 0, 0, 1;

    AVEC y_vec = createAVEC(y(0), y(1), y(2));
    VEC g;
    theta(k).grad(y_vec, g);

    for (int i = 0; i < n_y; i++)
      EXPECT_EQ(J(k, i), g[i]);
  }
}

TEST(MathMatrix, simple_Eq_tuned_dbl) {
  using stan::math::algebra_solver;
  using stan::math::var;

  int n_x = 2, n_y = 3;

  Eigen::VectorXd x(n_x);
  x << 1, 1;  // initial guess
  Eigen::VectorXd y(n_y);
  y << 5, 4, 2;
  std::vector<double> dummy_dat(0);
  std::vector<int> dummy_dat_int(0);

  Eigen::VectorXd theta;
  double xtol = 1e-6, ftol = 1e-6;
  int maxfev = 1e+4;

  theta = algebra_solver(simpleEq_functor(),
                         x, y, dummy_dat, dummy_dat_int,
                         0, xtol, ftol, maxfev);

  EXPECT_EQ(20, theta(0));
  EXPECT_EQ(2, theta(1));
}

///////////////////////////////////////////////////////////////////////////////

struct nonLinearEq_functor {
  template <typename T0, typename T1>
  inline
  Eigen::Matrix<typename boost::math::tools::promote_args<T0, T1>::type,
                Eigen::Dynamic, 1>
  operator()(const Eigen::Matrix<T0, Eigen::Dynamic, 1>& x,
             const Eigen::Matrix<T1, Eigen::Dynamic, 1>& y,
             const std::vector<double>& dat,
             const std::vector<int>& dat_int,
             std::ostream* pstream__) const {
    typedef typename boost::math::tools::promote_args<T0, T1>::type scalar;
    Eigen::Matrix<scalar, Eigen::Dynamic, 1> z(3);
    z(0) = x(2) - y(2);
    z(1) = x(0) * x(1) - y(0) * y(1);
    z(2) = (x(2) / x(0) + y(2) / y(0));
    return z;
  }
};

TEST(MathMatrix, nonLinearEq) {
  using stan::math::algebra_solver;
  using stan::math::sum;
  using stan::math::var;

  int n_x = 3, n_y = 3;
  for (int k = 0; k < n_x; k++) {
    Eigen::VectorXd x(n_x);
    x << -3, -3, 3;  // Note: need a pretty good guess to solve this one.
    Eigen::Matrix<var, Eigen::Dynamic, 1> y(n_y);
    y << 4, 6, 3;
    std::vector<double> dummy_dat(0);
    std::vector<int> dummy_dat_int(0);

    Eigen::Matrix<var, Eigen::Dynamic, 1> theta;

    theta = algebra_solver(nonLinearEq_functor(),
                           x, y, dummy_dat, dummy_dat_int);

    EXPECT_FLOAT_EQ(- y(0).val(), theta(0).val());
    EXPECT_FLOAT_EQ(- y(1).val(), theta(1).val());
    EXPECT_FLOAT_EQ(y(2).val(), theta(2).val());

    Eigen::MatrixXd J(n_x, n_y);
    J << -1, 0, 0, 0, -1, 0, 0, 0, 1;

    AVEC y_vec = createAVEC(y(0), y(1), y(2));
    VEC g;
    theta(k).grad(y_vec, g);

    double err = 1e-11;
    for (int i = 0; i < n_y; i++)
      EXPECT_NEAR(J(k, i), g[i], err);
  }
}

TEST(MathMatrix, nonLinearEq_dbl) {
  using stan::math::algebra_solver;

  int n_x = 3, n_y = 3;
  Eigen::VectorXd x(n_x);
  x << -3, -3, 3;  // Note: need a pretty good guess to solve this one.
  Eigen::VectorXd y(n_y);
  y << 4, 6, 3;
  std::vector<double> dummy_dat(0);
  std::vector<int> dummy_dat_int(0);

  Eigen::VectorXd theta;
  theta = algebra_solver(nonLinearEq_functor(),
                         x, y, dummy_dat, dummy_dat_int);

  EXPECT_FLOAT_EQ(- y(0), theta(0));
  EXPECT_FLOAT_EQ(- y(1), theta(1));
  EXPECT_FLOAT_EQ(y(2), theta(2));
}

///////////////////////////////////////////////////////////////////////////////

struct nonSquareEq_functor {
  template <typename T0, typename T1>
  inline
  Eigen::Matrix<typename boost::math::tools::promote_args<T0, T1>::type,
                Eigen::Dynamic, 1>
  operator()(const Eigen::Matrix<T0, Eigen::Dynamic, 1>& x,
             const Eigen::Matrix<T1, Eigen::Dynamic, 1>& y,
             const std::vector<double>& dat,
             const std::vector<int>& dat_int,
             std::ostream* pstream__) const {
    typedef typename boost::math::tools::promote_args<T0, T1>::type scalar;
    Eigen::Matrix<scalar, Eigen::Dynamic, 1> z(2);
    z(0) = x(0) - y(0);
    z(1) = x(1) * x(2) - y(1);
    return z;
  }
};

TEST(MathMatrix, error_conditions) {
  using stan::math::algebra_solver;
  using stan::math::sum;
  using stan::math::var;

  int n_x = 3, n_y = 2;
  Eigen::VectorXd x(n_x);
  x << 1, 1, 1;
  Eigen::Matrix<var, Eigen::Dynamic, 1> y(n_y);
  y << 4, 6;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);

  std::stringstream err_msg;
  err_msg << "algebra_solver: the ouput of the algebraic system has "
          << "dimension = 2, but should have the same dimension as x "
          << "(the vector of unknowns), which is: 3";
  std::string msg = err_msg.str();
  EXPECT_THROW_MSG(algebra_solver(nonSquareEq_functor(),
                                  x, y, dat, dat_int),
                   std::invalid_argument,
                   msg);

  Eigen::VectorXd x_bad(static_cast<Eigen::VectorXd::Index>(0));
  std::stringstream err_msg2;
  err_msg2 << "algebra_solver: initial guess has size 0, but "
            << "must have a non-zero size";
  std::string msg2 = err_msg2.str();
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x_bad, y, dat, dat_int),
                   std::invalid_argument,
                   msg2);

  typedef Eigen::Matrix<var, Eigen::Dynamic, 1> matrix_v;
  matrix_v y_bad(static_cast<matrix_v::Index>(0));
  std::stringstream err_msg3;
  err_msg3 << "algebra_solver: parameter vector has size 0, but "
           << "must have a non-zero size";
  std::string msg3 = err_msg3.str();
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y_bad, dat, dat_int),
                   std::invalid_argument,
                   msg3);

  double inf = std::numeric_limits<double>::infinity();
  Eigen::VectorXd x_bad_inf(n_x);
  x_bad_inf << inf, 1, 1;
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x_bad_inf, y, dat, dat_int),
                   std::domain_error,
                   "algebra_solver: initial guess is inf, but must be finite!");

  matrix_v y_bad_inf(3);
  y_bad_inf << inf, 1, 1;
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y_bad_inf, dat, dat_int),
                   std::domain_error,
                   "algebra_solver: parameter vector is inf, but must be finite!");

  std::vector<double> dat_bad_inf(1);
  dat_bad_inf[0] = inf;
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat_bad_inf, dat_int),
                   std::domain_error,
                   "algebra_solver: continuous data is inf, but must be finite!");

  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat, dat_int,
                                  0, -1, 1e-6, 1e+3),
                   std::invalid_argument,
                   "relative_tolerance");

  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat, dat_int,
                                  0, 1e-6, -1, 1e+3),
                   std::invalid_argument,
                   "function_tolerance");

  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat, dat_int,
                                  0, 1e-6, 1e-6, -1),
                   std::invalid_argument,
                   "max_num_steps");
}

TEST(MathMatrix, error_conditions_dbl) {
  using stan::math::algebra_solver;
  using stan::math::var;

  int n_x = 3, n_y = 2;
  Eigen::VectorXd x(n_x);
  x << 1, 1, 1;
  Eigen::VectorXd y(n_y);
  y << 4, 6;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);

  std::stringstream err_msg;
  err_msg << "algebra_solver: the ouput of the algebraic system has "
          << "dimension = 2, but should have the same dimension as x "
          << "(the vector of unknowns), which is: 3";
  std::string msg = err_msg.str();
  EXPECT_THROW_MSG(algebra_solver(nonSquareEq_functor(),
                                  x, y, dat, dat_int),
                   std::invalid_argument,
                   msg);

  Eigen::VectorXd x_bad(static_cast<Eigen::VectorXd::Index>(0));
  std::stringstream err_msg2;
  err_msg2 << "algebra_solver: initial guess has size 0, but "
            << "must have a non-zero size";
  std::string msg2 = err_msg2.str();
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x_bad, y, dat, dat_int),
                   std::invalid_argument,
                   msg2);

  typedef Eigen::VectorXd matrix_v;
  matrix_v y_bad(static_cast<matrix_v::Index>(0));
  std::stringstream err_msg3;
  err_msg3 << "algebra_solver: parameter vector has size 0, but "
           << "must have a non-zero size";
  std::string msg3 = err_msg3.str();
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y_bad, dat, dat_int),
                   std::invalid_argument,
                   msg3);

  double inf = std::numeric_limits<double>::infinity();
  Eigen::VectorXd x_bad_inf(n_x);
  x_bad_inf << inf, 1, 1;
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x_bad_inf, y, dat, dat_int),
                   std::domain_error,
                   "algebra_solver: initial guess is inf, but must be finite!");

  matrix_v y_bad_inf(3);
  y_bad_inf << inf, 1, 1;
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y_bad_inf, dat, dat_int),
                   std::domain_error,
                   "algebra_solver: parameter vector is inf, but must be finite!");

  std::vector<double> dat_bad_inf(1);
  dat_bad_inf[0] = inf;
  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat_bad_inf, dat_int),
                   std::domain_error,
                   "algebra_solver: continuous data is inf, but must be finite!");

  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat, dat_int,
                                  0, -1, 1e-6, 1e+3),
                   std::invalid_argument,
                   "relative_tolerance");

  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat, dat_int,
                                  0, 1e-6, -1, 1e+3),
                   std::invalid_argument,
                   "function_tolerance");

  EXPECT_THROW_MSG(algebra_solver(nonLinearEq_functor(),
                                  x, y, dat, dat_int,
                                  0, 1e-6, 1e-6, -1),
                   std::invalid_argument,
                   "max_num_steps");
}

///////////////////////////////////////////////////////////////////////////////

struct unsolvableEq_functor {
  template <typename T0, typename T1>
  inline
  Eigen::Matrix<typename boost::math::tools::promote_args<T0, T1>::type,
                Eigen::Dynamic, 1>
  operator()(const Eigen::Matrix<T0, Eigen::Dynamic, 1>& x,
             const Eigen::Matrix<T1, Eigen::Dynamic, 1>& y,
             const std::vector<double>& dat,
             const std::vector<int>& dat_int,
             std::ostream* pstream__) const {
    typedef typename boost::math::tools::promote_args<T0, T1>::type scalar;
    Eigen::Matrix<scalar, Eigen::Dynamic, 1> z(2);
    z(0) = x(0) * x(0) + y(0);
    z(1) = x(1) * x(1) + y(1);
    return z;
  }
};

TEST(MathMatrix, unsolvable) {
  using stan::math::algebra_solver;
  using stan::math::sum;
  using stan::math::var;

  Eigen::VectorXd x(2);
  x << 1, 1;
  Eigen::Matrix<var, Eigen::Dynamic, 1> y(2);
  y << 1, 1;  // should be positive
  Eigen::Matrix<var, Eigen::Dynamic, 1> theta;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);
  double relative_tolerance = 1e-6, function_tolerance = 1e-6;
  int max_num_steps = 1e+3;

  std::stringstream err_msg;
  err_msg << "algebra_solver: the norm of the algebraic function is: "
          << 1.41421  // sqrt(2)
          << " but should be lower than the function tolerance: "
          << function_tolerance
          << ". Consider increasing the relative tolerance and the"
          << " max_num_steps.";
  std::string msg = err_msg.str();
  EXPECT_THROW_MSG(algebra_solver(unsolvableEq_functor(),
                                  x, y, dat, dat_int, 0,
                                  relative_tolerance, function_tolerance,
                                  max_num_steps),
                   std::invalid_argument, msg);
}

TEST(MathMatrix, unsolvable_dbl) {
  using stan::math::algebra_solver;
  using stan::math::var;

  Eigen::VectorXd x(2);
  x << 1, 1;
  Eigen::VectorXd y(2);
  y << 1, 1;  // should be positive
  Eigen::VectorXd theta;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);
  double relative_tolerance = 1e-6, function_tolerance = 1e-6;
  int max_num_steps = 1e+3;

  std::stringstream err_msg;
  err_msg << "algebra_solver: the norm of the algebraic function is: "
          << 1.41421  // sqrt(2)
          << " but should be lower than the function tolerance: "
          << function_tolerance
          << ". Consider increasing the relative tolerance and the"
          << " max_num_steps.";
  std::string msg = err_msg.str();
  EXPECT_THROW_MSG(algebra_solver(unsolvableEq_functor(),
                                  x, y, dat, dat_int, 0,
                                  relative_tolerance, function_tolerance,
                                  max_num_steps),
                   std::invalid_argument, msg);
}

TEST(MathMatrix, max_num_steps) {
  using stan::math::algebra_solver;
  using stan::math::var;

  Eigen::VectorXd x(2);
  x << 1, 1;
  Eigen::Matrix<var, Eigen::Dynamic, 1> y(2);
  y << 1, 1;
  Eigen::VectorXd theta;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);
  double relative_tolerance = 1e-6, function_tolerance = 1e-6;
  int max_num_steps = 2;  // very low for test

  std::stringstream err_msg;
  err_msg << "algebra_solver: max number of iterations: "
          << max_num_steps
          << " exceeded.";
  std::string msg = err_msg.str();
  EXPECT_THROW_MSG(algebra_solver(unsolvableEq_functor(),
                                  x, y, dat, dat_int, 0,
                                  relative_tolerance, function_tolerance,
                                  max_num_steps),
                   std::invalid_argument, msg);
}

TEST(MathMatrix, max_num_steps_dbl) {
  using stan::math::algebra_solver;
  using stan::math::var;

  Eigen::VectorXd x(2), y(2);
  x << 1, 1;
  y << 1, 1;
  Eigen::VectorXd theta;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);
  double relative_tolerance = 1e-6, function_tolerance = 1e-6;
  int max_num_steps = 2;  // very low for test

  std::stringstream err_msg;
  err_msg << "algebra_solver: max number of iterations: "
          << max_num_steps
          << " exceeded.";
  std::string msg = err_msg.str();
  EXPECT_THROW_MSG(algebra_solver(unsolvableEq_functor(),
                                  x, y, dat, dat_int, 0,
                                  relative_tolerance, function_tolerance,
                                  max_num_steps),
                   std::invalid_argument, msg);
}

///////////////////////////////////////////////////////////////////////////////

// Degenerate roots: each solution can either be y(0) or y(1).
struct degenerateEq_functor {
  template <typename T0, typename T1>
  inline
  Eigen::Matrix<typename boost::math::tools::promote_args<T0, T1>::type,
                Eigen::Dynamic, 1>
  operator()(const Eigen::Matrix<T0, Eigen::Dynamic, 1>& x,
             const Eigen::Matrix<T1, Eigen::Dynamic, 1>& y,
             const std::vector<double>& dat,
             const std::vector<int>& dat_int,
             std::ostream* pstream__) const {
    typedef typename boost::math::tools::promote_args<T0, T1>::type scalar;
    Eigen::Matrix<scalar, Eigen::Dynamic, 1> z(2);
    z(0) = (x(0) - y(0)) * (x(1) - y(1));
    z(1) = (x(0) - y(1)) * (x(1) - y(0));
    return z;
  }
};

TEST(MathMatrix, degenerate) {
  using stan::math::algebra_solver;
  using stan::math::sum;
  using stan::math::var;

  int n_x = 2, n_y = 2;

  // This first initial guess produces the
  // solution x = {8, 8}
  for (int k = 0; k < n_x; k++) {
    Eigen::Matrix<var, Eigen::Dynamic, 1> y(2);
    y << 5, 8;
    Eigen::VectorXd x(2);
    x << 10, 1;  // Initial Guess
    Eigen::Matrix<var, Eigen::Dynamic, 1> theta;
    std::vector<double> dat(0);
    std::vector<int> dat_int(0);
    theta = algebra_solver(degenerateEq_functor(),
                         x, y, dat, dat_int);
    EXPECT_EQ(8, theta(0));
    EXPECT_EQ(8, theta(1));

    Eigen::MatrixXd J(n_x, n_y);
    J << 0, 1, 0, 1;

    AVEC y_vec = createAVEC(y(0), y(1));
    VEC g;
    theta(k).grad(y_vec, g);

    for (int l = 0; l < n_y; l++)
      EXPECT_EQ(J(k, l), g[l]);
  }

  // This next initial guess produces the
  // solution x = {5, 5}
  for (int k = 0; k < 1; k++) {
    Eigen::Matrix<var, Eigen::Dynamic, 1> y(2);
    y << 5, 8;
    Eigen::VectorXd x(2);
    x << 1, 1;  // Initial Guess
    Eigen::Matrix<var, Eigen::Dynamic, 1> theta;
    std::vector<double> dat(0);
    std::vector<int> dat_int(0);
    theta = algebra_solver(degenerateEq_functor(),
                           x, y, dat, dat_int);
    EXPECT_FLOAT_EQ(5, theta(0).val());
    EXPECT_FLOAT_EQ(5, theta(0).val());

    Eigen::MatrixXd J(n_x, n_y);
    J << 1, 0, 1, 0;

    AVEC y_vec = createAVEC(y(0), y(1));
    VEC g;
    theta(k).grad(y_vec, g);

    for (int l = 0; l < n_y; l++)
      EXPECT_NEAR(J(k, l), g[l], 1e-13);
  }
}

TEST(MathMatrix, degenerate_dbl) {
  using stan::math::algebra_solver;
  using stan::math::var;

  // This first initial guess produces the
  // solution x = {8, 8}
  Eigen::VectorXd y(2);
  y << 5, 8;
  Eigen::VectorXd x(2);
  x << 10, 1;  // Initial Guess
  Eigen::VectorXd theta;
  std::vector<double> dat(0);
  std::vector<int> dat_int(0);
  theta = algebra_solver(degenerateEq_functor(),
                         x, y, dat, dat_int);
  EXPECT_EQ(8, theta(0));
  EXPECT_EQ(8, theta(1));

  // This next initial guess produces the
  // solution x = {5, 5}
  x << 1, 1;  // Initial Guess
  theta = algebra_solver(degenerateEq_functor(),
                         x, y, dat, dat_int);
  EXPECT_FLOAT_EQ(5, theta(0));
  EXPECT_FLOAT_EQ(5, theta(0));
}
