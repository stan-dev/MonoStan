#ifndef STAN_LANG_GRAMMARS_STATEMENT_GRAMMAR_DEF_HPP
#define STAN_LANG_GRAMMARS_STATEMENT_GRAMMAR_DEF_HPP

#include <stan/lang/ast.hpp>
#include <stan/lang/grammars/common_adaptors_def.hpp>
#include <stan/lang/grammars/expression_grammar.hpp>
#include <stan/lang/grammars/indexes_grammar.hpp>
#include <stan/lang/grammars/semantic_actions.hpp>
#include <stan/lang/grammars/statement_grammar.hpp>
#include <stan/lang/grammars/var_decls_grammar.hpp>
#include <stan/lang/grammars/whitespace_grammar.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <sstream>
#include <string>
#include <vector>

BOOST_FUSION_ADAPT_STRUCT(stan::lang::assgn,
                          (stan::lang::variable, lhs_var_)
                          (std::vector<stan::lang::idx>, idxs_)
                          (stan::lang::expression, rhs_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::assignment,
                          (stan::lang::variable_dims, var_dims_)
                          (stan::lang::expression, expr_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::variable_dims,
                          (std::string, name_)
                          (std::vector<stan::lang::expression>, dims_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::distribution,
                          (std::string, family_)
                          (std::vector<stan::lang::expression>, args_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::for_statement,
                          (std::string, variable_)
                          (stan::lang::range, range_)
                          (stan::lang::statement, statement_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::return_statement,
                          (stan::lang::expression, return_value_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::break_continue_statement,
                          (std::string, generate_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::print_statement,
                          (std::vector<stan::lang::printable>, printables_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::reject_statement,
                          (std::vector<stan::lang::printable>, printables_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::increment_log_prob_statement,
                          (stan::lang::expression, log_prob_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::sample,
                          (stan::lang::expression, expr_)
                          (stan::lang::distribution, dist_)
                          (stan::lang::range, truncation_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::statements,
                          (std::vector<stan::lang::var_decl>, local_decl_)
                          (std::vector<stan::lang::statement>, statements_) )

namespace stan {

  namespace lang {

    template <typename Iterator>
    statement_grammar<Iterator>::statement_grammar(variable_map& var_map,
                                           std::stringstream& error_msgs)
      : statement_grammar::base_type(statement_r),
        var_map_(var_map),
        error_msgs_(error_msgs),
        expression_g(var_map, error_msgs),
        var_decls_g(var_map, error_msgs),
        statement_2_g(var_map, error_msgs, *this),
        indexes_g(var_map, error_msgs, expression_g) {
      using boost::spirit::qi::_1;
      using boost::spirit::qi::char_;
      using boost::spirit::qi::eps;
      using boost::spirit::qi::lexeme;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::no_skip;
      using boost::spirit::qi::string;
      using boost::spirit::qi::_pass;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::raw;

      using boost::spirit::qi::labels::_a;
      using boost::spirit::qi::labels::_r1;
      using boost::spirit::qi::labels::_r2;
      using boost::spirit::qi::labels::_r3;
      using boost::spirit::qi::labels::_r4;

      using boost::phoenix::begin;
      using boost::phoenix::end;

      // inherited features
      //   _r1 true if sample_r allowed
      //   _r2 source of variables allowed for assignments
      //   _r3 true if return_r allowed
      //   _r4 true if in loop (allowing break/continue)

      // raw[ ] just to wrap to get line numbers
      statement_r.name("statement");
      statement_r
        = raw[statement_sub_r(_r1, _r2, _r3, _r4)[assign_lhs_f(_val, _1)]]
        [add_line_number_f(_val, begin(_1), end(_1))];

      statement_sub_r.name("statement");
      statement_sub_r
        %= no_op_statement_r                        // key ";"
        | statement_seq_r(_r1, _r2, _r3, _r4)       // key "{"
        | increment_log_prob_statement_r(_r1, _r2)  // key "increment_log_prob"
        | increment_target_statement_r(_r1, _r2)    // key "target"
        | for_statement_r(_r1, _r2, _r3)            // key "for"
        | while_statement_r(_r1, _r2, _r3)          // key "while"
        | break_continue_statement_r(_r4)           // key "break", "continue"
        | statement_2_g(_r1, _r2, _r3, _r4)         // key "if"
        | print_statement_r(_r2)                    // key "print"
        | reject_statement_r(_r2)                   // key "reject"
        | return_statement_r(_r2)                   // key "return"
        | void_return_statement_r(_r2)              // key "return"
        | assignment_r(_r2)                         // lvalue "<-"
        | assgn_r(_r2)                              // var[idxs] <- expr
        | sample_r(_r1, _r2)                        // expression "~"
        | expression_g(_r2)                         // expression
        [expression_as_statement_f(_pass, _1,
                                   boost::phoenix::ref(error_msgs_))];

      // _r1, _r2, _r3, _r4 same as statement_r
      statement_seq_r.name("sequence of statements");
      statement_seq_r
        %= lit('{')
        > local_var_decls_r[assign_lhs_f(_a, _1)]
        > *statement_r(_r1, _r2, _r3, _r4)
        > lit('}')
        > eps[unscope_locals_f(_a, boost::phoenix::ref(var_map_))];

      local_var_decls_r
        %= var_decls_g(false, local_origin);  // - constants

      // inherited  _r1 = true if samples allowed as statements
      increment_log_prob_statement_r.name("increment log prob statement");
      increment_log_prob_statement_r
        %= (lit("increment_log_prob") >> no_skip[!char_("a-zA-Z0-9_")])
        > eps[deprecate_increment_log_prob_f(boost::phoenix::ref(error_msgs_))]
        > eps[validate_allow_sample_f(_r1, _pass,
                                      boost::phoenix::ref(error_msgs_))]
        > lit('(')
        > expression_g(_r2)
          [validate_non_void_expression_f(_1, _pass,
                                          boost::phoenix::ref(error_msgs_))]
        > lit(')')
        > lit(';');

      // just variant syntax for increment_log_prob_r (see above)
      increment_target_statement_r.name("increment target statement");
      increment_target_statement_r
        %= (lit("target") >> lit("+="))
        > eps[validate_allow_sample_f(_r1, _pass,
                                      boost::phoenix::ref(error_msgs_))]
        > expression_g(_r2)
          [validate_non_void_expression_f(_1, _pass,
                                          boost::phoenix::ref(error_msgs_))]
        > lit(';');

      // _r1, _r2, _r3, same as statement_r
      while_statement_r.name("while statement");
      while_statement_r
        = (lit("while") >> no_skip[!char_("a-zA-Z0-9_")])
        > lit('(')
        > expression_g(_r2)
          [add_while_condition_f(_val, _1, _pass,
                                 boost::phoenix::ref(error_msgs_))]
        > lit(')')
        > statement_r(_r1, _r2, _r3, true)
          [add_while_body_f(_val, _1)];

      // _r1 here == _r4 from statement_r
      break_continue_statement_r.name("break or continue statement");
      break_continue_statement_r
        %= (string("break") | string("continue"))
        > eps[validate_in_loop_f(_r1, _pass, boost::phoenix::ref(error_msgs_))]
        > lit(';');

      // _r1, _r2, _r3 same as statement_r
      for_statement_r.name("for statement");
      for_statement_r
        %= (lit("for") >> no_skip[!char_("a-zA-Z0-9_")])
        > lit('(')
        > identifier_r[add_loop_identifier_f(_1, _a, _pass,
                                         boost::phoenix::ref(var_map_),
                                         boost::phoenix::ref(error_msgs_))]
        > lit("in")
        > range_r(_r2)
        > lit(')')
        > statement_r(_r1, _r2, _r3, true)
        > eps
        [remove_loop_identifier_f(_a, boost::phoenix::ref(var_map_))];

      print_statement_r.name("print statement");
      print_statement_r
        %= (lit("print") >> no_skip[!char_("a-zA-Z0-9_")])
        > lit('(')
        > (printable_r(_r1) % ',')
        > lit(')');

      // reject
      reject_statement_r.name("reject statement");
      reject_statement_r
        %= (lit("reject") >> no_skip[!char_("a-zA-Z0-9_")])
        > lit('(')
        > (printable_r(_r1) % ',')
        > lit(')');

      printable_r.name("printable");
      printable_r
        %= printable_string_r
        | expression_g(_r1);

      printable_string_r.name("printable quoted string");
      printable_string_r
        %= lit('"')
        > no_skip[*char_("a-zA-Z0-9/~!@#$%^&*()`_+-={}|[]:;'<>?,./ ")]
        > lit('"');

      identifier_r.name("identifier");
      identifier_r
        %= (lexeme[char_("a-zA-Z")
                   >> *char_("a-zA-Z0-9_.")]);

      range_r.name("range expression pair, colon");
      range_r
        %= expression_g(_r1)
           [validate_int_expr_f(_1, _pass,
                                     boost::phoenix::ref(error_msgs_))]
        >> lit(':')
        >> expression_g(_r1)
           [validate_int_expr_f(_1, _pass,
                                     boost::phoenix::ref(error_msgs_))];

      // this one comes before assgn_r to deal with simple assignment
      assignment_r.name("variable assignment by expression");
      assignment_r
        %= var_lhs_r(_r1)
        >> assignment_operator_r
        > expression_rhs_r(_r1)
          [validate_assignment_f(_val, _r1, _pass,
                                 boost::phoenix::ref(var_map_),
                                 boost::phoenix::ref(error_msgs_))]
        > lit(';');

      assgn_r.name("indexed variable assginment statement");
      assgn_r
        %= var_r(_r1)
        >> indexes_g(_r1)
        >> assignment_operator_r
        >> (eps > expression_rhs_r(_r1))
           [validate_assgn_f(_val, _pass, boost::phoenix::ref(error_msgs_))]
        > lit(';');

      assignment_operator_r.name("assignment operator");
      assignment_operator_r
        %= lit("<-")
           [deprecate_old_assignment_op_f(boost::phoenix::ref(error_msgs_))]
        | (lit("=") >> no_skip[!char_("=")]);

      var_r.name("variable for left-hand side of assignment");
      var_r
        = identifier_r
          [identifier_to_var_f(_1, _r1, _val,  _pass,
                               boost::phoenix::ref(var_map_),
                               boost::phoenix::ref(error_msgs_))];

      // separate rule for name on expectation failure
      expression_rhs_r.name("expression assignable to left-hand side");
      expression_rhs_r
        %= expression_g(_r1);

      var_lhs_r.name("variable and array dimensions");
      var_lhs_r
        %= identifier_r
        >> opt_dims_r(_r1);

      opt_dims_r.name("array dimensions (optional)");
      opt_dims_r
        %=  *dims_r(_r1);

      dims_r.name("array dimensions");
      // uses silent test because errors will be reported in sliced rules
      dims_r
        %= lit('[')
        >> (expression_g(_r1)[validate_int_expr_silent_f(_1, _pass)] % ',')
        >> lit(']');

      // inherited  _r1 = true if samples allowed as statements
      sample_r.name("distribution of expression");
      sample_r
        %= (expression_g(_r2)
            >> lit('~'))
        > eps
          [validate_allow_sample_f(_r1, _pass,
                                   boost::phoenix::ref(error_msgs_))]
        > distribution_r(_r2)
        > -truncation_range_r(_r2)
        > lit(';')
        > eps
          [validate_sample_f(_val, boost::phoenix::ref(var_map_),
                             _pass, boost::phoenix::ref(error_msgs_))];

      distribution_r.name("distribution and parameters");
      distribution_r
        %= (identifier_r
            >> lit('(')
            >> -(expression_g(_r1) % ','))
        > lit(')');

      truncation_range_r.name("range pair");
      truncation_range_r
        %= lit('T')
        > lit('[')
        > -expression_g(_r1)
        > lit(',')
        > -expression_g(_r1)
        > lit(']');

      // _r1 = allow sampling, _r2 = var origin
      return_statement_r.name("return statement");
      return_statement_r
        %= (lit("return") >> no_skip[!char_("a-zA-Z0-9_")])
        >> expression_g(_r1)
        >> lit(';') [validate_return_allowed_f(_r1, _pass,
                                       boost::phoenix::ref(error_msgs_))];

      // _r1 = var origin
      void_return_statement_r.name("void return statement");
      void_return_statement_r
        = lit("return")[set_void_return_f(_val)]  // = expression()]
        >> lit(';')[validate_void_return_allowed_f(_r1, _pass,
                                        boost::phoenix::ref(error_msgs_))];

      no_op_statement_r.name("no op statement");
      no_op_statement_r
        %= lit(';')[set_no_op_f(_val)];
    }

  }
}
#endif
