#include <stan/math/rev/mat.hpp>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>
#include <test/unit/math/prim/arr/functor/harmonic_oscillator.hpp>
#include <test/unit/math/prim/arr/functor/mock_ode_functor.hpp>
#include <test/unit/math/prim/arr/functor/mock_throwing_ode_functor.hpp>

struct StanMathRevOdeCVode : public ::testing::Test {
  void SetUp() {
    stan::math::recover_memory();
    ts = std::vector<double>(1,10);
    t0 = 0;
    
  }
  std::stringstream msgs;
  std::vector<double> x;
  std::vector<int> x_int;
  double t0;
  std::vector<double> ts;
  N_Vector tmp1;
  N_Vector tmp2;
};

// helper object to initialize data structures which we can pass into
// the cvodes integrator object
struct cvodes2coupled {
  const size_t N_;
  const size_t S_;
  std::vector<double> state_;
  std::vector<double> state_dot_;
  double* p_state_;
  double* p_state_dot_;
  N_Vector  cvode_state_;
  N_Vector  cvode_state_dot_;
  N_Vector *cvode_state_sens_;
  N_Vector *cvode_state_sens_dot_;
  
  cvodes2coupled(const size_t N,
		 const size_t S)
    : N_(N), S_(S),
      state_(N, 0),
      state_dot_(N, 0),
      p_state_(&state_[0]),
      p_state_dot_(&state_dot_[0]),
      cvode_state_(N_VMake_Serial(N_, &state_[0])),
      cvode_state_dot_(N_VMake_Serial(N_, &state_dot_[0]))
  {
    if(S_ > 0) {
      cvode_state_sens_     = N_VCloneVectorArray_Serial(S_, cvode_state_);
      cvode_state_sens_dot_ = N_VCloneVectorArray_Serial(S_, cvode_state_);

      for(size_t s=0; s < S_ ; s++) N_VConst(RCONST(0.0), cvode_state_sens_[s]);
      for(size_t s=0; s < S_ ; s++) N_VConst(RCONST(0.0), cvode_state_sens_dot_[s]);
    }
  }

  ~cvodes2coupled() {
    // N_VDestroy_Serial should noop because
    // N_VMake_Serial sets own_data to false
    N_VDestroy_Serial(cvode_state_);
    if(S_ > 0) {
      N_VDestroyVectorArray_Serial(cvode_state_sens_    , S_);
      N_VDestroyVectorArray_Serial(cvode_state_sens_dot_, S_);
    }
  }

  std::vector<double> get_coupled_state() const {
    std::vector<double> y_coupled(N_ + N_ * S_, 0);

    std::copy(state_.begin(), state_.end(), y_coupled.begin());

    for(size_t s = 0; s < S_; s++) {
      for(size_t n = 0; n < N_; n++) {
	y_coupled[N_ + s * N_ + n] = NV_Ith_S(cvode_state_sens_[s], n);
      }
    }
    
    return(y_coupled);
  }

  std::vector<double> get_coupled_state_dot() const {
    std::vector<double> y_coupled_dot(N_ + N_ * S_, 0);

    std::copy(state_dot_.begin(), state_dot_.end(), y_coupled_dot.begin());

    for(size_t s = 0; s < S_; s++) {
      for(size_t n = 0; n < N_; n++) {
	y_coupled_dot[N_ + s * N_ + n] = NV_Ith_S(cvode_state_sens_dot_[s], n);
      }
    }
    
    return(y_coupled_dot);
  }
};

