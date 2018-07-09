#ifndef STAN_LANG_AST_FUN_IS_SPACE_HPP
#define STAN_LANG_AST_FUN_IS_SPACE_HPP

namespace stan {
  namespace lang {

    /**
     * Returns true if the specified character is an ASCII whitespace
     * character (space, newline, carriage return, tab).
     *
     * @param c character to test
     * @return true if character is whitespace
     */
    bool is_space(char c);

  }
}
#endif
