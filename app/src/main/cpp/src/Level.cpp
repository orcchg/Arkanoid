/*
 * Level.cpp
 *
 *  Created on: Apr 7, 2015
 *      Author: Maxim Alov <alovmax@yandex.ru>
 */

#include <algorithm>
#include <iterator>
#include <sstream>

#include "Level.h"
#include "logger.h"
#include "utils.h"

namespace game {

Level::Ptr Level::fromStringArray(const std::vector<std::string>& array, size_t length) {
  size_t* widths = new size_t[length];
  for (size_t i = 0; i < length; ++i) {
    widths[i] = array[i].length();
  }
  // all strings which are less than the longer one will be populated
  // with blank characters
  size_t max_width = *std::max_element(widths, widths + length);

  Level::Ptr level = std::shared_ptr<Level>(new Level(length, max_width));
  for (int r = 0; r < level->rows; ++r) {
    for (int c = 0; c < level->cols; ++c) {
      if (c < widths[r]) {
        level->blocks[r][c] = BlockUtils::charToBlock(array[r][c]);
      } else {
        level->blocks[r][c] = Block::NONE;
      }
    }
  }
  level->initial_cardinality = level->calculateCardinality();

  delete [] widths;
  widths = nullptr;

  return level;
}

size_t Level::toStringArray(std::vector<std::string>* array) const {
  for (int r = 0; r < rows; ++r) {
    std::string line = "";
    for (int c = 0; c < cols; ++c) {
      line += BlockUtils::blockToChar(blocks[r][c]);
    }
    array->emplace_back(line);
  }
  return rows;
}

void Level::toVertexArray(
    GLfloat width,
    GLfloat height,
    GLfloat x_offset,
    GLfloat y_offset,
    GLfloat* const array) const {

  util::setRectangleVertices(array, width, height, x_offset, y_offset, cols, rows);
}

void Level::fillColorArray(GLfloat* const array) const {
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      fillColorArrayAtBlock(array, r, c);
    }
  }
}

void Level::fillColorArrayAtBlock(GLfloat* const array, int row, int col) const {
  int upper_left_i  = 0  + 16 * (row * cols + col);
  int upper_right_i = 4  + 16 * (row * cols + col);
  int lower_left_i  = 8  + 16 * (row * cols + col);
  int lower_right_i = 12 + 16 * (row * cols + col);

  util::BGRA<GLfloat> bgra = BlockUtils::getBlockColor(blocks[row][col]);
  util::BGRA<GLfloat> bgra_edge = BlockUtils::getBlockEdgeColor(blocks[row][col]);

  util::setColor(bgra, &array[upper_left_i], 4);
  util::setColor(bgra_edge, &array[upper_right_i], 4);
  util::setColor(bgra_edge, &array[lower_left_i], 4);
  util::setColor(bgra_edge, &array[lower_right_i], 4);
}

void Level::setVulnerableBlock(int row, int col, Block value) {
  if (blocks[row][col] != Block::TITAN &&
      blocks[row][col] != Block::INVUL) {
    setBlock(row, col, value);
    initial_cardinality += BlockUtils::getCardinalityCost(value);
  }
}

