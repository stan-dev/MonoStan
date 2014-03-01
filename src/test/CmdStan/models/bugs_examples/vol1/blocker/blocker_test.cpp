#include <gtest/gtest.h>
#include <test/CmdStan/models/model_test_fixture.hpp>

class Models_BugsExamples_Vol1_Blocker : 
  public Model_Test_Fixture<Models_BugsExamples_Vol1_Blocker> {
protected:
  virtual void SetUp() {
  }
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("bugs_examples");
    model_path.push_back("vol1");
    model_path.push_back("blocker");
    model_path.push_back("blocker");
    return model_path;
  }

  static bool has_data() {
    return true;
  }

  static bool has_init() {
    return true;
  }

  static int num_iterations() {
    return 4000;
  }

  static std::vector<int> skip_chains_test() {
    std::vector<int> params_to_skip;
    // FIXME: remove this when forward sampling is available
    params_to_skip.push_back(chains->index("delta_new"));
    return params_to_skip;
  }

  static void populate_chains() {
    default_populate_chains();
  }

  static std::vector<std::pair<int, double> >
  get_expected_values() {
    using std::make_pair;
    std::vector<std::pair<int, double> > expected_values;
    expected_values.push_back(make_pair(chains->index("d"), -0.2492));
    expected_values.push_back(make_pair(chains->index("delta_new"), -0.2499));
    expected_values.push_back(make_pair(chains->index("sigma_delta"), 0.1189));
    
    return expected_values;
  }

};

INSTANTIATE_TYPED_TEST_CASE_P(Models_BugsExamples_Vol1_Blocker,
            Model_Test_Fixture,
            Models_BugsExamples_Vol1_Blocker);
