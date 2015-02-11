#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <test/test-models/good/lang/reject_transformed_data.hpp>

/* tests that stan program throws exception in transformed data block
   this block is part of the generated cpp object's constructor
*/


TEST(StanCommon, reject_transformed_data) {
  std::string error_msg = "user-specified rejection";

  std::fstream empty_data_stream(std::string("").c_str());
  stan::io::dump empty_data_context(empty_data_stream);
  empty_data_stream.close();
  std::stringstream model_output;
  model_output.str("");

  // instantiate model
  try {
    reject_transformed_data_model_namespace::reject_transformed_data_model* model 
      = new reject_transformed_data_model_namespace::reject_transformed_data_model(empty_data_context, &model_output);
    delete model;
  } catch (const std::domain_error& e) {
    if (std::string(e.what()).find(error_msg) == std::string::npos) {
      FAIL() << std::endl << "---------------------------------" << std::endl
             << "--- EXPECTED: error_msg=" << error_msg << std::endl
             << "--- FOUND: e.what()=" << e.what() << std::endl
             << "---------------------------------" << std::endl
             << std::endl;
    }
    return;
  }
  FAIL() << "model failed to do reject" << std::endl;

}

