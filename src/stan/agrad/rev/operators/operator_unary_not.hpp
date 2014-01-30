#ifndef __STAN__AGRAD__REV__OPERATORS__OPERATOR_UNARY_NOT_HPP__
#define __STAN__AGRAD__REV__OPERATORS__OPERATOR_UNARY_NOT_HPP__

#include <stan/agrad/rev/var.hpp>

namespace stan {
  namespace agrad {
    
    /**
     * Prefix logical negation for the value of variables (C++).  The
     * expression (!a) is equivalent to negating the scalar value of
     * the variable a.
     *
     * Note that this is the only logical operator defined for
     * variables.  Overridden logical conjunction (&&) and disjunction
     * (||) operators do not apply the same "short circuit" rules
     * as the built-in logical operators.  
     *
     * @param a Variable to negate.
     * @return True if variable is non-zero.
     */
    inline bool operator!(const var& a) {
      return !a.val();
    }

  }
}
#endif