void Level::changeVulnerableBlock(Mode mode, int row, int col) {
  switch (mode) {
    case Mode::UPGRADE:
      switch (blocks[row][col]) {
        case Block::ALUMINIUM:
        case Block::CLAY:
        case Block::SIMPLE:
        case Block::ZYGOTE_SPAWN:
          setBlock(row, col, Block::BRICK);
          initial_cardinality += BlockUtils::getCardinalityCost(Block::BRICK);
          break;
        case Block::FOG:
          setBlock(row, col, Block::GLASS);
          initial_cardinality += BlockUtils::getCardinalityCost(Block::GLASS);
          break;
        case Block::BRICK:
          setBlock(row, col, Block::IRON);
          initial_cardinality += BlockUtils::getCardinalityCost(Block::IRON);
          break;
        case Block::IRON:
        case Block::STEEL:
          setBlock(row, col, Block::PLUMBUM);
          initial_cardinality += BlockUtils::getCardinalityCost(Block::PLUMBUM);
          break;
        case Block::WATER:
          setBlock(row, col, Block::JELLY);
          initial_cardinality += BlockUtils::getCardinalityCost(Block::JELLY);
          break;
        case Block::JELLY:
          setBlock(row, col, Block::ROLLING);
          initial_cardinality += BlockUtils::getCardinalityCost(Block::ROLLING);
          break;
        default:
          initial_cardinality += BlockUtils::getCardinalityCost(blocks[row][col]);
          break;
      }
      break;
      case Mode::DEGRADE:
        switch (blocks[row][col]) {
          case Block::GLASS:
            setBlock(row, col, Block::FOG);
            initial_cardinality += BlockUtils::getCardinalityCost(Block::FOG);
            break;
          case Block::BRICK:
            setBlock(row, col, Block::SIMPLE);
            initial_cardinality += BlockUtils::getCardinalityCost(Block::SIMPLE);
            break;
          case Block::IRON:
          case Block::STEEL:
            setBlock(row, col, Block::BRICK);
            initial_cardinality += BlockUtils::getCardinalityCost(Block::BRICK);
            break;
          case Block::PLUMBUM:
            setBlock(row, col, Block::IRON);
            initial_cardinality += BlockUtils::getCardinalityCost(Block::IRON);
            break;
          case Block::ROLLING:
            setBlock(row, col, Block::JELLY);
            initial_cardinality += BlockUtils::getCardinalityCost(Block::JELLY);
            break;
          case Block::JELLY:
            setBlock(row, col, Block::WATER);
            initial_cardinality += BlockUtils::getCardinalityCost(Block::WATER);
            break;
          default:
            initial_cardinality += BlockUtils::getCardinalityCost(blocks[row][col]);
            break;
        }
        break;
  }
}

int Level::destroyVulnerableBlock(int row, int col) {
  int score = 0;
  Block block = getBlock(row, col);
  if (block != Block::TITAN && block != Block::INVUL) {
    score += BlockUtils::getCardinalityCost(block);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    setBlock(row, col, Block::NONE);
  }
  return score;
}

bool Level::isInner(int row, int col) const {
  Block top = Block::NONE;
  Block bottom = Block::NONE;
  Block left = Block::NONE;
  Block right = Block::NONE;

  bool top_result = false;
  if (row - 1 >= 0) {
    top = getBlock(row - 1, col);
    top_result = top != Block::NONE;
  } else {
    top_result = true;
  }

  bool bottom_result = false;
  if (row + 1 < rows) {
    bottom = getBlock(row + 1, col);
    bottom_result = bottom != Block::NONE;
  } else {
    bottom_result = true;
  }

  bool left_result = false;
  if (col - 1 >= 0) {
    left = getBlock(row, col - 1);
    left_result = left != Block::NONE;
  } else {
    left_result = true;
  }

  bool right_result = false;
  if (col + 1 < cols) {
    right = getBlock(row, col + 1);
    right_result = right != Block::NONE;
  } else {
    right_result = true;
  }

  return top_result && bottom_result && left_result && right_result;
}

void Level::setBlockImpacted(int row, int col) {
  Block block = getBlock(row, col);
  switch (block) {
    case Block::ULTRA:
      setBlock(row, col, Block::ULTRA_4);
      break;
    case Block::ULTRA_4:
      setBlock(row, col, Block::ULTRA_3);
      break;
    case Block::ULTRA_3:
      setBlock(row, col, Block::ULTRA_2);
      break;
    case Block::ULTRA_2:
      setBlock(row, col, Block::ULTRA_1);
      break;
    // ----------------------
    case Block::PLUMBUM:
      setBlock(row, col, Block::STEEL);
      break;
    // ----------------------
    case Block::IRON:
    case Block::STEEL:
      setBlock(row, col, Block::BRICK);
      break;
    // ----------------------
    case Block::QUICK:
      setBlock(row, col, Block::QUICK_2);
      break;
    case Block::QUICK_2:
      setBlock(row, col, Block::QUICK_1);
      break;
    // ----------------------
    case Block::BRICK:
      setBlock(row, col, Block::SIMPLE);
      break;
    case Block::GLASS:
      setBlock(row, col, Block::GLASS_1);
      break;
    // ----------------------
    case Block::ZYGOTE:
      setBlock(row, col, Block::ZYGOTE_1);
      break;
    // ----------------------
    case Block::ALUMINIUM:
    case Block::ARTIFICAL:
    case Block::CLAY:
    case Block::DESTROY:
    case Block::ELECTRO:
    case Block::FOG:
    case Block::GLASS_1:
    case Block::HYPER:
    case Block::JELLY:
    case Block::KNOCK_VERTICAL:
    case Block::KNOCK_HORIZONTAL:
    case Block::MAGIC:
    case Block::ORIGIN:
    case Block::ROLLING:
    case Block::SIMPLE:
    case Block::WATER:
    // ----------------------
    case Block::NETWORK:
    case Block::QUICK_1:
    case Block::ULTRA_1:
    case Block::YOGURT:
    case Block::YOGURT_1:
    case Block::ZYGOTE_1:
    case Block::ZYGOTE_SPAWN:
      setBlock(row, col, Block::NONE);
      break;
    // ----------------------
    case Block::MIDAS:
      setBlock(row, col, Block::TITAN);
      break;
    case Block::EXTRA:
      setBlock(row, col, Block::INVUL);
      break;
    // ----------------------
    case Block::TITAN:
    case Block::INVUL:
    case Block::NONE:
    default:
      // no-op
      break;
  }
}

