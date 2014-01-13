#include <gtest/gtest.h>
#include <test/CmdStan/models/model_test_fixture.hpp>

class Models_BugsExamples_Vol1_Magnesium : 
  public Model_Test_Fixture<Models_BugsExamples_Vol1_Magnesium> {
protected:
  virtual void SetUp() {
  }
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("bugs_examples");
    model_path.push_back("vol1");
    model_path.push_back("magnesium");
    model_path.push_back("magnesium");
    return model_path;
  }

  static bool has_data() {
    return true;
  }
  
  static bool has_init() {
    return true;
  }

  static int num_iterations() {
    return 8000;
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
    using std::make_pair;
    std::vector<std::pair<int, double> > expected_values;
    
    expected_values.push_back(make_pair(chains->index("OR[1]"), 0.4591));
    expected_values.push_back(make_pair(chains->index("tau[1]"), 0.5845));

    expected_values.push_back(make_pair(chains->index("OR[2]"), 0.4179));
    expected_values.push_back(make_pair(chains->index("tau[2]"), 1.081));
    
    expected_values.push_back(make_pair(chains->index("OR[3]"), 0.4368));
    expected_values.push_back(make_pair(chains->index("tau[3]"), 0.8119));

    expected_values.push_back(make_pair(chains->index("OR[4]"), 0.4639));
    expected_values.push_back(make_pair(chains->index("tau[4]"), 0.5084));

    expected_values.push_back(make_pair(chains->index("OR[5]"), 0.483));
    expected_values.push_back(make_pair(chains->index("tau[5]"), 0.5245));

    expected_values.push_back(make_pair(chains->index("OR[6]"), 0.4347));
    expected_values.push_back(make_pair(chains->index("tau[6]"), 0.5736));
    
    return expected_values;
  }

};

INSTANTIATE_TYPED_TEST_CASE_P(Models_BugsExamples_Vol1_Magnesium,
            Model_Test_Fixture,
            Models_BugsExamples_Vol1_Magnesium);
