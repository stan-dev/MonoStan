#ifndef STAN_MCMC_HMC_INTEGRATORS_BASE_INTEGRATOR_HPP
#define STAN_MCMC_HMC_INTEGRATORS_BASE_INTEGRATOR_HPP

#include <stan/interface_callbacks/writer/base_writer.hpp>

namespace stan {
  namespace mcmc {

    template <typename Hamiltonian>
    class base_integrator {
    public:
      explicit base_integrator(interface_callbacks::writer::base_writer& writer)
        : writer_(writer) {}

      virtual void evolve(typename Hamiltonian::PointType& z,
                          Hamiltonian& hamiltonian,
                          const double epsilon) = 0;

    protected:
      interface_callbacks::writer::base_writer& writer_;
    };

  }  // mcmc
}  // stan

#endif
