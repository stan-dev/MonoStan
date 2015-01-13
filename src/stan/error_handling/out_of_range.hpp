#ifndef STAN__ERROR_HANDLING__OUT_OF_RANGE_HPP
#define STAN__ERROR_HANDLING__OUT_OF_RANGE_HPP

#include <typeinfo>
#include <string>
#include <sstream>
#include <stdexcept>
#include <stan/meta/traits.hpp>

namespace stan {
  namespace math {

    /**
     * Throw an out_of_range exception with a consistently formatted message.
     * 
     * This is an abstraction for all Stan functions to use when throwing
     * out of range. This will allow us to change the behavior for all
     * functions at once.
     *
     * The message is:
     * "<function>: index <index> out of range; expecting index to be between "
     * "1 and <max><msg1><msg2>"
     *
     * @param function Name of the function
     * @param max Max
     * @param index Index
     * @param msg1 Message to print. Default is "".
     * @param msg2 Message to print. Default is "".
     */
    inline void out_of_range(const std::string& function,
                             const int max,
                             const int index,
                             const std::string& msg1 = "",
                             const std::string& msg2 = "") {
      std::ostringstream message;
      
      message << function << ": accessing element out of range. "
              << "index " << index << " out of range; "
              << "expecting index to be between "
              << stan::error_index::value << " and " 
              << stan::error_index::value - 1 + max
              << msg1
              << msg2;

      throw std::out_of_range(message.str());
    }

  }
}
#endif