// ******************** DV ****************************
TEST_F(StanMathRevOdeCVode, cvodes_integrator_dv) {
  using stan::math::cvodes_integrator;

  harm_osc_ode_fun harm_osc;

  const size_t N = 2;
  const int M = 1;
  const size_t S = 1;

  std::vector<stan::math::var> theta;
  //std::vector<double> coupled_y0;
  std::vector<double> y0;
  //std::vector<double> dy_dt(N);

  double gamma(0.15);
  t0 = 0;

  theta.push_back(gamma);
  y0.push_back(1.0);
  y0.push_back(0.5);

  /*
  coupled_y0.push_back(1.0);
  coupled_y0.push_back(0.5);
  coupled_y0.push_back(1.0);
  coupled_y0.push_back(2.0);
  */
  
  cvodes_integrator<harm_osc_ode_fun, double, stan::math::var>
    integrator(harm_osc, y0, t0, theta, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  cvodes2coupled nvec(N, S);

  nvec.state_[0] = 1.0;
  nvec.state_[1] = 0.5;
  NV_Ith_S(nvec.cvode_state_sens_[0], 0) = 1.0;
  NV_Ith_S(nvec.cvode_state_sens_[0], 1) = 2.0;

  integrator.ode_rhs(t0, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator);
  
  integrator.ode_rhs_sens(M, t0,
                          nvec.cvode_state_,
                          nvec.cvode_state_dot_,
                          nvec.cvode_state_sens_,
                          nvec.cvode_state_sens_dot_,
                          &integrator,
                          tmp1, tmp2
                          );

  std::vector<double> dy_dt_coupled = nvec.get_coupled_state_dot();

  EXPECT_FLOAT_EQ(0.5, dy_dt_coupled[0]);
  EXPECT_FLOAT_EQ(-1.075, dy_dt_coupled[1]);
  EXPECT_FLOAT_EQ(2, dy_dt_coupled[2]);
  EXPECT_FLOAT_EQ(-1.8, dy_dt_coupled[3]);
}
/* obsolete
TEST_F(StanMathRevOdeCVode, initial_state_dv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;

  std::vector<double> y0_d(N, 0.0);
  std::vector<double> theta(M, 0.0);
  std::vector<var> theta_v(M, 0.0);

  for (size_t n = 0; n < N; n++)
    y0_d[n] = n+1;
  for (size_t m = 0; m < M; m++) {
    theta[m] = 10 * (m+1);
    theta_v[m] = 10 * (m+1);
  }

  cvodes_integrator<mock_ode_functor, double, var>
    integrator_dv(base_ode, y0_d, t0, theta, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  std::vector<double> state = integrator_dv.initial_state();
  for (size_t n = 0; n < N; n++)
    EXPECT_FLOAT_EQ(y0_d[n], state[n])
      << "initial state gets the initial values";
  for (size_t n = N; n < state.size(); n++)
    EXPECT_FLOAT_EQ(0.0, state[n]);
}
*/
/* obsolete
TEST_F(StanMathRevOdeCVode, size_dv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;

  std::vector<double> y0_d(N, 0.0);
  std::vector<var> theta(M, 0.0);

  cvodes_integrator<mock_ode_functor, double, var>
    integrator_dv(base_ode, y0_d, t0, theta, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  EXPECT_EQ(N + N * M, integrator_dv.size());
}
*/
TEST_F(StanMathRevOdeCVode, memory_recovery_dv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;
  const size_t S = M;

  std::vector<double> y0_d(N, 0.0);
  std::vector<var> theta(M, 0.0);

  cvodes_integrator<mock_ode_functor, double, var>
    integrator_dv(base_ode, y0_d, t0, theta, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  //std::vector<double> y(3,0);
  //std::vector<double> dy_dt(3,0);
  double t = 10;

  cvodes2coupled nvec(N, S);

  EXPECT_TRUE(stan::math::empty_nested());
  EXPECT_NO_THROW(integrator_dv.ode_rhs(t, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator_dv));
  
  EXPECT_NO_THROW(integrator_dv.ode_rhs_sens((int)M, t,
                                             nvec.cvode_state_,
                                             nvec.cvode_state_dot_,
                                             nvec.cvode_state_sens_,
                                             nvec.cvode_state_sens_dot_,
                                             &integrator_dv,
                                             tmp1, tmp2
                                             ));
  EXPECT_TRUE(stan::math::empty_nested());
}

TEST_F(StanMathRevOdeCVode, memory_recovery_exception_dv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  std::string message = "ode throws";

  const size_t N = 3;
  const size_t M = 4;
  const size_t S = M;

  for (size_t n = 0; n < N+1; n++) {
    std::stringstream scoped_message;
    scoped_message << "iteration " << n;
    SCOPED_TRACE(scoped_message.str());
    mock_throwing_ode_functor<std::logic_error> throwing_ode(message, 1);

    std::vector<double> y0_d(N, 0.0);
    std::vector<var> theta(M, 0.0);

    cvodes_integrator<mock_throwing_ode_functor<std::logic_error>,
                      double, var>
      integrator_dv(throwing_ode, y0_d, t0, theta, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

    std::vector<double> y(3,0);
    std::vector<double> dy_dt(3,0);
    double t = 10;

    cvodes2coupled nvec(N, S);
    
    EXPECT_TRUE(stan::math::empty_nested());
    EXPECT_THROW_MSG(integrator_dv.ode_rhs(t, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator_dv),
                     std::logic_error,
                     message);
    EXPECT_TRUE(stan::math::empty_nested());
  }
}

// ******************** VD ****************************

TEST_F(StanMathRevOdeCVode, cvodes_integrator_vd) {
  using stan::math::cvodes_integrator;

  harm_osc_ode_fun harm_osc;

  const size_t N = 2;
  const int M = 1;
  const size_t S = N;

  std::vector<double> theta;
  //std::vector<double> coupled_y0;
  std::vector<stan::math::var> y0_var;
  std::vector<double> y0_d;
  double t0;
  //std::vector<double> dy_dt;

  //stan::math::var dummy2;
  //dummy2 = 1.0;

  double gamma(0.15);
  t0 = 0;

  theta.push_back(gamma);


  // note: here the old test was inconsistent in that coupled_y0[0]
  // and coupled_y0[1] should have been set to 0 instead.
  
  //coupled_y0.push_back(1.0);
  //coupled_y0.push_back(0.5);
  //coupled_y0.push_back(1.0);
  //coupled_y0.push_back(3.0);
  //coupled_y0.push_back(2.0);
  //coupled_y0.push_back(5.0);
  
  y0_var.push_back(1.0);
  y0_var.push_back(0.5);
  y0_d.push_back(1.0);
  y0_d.push_back(0.5);

  cvodes_integrator<harm_osc_ode_fun, stan::math::var, double>
    integrator(harm_osc, y0_var, t0, theta, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  cvodes2coupled nvec(N, S);

  //nvec.state_[0] = 2*y0_d[0]; // use this to match old expectation
  //nvec.state_[1] = 2*y0_d[1]; // use this to match old expectation
  nvec.state_[0] = y0_d[0];
  nvec.state_[1] = y0_d[1];
  NV_Ith_S(nvec.cvode_state_sens_[0], 0) = 1.0 + 1.0;
  NV_Ith_S(nvec.cvode_state_sens_[0], 1) = 3.0;
  NV_Ith_S(nvec.cvode_state_sens_[1], 0) = 2.0;
  NV_Ith_S(nvec.cvode_state_sens_[1], 1) = 5.0 + 1.0;

  integrator.ode_rhs(t0, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator);
  
  integrator.ode_rhs_sens(M, t0,
                          nvec.cvode_state_,
                          nvec.cvode_state_dot_,
                          nvec.cvode_state_sens_,
                          nvec.cvode_state_sens_dot_,
                          &integrator,
                          tmp1, tmp2
                          );

  std::vector<double> dy_dt_coupled = nvec.get_coupled_state_dot();

  //EXPECT_FLOAT_EQ(1.0, dy_dt_coupled[0]); // old expectation
  //EXPECT_FLOAT_EQ(-2.0 - 0.15 * 1.0, dy_dt_coupled[1]); // old expectation
  EXPECT_FLOAT_EQ(0.5, dy_dt_coupled[0]);
  EXPECT_FLOAT_EQ(-1.0 - 0.15 * 0.5, dy_dt_coupled[1]);
  EXPECT_FLOAT_EQ(0 + 1.0 * 0 + 3.0 * 1 + 0, dy_dt_coupled[2]);
  EXPECT_FLOAT_EQ(-1.0 - 1.0 * 1.0 - 0.15 * 3.0, dy_dt_coupled[3]);
  EXPECT_FLOAT_EQ(1.0 + 2.0 * 0 + 5.0 * 1.0, dy_dt_coupled[4]);
  EXPECT_FLOAT_EQ(-0.15 - 1.0 * 2.0 - 0.15 * 5.0, dy_dt_coupled[5]);
}
/* obsolete
TEST_F(StanMathRevOdeCVode, initial_state_vd) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;

  std::vector<double> y0_d(N, 0.0);
  std::vector<double> theta_d(M, 0.0);

  for (size_t n = 0; n < N; n++)
    y0_d[n] = n + 1;
  for (size_t m = 0; m < M; m++)
    theta_d[m] = 10 * (m + 1);

  cvodes_integrator<mock_ode_functor, var, double>
    integrator_vd(base_ode, y0_d, t0, theta_d, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  std::vector<double> state;

  state = integrator_vd.initial_state();
  for (size_t n = 0; n < N; n++)
    EXPECT_FLOAT_EQ(y0_d[n], state[n])
      << "initial state";
  for (size_t s = 0; s < N; s++)
    for (size_t n = 0; n < N; n++)
      EXPECT_FLOAT_EQ(n == s ? 1.0 : 0.0, state[N + s*N + n]);
}
*/
/* obsolete
TEST_F(StanMathRevOdeCVode, size_vd) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;

  std::vector<var> y0_v(N, 0.0);
  std::vector<double> theta_d(M, 0.0);

  cvodes_integrator<mock_ode_functor, var, double>
    integrator_vd(base_ode, y0_v, t0, theta_d, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  EXPECT_EQ(N + N * N, integrator_vd.size());
}
*/
TEST_F(StanMathRevOdeCVode, memory_recovery_vd) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;
  const size_t S = N;

  std::vector<var> y0_v(N, 0.0);
  std::vector<double> theta_d(M, 0.0);

  cvodes_integrator<mock_ode_functor, var, double>
    integrator_vd(base_ode, y0_v, t0, theta_d, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  //std::vector<double> y(3,0);
  //std::vector<double> dy_dt(3,0);
  double t = 10;

  cvodes2coupled nvec(N, S);

  EXPECT_TRUE(stan::math::empty_nested());
  EXPECT_NO_THROW(integrator_vd.ode_rhs(t, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator_vd));
  
  EXPECT_NO_THROW(integrator_vd.ode_rhs_sens((int)M, t,
                                             nvec.cvode_state_,
                                             nvec.cvode_state_dot_,
                                             nvec.cvode_state_sens_,
                                             nvec.cvode_state_sens_dot_,
                                             &integrator_vd,
                                             tmp1, tmp2
                                             ));
  EXPECT_TRUE(stan::math::empty_nested());
}

TEST_F(StanMathRevOdeCVode, memory_recovery_exception_vd) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  std::string message = "ode throws";

  const size_t N = 3;
  const size_t M = 4;
  const size_t S = N;
  for (size_t n = 0; n < N+1; n++) {
    std::stringstream scoped_message;
    scoped_message << "iteration " << n;
    SCOPED_TRACE(scoped_message.str());
    mock_throwing_ode_functor<std::logic_error> throwing_ode(message, 1);

    std::vector<var> y0_v(N, 0.0);
    std::vector<double> theta_d(M, 0.0);

    cvodes_integrator<mock_throwing_ode_functor<std::logic_error>,
                             var, double>
      integrator_vd(throwing_ode, y0_v, t0, theta_d, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

    double t = 10;

    cvodes2coupled nvec(N, S);

    EXPECT_TRUE(stan::math::empty_nested());
    EXPECT_THROW_MSG(integrator_vd.ode_rhs(t, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator_vd),
                     std::logic_error,
                     message);
    EXPECT_TRUE(stan::math::empty_nested());
  }
}


// ******************** VV ****************************

TEST_F(StanMathRevOdeCVode, cvodes_integrator_vv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;

  const size_t N = 2;
  const int M = 1;
  const size_t S = 1 + 2;

  std::vector<var> y0_v;
  y0_v.push_back(1.0);
  y0_v.push_back(0.5);

  std::vector<var> theta_v;
  theta_v.push_back(0.15);

  harm_osc_ode_fun harm_osc;

  cvodes_integrator<harm_osc_ode_fun, stan::math::var, stan::math::var>
    integrator(harm_osc, y0_v, t0, theta_v, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  //std::vector<double> coupled_y0(8, 0);

  t0 = 0;

  //std::vector<double> dy_dt;
  //system(coupled_y0, dy_dt, t0);

  std::vector<double> y0_d(2);
  y0_d[0] = 1.0;
  y0_d[1] = 0.5;

  std::vector<double> theta_d(1);
  theta_d[0] = 0.15;

  cvodes2coupled nvec(N, S);

  nvec.state_ = y0_d;
  NV_Ith_S(nvec.cvode_state_sens_[0], 0) = 1.0;
  NV_Ith_S(nvec.cvode_state_sens_[0], 1) = 0.0;
  NV_Ith_S(nvec.cvode_state_sens_[1], 0) = 0.0;
  NV_Ith_S(nvec.cvode_state_sens_[1], 1) = 1.0;

  integrator.ode_rhs(t0, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator);
  
  integrator.ode_rhs_sens(M, t0,
                          nvec.cvode_state_,
                          nvec.cvode_state_dot_,
                          nvec.cvode_state_sens_,
                          nvec.cvode_state_sens_dot_,
                          &integrator,
                          tmp1, tmp2
                          );

  std::vector<double> dy_dt_coupled = nvec.get_coupled_state_dot();
  
  std::vector<double>
    dy_dt_base = harm_osc(0.0, y0_d, theta_d, x, x_int, &msgs);

  EXPECT_FLOAT_EQ(dy_dt_base[0], dy_dt_coupled[0]);
  EXPECT_FLOAT_EQ(dy_dt_base[1], dy_dt_coupled[1]);
  EXPECT_FLOAT_EQ(0, dy_dt_coupled[2]);
  EXPECT_FLOAT_EQ(-1, dy_dt_coupled[3]);
  EXPECT_FLOAT_EQ(1, dy_dt_coupled[4]);
  EXPECT_FLOAT_EQ(-0.15, dy_dt_coupled[5]);
  EXPECT_FLOAT_EQ(0, dy_dt_coupled[6]);
  EXPECT_FLOAT_EQ(-0.5, dy_dt_coupled[7]);
}
/* obsolete
TEST_F(StanMathRevOdeCVode, initial_state_vv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;

  std::vector<double> y0_d(N, 0.0);
  std::vector<double> theta_d(M, 0.0);

  for (size_t n = 0; n < N; n++)
    y0_d[n] = n+1;
  for (size_t m = 0; m < M; m++)
    theta_d[m] = 10 * (m+1);

  cvodes_integrator<mock_ode_functor, var, var>
    integrator_vv(base_ode, y0_d, t0, theta_d, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  std::vector<double>  state = integrator_vv.initial_state();
  for (size_t n = 0; n < N; n++)
    EXPECT_FLOAT_EQ(y0_d[n], state[n])
      << "initial state";
  for (size_t s = 0; s < N; s++)
    for (size_t n = 0; n < N; n++)
      EXPECT_FLOAT_EQ(n == s ? 1.0 : 0.0, state[N + s*N + n]);
  for (size_t s = N; s < N+M; s++)
    for (size_t n = 0; n < N; n++)
      EXPECT_FLOAT_EQ(0.0, state[N + s*N + n]);
}
*/
/* obsolete
TEST_F(StanMathRevOdeCVode, size_vv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;

  std::vector<var> y0_v(N, 0.0);
  std::vector<var> theta_v(M, 0.0);

  cvodes_integrator<mock_ode_functor, var, var>
    integrator_vv(base_ode, y0_v, t0, theta_v, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  EXPECT_EQ(N + N * N + N * M, integrator_vv.size());
}
*/
TEST_F(StanMathRevOdeCVode, memory_recovery_vv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  mock_ode_functor base_ode;

  const size_t N = 3;
  const size_t M = 4;
  const size_t S = N + M;

  std::vector<var> y0_v(N, 0.0);
  std::vector<var> theta_v(M, 0.0);

  cvodes_integrator<mock_ode_functor, var, var>
    integrator_vv(base_ode, y0_v, t0, theta_v, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

  //std::vector<double> y(3,0);
  //std::vector<double> dy_dt(3,0);
  double t = 10;

  cvodes2coupled nvec(N, S);

  EXPECT_TRUE(stan::math::empty_nested());
  EXPECT_NO_THROW(integrator_vv.ode_rhs(t, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator_vv));
  
  EXPECT_NO_THROW(integrator_vv.ode_rhs_sens((int)M, t,
                                             nvec.cvode_state_,
                                             nvec.cvode_state_dot_,
                                             nvec.cvode_state_sens_,
                                             nvec.cvode_state_sens_dot_,
                                             &integrator_vv,
                                             tmp1, tmp2
                                             ));
  EXPECT_TRUE(stan::math::empty_nested());
}

TEST_F(StanMathRevOdeCVode, memory_recovery_exception_vv) {
  using stan::math::cvodes_integrator;
  using stan::math::var;
  std::string message = "ode throws";

  const size_t N = 3;
  const size_t M = 4;
  const size_t S = N+M;
  for (size_t n = 0; n < N + 1; n++) {
    std::stringstream scoped_message;
    scoped_message << "iteration " << n;
    SCOPED_TRACE(scoped_message.str());
    mock_throwing_ode_functor<std::logic_error> throwing_ode(message, 1);

    std::vector<var> y0_v(N, 0.0);
    std::vector<var> theta_v(M, 0.0);

    cvodes_integrator<mock_throwing_ode_functor<std::logic_error>, var, var>
      integrator_vv(throwing_ode, y0_v, t0, theta_v, x, x_int, ts, 1e-8, 1e-10, 1e6, 1, &msgs);

    double t = 10;

    cvodes2coupled nvec(N, S);

    EXPECT_TRUE(stan::math::empty_nested());
    EXPECT_THROW_MSG(integrator_vv.ode_rhs(t, nvec.cvode_state_, nvec.cvode_state_dot_, &integrator_vv),
                     std::logic_error,
                     message);
    EXPECT_TRUE(stan::math::empty_nested());
  }
}

