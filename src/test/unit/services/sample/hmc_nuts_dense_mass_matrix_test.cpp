#include <stan/services/sample/hmc_nuts_dense_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_dense_e.hpp>
#include <gtest/gtest.h>
#include <stan/io/empty_var_context.hpp>
#include <test/test-models/good/mcmc/hmc/common/gauss3D.hpp>
#include <test/unit/services/instrumented_callbacks.hpp>
#include <test/unit/services/check_adaptation.hpp>
#include <iostream>

/** 
 * Use 3-param model test-models/good/mcmc/hmc/common/gauss3D
 * fix seed 12345, test against specified inv mass matrix
 * Tests crafted by running samplers with test config
 * to capture resulting inverse mass matrix values.
 */

class ServicesSampleHmcNutsDenseEMassMatrix : public testing::Test {
public:
  ServicesSampleHmcNutsDenseEMassMatrix()
    : model(context, &model_log) {}

  std::stringstream model_log;
  stan::test::unit::instrumented_writer message, init, error;
  stan::test::unit::instrumented_writer parameter, diagnostic;
  stan::io::empty_var_context context;
  stan_model model;
};

TEST_F(ServicesSampleHmcNutsDenseEMassMatrix, ident_no_adapt) {
  unsigned int random_seed = 12345;
  unsigned int chain = 1;
  double init_radius = 2;
  int num_warmup = 0;
  int num_samples = 2;
  int num_thin = 1;
  bool save_warmup = false;
  int refresh = 0;
  double stepsize = 1;
  double stepsize_jitter = 0;
  int max_depth = 10;
  stan::test::unit::instrumented_interrupt interrupt;
  EXPECT_EQ(interrupt.call_count(), 0);

  int return_code =
    stan::services::sample::hmc_nuts_dense_e(model,
                                             context,
                                             random_seed,
                                             chain,
                                             init_radius,
                                             num_warmup,
                                             num_samples,
                                             num_thin,
                                             save_warmup,
                                             refresh,
                                             stepsize,
                                             stepsize_jitter,
                                             max_depth,
                                             interrupt,
                                             message,
                                             error,
                                             init,
                                             parameter,
                                             diagnostic);
  EXPECT_EQ(0, return_code);

  stan::io::dump dmp = 
    stan::services::util::create_ident_dense_mass_matrix(3);
  stan::io::var_context& inv_mass_matrix = dmp;
  std::vector<double> dense_vals
    = inv_mass_matrix.vals_r("mass_matrix");
  // check returned mass matrix
  stan::test::unit::check_adaptation(3, 3, dense_vals, parameter, 0.05);
}

TEST_F(ServicesSampleHmcNutsDenseEMassMatrix, ident_adapt_250) {
  unsigned int random_seed = 12345;
  unsigned int chain = 1;
  double init_radius = 2;
  int num_warmup = 250;
  int num_samples = 0;
  int num_thin = 1;
  bool save_warmup = false;
  int refresh = 0;
  double stepsize = 1;
  double stepsize_jitter = 0;
  int max_depth = 10;
  double delta = .8;
  double gamma = .05;
  double kappa = .75;
  double t0 = 10;
  unsigned int init_buffer = 75;
  unsigned int term_buffer = 50;
  unsigned int window = 25;
  stan::test::unit::instrumented_interrupt interrupt;
  EXPECT_EQ(interrupt.call_count(), 0);

  // run sampler with ident mass matrix, 250 warmups
  int return_code =
    stan::services::sample::hmc_nuts_dense_e_adapt(model,
                                                   context,
                                                   random_seed,
                                                   chain,
                                                   init_radius,
                                                   num_warmup,
                                                   num_samples,
                                                   num_thin,
                                                   save_warmup,
                                                   refresh,
                                                   stepsize,
                                                   stepsize_jitter,
                                                   max_depth,
                                                   delta,
                                                   gamma,
                                                   kappa,
                                                   t0,
                                                   init_buffer,
                                                   term_buffer,
                                                   window,
                                                   interrupt,
                                                   message,
                                                   error,
                                                   init,
                                                   parameter,
                                                   diagnostic);
  EXPECT_EQ(0, return_code);

  // result of running adaptive sampler w/ ident mass matrix:
  // 1.21622, -0.0271004, 0.00515814
  // -0.0271004, 0.59758, -0.0153581
  // 0.00515814, -0.0153581, 0.863086
  std::vector<double> dense_vals;
  dense_vals.push_back(1.21622);
  dense_vals.push_back(-0.0271004);
  dense_vals.push_back(0.00515814);
  dense_vals.push_back(-0.0271004);
  dense_vals.push_back(0.59758);
  dense_vals.push_back(-0.0153581);
  dense_vals.push_back(0.00515814);
  dense_vals.push_back(-0.0153581);
  dense_vals.push_back(0.863086);
  stan::test::unit::check_adaptation(3, 3, dense_vals, parameter, 0.1);
}

