#ifndef STAN__ERROR_HANDLING__SCALAR__CHECK_POSITIVE_INDEX_HPP
#define STAN__ERROR_HANDLING__SCALAR__CHECK_POSITIVE_INDEX_HPP

#include <sstream>
#include <stan/error_handling/invalid_argument.hpp>

namespace stan {
  namespace error_handling {

    /**
     * Return <code>true</code> if <code>val</code> is positive.
     *
     * @param function Function name (for error messages)
     * @param name Variable name (for error messages)
     * @param expr Expression for the dimension size (for error messages)
     * @param val Index value to check
     *
     * @return <code>true</code> if <code>val</code> is greater than 0.
     * @throw <code>std::invalid_argument</code> if <code>val</code> is
     *   zero or negative.
     */
    inline bool check_positive_index(const std::string& function,
                                     const std::string& name,
                                     const std::string& expr,
                                     const int val) {
      if (val <= 0) {
        std::stringstream msg;
        msg << "; dimension size expression = " << expr;
        invalid_argument(function, name, val, 
                         "must have a positive size, but is ",
                         msg.str());
      }
      return true;
    }

  }
}
#endif