int Level::modifyBlocksAround(int row, int col, Block type, bool ignoreNone, std::vector<RowCol>* output) {
  int score = 0;
  if (row - 2 >= 0) {
    Block block = getBlock(row - 2, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row - 2, col, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row - 2, col);
    }
  }

  if (row - 1 >= 0) {
    Block block = getBlock(row - 1, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row - 1, col, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row - 1, col);
    }
    if (col - 1 >= 0) {
      Block block = getBlock(row - 1, col - 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      setVulnerableBlock(row - 1, col - 1, type);
      if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
        output->emplace_back(row - 1, col - 1);
      }
    }
    if (col + 1 < cols) {
      Block block = getBlock(row - 1, col + 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      setVulnerableBlock(row - 1, col + 1, type);
      if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
        output->emplace_back(row - 1, col + 1);
      }
    }
  }

  if (row + 1 < rows) {
    Block block = getBlock(row + 1, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row + 1, col, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row + 1, col);
    }
    if (col - 1 >= 0) {
      Block block = getBlock(row + 1, col - 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      setVulnerableBlock(row + 1, col - 1, type);
      if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
        output->emplace_back(row + 1, col - 1);
      }
    }
    if (col + 1 < cols) {
      Block block = getBlock(row + 1, col + 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      setVulnerableBlock(row + 1, col + 1, type);
      if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
        output->emplace_back(row + 1, col + 1);
      }
    }
  }

  if (row + 2 < rows) {
    Block block = getBlock(row + 2, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row + 2, col, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row + 2, col);
    }
  }

  // ------------------------
  if (col - 2 >= 0) {
    Block block = getBlock(row, col - 2);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row, col - 2, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row, col - 2);
    }
  }
  if (col - 1 >= 0) {
    Block block = getBlock(row, col - 1);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row, col - 1, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row, col - 1);
    }
  }
  if (col + 1 < cols) {
    Block block = getBlock(row, col + 1);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row, col + 1, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row, col + 1);
    }
  }
  if (col + 2 < cols) {
    Block block = getBlock(row, col + 2);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    setVulnerableBlock(row, col + 2, type);
    if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
      output->emplace_back(row, col + 2);
    }
  }
  return score;
}