TEST_F(ServicesSampleHmcNutsDenseEMassMatrix, use_mass_matrix_no_adapt) {
  unsigned int random_seed = 12345;
  unsigned int chain = 1;
  double init_radius = 2;
  int num_warmup = 0;
  int num_samples = 2;
  int num_thin = 1;
  bool save_warmup = false;
  int refresh = 0;
  double stepsize = 1;
  double stepsize_jitter = 0;
  int max_depth = 10;
  stan::test::unit::instrumented_interrupt interrupt;
  EXPECT_EQ(interrupt.call_count(), 0);

  std::string txt =
    "mass_matrix <- structure(c("
    "0.640211, 0.156096, -0.374048, "
    "0.156096, 1.41239, -0.0412753, "
    "-0.374048, -0.0412753, 1.29567 "
    "), .Dim  = c(3,3))";
  std::stringstream in(txt);
  stan::io::dump dump(in);
  stan::io::var_context& inv_mass_matrix = dump;

  int return_code =
    stan::services::sample::hmc_nuts_dense_e(model,
                                             context,
                                             inv_mass_matrix,
                                             random_seed,
                                             chain,
                                             init_radius,
                                             num_warmup,
                                             num_samples,
                                             num_thin,
                                             save_warmup,
                                             refresh,
                                             stepsize,
                                             stepsize_jitter,
                                             max_depth,
                                             interrupt,
                                             message,
                                             error,
                                             init,
                                             parameter,
                                             diagnostic);
  EXPECT_EQ(0, return_code);

  std::vector<double> dense_vals(9);
  dense_vals = inv_mass_matrix.vals_r("mass_matrix");
  stan::test::unit::check_adaptation(3, 3, dense_vals, parameter, 0.05);
}

TEST_F(ServicesSampleHmcNutsDenseEMassMatrix, use_mass_matrix_skip_adapt) {
  unsigned int random_seed = 12345;
  unsigned int chain = 1;
  double init_radius = 2;
  int num_warmup = 0;
  int num_samples = 2;
  int num_thin = 1;
  bool save_warmup = false;
  int refresh = 0;
  double stepsize = 1;
  double stepsize_jitter = 0;
  int max_depth = 10;
  double delta = .8;
  double gamma = .05;
  double kappa = .75;
  double t0 = 10;
  unsigned int init_buffer = 75;
  unsigned int term_buffer = 50;
  unsigned int window = 25;
  stan::test::unit::instrumented_interrupt interrupt;
  EXPECT_EQ(interrupt.call_count(), 0);

  std::string txt =
    "mass_matrix <- structure(c("
    "0.640211, 0.156096, -0.374048, "
    "0.156096, 1.41239, -0.0412753, "
    "-0.374048, -0.0412753, 1.29567 "
    "), .Dim  = c(3,3))";
  std::stringstream in(txt);
  stan::io::dump dump(in);
  stan::io::var_context& inv_mass_matrix = dump;

  int return_code =
    stan::services::sample::hmc_nuts_dense_e_adapt(model,
                                                   context,
                                                   inv_mass_matrix,
                                                   random_seed,
                                                   chain,
                                                   init_radius,
                                                   num_warmup,
                                                   num_samples,
                                                   num_thin,
                                                   save_warmup,
                                                   refresh,
                                                   stepsize,
                                                   stepsize_jitter,
                                                   max_depth,
                                                   delta,
                                                   gamma,
                                                   kappa,
                                                   t0,
                                                   init_buffer,
                                                   term_buffer,
                                                   window,
                                                   interrupt,
                                                   message,
                                                   error,
                                                   init,
                                                   parameter,
                                                   diagnostic);

  EXPECT_EQ(0, return_code);

  std::vector<double> dense_vals(9);
  dense_vals = inv_mass_matrix.vals_r("mass_matrix");
  stan::test::unit::check_adaptation(3, 3, dense_vals, parameter, 0.05);
}

