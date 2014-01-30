#include <stan/gm/command.hpp>
#include <gtest/gtest.h>
#include <test/test-models/no-main/gm/test_lp.cpp>

typedef test_lp_model_namespace::test_lp_model Model;
typedef boost::ecuyer1988 rng_t;
typedef stan::mcmc::adapt_unit_e_nuts<Model, rng_t> sampler;

class GmCommand : public testing::Test {
public:
  void SetUp() {
    std::fstream empty_data_stream(std::string("").c_str());
    stan::io::dump empty_data_context(empty_data_stream);
    empty_data_stream.close();
    
    model = new Model(empty_data_context, &std::cout);
    base_rng.seed(123456);
    
    sampler_ptr = new sampler((*model), base_rng, &std::cout, &std::cout);
    sampler_ptr->set_nominal_stepsize(1);
    sampler_ptr->set_stepsize_jitter(0);
    sampler_ptr->set_max_depth(10);

    delta = 0.8;
    gamma = 0.05;
    kappa = 0.75;
    t0 = 10;
    
    z_0.resize(model->num_params_r());
    z_0.fill(0);
    z_init.resize(model->num_params_r());
    z_init.fill(1);
  }
  
  void TearDown() {
    delete sampler_ptr;
    delete model;
  }
  
  rng_t base_rng;
  Model* model;
  sampler* sampler_ptr;
  double delta, gamma, kappa, t0;
  Eigen::VectorXd z_0;
  Eigen::VectorXd z_init;
};

TEST_F(GmCommand, init_adapt_z_0) {
  EXPECT_TRUE(stan::gm::init_adapt<sampler>(sampler_ptr,
                                            delta, gamma, kappa, t0,
                                            z_0));
  EXPECT_FLOAT_EQ(0.125, sampler_ptr->get_nominal_stepsize());

  for (size_t n = 0; n < model->num_params_r(); n++) {
    EXPECT_FLOAT_EQ(z_0[n], sampler_ptr->z().q[n]);
  }
}


TEST_F(GmCommand, init_adapt_z_init) {
  EXPECT_TRUE(stan::gm::init_adapt<sampler>(sampler_ptr,
                                            delta, gamma, kappa, t0,
                                            z_init));
  EXPECT_FLOAT_EQ(0.25, sampler_ptr->get_nominal_stepsize());
  for (size_t n = 0; n < model->num_params_r(); n++) {
    EXPECT_FLOAT_EQ(z_init[n], sampler_ptr->z().q[n]);
  }
}
