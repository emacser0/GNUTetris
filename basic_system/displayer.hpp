#ifndef BASIC_SYSTEM_DISPLAYER
#define BASIC_SYSTEM_DISPLAYER
#include <iostream>
#include "env.hpp"
namespace basic_system {
  template <const int xsize,const int ysize>
  class Displayer {
  public:
    Displayer(Grid2D<xsize,ysize> *grid) {
      this->_grid=grid;
    }
    inline std::ostream& clear(std::ostream &stream) {
      stream << "\033c";
      return stream;
    }
    inline std::ostream& operator>>=(std::ostream &stream) {
      clear(stream);
      for(int i=0;i<ysize;i++) {
        for(int j=0;j<xsize;j++) {
          stream << (*_grid)[i][j];
        }
        stream << "\n";
      }
      return stream;
   }
  protected:
    Grid2D<xsize,ysize> *_grid;
  private:
    std::ostream& display(std::ostream &stream) {
      return this >>= stream;
    }
    std::ostream& display() {
      return this >>= std::cout;
    }
  };
}
#endif
