#ifndef BASIC_SYSTEM_GRID_DRAWER
#define BASIC_SYSTEM_GRID_DRAWER
#include <cmath>
#include "grid.hpp"
#include "grid_displayer.hpp"
namespace basic_system {
template <const int xsize,const int ysize>
  class GridDrawer {
  public:
    GridDrawer(Grid2D<xsize,ysize> *grid) {
      _grid=grid;
    }
    void draw_wall(unsigned char wall_ch) {
      for(int i=0;i<ysize;i++) {
        if(i<xsize) {
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
      clear_grid(0,xsize-1,0,ysize-1);
    }
    void clear_grid_witout_wall() {
      clear_grid(1,xsize-2,1,ysize-2);
    }
    void clear_line_horizon(int y) {
      clear_grid(0,xsize-1,y,y);
    }
    void clear_line_vertical(int x) {
      clear_grid(x,x,0,ysize-1);
    }
    void clear_line_horizon_without_wall(int y) {
      clear_grid(1,xsize-2,y,y);
    }
    void clear_line_vertical_without_wall(int x) {
      clear_grid(x,x,1,ysize-2);
    }
    inline GridDrawer<xsize,ysize>&
    operator<<=(unsigned char& ch) {
      draw_wall(ch);
      return *this;
    }
    inline GridDrawer<xsize,ysize>&
    operator<<=(unsigned char ch) {
      draw_wall(ch);
      return *this;
    }
    inline GridDisplayer<xsize,ysize>&
    operator>>=(GridDisplayer<xsize,ysize>& displayer) {
      return displayer;
    }
    inline GridDrawer<xsize,ysize>&
    operator!() {
      clear_grid_witout_wall();
      return *this;
    }
  protected:
    Grid2D<xsize,ysize> *_grid;
  private:
    void fill_grid(unsigned char ch,int xs,int xe,int ys, int ye) {
      for(int i=ys;i<=ye;i++) {
        for(int j=xs;j<=xe;j++) {
          (*_grid)[i][j]=ch;
        }
      }
    }
    void clear_grid(int xs,int xe,int ys, int ye) {
      fill_grid(' ',xs,xe,ys,ye);
    }
  };
}
#endif