int Level::changeBlocksAround(int row, int col, Mode mode, std::vector<RowCol>* output) {
  int score = 0;
  if (row - 2 >= 0) {
    Block block = getBlock(row - 2, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row - 2, col);
    output->emplace_back(row - 2, col);
  }

  if (row - 1 >= 0) {
    Block block = getBlock(row - 1, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row - 1, col);
    output->emplace_back(row - 1, col);
    if (col - 1 >= 0) {
      Block block = getBlock(row - 1, col - 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      changeVulnerableBlock(mode, row - 1, col - 1);
      output->emplace_back(row - 1, col - 1);
    }
    if (col + 1 < cols) {
      Block block = getBlock(row - 1, col + 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      changeVulnerableBlock(mode, row - 1, col + 1);
      output->emplace_back(row - 1, col + 1);
    }
  }

  if (row + 1 < rows) {
    Block block = getBlock(row + 1, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row + 1, col);
    output->emplace_back(row + 1, col);
    if (col - 1 >= 0) {
      Block block = getBlock(row + 1, col - 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      changeVulnerableBlock(mode, row + 1, col - 1);
      output->emplace_back(row + 1, col - 1);
    }
    if (col + 1 < cols) {
      Block block = getBlock(row + 1, col + 1);
      initial_cardinality -= BlockUtils::getCardinalityCost(block);
      score += BlockUtils::getBlockScore(block);
      changeVulnerableBlock(mode, row + 1, col + 1);
      output->emplace_back(row + 1, col + 1);
    }
  }

  if (row + 2 < rows) {
    Block block = getBlock(row + 2, col);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row + 2, col);
    output->emplace_back(row + 2, col);
  }

  // ------------------------
  if (col - 2 >= 0) {
    Block block = getBlock(row, col - 2);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row, col - 2);
    output->emplace_back(row, col - 2);
  }
  if (col - 1 >= 0) {
    Block block = getBlock(row, col - 1);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row, col - 1);
    output->emplace_back(row, col - 1);
  }
  if (col + 1 < cols) {
    Block block = getBlock(row, col + 1);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row, col + 1);
    output->emplace_back(row, col + 1);
  }
  if (col + 2 < cols) {
    Block block = getBlock(row, col + 2);
    initial_cardinality -= BlockUtils::getCardinalityCost(block);
    score += BlockUtils::getBlockScore(block);
    changeVulnerableBlock(mode, row, col + 2);
    output->emplace_back(row, col + 2);
  }
  return score;
}

int Level::destroyBlocksAround(int row, int col, std::vector<RowCol>* output) {
  return modifyBlocksAround(row, col, Block::NONE, true, output);
}

int Level::modifyBlocksBehind(int row, int col, Block type, bool ignoreNone, Direction direction, std::vector<RowCol>* output) {
  int score = 0;
  switch (direction) {
    case Direction::UP:
      --row;
      while (row >= 0) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          output->emplace_back(row, col);
        }
        --row;
      }
      break;
    case Direction::DOWN:
      ++row;
      while (row < rows) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          output->emplace_back(row, col);
        }
        ++row;
      }
      break;
    case Direction::RIGHT:
      ++col;
      while (col < cols) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          output->emplace_back(row, col);
        }
        ++col;
      }
      break;
    case Direction::LEFT:
      --col;
      while (col >= 0) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          output->emplace_back(row, col);
        }
        --col;
      }
      break;
    case Direction::NONE:
    default:
      break;
  }
  return score;
}

int Level::modifyOneBlockBehind(int row, int col, Block type, bool ignoreNone, Direction direction, RowCol* output) {
  int score = 0;
  switch (direction) {
    case Direction::UP:
      --row;
      if (row >= 0) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          *output = RowCol(row, col);
        }
      }
      break;
    case Direction::DOWN:
      ++row;
      if (row < rows) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          *output = RowCol(row, col);
        }
      }
      break;
    case Direction::RIGHT:
      ++col;
      if (col < cols) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          *output = RowCol(row, col);
        }
      }
      break;
    case Direction::LEFT:
      --col;
      if (col >= 0) {
        Block block = getBlock(row, col);
        initial_cardinality -= BlockUtils::getCardinalityCost(block);
        score += BlockUtils::getBlockScore(block);
        setVulnerableBlock(row, col, type);
        if (!(ignoreNone && (block == Block::NONE || block == Block::TITAN || block == Block::INVUL))) {
          *output = RowCol(row, col);
        }
      }
      break;
    case Direction::NONE:
    default:
      break;
  }
  return score;
}

int Level::destroyBlocksBehind(int row, int col, Direction direction, std::vector<RowCol>* output) {
  return modifyBlocksBehind(row, col, Block::NONE, true, direction, output);
}

int Level::destroyOneBlockBehind(int row, int col, Direction direction, RowCol* output) {
  return modifyOneBlockBehind(row, col, Block::NONE, true, direction, output);
}

