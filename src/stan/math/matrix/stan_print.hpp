#ifndef STAN__MATH__MATRIX__STAN_PRINT_HPP
#define STAN__MATH__MATRIX__STAN_PRINT_HPP

#include <vector>
#include <stan/math/matrix/Eigen.hpp>

namespace stan {
  namespace math {
    // prints used in generator for print() statements in modeling language

    template <typename T>
    void stan_print(std::ostream* o, const T& x) {
      *o << x;
    }
    
    template <typename T>
    void stan_print(std::ostream* o, const std::vector<T>& x) {
      *o << '[';
      for (int i = 0; i < x.size(); ++i) {
        if (i > 0) *o << ',';
        stan_print(o,x[i]);
      }
      *o << ']';
    }

    template <typename T>
    void stan_print(std::ostream* o, const Eigen::Matrix<T,Eigen::Dynamic,1>& x) {
      *o << '[';
      for (int i = 0; i < x.size(); ++i) {
        if (i > 0) *o << ',';
        stan_print(o,x(i));
      }
      *o << ']';
    }

    template <typename T>
    void stan_print(std::ostream* o, const Eigen::Matrix<T,1,Eigen::Dynamic>& x) {
      *o << '[';
      for (int i = 0; i < x.size(); ++i) {
        if (i > 0) *o << ',';
        stan_print(o,x(i));
      }
      *o << ']';
    }

    template <typename T>
    void stan_print(std::ostream* o, 
                    const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic>& x) {
      *o << '[';
      for (int i = 0; i < x.rows(); ++i) {
        if (i > 0) *o << ',';
        stan_print(o,x.row(i));
      }
      *o << ']';
    }

  }
}
#endif
