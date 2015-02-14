#ifndef STAN__MATH__PRIM__MAT__FUN__GET_LP_HPP
#define STAN__MATH__PRIM__MAT__FUN__GET_LP_HPP

#include <boost/math/tools/promotion.hpp>
#include <stan/math/prim/mat/fun/accumulator.hpp>

namespace stan {

  namespace math {

    template <typename T_lp, typename T_lp_accum>
    inline
    typename boost::math::tools::promote_args<T_lp, T_lp_accum>::type
    get_lp(const T_lp& lp,
           const stan::math::accumulator<T_lp_accum>& lp_accum) {
      return lp + lp_accum.sum();
    }

  }

}

#endif
