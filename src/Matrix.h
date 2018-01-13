#ifndef FLIT_MATRIX_H
#define FLIT_MATRIX_H

#include "Vector.h"

#include <initializer_list>   // for std::initializer_list
#include <iostream>           // for std::cout
#include <ostream>            // for std::ostream
#include <vector>             // for std::vector

namespace flit {

template<typename T>
class Matrix {
  std::vector<std::vector<T>> data;
public:
  Matrix(unsigned rows, unsigned cols):
    data(rows, std::vector<T>(cols, 0)){}
  Matrix(Matrix<T> const &m):data(m.data){}
  Matrix(std::initializer_list<std::initializer_list<T>> l):
    data(l.size(), std::vector<T>(l.begin()->size())){
    int x = 0; int y = 0;
    for(auto r: l){
      for(auto i: r){
        data[x][y] = i;
        ++y;
      }
      ++x; y = 0;
    }
  }

  friend class Vector<T>;
  template<class U>
  friend std::ostream& operator<<(std::ostream& os, Matrix<U> const &a);


  bool
  operator==(Matrix<T> const &rhs) const {
    bool retVal = true;
    for(uint x = 0; x < data.size(); ++x){
      for(uint y = 0; y < data[0].size(); ++y){
        if(data[x][y] != rhs.data[x][y]){
          info_stream << "in: " << __func__ << std::endl;
          info_stream << "for x,y: " << x << ":" << y << std::endl;
          info_stream << "this = " << data[x][y] << "; rhs = " << rhs.data[x][y] << std::endl;
          retVal = false;
          break;
        }
      }
    }
    return retVal;
  }

  Matrix<T>
  operator*(T const &sca){
    Matrix<T> retVal(data.size(), data[0].size());
    for(uint x = 0; x < data.size(); ++x){
      for(uint y =0; y < data[0].size(); ++y){
        retVal.data[x][y] = data[x][y] * sca;
      }
    }
    return retVal;
  }

  //precond: this.w = rhs.h, duh
  Matrix<T>
  operator*(Matrix<T> const &rhs){
    Matrix<T> retVal(data.size(), rhs.data[0].size());
    for(uint bcol = 0; bcol < rhs.data[0].size(); ++bcol){
      for(uint x = 0; x < data.size(); ++x){
        for(uint y = 0; y < data[0].size(); ++y){
          retVal.data[x][bcol] += data[x][y] * rhs.data[y][bcol];
        }
      }
    }
    return retVal;
  }

  //precond: dim(v) == 3
  static
  Matrix<T>
  SkewSymCrossProdM(Vector<T> const &v){
    return Matrix<T>(
      {{0, -v[2], v[1]},
       {v[2], 0, -v[0]},
       {-v[1], v[0], 0}});
  }

  static
  Matrix<T>
  Identity(size_t dims){
    Matrix<T> retVal(dims, dims);
    for(size_t x = 0; x < dims; ++x){
      for(size_t y =0; y < dims; ++y){
        if(x == y) retVal.data[x][y] = 1;
        else retVal.data[x][y] = 0;
      }
    }
    return retVal;
  }

  Vector<T>
  operator*(Vector<T> const &v) const {
    Vector<T> retVal(data.size());
    int resI = 0;
    for(auto row: data){
      for(size_t i = 0; i < row.size(); ++i){
        retVal[resI] += row[i] * v[i];
      }
      ++resI;
    }
    return retVal;
  }

  Matrix<T>
  operator+(Matrix<T> const&rhs) const{
    Matrix<T> retVal(rhs);
    int x = 0; int y = 0;
    for(auto r: data){
      for(auto i: r){
        retVal.data[x][y] = i + rhs.data[x][y];
        ++y;
      }
      y = 0; ++x;
    }
    return retVal;
  }

  void
  print() const {
    std::cout << *this;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Matrix<T> const &m){
  for(auto r: m.data){
    for(auto i: r){
      os << i << '\t';
    }
    os << std::endl;
  }
  return os;
}

} // end of namespace flit

#endif // FLIT_MATRIX_H