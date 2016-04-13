#include <stan/interface_callbacks/writer/stream_writer.hpp>
#include <stan/mcmc/hmc/nuts/unit_e_nuts.hpp>
#include <boost/random/additive_combine.hpp>
#include <test/test-models/good/mcmc/hmc/common/gauss3D.hpp>
#include <stan/io/dump.hpp>
#include <fstream>

#include <gtest/gtest.h>

typedef boost::ecuyer1988 rng_t;

TEST(McmcUnitENuts, build_tree) {
  rng_t base_rng(4839294);

  stan::mcmc::unit_e_point z_init(3);
  z_init.q(0) = 1;
  z_init.q(1) = -1;
  z_init.q(2) = 1;
  z_init.p(0) = -1;
  z_init.p(1) = 1;
  z_init.p(2) = -1;

  std::stringstream output;
  stan::interface_callbacks::writer::stream_writer writer(output);

  std::fstream empty_stream("", std::fstream::in);
  stan::io::dump data_var_context(empty_stream);
  gauss3D_model_namespace::gauss3D_model model(data_var_context);

  stan::mcmc::unit_e_nuts<gauss3D_model_namespace::gauss3D_model, rng_t>
    sampler(model, base_rng);

  sampler.z() = z_init;
  sampler.init_hamiltonian(writer);
  sampler.set_nominal_stepsize(0.1);
  sampler.set_stepsize_jitter(0);
  sampler.sample_stepsize();

  stan::mcmc::ps_point z_propose = z_init;

  Eigen::VectorXd rho = z_init.p;
  double sum_weight = 0;

  double H0 = -0.1;
  int n_leapfrog = 0;
  double sum_metro_prob = 0;

  bool valid_subtree = sampler.build_tree(3, rho, z_propose,
                                          H0, 1, n_leapfrog, sum_weight,
                                          sum_metro_prob, writer);

  EXPECT_EQ(0.1, sampler.get_nominal_stepsize());

  EXPECT_TRUE(valid_subtree);

  EXPECT_FLOAT_EQ(-11.401228, rho(0));
  EXPECT_FLOAT_EQ(11.401228, rho(1));
  EXPECT_FLOAT_EQ(-11.401228, rho(2));

  EXPECT_FLOAT_EQ(-0.022019938, sampler.z().q(0));
  EXPECT_FLOAT_EQ(0.022019938, sampler.z().q(1));
  EXPECT_FLOAT_EQ(-0.022019938, sampler.z().q(2));

  EXPECT_FLOAT_EQ(-1.4131583, sampler.z().p(0));
  EXPECT_FLOAT_EQ(1.4131583, sampler.z().p(1));
  EXPECT_FLOAT_EQ(-1.4131583, sampler.z().p(2));

  EXPECT_EQ(8, n_leapfrog);
  EXPECT_FLOAT_EQ(0.36134657, sum_weight);
  EXPECT_FLOAT_EQ(0.36134657, sum_metro_prob);

  EXPECT_EQ("", output.str());
}

TEST(McmcUnitENuts, transition) {
  rng_t base_rng(4839294);

  stan::mcmc::unit_e_point z_init(3);
  z_init.q(0) = 1;
  z_init.q(1) = -1;
  z_init.q(2) = 1;
  z_init.p(0) = -1;
  z_init.p(1) = 1;
  z_init.p(2) = -1;

  std::stringstream output;
  stan::interface_callbacks::writer::stream_writer writer(output);

  std::fstream empty_stream("", std::fstream::in);
  stan::io::dump data_var_context(empty_stream);
  gauss3D_model_namespace::gauss3D_model model(data_var_context);

  stan::mcmc::unit_e_nuts<gauss3D_model_namespace::gauss3D_model, rng_t>
    sampler(model, base_rng);

  sampler.z() = z_init;
  sampler.init_hamiltonian(writer);
  sampler.set_nominal_stepsize(0.1);
  sampler.set_stepsize_jitter(0);
  sampler.sample_stepsize();

  stan::mcmc::sample init_sample(z_init.q, 0, 0);

  stan::mcmc::sample s = sampler.transition(init_sample, writer);

  EXPECT_FLOAT_EQ(1, s.cont_params()(0));
  EXPECT_FLOAT_EQ(-1, s.cont_params()(1));
  EXPECT_FLOAT_EQ(1, s.cont_params()(2));
  EXPECT_FLOAT_EQ(-1.5, s.log_prob());
  EXPECT_FLOAT_EQ(0.99629, s.accept_stat());
  EXPECT_EQ("", output.str());
}
