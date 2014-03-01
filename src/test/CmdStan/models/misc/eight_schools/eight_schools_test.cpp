#include <gtest/gtest.h>
#include <test/CmdStan/models/model_test_fixture.hpp>

class Models_Misc_EightSchools : 
  public Model_Test_Fixture<Models_Misc_EightSchools> {
protected:
  virtual void SetUp() {
  }
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("misc");
    model_path.push_back("eight_schools");
    model_path.push_back("eight_schools");
    return model_path;
  }
  
  static bool has_data() {
    return true;
  }

  static bool has_init() {
    return false;
  }

  static int num_iterations() {
    return iterations;
  }

  static std::vector<int> skip_chains_test() {
    std::vector<int> params_to_skip;
    return params_to_skip;
  }

  static void populate_chains() {
    default_populate_chains();
  }

  static std::vector<std::pair<int, double> >
  get_expected_values() {
    std::vector<std::pair<int, double> > expected_values;
    return expected_values;
  }
};

INSTANTIATE_TYPED_TEST_CASE_P(Models_Misc_EightSchools,
            Model_Test_Fixture,
            Models_Misc_EightSchools);
