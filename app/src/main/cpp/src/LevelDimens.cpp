#include "LevelDimens.h"

namespace game {

void LevelDimens::getBlockDimens(
    int row,
    int col,
    GLfloat* top_border,
    GLfloat* bottom_border,
    GLfloat* left_border,
    GLfloat* right_border) {

  *top_border = row * block_height;
  *bottom_border = (row + 1) * block_height;
  *left_border = col * block_width;
  *right_border = (col + 1) * block_width;
}

}
