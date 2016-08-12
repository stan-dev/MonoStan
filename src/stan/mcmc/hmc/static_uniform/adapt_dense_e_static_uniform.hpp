#ifndef STAN_MCMC_HMC_STATIC_UNIFORM_ADAPT_DENSE_E_STATIC_UNIFORM_HPP
#define STAN_MCMC_HMC_STATIC_UNIFORM_ADAPT_DENSE_E_STATIC_UNIFORM_HPP

#include <stan/mcmc/stepsize_covar_adapter.hpp>
#include <stan/mcmc/hmc/static_uniform/dense_e_static_uniform.hpp>

namespace stan {
  namespace mcmc {
    /**
     * Hamiltonian Monte Carlo implementation that uniformly samples
     * from trajectories with a static integration time with a
     * Gaussian-Euclidean disintegration and adaptive dense metric and
     * adaptive step size
     */
    template <typename Model, class BaseRNG>
    class adapt_dense_e_static_uniform:
      public dense_e_static_uniform<Model, BaseRNG>,
      public stepsize_covar_adapter {
    public:
      adapt_dense_e_static_uniform(const Model& model, BaseRNG& rng):
        dense_e_static_uniform<Model, BaseRNG>(model, rng),
        stepsize_covar_adapter(model.num_params_r()) { }

      ~adapt_dense_e_static_uniform() { }

      sample
      transition(sample& init_sample,
                 callbacks::writer& info_writer,
                 callbacks::writer& error_writer) {
        sample s
          = dense_e_static_uniform<Model, BaseRNG>::transition(init_sample,
                                                               info_writer,
                                                               error_writer);

        if (this->adapt_flag_) {
          this->stepsize_adaptation_.learn_stepsize(this->nom_epsilon_,
                                                    s.accept_stat());

          bool update = this->covar_adaptation_.learn_covariance
            (this->z_.mInv, this->z_.q);

          if (update) {
            this->init_stepsize(info_writer, error_writer);
            this->stepsize_adaptation_.set_mu(log(10 * this->nom_epsilon_));
            this->stepsize_adaptation_.restart();
          }
        }
        return s;
      }

      void disengage_adaptation() {
        base_adapter::disengage_adaptation();
        this->stepsize_adaptation_.complete_adaptation(this->nom_epsilon_);
      }
    };
  }  // mcmc
}  // stan

#endif
