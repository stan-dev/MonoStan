#include <gtest/gtest.h>
#include <stan/mcmc/hmc/hamiltonians/ps_point.hpp>

namespace stan {

  namespace mcmc {

    class ps_point_test : public ::testing::Test
    {
    public:
      typedef Eigen::Matrix<double, Eigen::Dynamic, 1> vector_t;
      typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_t;

      static void fast_vector_copy()
      {
	vector_t from3(3); from3 << 5.25, 3.125, -6.5;
	vector_t to3(12);
	ps_point::_fast_vector_copy(to3, from3);

	EXPECT_EQ(from3, to3);

	int zero = 0;
	vector_t from0(zero);
	vector_t to0(7);
	ps_point::_fast_vector_copy(to0, from0);

	EXPECT_EQ(from0, to0);    
      }

      static void fast_matrix_copy()
      {
	matrix_t from2_3(2, 3);
	from2_3 << 5, 2, 7, -3, 4, -9;
	matrix_t to2_3(1, 13);
	ps_point::_fast_matrix_copy(to2_3, from2_3);

	EXPECT_EQ(from2_3, to2_3);

	int zero = 0;

	matrix_t from2_0(2, zero);
	matrix_t to2_0(8, 3);
	ps_point::_fast_matrix_copy(to2_0, from2_0);

	EXPECT_EQ(from2_0, to2_0);

	matrix_t from0_5(zero, 5);
	matrix_t to0_5(7, 4);
	ps_point::_fast_matrix_copy(to0_5, from0_5);

	EXPECT_EQ(from0_5, to0_5);
      }
  
    };

    TEST(psPoint, fastVectorCopy) {
      ps_point_test::fast_vector_copy();
    }

    TEST(psPoint, fastMatrixCopy) {
      ps_point_test::fast_matrix_copy();
    }
  }
}
