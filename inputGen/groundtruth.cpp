#include "groundtruth.h"

#include <flit.h>

// Only store these locally because we want multiple compiled copies
namespace {
  template<typename T> TruthType<T>
  runGroundtruth_impl(std::string testName,
                      std::function<T()> randGen)
  {
    using flit::Vector;

    auto test = flit::getTests()[testName]->get<T>();
    auto input = test->getDefaultInput();
    input = Vector<T>(test->getInputsPerRun(), randGen).getData();
    auto scores = test->run(input);

    // Return only the first score.  Ignore the key
    return { input, std::get<0>(scores.begin()->second) };
  }
} // end of unnamed namespace

TruthType<float>
runGroundtruth(const std::string &testName, std::function<float()> randGen) {
  return runGroundtruth_impl(testName, randGen);
}

TruthType<double>
runGroundtruth(const std::string &testName, std::function<double()> randGen) {
  return runGroundtruth_impl(testName, randGen);
}

TruthType<long double>
runGroundtruth(const std::string &testName, std::function<long double()> randGen) {
  return runGroundtruth_impl(testName, randGen);
}

