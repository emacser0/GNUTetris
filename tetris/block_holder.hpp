#ifndef BLOCK_HOLDER
#define BLOCK_HOLDER
#include "../GNUgbs/basic_system.hpp"
#include "env.hpp"
#include "block.hpp"
namespace tetris {
  class BlockHolder {
  public:
    BlockHolder() {
      _holded=false;
      _cooldown=false;
    }
    ~BlockHolder() {

    }
    bool
    hold_block(Block &block) {
      if(!_cooldown) {
        if(_holded==false) {
          _holded_block=block;
          _holded=true;
          return false;
        }
        else {
          _blockcache=_holded_block;
          _holded_block=block;
          block=_blockcache;
          _cooldown=true;
          return true;
        }
      }
      return true;
    }
    void
    operator!() {
      _cooldown=false;
    }
    basic_system::GridDrawer<4,4>&
    operator>>=(basic_system::GridDrawer<4,4> &gd) {
      if(_holded) {
        gd.draw_block(_holded_block.block, 0, 0);
      }
      return gd;
    }
  protected:
    Block _holded_block;
    Block _blockcache;
    bool _holded;
    bool _cooldown;
  private:
  };
}
#endif
