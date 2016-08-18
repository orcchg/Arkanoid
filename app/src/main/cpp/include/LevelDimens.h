#ifndef __ARKANOID_LEVEL_DIMENS__H__
#define __ARKANOID_LEVEL_DIMENS__H__

#include <GLES/gl.h>

namespace game {

/// @brief Measured level dimensions.
class LevelDimens {
public:
  constexpr static float blockWidth = 0.2f;
  constexpr static float blockHeight = 0.1f;

  LevelDimens(
      int rows,
      int cols,
      GLfloat w,
      GLfloat h,
      GLfloat bw,
      GLfloat bh)
    : rows(rows)
    , cols(cols)
    , width(w)
    , height(h)
    , block_width(bw)
    , block_height(bh) {
  }

  void getBlockDimens(
      int row,
      int col,
      GLfloat* top_border,
      GLfloat* bottom_border,
      GLfloat* left_border,
      GLfloat* right_border);

  inline int getRows() const { return rows; }
  inline int getCols() const { return cols; }
  inline GLfloat getWidth() const { return width; }
  inline GLfloat getHeight() const { return height; }
  inline GLfloat getBlockWidth() const { return block_width; }
  inline GLfloat getBlockHeight() const { return block_height; }

private:
  int rows, cols;
  GLfloat width, height, block_width, block_height;

};

}

#endif  // __ARKANOID_LEVEL_DIMENS__H__
