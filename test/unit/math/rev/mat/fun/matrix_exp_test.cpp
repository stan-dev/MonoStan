#include <stan/math/rev/mat.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/rev/mat/fun/expect_matrix_eq.hpp>
#include <test/unit/math/rev/mat/fun/util.hpp>

using stan::math::matrix_v;

TEST(MathMatrix, expm) {
       
    matrix_v m1(1,1), m2(1,1), m1_exp;
    m1 << 0;
    m2 << 1;
    m1_exp = expm(m1);
    expect_matrix_eq(m2, m1_exp);
    
    AVEC x = createAVEC(m1(0,0));
    VEC g;
    m1_exp(0,0).grad(x,g);
    EXPECT_FLOAT_EQ(m1_exp(0,0).val(), g[0]); 
    
}

TEST(MathMatrix, expm_2x2) {
    
    // example from Moler & Van Loan, 2003
    for (size_t k = 0; k < 2; k++) {
    	for (size_t l = 0; l < 2; l++) {
    
    		matrix_v m1(2,2), m2(2,2), m1_exp,
    				 dm1_exp_da(2,2), dm1_exp_db(2,2);
    
    		AVAR a = -1.0, b = -17.0;
   
    		m1 << -2*a + 3*b, 1.5*a - 1.5*b, -4*a + 4*b, 3*a - 2*b;
   			m2 << -.735759, .551819, -1.471518, 1.103638;
    		m1_exp = expm(m1);
    		expect_matrix_eq(m2, m1_exp);
    
    		dm1_exp_da << -2*exp(a), 1.5*exp(a), -4*exp(a), 3*exp(a);
    		dm1_exp_db << 3*exp(b), -1.5*exp(b), 4*exp(b), -2*exp(b);
    
    		AVEC x = createAVEC(a, b);
    		VEC g;
    		m1_exp(k, l).grad(x, g);
    		EXPECT_FLOAT_EQ(dm1_exp_da(k, l).val(), g[0]);
    		EXPECT_FLOAT_EQ(dm1_exp_db(k, l).val(), g[1]);
    	}
    }
 
}

TEST(MathMatrix, expm3) {
    matrix_v m1(0,0), m2(1,2);
    
    m2 << 1, 2;
    
    EXPECT_THROW(expm(m1), std::invalid_argument);
    EXPECT_THROW(expm(m2), std::invalid_argument);
}