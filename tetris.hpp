#ifndef TETRIS
#define TETRIS
#include "basic_system/basic_system.hpp"
namespace tetris {
  const int nextblock = 5;
  const int blocknumber = 7;
  const int x_size=12,y_size=22;
  const int startx=4,starty=1;
  const unsigned int tetromino[7]=
    {//I,J,L,O,S,T,Z
      0x0F00,
      0x0470,
      0x02E0,
      0x0660,
      0x0360,
      0x04E0,
      0x0C60
    };
  //b is an alias of block

  struct Block{
    int x,y,block;
  };
  template <const int xsize,const int ysize>
  class CollideChecker;
  template <const int xsize,const int ysize>
  class BlockManager {
  public:
    BlockManager() {
      _seq=basic_system::random::random_sequence<blocknumber>();
    }
    unsigned int
    rotate_r(unsigned int b) {
      return
        (((b&0x8000)>>3)|((b&0x800)<<2)|((b&0x80)<<7)|((b&0x8)<<12))|
        (((b&0x4000)>>6)|((b&0x400)>>1)|((b&0x40)<<4)|((b&0x4)<<9))|
        (((b&0x2000)>>9)|((b&0x200)>>4)|((b&0x20)<<1)|((b&0x2)<<6))|
        (((b&0x1000)>>12)|((b&0x100)>>7)|((b&0x10)>>2)|((b&0x1)<<3));
    }
    unsigned int
    rotate_l(unsigned int b) {
      return
        (((b&0x8000)>>12)|((b&0x800)>>9)|((b&0x80)>>6)|((b&0x8)>>3))|
        (((b&0x4000)>>7)|((b&0x400)>>4)|((b&0x40)>>1)|((b&0x4)<<2))|
        (((b&0x2000)>>2)|((b&0x200)<<1)|((b&0x20)<<4)|((b&0x2)<<7))|
        (((b&0x1000)<<3)|((b&0x100)<<6)|((b&0x10)<<9)|((b&0x1)<<12));
    }
    void move_block(int dx,int dy) {
      _cur_block.x+=dx;
      _cur_block.y+=dy;
    }
    void change_block() {
      int blockidx;
      _seq >>= blockidx;
      _cur_block.block=tetromino[blockidx];
      _cur_block.x=startx;
      _cur_block.y=starty;
    }
    inline BlockManager<xsize,ysize>&
    operator!() {
      change_block();
      return *this;
    }
    inline basic_system::Drawer<xsize,ysize>&
    operator>>=(basic_system::Drawer<xsize,ysize> &drawer) {
      drawer.draw_block(_cur_block.block,_cur_block.x,_cur_block.y);
      return drawer;
    }
    inline bool
    operator>>=(CollideChecker<xsize,ysize> &cc) {
      return cc.check(_cur_block.block,_cur_block.x,_cur_block.y);
    }
    template <typename T>
    inline BlockManager<xsize,ysize>&
    operator<<=(std::array<T,2> d) {
      _cur_block.x+=d[0];
      _cur_block.y+=d[1];
      return *this;
    }
    basic_system::random::random_sequence<blocknumber> _seq;
  protected:
    Block _cur_block;
  private:
  };
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
  namespace scoring {
    unsigned int score,spin[7];
  }
}
#endif
