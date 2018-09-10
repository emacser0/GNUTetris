#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <functional>
#include <string>
#include <algorithm>
#include <ncursesw/ncurses.h>
namespace basic_system {
  namespace random {
    template <typename T,const int s,const int e>
    class mt_gen {
    public:
      mt_gen() {
        generator =
          std::bind(std::uniform_int_distribution<T>(s,e),
                    std::mt19937
                    (std::chrono::
                     high_resolution_clock::
                     now().time_since_epoch().
                     count()));
      };
      inline T& operator>>=(T &v) {
        v=generator();
        return v;
      }
      std::function<int ()> generator;
    protected:
    private:
    };
    template<int len>
    class random_sequence {
    public:
      random_sequence() {
        for(int i=0;i<len;i++) {
          _seq[i]=i;
        }
        generator_r=mt_gen<int,0,len-1>();
        shuffle_seq();
        index=0;
      }
      void
      shuffle_seq() {
        int t,i_rand;
        for(int i=0;i<len;i++) {
          generator_r >>= i_rand;
          t=_seq[i];
          _seq[i]=_seq[i_rand];
          _seq[i_rand]=t;
        }
      }
      void
      shuffle_seq_std() {
        std::random_shuffle(_seq.begin(),_seq.end());
      }
      inline int& operator>>=(int &v) {
        if(index==7) {
          shuffle_seq();
          index=0;
        }
        v = _seq[index++];
        return v;
      }
    protected:
      mt_gen<int,0,len-1> generator_r;
      std::array<int,len> _seq;
      int index;
    private:
    };
  }


  template <const int size>
  class Grid1D {
  public:
    Grid1D() {
      Grid1D(' ');
    }
    Grid1D(unsigned char ch) {
      for(int i=0;i<size;i++) {
        _grid[i]=ch;
      }
    }
    inline unsigned char&
    operator[](const int idx) {
      return _grid[idx];
    }
  protected:
    std::array<unsigned char,size> _grid;
  private:
  };

  template <const int xsize,const int ysize>
  class Grid2D {
  public:
    Grid2D() {
      for(int i=0;i<ysize;i++) {
        _grid[i]=Grid1D<xsize>(' ');
      }
    }
    Grid2D(unsigned char ch) {
      for(int i=0;i<ysize;i++) {
        _grid[i]=Grid1D<xsize>(ch);
      }
    }
    inline Grid1D<xsize>&
    operator[](const int idx) {
      return _grid[idx];
    }
  private:
    std::array<Grid1D<xsize>,ysize> _grid;
  };
  template <const int xsize,const int ysize>
  class Displayer;

  template <const int xsize,const int ysize>
  class Drawer {
  public:
    Drawer(Grid2D<xsize,ysize> *grid) {
      _grid=grid;
    }
    void draw_wall(char wall_ch) {
      for(int i=0;i<ysize;i++) {
        if(i<10) {
          (*_grid)[0][i]=wall_ch;
          (*_grid)[ysize-1][i]=wall_ch;
        }
        (*_grid)[i][0]=wall_ch;
        (*_grid)[i][xsize-1]=wall_ch;
      }
    }
    void draw_block(unsigned int block,int x,int y) {
      int calculated=std::pow(16,3);
      int table[4]={0x8,0x4,0x2,0x1};
      for(int i=0;i<4;i++,calculated/=16) {
        for(int j=0;j<4;j++) {
          (*_grid)[y+i][x+j]=(block&(table[j]*calculated))?'#':
            (*_grid)[y+i][x+j];
        }
      }
    }
    void clear_grid_all() {
      clear_grid(0,xsize,0,ysize);
    }
    void clear_grid_witout_wall() {
      clear_grid(1,xsize-1,1,ysize-1);
    }
    void clear_line_horizon(int y) {
      clear_grid(0,xsize,y,y+1);
    }
    void clear_line_vertical(int x) {
      clear_grid(x,x+1,0,ysize);
    }
    void clear_line_horizon_without_wall(int y) {
      clear_grid(1,xsize-1,y,y+1);
    }
    void clear_line_vertical_without_wall(int x) {
      clear_grid(x,x+1,1,ysize-1);
    }
    inline Drawer<xsize,ysize>&
    operator<<=(unsigned char& ch) {
      draw_wall(ch);
      return *this;
    }
    inline Drawer<xsize,ysize>&
    operator<<=(unsigned char ch) {
      draw_wall(ch);
      return *this;
    }
    inline Displayer<xsize,ysize>&
    operator>>=(Displayer<xsize,ysize>& displayer) {
      return displayer;
    }
    inline Drawer<xsize,ysize>&
    operator!() {
      clear_grid_witout_wall();
      return *this;
    }
  protected:
    Grid2D<xsize,ysize> *_grid;
  private:
    void fill_grid(unsigned char ch,int xs,int xe,int ys, int ye) {
      for(int i=ys;i<ye;i++) {
        for(int j=xs;j<xe;j++) {
          (*_grid)[i][j]=ch;
        }
      }
    }
    void clear_grid(int xs,int xe,int ys, int ye) {
      fill_grid(' ',xs,xe,ys,ye);
    }
  };

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



namespace bs = basic_system;
namespace ts = tetris;
int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  auto grid=bs::Grid2D<ts::x_size,ts::y_size>();
  auto displayer = bs::Displayer<ts::x_size,ts::y_size>(&grid);
  auto drawer = bs::Drawer<ts::x_size,ts::y_size>(&grid);
  auto collide_checker=ts::
    CollideChecker<ts::x_size,ts::y_size>(&grid);
  auto block_manager=ts::
    BlockManager<ts::x_size,ts::y_size>();
  int time=100;
  std::array<int,2> move_d={0,1};
  int freq=100;
  !block_manager;
  drawer <<= '@';
  while(time--) {
    ((block_manager >>= drawer) >>= displayer) >>= std::cout;
    !drawer;
    block_manager <<= move_d;
    if(block_manager >>= collide_checker) {
      !block_manager;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
}
