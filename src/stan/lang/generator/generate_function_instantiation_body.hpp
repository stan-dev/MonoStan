#ifndef STAN_LANG_GENERATOR_GENERATE_FUNCTION_INSTANTIATION_BODY_HPP
#define STAN_LANG_GENERATOR_GENERATE_FUNCTION_INSTANTIATION_BODY_HPP

#include <stan/lang/ast.hpp>
#include <stan/lang/generator/generate_function_name.hpp>
#include <stan/lang/generator/generate_function_instantiation_template_parameters.hpp>
#include <ostream>
#include <vector>
#include <string>

namespace stan {
  namespace lang {

    /**
     * Generate the body for a function instantiation (e.g., the 
     * call to the templated function with all templated arguments based on 
     * double).
     * Requires precalculated flags for whether it is an RNG, uses the log
     * density accumulator or is a probability function, to the
     * specified stream.
     *
     * @param[in] fun function declaration
     * @param[in] is_rng true if function is an RNG
     * @param[in] is_lp true if function accesses log density
     * accumulator
     * @param[in] is_log true if function is log probability function
     * @param[in,out] o stream for generating
     */
    void generate_function_instantiation_body(const function_decl_def& fun,
                                    bool is_rng, bool is_lp, bool is_log,
                                    const std::string& rng_class,
                                    std::ostream& o) {
      o << "{" << EOL;
      o << "\t";
      if (!fun.return_type_.is_void()) {
        o << "return ";
      }
      generate_function_name(fun, o);
      generate_function_instantiation_template_parameters(
        fun, is_rng, is_lp, is_log, rng_class, o);

      o << "(";
      for (size_t arg_i = 0; arg_i < fun.arg_decls_.size(); ++arg_i) {
        o << fun.arg_decls_[arg_i].name_;
        if (arg_i + 1 < fun.arg_decls_.size()) {
          o << ", ";
        }
      }
      if ((is_rng || is_lp) && fun.arg_decls_.size() > 0)
        o << ", ";
      if (is_rng)
        o << "base_rng__";
      else if (is_lp)
          o << "lp__, lp_accum__";
      if (is_rng || is_lp || fun.arg_decls_.size() > 0)
        o << ", ";
      o << "pstream__";

      o << ");" << EOL;
      o << "}" << EOL;
    }
  }
}


#endif
