#ifndef BASIC_SYSTEM
#define BASIC_SYSTEM
#include <iostream>
#include <array>
#include <cmath>
#include "random.hpp"
namespace basic_system {
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
#endif