// run model for 300 iterations, starting w/ non-ident mass matrix
// mass matrix should change but evolve to identity matrix
TEST_F(ServicesSampleHmcNutsDenseEMassMatrix, use_mass_matrix_continue_300) {
  unsigned int random_seed = 12345;
  unsigned int chain = 1;
  double init_radius = 2;
  int num_warmup = 300;
  int num_samples = 0;
  int num_thin = 5;
  bool save_warmup = false;
  int refresh = 0;
  double stepsize = 1;
  double stepsize_jitter = 0;
  int max_depth = 10;
  double delta = .8;
  double gamma = .05;
  double kappa = .75;
  double t0 = 10;
  unsigned int init_buffer = 75;
  unsigned int term_buffer = 50;
  unsigned int window = 25;
  stan::test::unit::instrumented_interrupt interrupt;
  EXPECT_EQ(interrupt.call_count(), 0);

  std::string txt =
    "mass_matrix <- structure(c("
    "0.640211, 0.156096, -0.374048, "
    "0.156096, 1.41239, -0.0412753, "
    "-0.374048, -0.0412753, 1.29567 "
    "), .Dim  = c(3,3))";
  std::stringstream in(txt);
  stan::io::dump dump(in);
  stan::io::var_context& inv_mass_matrix = dump;

  int return_code =
    stan::services::sample::hmc_nuts_dense_e_adapt(model,
                                                   context,
                                                   inv_mass_matrix,
                                                   random_seed,
                                                   chain,
                                                   init_radius,
                                                   num_warmup,
                                                   num_samples,
                                                   num_thin,
                                                   save_warmup,
                                                   refresh,
                                                   stepsize,
                                                   stepsize_jitter,
                                                   max_depth,
                                                   delta,
                                                   gamma,
                                                   kappa,
                                                   t0,
                                                   init_buffer,
                                                   term_buffer,
                                                   window,
                                                   interrupt,
                                                   message,
                                                   error,
                                                   init,
                                                   parameter,
                                                   diagnostic);

  EXPECT_EQ(0, return_code);

  // check mass matrix has evolved
  std::vector<double> input_diag_vals(9);
  input_diag_vals = inv_mass_matrix.vals_r("mass_matrix");
  stan::test::unit::check_different(3, 3, input_diag_vals, parameter, 0.001);

  // check mass matrix has not evolved to identity matrix
  stan::io::dump dmp = 
    stan::services::util::create_ident_dense_mass_matrix(3);
  stan::io::var_context& ident_mass_matrix = dmp;
  std::vector<double> ident_dense_vals
    = ident_mass_matrix.vals_r("mass_matrix");
  stan::test::unit::check_different(3, 3, ident_dense_vals, parameter, 0.001);

}

// run model for 2000 iterations, starting w/ non-ident mass matrix
// resulting mass matrix should be very close to identity matrix
TEST_F(ServicesSampleHmcNutsDenseEMassMatrix, use_mass_matrix_continue_2000) {
  unsigned int random_seed = 12345;
  unsigned int chain = 1;
  double init_radius = 2;
  int num_warmup = 2000;
  int num_samples = 0;
  int num_thin = 1;
  bool save_warmup = false;
  int refresh = 0;
  double stepsize = 1;
  double stepsize_jitter = 0;
  int max_depth = 10;
  double delta = .8;
  double gamma = .05;
  double kappa = .75;
  double t0 = 10;
  unsigned int init_buffer = 75;
  unsigned int term_buffer = 50;
  unsigned int window = 25;
  stan::test::unit::instrumented_interrupt interrupt;
  EXPECT_EQ(interrupt.call_count(), 0);

  std::string txt =
    "mass_matrix <- structure(c("
    "0.640211, 0.156096, -0.374048, "
    "0.156096, 1.41239, -0.0412753, "
    "-0.374048, -0.0412753, 1.29567 "
    "), .Dim  = c(3,3))";

  std::stringstream in(txt);
  stan::io::dump dump(in);
  stan::io::var_context& inv_mass_matrix = dump;

  int return_code =
    stan::services::sample::hmc_nuts_dense_e_adapt(model,
                                                   context,
                                                   inv_mass_matrix,
                                                   random_seed,
                                                   chain,
                                                   init_radius,
                                                   num_warmup,
                                                   num_samples,
                                                   num_thin,
                                                   save_warmup,
                                                   refresh,
                                                   stepsize,
                                                   stepsize_jitter,
                                                   max_depth,
                                                   delta,
                                                   gamma,
                                                   kappa,
                                                   t0,
                                                   init_buffer,
                                                   term_buffer,
                                                   window,
                                                   interrupt,
                                                   message,
                                                   error,
                                                   init,
                                                   parameter,
                                                   diagnostic);

  EXPECT_EQ(0, return_code);

  // 2000 warmup steps should push matrix to ident
  stan::io::dump dmp = 
    stan::services::util::create_ident_dense_mass_matrix(3);
  stan::io::var_context& result_mass_matrix = dmp;
  std::vector<double> dense_vals
    = result_mass_matrix.vals_r("mass_matrix");
  stan::test::unit::check_adaptation(3, 3, dense_vals, parameter, 0.05);
}
