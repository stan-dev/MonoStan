#ifndef STAN__ERROR_HANDLING__MATRIX__CHECK_POS_SEMIDEFINITE_HPP
#define STAN__ERROR_HANDLING__MATRIX__CHECK_POS_SEMIDEFINITE_HPP

#include <sstream>

#include <boost/math/special_functions/fpclassify.hpp>

#include <stan/error_handling/scalar/dom_err.hpp>
#include <stan/error_handling/matrix/constraint_tolerance.hpp>
#include <stan/math/matrix/Eigen.hpp>
#include <stan/math/matrix/meta/index_type.hpp>
#include <stan/math/matrix/value_of_rec.hpp>

namespace stan {

  namespace error_handling {

    /**
     * Return <code>true</code> if the specified matrix is positive definite
     *
     * NOTE: symmetry is NOT checked by this function
     * 
     * @param function
     * @param y Matrix to test.
     * @param name
     * @return <code>true</code> if the matrix is positive semi-definite.
     * @return throws if any element in y is nan
     * @tparam T Type of scalar.
     */
    // FIXME: update warnings (message has (0,0) item)
    template <typename T_y>
    inline bool 
    check_pos_semidefinite(const char* function,
                           const char* name,
                           const Eigen::Matrix<T_y,
                                               Eigen::Dynamic,
                                               Eigen::Dynamic>& y) {
      using Eigen::Dynamic;
      using Eigen::Matrix;
      using stan::math::index_type;
      using stan::math::value_of_rec;

      typedef typename index_type<Matrix<T_y,Dynamic,Dynamic> >::type size_type;

      if (y.rows() == 1 && !(y(0,0) >= 0.0)) {
        std::ostringstream msg;
        msg << "is not positive semi-definite. " 
            << name << "(0,0) is ";
        std::string msg_str(msg.str());
        dom_err(function, name, y(0,0),
                msg_str.c_str());
      }
      Eigen::LDLT< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > cholesky 
        = value_of_rec(y).ldlt();
      if(cholesky.info() != Eigen::Success || (cholesky.vectorD().array() < 0.0).any()) {
        std::ostringstream msg;
        msg << "is not positive semi-definite. " 
            << name << "(0,0) is ";
        std::string msg_str(msg.str());
        dom_err(function, name, y(0,0),
                msg_str.c_str());
      }
      for (int i = 0; i < y.size(); i++)
        if (boost::math::isnan(value_of_rec(y(i)))) {
          std::ostringstream msg;
          msg << "is not positive semi-definite. " 
                  << name << "(0,0) is ";
          std::string msg_str(msg.str());
          dom_err(function, name, y(0,0), 
                  msg_str.c_str(), "");
        }
      return true;
    }

  }
}
#endif
