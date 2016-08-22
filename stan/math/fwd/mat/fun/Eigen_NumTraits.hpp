#ifndef STAN_MATH_FWD_MAT_FUN_EIGEN_NUMTRAITS_HPP
#define STAN_MATH_FWD_MAT_FUN_EIGEN_NUMTRAITS_HPP

#include <stan/math/fwd/core.hpp>
#include <stan/math/fwd/core/std_numeric_limits.hpp>
#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <limits>

namespace Eigen {

  /**
   * Numerical traits template override for Eigen for automatic
   * gradient variables.
   */
  template <typename T>
  struct NumTraits<stan::math::fvar<T> >
    : GenericNumTraits<stan::math::fvar<T> > {

    enum {
      RequireInitialization = 1,
      /**< stan::math::fvar requires initialization */
      ReadCost = 2 * NumTraits<double>::ReadCost,
      /**< twice the cost to copy a double */
      AddCost = 2 * NumTraits<T>::AddCost,
      /**< 2 * AddCost <br>
         (a + b) = a + b <br>
         (a + b)' = a' + b' */
      MulCost = 3 * NumTraits<T>::MulCost + NumTraits<T>::AddCost
      /**< 3 * MulCost + AddCost <br>
         (a * b) = a * b <br>
         (a * b)' = a' * b + a * b' */
    };
  };

  namespace internal {
    /**
     * Implemented this for printing to stream.
     */
    template<typename T>
    struct significant_decimals_default_impl<stan::math::fvar<T>, false> {
      static inline int run() {
        using std::ceil;
        using std::log;
        return cast<double, int>
          (ceil(-log(std::numeric_limits<double>::epsilon())
                / log(10.0)));
      }
    };

  }
}
#endif
