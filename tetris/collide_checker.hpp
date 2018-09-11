#ifndef COLLIDE_CHECKER
#define COLLIDE_CHECKER
#include <cmath>
#include "../basic_system/basic_system.hpp"
#include "env.hpp"
namespace tetris {
  template <const int xsize,const int ysize>
  class CollideChecker {
  public:
    CollideChecker(basic_system::Grid2D<xsize,ysize> *grid) {
      _grid=grid;
    }
    bool check(unsigned int block,int x,int y) {
      int calculated=std::pow(16,3);
      int table[4]={0x8,0x4,0x2,0x1};
      for(int i=0;i<4;i++,calculated/=16) {
        for(int j=0;j<4;j++) {
          if((*_grid)[y+i][x+j]!=' '&&
             (block&(table[j]*calculated))) {
            return true;
          }
        }
      }
      return false;
    }
  protected:
  private:
    basic_system::Grid2D<xsize,ysize> *_grid;
  };
}

#endif
