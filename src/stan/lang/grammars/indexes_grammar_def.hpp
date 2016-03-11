#ifndef STAN_LANG_GRAMMARS_INDEXES_GRAMMAR_DEF_HPP
#define STAN_LANG_GRAMMARS_INDEXES_GRAMMAR_DEF_HPP

#include <stan/lang/ast.hpp>
#include <stan/lang/grammars/indexes_grammar.hpp>
#include <stan/lang/grammars/semantic_actions.hpp>
#include <stan/lang/grammars/whitespace_grammar.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

BOOST_FUSION_ADAPT_STRUCT(stan::lang::uni_idx,
                          (stan::lang::expression, idx_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::multi_idx,
                          (stan::lang::expression, idxs_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::lb_idx,
                          (stan::lang::expression, lb_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::ub_idx,
                          (stan::lang::expression, ub_) )

BOOST_FUSION_ADAPT_STRUCT(stan::lang::lub_idx,
                          (stan::lang::expression, lb_)
                          (stan::lang::expression, ub_) )

namespace stan {

  namespace lang {

    template <typename Iterator>
    indexes_grammar<Iterator>::indexes_grammar(variable_map& var_map,
                                               std::stringstream& error_msgs,
                                               expression_grammar<Iterator>& eg)
      : indexes_grammar::base_type(indexes_r),
        var_map_(var_map),
        error_msgs_(error_msgs),
        expression_g(eg) {
      using boost::spirit::qi::eps;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_r1;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::_pass;

      indexes_r.name("indexes (zero or more)");
      indexes_r
        %=  lit("[")
        > (index_r(_r1) % ',')
        > lit("]");

      index_r.name("index expression, one of: "
                   " int, int[], int:, :int, int:int, :)");
      index_r
        %= lub_index_r(_r1)
        | lb_index_r(_r1)
        | uni_index_r(_r1)
        | multi_index_r(_r1)
        | ub_index_r(_r1)
        | omni_index_r(_r1);

      lub_index_r.name("index expression, one of: "
                       " int, int[], int:, :int, int:int, :)");
      lub_index_r
        %= int_expression_r(_r1)
        >> lit(":")
        >> int_expression_r(_r1)
        > eps;

      lb_index_r.name("index expression, one of: "
                      " int, int[], int:, :int, int:int, :)");
      lb_index_r
        %= int_expression_r(_r1)
        >> lit(":")
        > eps;

      uni_index_r.name("index expression, one of: "
                       " int, int[], int:, :int, int:int, :)");
      uni_index_r
        %= int_expression_r(_r1);

      multi_index_r.name("index expression, one of: "
                         " int, int[], int:, :int, int:int, :)");
      multi_index_r
        %= expression_g(_r1)
           [validate_ints_expression_f(_1, _pass,
                                       boost::phoenix::ref(error_msgs_))]
        > eps;

      ub_index_r.name("index expression, one of: "
                      " int, int[], int:, :int, int:int, :)");
      ub_index_r
        %= lit(":")
        >> int_expression_r(_r1)
        > eps;

      omni_index_r.name("index expression, one of: "
                        " int, int[], int:, :int, int:int, :)");
      omni_index_r
        = lit(":")[set_omni_idx_f(_val)]
        |  eps[set_omni_idx_f(_val)];

      int_expression_r.name("integer expression");
      int_expression_r
        %= expression_g(_r1)[validate_int_expression_f(_1, _pass)];
    }

  }
}
#endif
