#ifndef TETRIS_ENV
#define TETRIS_ENV
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

  template <const int xsize,const int ysize>
  class CollideChecker;

  template <const int xsize,const int ysize>
  class BlockManager;
}
#endif
