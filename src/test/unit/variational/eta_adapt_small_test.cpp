#include <test/test-models/good/variational/eta_should_be_small.hpp>
#include <stan/variational/advi.hpp>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <boost/random/additive_combine.hpp> // L'Ecuyer RNG

typedef boost::ecuyer1988 rng_t;

class eta_adapt_small_test : public ::testing::Test {
public:
  void SetUp() {
    static const std::string DATA = "";
    std::stringstream data_stream(DATA);
    stan::io::dump data_var_context(data_stream);

    model_ = new stan_model(data_var_context, &model_stream_);
    cont_params_ = Eigen::VectorXd::Zero(model_->num_params_r());
    base_rng_.seed(727802408);
    model_stream_.str("");
    print_stream_.str("");
    output_stream_.str("");
    diagnostic_stream_.str("");

    advi_meanfield_ = new stan::variational::advi<stan_model, stan::variational::normal_meanfield, rng_t>
      (*model_, cont_params_, base_rng_,
       1, 100,
       100, 1,
       &print_stream_,
       &output_stream_,
       &diagnostic_stream_);

    advi_fullrank_ = new stan::variational::advi<stan_model, stan::variational::normal_fullrank, rng_t>
      (*model_, cont_params_, base_rng_,
       1, 100,
       100, 1,
       &print_stream_,
       &output_stream_,
       &diagnostic_stream_);
  }

  void TearDown() {
    delete advi_meanfield_;
    delete advi_fullrank_;
    delete model_;
  }

  stan::variational::advi<stan_model, stan::variational::normal_meanfield, rng_t> *advi_meanfield_;
  stan::variational::advi<stan_model, stan::variational::normal_fullrank, rng_t> *advi_fullrank_;
  std::stringstream model_stream_;
  std::stringstream print_stream_;
  std::stringstream output_stream_;
  std::stringstream diagnostic_stream_;

  stan_model *model_;
  rng_t base_rng_;
  Eigen::VectorXd cont_params_;
};


TEST_F(eta_adapt_small_test, eta_should_be_small) {
  stan::variational::normal_meanfield meanfield_init =
    stan::variational::normal_meanfield(cont_params_);
  stan::variational::normal_fullrank fullrank_init =
    stan::variational::normal_fullrank(cont_params_);

  EXPECT_EQ(0.1, advi_meanfield_->adapt_eta(meanfield_init, 50));
  EXPECT_EQ(0.1, advi_fullrank_->adapt_eta(fullrank_init, 50));
}
