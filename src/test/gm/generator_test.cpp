#include <iostream>
#include <sstream>
#include <boost/random/additive_combine.hpp> // L'Ecuyer RNG
#include <stan/agrad/rev.hpp>
#include <stan/gm/ast.hpp>
#include <stan/gm/generator.hpp>
#include <stan/io/dump.hpp>
#include <test/gm/model_specs/no_main/test_lp.cpp>

#include <gtest/gtest.h>

void test_print_string_literal(const std::string& s,
                               const std::string& s_exp) {
  std::stringstream ss;
  stan::gm::print_string_literal(ss,s);
  EXPECT_EQ(s_exp, ss.str());
}

void test_print_quoted_expression(const stan::gm::expression& e,
                                  const std::string& e_exp) {
  std::stringstream ss;
  stan::gm::print_quoted_expression(ss,e);
  EXPECT_EQ(e_exp, ss.str());
}

TEST(gm,printStringLiteral) {
  test_print_string_literal("","\"\"");
  test_print_string_literal("\\d\\","\"\\\\d\\\\\"");
  test_print_string_literal("ab\"c", "\"ab\\\"c\"");
  test_print_string_literal("'hey,' he said.","\"\\'hey,\\' he said.\"");
}

TEST(gm,printQuotedExpression) {
  using stan::gm::expression;
  using stan::gm::index_op;
  using stan::gm::int_literal;
  using stan::gm::variable;
  using std::vector;
  test_print_quoted_expression(int_literal(1),"\"1\"");
  vector<expression> args;

  expression expr(variable("foo"));
  vector<vector<expression> > dimss;
  vector<expression> dim;
  dim.push_back(int_literal(1));
  dimss.push_back(dim);
  std::string s_exp = "\"get_base1(foo,1,\\\"foo\\\",1)\"";
  test_print_quoted_expression(index_op(expr,dimss),
                               s_exp);
}


TEST(gm, logProbPolymorphismDouble) {
  using std::vector;
  using Eigen::Matrix;
  using Eigen::Dynamic;

  std::string txt = "foo <- 3\nbar <- 4";
  std::stringstream in(txt);
  stan::io::dump dump(in);

  test_lp_namespace::test_lp model(dump);

  std::vector<double> params_r(2);
  params_r[0] = 1.0;
  params_r[1] = -3.2;

  std::vector<int> params_i;
  
  Matrix<double, Dynamic, 1> params_r_vec(2);
  for (int i = 0; i < 2; ++i)
    params_r_vec(i) = params_r[i];

  double lp1 = model.log_prob<true,true>(params_r, params_i, 0);
  double lp2 = model.log_prob<true,true>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1, lp2);

  lp1 = model.log_prob<true,false>(params_r, params_i, 0);
  lp2 = model.log_prob<true,false>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1, lp2);

  lp1 = model.log_prob<false,true>(params_r, params_i, 0);
  lp2 = model.log_prob<false,true>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1, lp2);

  lp1 = model.log_prob<false,false>(params_r, params_i, 0);
  lp2 = model.log_prob<false,false>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1, lp2);

  // only test write_csv for doubles -- no var allowed
  std::stringstream s1;
  std::stringstream s2;
  boost::ecuyer1988 rng(123);
  model.write_csv(rng,params_r,params_i,s1,0);
  model.write_csv(rng,params_r_vec,s2,0);
  EXPECT_EQ(s1.str(), s2.str());

  // only test generate_inits for doubles -- no var allowed
  std::string init_txt = "y <- c(-2.9,1.2)";
  std::stringstream init_in(init_txt);
  stan::io::dump init_dump(init_in);
  std::vector<int> params_i_init;
  std::vector<double> params_r_init;
  model.transform_inits(init_dump, params_i_init, params_r_init);
  EXPECT_EQ(0U, params_i_init.size());
  EXPECT_EQ(2U, params_r_init.size());

  Matrix<double,Dynamic,1> params_r_vec_init;
  model.transform_inits(init_dump, params_r_vec_init);
  EXPECT_EQ(int(params_r.size()), params_r_vec_init.size());
  for (int i = 0; i < params_r_vec_init.size(); ++i)
    EXPECT_FLOAT_EQ(params_r_init[i], params_r_vec_init(i));

  // only test write_array for doubles --- no var allowed
  std::vector<double> params_r_write(2);
  params_r_write[0] = -3.2;  
  params_r_write[1] = 1.79;
  std::vector<int> params_i_write;
  
  Matrix<double,Dynamic,1> params_r_vec_write(2);
  params_r_vec_write << -3.2, 1.79;

  for (int incl_tp = 0; incl_tp < 2; ++incl_tp) {
    for (int incl_gq = 0; incl_gq < 2; ++incl_gq) { 
      std::vector<double> vars_write;
      Matrix<double,Dynamic,1> vars_vec_write(17);
      model.write_array(rng, params_r_write, params_i_write, vars_write, incl_tp, incl_gq, 0);
      model.write_array(rng, params_r_vec_write, vars_vec_write, incl_tp, incl_gq, 0);
      EXPECT_EQ(int(vars_write.size()), vars_vec_write.size());
      for (int i = 0; i < vars_vec_write.size(); ++i)
        EXPECT_FLOAT_EQ(vars_write[i], vars_vec_write(i));
    }
  }

}
TEST(gm, logProbPolymorphismVar) {
  using std::vector;
  using Eigen::Matrix;
  using Eigen::Dynamic;
  using stan::agrad::var;

  std::string txt = "foo <- 3\nbar <- 4";
  std::stringstream in(txt);
  stan::io::dump dump(in);

  test_lp_namespace::test_lp model(dump);

  std::vector<var> params_r(2);
  params_r[0] = 1.0;
  params_r[1] = -3.2;

  std::vector<int> params_i;
  
  Matrix<var, Dynamic, 1> params_r_vec(2);
  for (int i = 0; i < 2; ++i)
    params_r_vec(i) = params_r[i];

  var lp1 = model.log_prob<true,true>(params_r, params_i, 0);
  var lp2 = model.log_prob<true,true>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1.val(), lp2.val());

  lp1 = model.log_prob<true,false>(params_r, params_i, 0);
  lp2 = model.log_prob<true,false>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1.val(), lp2.val());

  lp1 = model.log_prob<false,true>(params_r, params_i, 0);
  lp2 = model.log_prob<false,true>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1.val(), lp2.val());

  lp1 = model.log_prob<false,false>(params_r, params_i, 0);
  lp2 = model.log_prob<false,false>(params_r_vec, 0);
  EXPECT_FLOAT_EQ(lp1.val(), lp2.val());
}

// * write_csv
// * transform_inits
