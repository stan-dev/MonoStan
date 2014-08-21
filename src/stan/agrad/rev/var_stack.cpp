#ifndef __STAN__AGRAD__REV__VAR_STACK_CPP__
#define __STAN__AGRAD__REV__VAR_STACK_CPP__

#include <stan/agrad/rev/var_stack.hpp>

namespace stan {

  namespace agrad {

    std::vector<chainable*> var_stack_;
    std::vector<chainable*> var_nochain_stack_;
    std::vector<chainable_alloc*> var_alloc_stack_;
    memory::stack_alloc memalloc_;

    extern std::vector<size_t> var_stack_sizes_;
    extern std::vector<size_t> var_nochain_stack_sizes_;
    extern std::vector<size_t> var_alloc_stack_starts_;
  }

}

#endif
