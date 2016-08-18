/*
 * Level.h
 *
 *  Created on: Apr 7, 2015
 *      Author: Maxim Alov <alovmax@yandex.ru>
 */

#ifndef INCLUDE_LEVEL_H_
#define INCLUDE_LEVEL_H_

#include <memory>
#include <string>
#include <vector>

#include <GLES/gl.h>

#include "Block.h"
#include "logger.h"
#include "Prize.h"
#include "RowCol.h"

namespace game {

enum class Direction : int {
  NONE = 0,
  UP = 1,
  DOWN = 2,
  RIGHT = 3,
  LEFT = 4
};

enum class Mode : int {
  NONE = 0,
  UPGRADE = 1,
  DEGRADE = 2
};

/**
 * @class Level Level.h "include/Level.h"
 * @brief Represents game level.
 */
class Level {
public:
  typedef std::shared_ptr<Level> Ptr;

  ~Level() noexcept;

  /** @defgroup Convert Convert functions to other data types.
   * @{
   */
  /// @brief Decode string array and get Level instance from it.
  /// @param array Input string array.
  /// @param length Size of imput string array.
  /// @return Level instance.
  static Level::Ptr fromStringArray(const std::vector<std::string>& array, size_t length);

  /// @brief Converts this Level instance to string array.
  /// @param array Pointer to an output string array.
  /// @return Size of output array.
  size_t toStringArray(std::vector<std::string>* array) const;

  /// @brief Converts this Level instance to vertex array.
  /// @param width Width of each block to display.
  /// @param height Height of each block to display.
  /// @param x_offset Horizontal offset from zero for each vertex
  /// in OpenGL coordinate system.
  /// @param y_offset Vertical offset from zero for each vertex
  /// in OpenGL coordinate system.
  /// @param array Output vertex array.
  /// @return Size of output array.
  /// @details Memory for output array should be allocated manually
  /// by Client, required size for allocation is 16 * cols * rows.
  /// @note Each vertex has 2 coordinates (x, y, z=0, w=1).
  void toVertexArray(
      GLfloat width,
      GLfloat height,
      GLfloat x_offset,
      GLfloat y_offset,
      GLfloat* const array) const;
  /** @} */  // end of Convert group

  /// @brief Fills input array with color values corresponding to
  /// blocks in this Level instance.
  /// @param array Output color array.
  /// @details Memory for output array should be allocated manually
  /// by Client, required size for allocation is 16 * cols * rows.
  void fillColorArray(GLfloat* const array) const;
  /// @brief Fills input array with color values at position corresponding
  /// to the specified block in this Level instance.
  /// @param array Output color array.
  /// @param row Row index of specified block.block != Block::
  /// @param col Column index of specified block.
  /// @details Memory for output array should be allocated manually
  /// by Client, required size for allocation is 16 * cols * rows.
  void fillColorArrayAtBlock(GLfloat* const array, int row, int col) const;

  /// @brief Returns height of level.
  inline int numRows() const { return rows; }
  /// @brief Returns width of level.
  inline int numCols() const { return cols; }
  /// @brief Returns total blocks of level.
  inline int size() const { return rows * cols; }
  /// @brief Gets block generator instance.
  inline BlockGenerator& getGenerator() { return generator; }
  /// @brief Gets prize generator instance.
  inline PrizeGenerator& getPrizeGenerator() { return prize_generator; }
  /// @brief Gets block by row and column indices.
  inline Block getBlock(int row, int col) const { return blocks[row][col]; }
  /// @brief Sets the block by row and column indices.
  inline void setBlock(int row, int col, Block value) { blocks[row][col] = value; }
  /// @brief Sets the block by row and column indices only
  /// in case it is vulnerable.
  /// @note Re-calculates cardinality.
  void setVulnerableBlock(int row, int col, Block value);
  /// @brief Changes the block by row and column indices only
  /// in case it is vulnerable, according to the given mode.
  /// @note Re-calculates cardinality.
  void changeVulnerableBlock(Mode mode, int row, int col);
  /// @brief Destroys vulnerable block.
  /// @return Score of destroyed block.
  /// @note Re-calculates cardinality.
  int destroyVulnerableBlock(int row, int col);
  /// @brief Gets recorded cardinality.
  inline int getCardinality() const { return initial_cardinality; }
  /// @brief Forced way to drop cardinality for instant victory.
  inline void forceDropCardinality() { initial_cardinality = 1; }
  /// @brief Decrements recorded cardinality due to impact of some block.
  /// @return Updated cardinality.
  inline int blockImpact() { return --initial_cardinality; }
  /// @brief Checks whether specified block is surrounded with 4 other blocks.
  bool isInner(int row, int col) const;

