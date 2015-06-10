#include <stan/variational/advi_params_meanfield.hpp>
#include <vector>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>

TEST(advi_params_meanfield_test, dimension) {

  Eigen::Vector3d mu;
  mu << 5.7, -3.2, 0.1332;

  Eigen::Vector3d omega;
  omega << -0.42, 0.8922, 13.4;

  stan::variational::advi_params_meanfield my_advi_params_meanfield(mu, omega);

  EXPECT_FLOAT_EQ(mu.size(), my_advi_params_meanfield.dimension());
  EXPECT_FLOAT_EQ(omega.size(), my_advi_params_meanfield.dimension());

}

TEST(advi_params_meanfield_test, mean_vector) {

  Eigen::Vector3d mu;
  mu << 5.7, -3.2, 0.1332;

  Eigen::Vector3d omega;
  omega << -0.42, 0.8922, 13.4;

  stan::variational::advi_params_meanfield my_advi_params_meanfield(mu, omega);

  const Eigen::Vector3d& mu_out = my_advi_params_meanfield.mu();

  for (int i = 0; i < my_advi_params_meanfield.dimension(); ++i)
    EXPECT_FLOAT_EQ(mu(i), mu_out(i));

  double nan = std::numeric_limits<double>::quiet_NaN();
  Eigen::Vector3d mu_nan = Eigen::VectorXd::Constant(3, nan);
  EXPECT_THROW(stan::variational::advi_params_meanfield my_advi_params_meanfield_nan(mu_nan, omega),
                   std::domain_error);
  EXPECT_THROW(my_advi_params_meanfield.set_mu(mu_nan),
                   std::domain_error);
  Eigen::Vector3d omega_nan = Eigen::VectorXd::Constant(3,nan);
  EXPECT_THROW(stan::variational::advi_params_meanfield my_advi_params_meanfield_nan(mu, omega_nan);,
                   std::domain_error);

}

TEST(advi_params_meanfield_test, omega_vector) {

  Eigen::Vector3d mu;
  mu << 5.7, -3.2, 0.1332;

  Eigen::Vector3d omega;
  omega << -0.42, 0.8922, 13.4;

  stan::variational::advi_params_meanfield my_advi_params_meanfield(mu, omega);

  const Eigen::Vector3d& omega_out = my_advi_params_meanfield.omega();

  for (int i = 0; i < my_advi_params_meanfield.dimension(); ++i)
    EXPECT_FLOAT_EQ(omega(i), omega_out(i));

  double nan = std::numeric_limits<double>::quiet_NaN();
  Eigen::Vector3d omega_nan = Eigen::VectorXd::Constant(3, nan);

  EXPECT_THROW(my_advi_params_meanfield.set_omega(omega_nan);,
                   std::domain_error);

}

TEST(advi_params_meanfield_test, entropy) {

  Eigen::Vector3d mu;
  mu << 5.7, -3.2, 0.1332;

  Eigen::Vector3d omega;
  omega << -0.42, 0.8922, 13.4;

  double entropy_true = 18.129015599614018;

  stan::variational::advi_params_meanfield my_advi_params_meanfield(mu, omega);

  const double entropy_out = my_advi_params_meanfield.entropy();

  EXPECT_FLOAT_EQ(entropy_out, entropy_true);

}

TEST(advi_params_meanfield_test, transform_loc_scale) {

  Eigen::Vector3d mu;
  mu << 5.7, -3.2, 0.1332;

  Eigen::Vector3d omega;
  omega << -0.42, 0.8922, 13.4;

  stan::variational::advi_params_meanfield my_advi_params_meanfield(mu, omega);

  Eigen::Vector3d x;
  x << 7.1, -9.2, 0.59;

  Eigen::Vector3d x_transformed;
  x_transformed << 1.036503242068690e01,  -2.565253407151558e01,
         3.894020358120325e05;

  Eigen::Vector3d x_result;
  x_result = my_advi_params_meanfield.loc_scale_transform(x);

  for (int i = 0; i < my_advi_params_meanfield.dimension(); ++i)
    EXPECT_FLOAT_EQ(x_result(i), x_transformed(i));

  double nan = std::numeric_limits<double>::quiet_NaN();
  Eigen::Vector3d x_nan = Eigen::VectorXd::Constant(3, nan);

  EXPECT_THROW(my_advi_params_meanfield.loc_scale_transform(x_nan);,
                   std::domain_error);

}
