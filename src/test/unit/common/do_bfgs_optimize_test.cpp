#include <gtest/gtest.h>
#include <stan/common/do_bfgs_optimize.hpp>
#include <stan/optimization/bfgs.hpp>
#include <test/test-models/no-main/optimization/rosenbrock.cpp>
#include <boost/random/uniform_real_distribution.hpp>

typedef rosenbrock_model_namespace::rosenbrock_model Model;
typedef boost::ecuyer1988 rng_t; // (2**50 = 1T samples, 1000 chains)

TEST(Common, do_bfgs_optimize) {
  typedef stan::optimization::BFGSLineSearch<Model,stan::optimization::BFGSUpdate_HInv<> > Optimizer_BFGS;

  std::vector<double> cont_vector(2);
  cont_vector[0] = -1; cont_vector[1] = 1;
  std::vector<int> disc_vector;

  static const std::string DATA = "";
  std::stringstream data_stream(DATA);
  stan::io::dump dummy_context(data_stream);
  Model model(dummy_context);

  Optimizer_BFGS bfgs(model, cont_vector, disc_vector, &std::cout);

  double lp = 0;
  bool save_iterations = true;
  int refresh = 0;
  int return_code;
  unsigned int random_seed = 0;
  rng_t base_rng(random_seed);

  std::fstream* output_stream = 0;

  return_code = stan::common::do_bfgs_optimize(model,bfgs, base_rng,
                                               lp, cont_vector, disc_vector,
                                               output_stream, &std::cout, 
                                               save_iterations, refresh);
  EXPECT_FLOAT_EQ(return_code, 0);

  typedef stan::optimization::BFGSLineSearch<Model,stan::optimization::LBFGSUpdate<> > Optimizer_LBFGS;
  Optimizer_LBFGS lbfgs(model, cont_vector, disc_vector, &std::cout);

  return_code = stan::common::do_bfgs_optimize(model,lbfgs, base_rng,
                                               lp, cont_vector, disc_vector,
                                               output_stream, &std::cout, 
                                               save_iterations, refresh);
  EXPECT_FLOAT_EQ(return_code, 0);
}
