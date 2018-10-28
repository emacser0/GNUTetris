#ifndef BASIC_SYSTEM_GRID
#define BASIC_SYSTEM_GRID
#include <array>
namespace basic_system {
  template <const size_t size>
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

  template <const size_t xsize,const size_t ysize>
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
}
#endif
