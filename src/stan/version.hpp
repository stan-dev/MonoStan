#ifndef STAN_VERSION_HPP
#define STAN_VERSION_HPP

#include <string>
#define STRING_EXPAND(s) #s
#define STRING(s) STRING_EXPAND(s)

#define STAN_MAJOR 2
#define STAN_MINOR 6
#define STAN_PATCH 3

namespace stan {

  /** Major version number for Stan package. */
  const std::string MAJOR_VERSION = STRING(STAN_MAJOR);

  /** Minor version number for Stan package. */
  const std::string MINOR_VERSION = STRING(STAN_MINOR);

  /** Patch version for Stan package. */
  const std::string PATCH_VERSION = STRING(STAN_PATCH);

}

#endif