bool Level::modifyBlockNear(int row, int col, Block type, RowCol* output) {
  if (row - 1 >= 0) {
    if (col - 1 >= 0) {
      auto block = getBlock(row - 1, col - 1);
      if (block == Block::NONE) {
        setVulnerableBlock(row - 1, col - 1, type);
        *output = RowCol(row - 1, col - 1);
        return true;
      }
    }
    if (col + 1 < cols) {
      auto block = getBlock(row - 1, col + 1);
      if (block == Block::NONE) {
        setVulnerableBlock(row - 1, col + 1, type);
        *output = RowCol(row - 1, col + 1);
        return true;
      }
    }
  }
  if (row + 1 < rows) {
    if (col - 1 >= 0) {
      auto block = getBlock(row + 1, col - 1);
      if (block == Block::NONE) {
        setVulnerableBlock(row + 1, col - 1, type);
        *output = RowCol(row + 1, col - 1);
        return true;
      }
    }
    if (col + 1 < cols) {
      auto block = getBlock(row + 1, col + 1);
      if (block == Block::NONE) {
        setVulnerableBlock(row + 1, col + 1, type);
        *output = RowCol(row + 1, col + 1);
        return true;
      }
    }
  }
  if (row - 1 >= 0){
    auto block = getBlock(row - 1, col);
    if (block == Block::NONE) {
      setVulnerableBlock(row - 1, col, type);
      *output = RowCol(row - 1, col);
      return true;
    }
  }
  if (row + 1 < rows) {
    auto block = getBlock(row + 1, col);
    if (block == Block::NONE) {
      setVulnerableBlock(row + 1, col, type);
      *output = RowCol(row + 1, col);
      return true;
    }
  }
  if (col - 1 >= 0) {
    auto block = getBlock(row, col - 1);
    if (block == Block::NONE) {
      setVulnerableBlock(row, col - 1, type);
      *output = RowCol(row, col - 1);
      return true;
    }
  }
  if (col + 1 < cols) {
    auto block = getBlock(row, col + 1);
    if (block == Block::NONE) {
      setVulnerableBlock(row, col + 1, type);
      *output = RowCol(row, col + 1);
      return true;
    }
  }
  return false;
}

void Level::findBlocks(Block type, std::vector<RowCol>* output) {
  if (type != Block::NONE) {
    findBlocksAllowNone(type, output);
  }
}

void Level::findBlocksAllowNone(Block type, std::vector<RowCol>* output) {
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (blocks[r][c] == type) {
        output->emplace_back(r, c);
      }
    }
  }
}

void Level::findBlocksBackward(Block type, std::vector<RowCol>* output) {
  if (type != Block::NONE) {
    findBlocksBackwardAllowNone(type, output);
  }
}

void Level::findBlocksBackwardAllowNone(Block type, std::vector<RowCol>* output) {
  for (int r = rows - 1; r >= 0; --r) {
    for (int c = cols - 1; c >= 0; --c) {
      if (blocks[r][c] == type) {
        output->emplace_back(r, c);
      }
    }
  }
}

Block Level::generatePresentBlock() {
  Block block = Block::NONE;
  if (!checkOrdinaryBlocksPresent()) {
    return block;
  }

  bool is_present = false;
  do {
    block = generator.generateBlock();
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        if (blocks[r][c] == block) {
          is_present = true;
        }
      }
    }
  } while (!is_present);
  return block;
}

void Level::print() const {
  std::vector<std::string> array;
  array.reserve(rows);
  toStringArray(&array);
  std::ostringstream oss;
  oss.str("\n");
  std::copy(array.begin(), array.end(), std::ostream_iterator<std::string>(oss, "\n"));
  MSG("%s", oss.str().c_str());
}

// ----------------------------------------------------------------------------
Level::Level(int rows, int cols)
  : rows(rows)
  , cols(cols)
  , initial_cardinality(0)
  , blocks(new Block*[rows])
  , generator()
  , prize_generator() {
  for (int r = 0; r < rows; ++r) {
    blocks[r] = new Block[cols];
  }
}

Level::~Level() {
  for (int r = 0; r < rows; ++r) {
    delete [] blocks[r];
    blocks[r] = nullptr;
  }
  delete [] blocks;
  blocks = nullptr;
}

int Level::calculateCardinality() const {
  int cardinality = 0;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      cardinality += BlockUtils::getCardinalityCost(blocks[r][c]);
    }
  }
  return cardinality;
}

bool Level::checkOrdinaryBlocksPresent() const {
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (BlockUtils::isOrdinaryBlock(blocks[r][c])) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace game
