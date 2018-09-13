#ifndef BLOCK_HOLDER
#define BLOCK_HOLDER
#include "../basic_system/basic_system.hpp"
#include "env.hpp"
#include "block.hpp"
namespace tetris {
  class BlockHolder {
  public:
    BlockHolder() {
      _holded_block=NULL;
    }
    ~BlockHolder() {

    }
    void
    hold_block(Block &block) {
      _holded_block=&block;
    }
  protected:
    Block* _holded_block;
  private:
  };
}
#endif
