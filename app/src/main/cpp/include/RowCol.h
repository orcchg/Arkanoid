#ifndef __ARKANOID_ROWCOL__H__
#define __ARKANOID_ROWCOL__H__

#include "Block.h"

namespace game {

struct RowCol {
  int row, col;
  Block block;

  RowCol(int row = 0, int col = 0, Block block = Block::NONE)
    : row(row), col(col), block(block) {
  }
};

}

#endif  // __ARKANOID_ROWCOL__H__