  /** @defgroup Modifiers Functions modifying blocks.
   * @{
   */
  /// @brief Updates specified block after single impact.
  /// @param row Row index of impacted block.
  /// @param col Column index of impacted block.
  void setBlockImpacted(int row, int col);
  /// @brief Modifies blocks around certain block.
  /// @param row Row index of certain block.
  /// @param col Column index of certain block.
  /// @param type Type the block will be modified to.
  /// @param ignoreNone Whether to ignore NONE blocks.
  /// @param output Array of valid indices of influenced blocks.
  /// @return Score of affected blocks.
  int modifyBlocksAround(int row, int col, Block type, bool ignoreNone, std::vector<RowCol>* output);
  /// @brief Upgrades or degrades blocks around certain block.
  /// @param row Row index of certain block.
  /// @param col Column index of certain block.
  /// @param mode Upgrade or degrade nearest blocks.
  /// @param output Array of valid indices of influenced blocks.
  /// @return Score of affected blocks.
  int changeBlocksAround(int row, int col, Mode mode, std::vector<RowCol>* output);
  /// @brief Destroys blocks around certain block.
  /// @return Score of affected blocks.
  int destroyBlocksAround(int row, int col, std::vector<RowCol>* output);
  /// @brief Modifies blocks behind certain block.
  /// @param row Row index of certain block.
  /// @param col Column index of certain block.
  /// @param type Type the block will be modified to.
  /// @param ignoreNone Whether to ignore NONE blocks.
  /// @param direction Direction behind the block.
  /// @param output Array of valid indices of influenced blocks.
  /// @return Score of affected blocks.
  int modifyBlocksBehind(int row, int col, Block type, bool ignoreNone, Direction direction, std::vector<RowCol>* output);
  /// @brief Same as above, but modifies one block behind certain block.
  /// @return Score of affected block.
  int modifyOneBlockBehind(int row, int col, Block type, bool ignoreNone, Direction direction, RowCol* output);
  /// @brief Destroys blocks behind certain block.
  /// @return Score of affected blocks.
  int destroyBlocksBehind(int row, int col, Direction direction, std::vector<RowCol>* output);
  /// @brief Destroys one block behind certain block.
  /// @return Score of affected block.
  int destroyOneBlockBehind(int row, int col, Direction direction, RowCol* output);
  /// @brief Modifies one block near certain block where possible.
  /// @param row Row index of certain block.
  /// @param col Column index of certain block.
  /// @param type Type the block will be modified to.
  /// @param output Valid indices of influenced block.
  /// @return TRUE is place for near block is found, FALSE otherwise.
  bool modifyBlockNear(int row, int col, Block type, RowCol* output);
  /// @brief Finds all blocks of given type.
  /// @param type Type of block to be found.
  /// @param output Array of valid indices of found blocks.
  void findBlocks(Block type, std::vector<RowCol>* output);
  /// @brief Same as above, allows NONE blocks.
  void findBlocksAllowNone(Block type, std::vector<RowCol>* output);
  /// @brief Same as above, but searches backwards.
  void findBlocksBackward(Block type, std::vector<RowCol>* output);
  /// @brief Same as above, allows NONE blocks.
  void findBlocksBackwardAllowNone(Block type, std::vector<RowCol>* output);
  /// @brief Generates ordinary block which presents in current level.
  Block generatePresentBlock();
  /** @} */  // end of Modifiers group

  void print() const;

private:
  Level(int rows, int cols);

  /// @brief Calculates current cardinality of this Level instance.
  int calculateCardinality() const;
  /// @brief Checks whether there are any of ordinary blocks in current level.
  bool checkOrdinaryBlocksPresent() const;

  int rows, cols;
  int initial_cardinality;
  Block** blocks;
  BlockGenerator generator;
  PrizeGenerator prize_generator;
};

}  // namespace game

#endif /* INCLUDE_LEVEL_H_ */
