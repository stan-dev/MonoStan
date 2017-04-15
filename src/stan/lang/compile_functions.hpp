#ifndef STAN_LANG_COMPILE_FUNCTIONS_HPP
#define STAN_LANG_COMPILE_FUNCTIONS_HPP

#include <stan/lang/ast.hpp>
#include <stan/lang/generator/generate_standalone_functions.hpp>
#include <stan/lang/parser.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace stan {
  namespace lang {

    /**
     * Read a Stan file with only the functions block from the
     * specified input, parse it, and write the C++ code for it
     * to the specified output.
     *
     * @param[in] msgs Output stream for warning messages
     * @param[in] stan_funcs_in Stan model specification
     * @param[in] cpp_out C++ code output stream
     * @param[in] namespaces Vector of namespace to generate the functions in
     * @param[in] allow_undefined Permit undefined functions?
     *
     * @return <code>false</code> if code could not be generated
     *    due to syntax error in the functions file;
     *    <code>true</code> otherwise.
     */
    bool compile_functions(std::ostream* msgs,
                 std::istream& stan_funcs_in,
                 std::ostream& cpp_out,
                 const std::vector<std::string>& namespaces,
                 const bool allow_undefined = false) {
      program prog;

      //TODO(martincerny) the model_name below probably should not be a constant
      //but it seems to have no impact on the generated .hpp file
      bool parsed_ok = parse(msgs, stan_funcs_in,
                             "functions_only_model", 
                             prog, allow_undefined);
      if (!parsed_ok)
        return false;  // syntax error in program
        
      //TODO(martincerny) raise error or warning when other blocks than 
      //functions are present
      generate_standalone_functions(prog, namespaces, cpp_out);
      return true;
    }


  }
}
#endif
