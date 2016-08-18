#ifndef __ARKANOID_BLOCK__H__
#define __ARKANOID_BLOCK__H__

#include <random>
#include <string>
#include "rgbstruct.h"

namespace game {

enum class Block : int {
  NONE = 0,        //! ' ' - not disturbing

  /* Action blocks */
  DESTROY = 1,             //! 'D' - [ 1 ] lost ball automatically
  ELECTRO = 2,             //! 'E' - [ 1 ] destroys nearest blocks
  HYPER = 3,               //! 'H' - [ 1 ] teleports ball randomly (not lost)
  KNOCK_VERTICAL = 4,      //! 'K' - [ 1 ] destroys blocks behind
  KNOCK_HORIZONTAL = 5,    //! '#' - [ 1 ] destroys blocks behind
  MAGIC = 6,               //! 'M' - [ 1 ] transforms nearest blocks
  NETWORK = 7,             //! 'N' - [ 2 ] destroys all other NETWORK blocks
  ORIGIN = 8,              //! 'O' - [ 1 ] puts ball into initial position
  QUICK = 9,               //! 'Q' - [ 3 ] degrades nearest blocks
  ULTRA = 10,              //! 'U' - [ 5 ] win level
  YOGURT = 11,             //! 'Y' - [ 1 ] transforms nearest to YOUGURT blocks
  ZYGOTE = 12,             //! 'Z' - [ 2 ] produces additional blocks

  /* Invulnerable blocks */
  TITAN = 13,      //! 'T' - invulnerable
  INVUL = 14,      //! 'V' - invulnerable
  EXTRA = 15,      //! 'X' - [ 1 ] transforms to INVUL
  MIDAS = 16,      //! '$' - [ 1 ] transforms nearest to TITAN

  /* Auxiliary blocks */
  GLASS_1 = 17,   //! '['
  ARTIFICAL = 18,  //! ']'
  QUICK_2 = 19, QUICK_1 = 20,  //! '{' '}'
  ULTRA_4 = 21, ULTRA_3 = 22, ULTRA_2 = 23, ULTRA_1 = 24,  //! '%' '^' '&' '*'
  YOGURT_1 = 25,  //! '('
  ZYGOTE_1 = 26,  //! ')'

  /* Ordinary blocks */
  ALUMINIUM = 27,   //! 'A' - [ 1 ]
  BRICK = 28,       //! 'B' - [ 2 ]
  CLAY = 29,        //! 'C' - [ 1 ] small disturbing
  FOG = 30,         //! 'F' - [ 1 ] not disturbing
  GLASS = 31,       //! 'G' - [ 2 ] not disturbing
  IRON = 32,        //! 'I' - [ 3 ]
  JELLY = 33,       //! 'J' - [ 1 ] large disturbing
  STEEL = 34,       //! 'L' - [ 3 ]
  PLUMBUM = 35,     //! 'P' - [ 4 ]
  ROLLING = 36,     //! 'R' - [ 1 ] random disturbing
  SIMPLE = 37,      //! 'S' - [ 1 ]
  WATER = 38,       //! 'W' - [ 1 ] small disturbing
  ZYGOTE_SPAWN = 39 //! '@' - [ 1 ] large disturbing
};

class BlockUtils {
public:
  constexpr static int ordinaryBlockOffset = 27;
  constexpr static int totalOrdinaryBlocks = 13;

  static Block charToBlock(char ch);
  static char blockToChar(Block block);
  static bool isOrdinaryBlock(Block block);
  static int getCardinalityCost(Block block);
  static int getBlockScore(Block block);
  static util::BGRA<GLfloat> getBlockColor(Block block);
  static util::BGRA<GLfloat> getBlockEdgeColor(Block block);
  static std::string getBlockTexture(Block block);
  static bool cardinalityAffectingBlock(Block block);
  static bool cardinalityNotAffectingVisibleBlock(Block block);
};

class BlockGenerator {
public:
  BlockGenerator();
  Block generateBlock();  //!< Generates random ordinary block

private:
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;
};

}

#endif  // __ARKANOID_BLOCK__H__
