#ifndef BLOCK_STACKER
#define BLOCK_STACKER
#include <set>
#include "../basic_system/basic_system.hpp"
#include "env.hpp"
namespace tetris {
  template <const int xsize,const int ysize>
  class BlockStacker {
  public:
    void add_block(unsigned int block, int x, int y) {
      int calculated=std::pow(16,3);
      int table[4]={0x8,0x4,0x2,0x1};
      for(int i=0;i<4;i++,calculated/=16) {
        for(int j=0;j<4;j++) {
          if((block&(table[j]*calculated))) {
            blocks.insert({x+j,y+i});
          }
        }
      }
    }
    basic_system::Drawer<xsize,ysize>&
    operator>>=(basic_system::Drawer<xsize,ysize>& drawer) {
      for(auto i : blocks) {
        drawer.draw_block(0x8000, i[0],i[1]);
      }
      return drawer;
    }
  protected:
    std::set<std::array<int,2>> blocks;
  private:
  };
}
#endif
