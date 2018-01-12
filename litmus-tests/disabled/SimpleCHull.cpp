#include "simple_convex_hull.h"

#include <flit.h>

#include <cmath>
#include <cstdio>
#include <typeinfo>

template <typename T>
class SimpleCHull: public flit::TestBase<T> {
public:
  SimpleCHull(std::string id) : flit::TestBase<T>(std::move(id)) {}

  virtual size_t getInputsPerRun(){ return 0; }
  virtual std::vector<T> getDefaultInput(){ return {}; }

protected:
  virtual flit::KernelFunction<T>* getKernel() { return nullptr; }

  virtual flit::Variant run_impl(const std::vector<T>& ti) {
    FLIT_UNUSED(ti);
    CHullEdges.clear();
    PointList.clear();
    ReadInputs(fopen("data/random_input", "r"));
    SimpleComputeConvexhull<T>();
    return getEdgeCount();
  }

protected:
  using flit::TestBase<T>::id;
};

REGISTER_TYPE(SimpleCHull)
