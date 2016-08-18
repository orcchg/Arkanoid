#include <chrono>
#include "Block.h"

namespace game {

Block BlockUtils::charToBlock(char ch) {
  switch (ch) {
    case 'a':
    case 'A':
      return Block::ALUMINIUM;
    case 'b':
    case 'B':
      return Block::BRICK;
    case 'c':
    case 'C':
      return Block::CLAY;
    case 'd':
    case 'D':
      return Block::DESTROY;
    case 'e':
    case 'E':
      return Block::ELECTRO;
    case 'f':
    case 'F':
      return Block::FOG;
    case 'g':
    case 'G':
      return Block::GLASS;
    case 'h':
    case 'H':
      return Block::HYPER;
    case 'i':
    case 'I':
      return Block::IRON;
    case 'j':
    case 'J':
      return Block::JELLY;
    case 'k':
    case 'K':
      return Block::KNOCK_VERTICAL;
    case '#':
      return Block::KNOCK_HORIZONTAL;
    case 'l':
    case 'L':
      return Block::STEEL;
    case 'm':
    case 'M':
      return Block::MAGIC;
    case '$':
      return Block::MIDAS;
    case 'n':
    case 'N':
      return Block::NETWORK;
    case 'o':
    case 'O':
      return Block::ORIGIN;
    case 'p':
    case 'P':
      return Block::PLUMBUM;
    case 'q':
    case 'Q':
      return Block::QUICK;
    case 'r':
    case 'R':
      return Block::ROLLING;
    case 's':
    case 'S':
      return Block::SIMPLE;
    case 't':
    case 'T':
      return Block::TITAN;
    case 'u':
    case 'U':
      return Block::ULTRA;
    case 'v':
    case 'V':
      return Block::INVUL;
    case 'w':
    case 'W':
      return Block::WATER;
    case 'x':
    case 'X':
      return Block::EXTRA;
    case 'y':
    case 'Y':
      return Block::YOGURT;
    case 'z':
    case 'Z':
      return Block::ZYGOTE;
    case '@':
      return Block::ZYGOTE_SPAWN;

    // ------------------------------------------
    case '[':
      return Block::GLASS_1;
    case ']':
      return Block::ARTIFICAL;
    case '{':
      return Block::QUICK_2;
    case '}':
      return Block::QUICK_1;
    case '%':
      return Block::ULTRA_4;
    case '^':
      return Block::ULTRA_3;
    case '&':
      return Block::ULTRA_2;
    case '*':
      return Block::ULTRA_1;
    case '(':
      return Block::YOGURT_1;
    case ')':
      return Block::ZYGOTE_1;

    case ' ':
    default:
      return Block::NONE;
  }
}

char BlockUtils::blockToChar(Block block) {
  switch (block) {
    case Block::ALUMINIUM:
      return 'A';
    case Block::BRICK:
      return 'B';
    case Block::CLAY:
      return 'C';
    case Block::DESTROY:
      return 'D';
    case Block::ELECTRO:
      return 'E';
    case Block::FOG:
      return 'F';
    case Block::GLASS:
      return 'G';
    case Block::HYPER:
      return 'H';
    case Block::IRON:
      return 'I';
    case Block::JELLY:
      return 'J';
    case Block::KNOCK_VERTICAL:
      return 'K';
    case Block::KNOCK_HORIZONTAL:
      return '#';
    case Block::STEEL:
      return 'L';
    case Block::MAGIC:
      return 'M';
    case Block::MIDAS:
      return '$';
    case Block::NETWORK:
      return 'N';
    case Block::ORIGIN:
      return 'O';
    case Block::PLUMBUM:
      return 'P';
    case Block::QUICK:
      return 'Q';
    case Block::ROLLING:
      return 'R';
    case Block::SIMPLE:
      return 'S';
    case Block::TITAN:
      return 'T';
    case Block::ULTRA:
      return 'U';
    case Block::INVUL:
      return 'V';
    case Block::WATER:
      return 'W';
    case Block::EXTRA:
      return 'X';
    case Block::YOGURT:
      return 'Y';
    case Block::ZYGOTE:
      return 'Z';
    case Block::ZYGOTE_SPAWN:
      return '@';

    // ------------------------------------------
    case Block::GLASS_1:
      return '[';
    case Block::ARTIFICAL:
      return ']';
    case Block::QUICK_2:
      return '{';
    case Block::QUICK_1:
      return '}';
    case Block::ULTRA_4:
      return '%';
    case Block::ULTRA_3:
      return '^';
    case Block::ULTRA_2:
      return '&';
    case Block::ULTRA_1:
      return '*';
    case Block::YOGURT_1:
      return '(';
    case Block::ZYGOTE_1:
      return ')';

    case Block::NONE:
    default:
      return ' ';
  }
}

bool BlockUtils::isOrdinaryBlock(Block block) {
  return block == Block::ALUMINIUM ||
      block == Block::BRICK ||
      block == Block::CLAY ||
      block == Block::FOG ||
      block == Block::GLASS ||
      block == Block::IRON ||
      block == Block::JELLY ||
      block == Block::STEEL ||
      block == Block::PLUMBUM ||
      block == Block::ROLLING ||
      block == Block::SIMPLE ||
      block == Block::WATER ||
      block == Block::ZYGOTE_SPAWN;
}

int BlockUtils::getCardinalityCost(Block block) {
  switch (block) {
    case Block::ULTRA:
      return 5;
    case Block::PLUMBUM:
    case Block::ULTRA_4:
      return 4;
    case Block::IRON:
    case Block::STEEL:
    case Block::QUICK:
    case Block::ULTRA_3:
      return 3;
    case Block::BRICK:
    case Block::GLASS:
    case Block::ZYGOTE:
    case Block::QUICK_2:
    case Block::ULTRA_2:
      return 2;
    case Block::ALUMINIUM:
    case Block::CLAY:
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
    case Block::YOGURT:
    case Block::YOGURT_1:
    case Block::NETWORK:
    case Block::QUICK_1:
    case Block::ULTRA_1:
    case Block::ZYGOTE_1:
    case Block::ZYGOTE_SPAWN:
      return 1;

    default:
    case Block::DESTROY:
    case Block::MIDAS:
    case Block::TITAN:
    case Block::INVUL:
    case Block::EXTRA:
    case Block::NONE:
    case Block::ARTIFICAL:
      return 0;
  }
}

int BlockUtils::getBlockScore(Block block) {
  int score = 0;
  switch (block) {
    case Block::ULTRA:     score += 127;
    case Block::ULTRA_4:   score += 125;
    case Block::ULTRA_3:   score += 122;
    case Block::ULTRA_2:   score += 111;
    case Block::ULTRA_1:   score += 100;
      break;
    case Block::PLUMBUM:   score += 47;
    case Block::STEEL:     score += 39;
    case Block::IRON:      score += 37;
    case Block::ALUMINIUM: score += 25;
      break;
    case Block::QUICK:     score += 28;
    case Block::QUICK_2:   score += 27;
    case Block::QUICK_1:   score += 24;
      break;
    case Block::BRICK:     score += 24;
      break;
    case Block::ROLLING:   score += 5;
    case Block::JELLY:     score += 11;
    case Block::CLAY:      score += 12;
      break;
    case Block::WATER:     score += 5;
    case Block::SIMPLE:    score += 4;
      break;
    case Block::KNOCK_VERTICAL:
    case Block::KNOCK_HORIZONTAL:
      score += 15;
      break;
    case Block::ZYGOTE:    score += 3;
    case Block::ZYGOTE_1:  score += 2;
    case Block::YOGURT:    score += 7;
    case Block::YOGURT_1:  score += 1;
    case Block::ZYGOTE_SPAWN: score += 1;
      break;
    case Block::GLASS:     score += 1;
    case Block::GLASS_1:   score += 1;
    case Block::FOG:       score += 1;
      break;
    case Block::NETWORK:   score += 11;
    case Block::ELECTRO:   score += 9;
      break;
    case Block::HYPER:     score += 1;
    case Block::MAGIC:     score += 8;
    case Block::ORIGIN:    score += 2;
      break;

    default:
    case Block::DESTROY:
    case Block::MIDAS:
    case Block::TITAN:
    case Block::INVUL:
    case Block::EXTRA:
    case Block::NONE:
    case Block::ARTIFICAL:
      break;
  }
  return score;
}

util::BGRA<GLfloat> BlockUtils::getBlockColor(Block block) {
  util::BGRA<GLfloat> bgra;
  switch (block) {
    default:
    case Block::NONE:
      bgra = util::BGRA<GLfloat>(util::TRANSPARENT);
      break;
    case Block::ALUMINIUM:
      bgra = util::BGRA<GLfloat>(util::ALUMINIUM);
      break;
    case Block::ARTIFICAL:
      bgra = util::BGRA<GLfloat>(util::ARTIFICAL);
      break;
    case Block::BRICK:
      bgra = util::BGRA<GLfloat>(util::BRICK);
      break;
    case Block::CLAY:
      bgra = util::BGRA<GLfloat>(util::CLAY);
      break;
    case Block::DESTROY:
      bgra = util::BGRA<GLfloat>(util::DESTROY);
      break;
    case Block::ELECTRO:
      bgra = util::BGRA<GLfloat>(util::ELECTRO);
      break;
    case Block::FOG:
      bgra = util::BGRA<GLfloat>(util::FOG);
      break;
    case Block::GLASS:
    case Block::GLASS_1:
      bgra = util::BGRA<GLfloat>(util::GLASS);
      break;
    case Block::HYPER:
      bgra = util::BGRA<GLfloat>(util::HYPER);
      break;
    case Block::IRON:
      bgra = util::BGRA<GLfloat>(util::IRON);
      break;
    case Block::JELLY:
      bgra = util::BGRA<GLfloat>(util::JELLY);
      break;
    case Block::KNOCK_VERTICAL:
    case Block::KNOCK_HORIZONTAL:
      bgra = util::BGRA<GLfloat>(util::KNOCK);
      break;
    case Block::STEEL:
      bgra = util::BGRA<GLfloat>(util::STEEL);
      break;
    case Block::MAGIC:
      bgra = util::BGRA<GLfloat>(util::MAGIC);
      break;
    case Block::MIDAS:
      bgra = util::BGRA<GLfloat>(util::MIDAS);
      break;
    case Block::NETWORK:
      bgra = util::BGRA<GLfloat>(util::NETWORK);
      break;
    case Block::ORIGIN:
      bgra = util::BGRA<GLfloat>(util::ORIGIN);
      break;
    case Block::PLUMBUM:
      bgra = util::BGRA<GLfloat>(util::PLUMBUM);
      break;
    case Block::QUICK:
    case Block::QUICK_1:
    case Block::QUICK_2:
      bgra = util::BGRA<GLfloat>(util::QUICK);
      break;
    case Block::ROLLING:
      bgra = util::BGRA<GLfloat>(util::ROLLING);
      break;
    case Block::SIMPLE:
      bgra = util::BGRA<GLfloat>(util::SIMPLE);
      break;
    case Block::TITAN:
      bgra = util::BGRA<GLfloat>(util::TITAN);
      break;
    case Block::ULTRA:
    case Block::ULTRA_1:
    case Block::ULTRA_2:
    case Block::ULTRA_3:
    case Block::ULTRA_4:
      bgra = util::BGRA<GLfloat>(util::ULTRA);
      break;
    case Block::INVUL:
      bgra = util::BGRA<GLfloat>(util::INVUL);
      break;
    case Block::WATER:
      bgra = util::BGRA<GLfloat>(util::WATER);
      break;
    case Block::EXTRA:
      bgra = util::BGRA<GLfloat>(util::EXTRA);
      break;
    case Block::YOGURT:
    case Block::YOGURT_1:
      bgra = util::BGRA<GLfloat>(util::YOGURT);
      break;
    case Block::ZYGOTE:
    case Block::ZYGOTE_1:
      bgra = util::BGRA<GLfloat>(util::ZYGOTE);
      break;
    case Block::ZYGOTE_SPAWN:
      bgra = util::BGRA<GLfloat>(util::ZYGOTE_SPAWN);
      break;
  }
  return bgra;
}

util::BGRA<GLfloat> BlockUtils::getBlockEdgeColor(Block block) {
  util::BGRA<GLfloat> bgra_edge;
  switch (block) {
    default:
    case Block::NONE:
      bgra_edge = util::BGRA<GLfloat>(util::TRANSPARENT);
      break;
    case Block::ALUMINIUM:
      bgra_edge = util::BGRA<GLfloat>(util::ALUMINIUM_EDGE);
      break;
    case Block::ARTIFICAL:
      bgra_edge = util::BGRA<GLfloat>(util::ARTIFICAL_EDGE);
      break;
    case Block::BRICK:
      bgra_edge = util::BGRA<GLfloat>(util::BRICK_EDGE);
      break;
    case Block::CLAY:
      bgra_edge = util::BGRA<GLfloat>(util::CLAY_EDGE);
      break;
    case Block::DESTROY:
      bgra_edge = util::BGRA<GLfloat>(util::DESTROY_EDGE);
      break;
    case Block::ELECTRO:
      bgra_edge = util::BGRA<GLfloat>(util::ELECTRO_EDGE);
      break;
    case Block::FOG:
      bgra_edge = util::BGRA<GLfloat>(util::FOG_EDGE);
      break;
    case Block::GLASS:
    case Block::GLASS_1:
      bgra_edge = util::BGRA<GLfloat>(util::GLASS_EDGE);
      break;
    case Block::HYPER:
      bgra_edge = util::BGRA<GLfloat>(util::HYPER_EDGE);
      break;
    case Block::IRON:
      bgra_edge = util::BGRA<GLfloat>(util::IRON_EDGE);
      break;
    case Block::JELLY:
      bgra_edge = util::BGRA<GLfloat>(util::JELLY_EDGE);
      break;
    case Block::KNOCK_VERTICAL:
    case Block::KNOCK_HORIZONTAL:
      bgra_edge = util::BGRA<GLfloat>(util::KNOCK_EDGE);
      break;
    case Block::STEEL:
      bgra_edge = util::BGRA<GLfloat>(util::STEEL_EDGE);
      break;
    case Block::MAGIC:
      bgra_edge = util::BGRA<GLfloat>(util::MAGIC_EDGE);
      break;
    case Block::MIDAS:
      bgra_edge = util::BGRA<GLfloat>(util::MIDAS_EDGE);
      break;
    case Block::NETWORK:
      bgra_edge = util::BGRA<GLfloat>(util::NETWORK_EDGE);
      break;
    case Block::ORIGIN:
      bgra_edge = util::BGRA<GLfloat>(util::ORIGIN_EDGE);
      break;
    case Block::PLUMBUM:
      bgra_edge = util::BGRA<GLfloat>(util::PLUMBUM_EDGE);
      break;
    case Block::QUICK:
    case Block::QUICK_1:
    case Block::QUICK_2:
      bgra_edge = util::BGRA<GLfloat>(util::QUICK_EDGE);
      break;
    case Block::ROLLING:
      bgra_edge = util::BGRA<GLfloat>(util::ROLLING_EDGE);
      break;
    case Block::SIMPLE:
      bgra_edge = util::BGRA<GLfloat>(util::SIMPLE_EDGE);
      break;
    case Block::TITAN:
      bgra_edge = util::BGRA<GLfloat>(util::TITAN_EDGE);
      break;
    case Block::ULTRA:
    case Block::ULTRA_1:
    case Block::ULTRA_2:
    case Block::ULTRA_3:
    case Block::ULTRA_4:
      bgra_edge = util::BGRA<GLfloat>(util::ULTRA_EDGE);
      break;
    case Block::INVUL:
      bgra_edge = util::BGRA<GLfloat>(util::INVUL_EDGE);
      break;
    case Block::WATER:
      bgra_edge = util::BGRA<GLfloat>(util::WATER_EDGE);
      break;
    case Block::EXTRA:
      bgra_edge = util::BGRA<GLfloat>(util::EXTRA_EDGE);
      break;
    case Block::YOGURT:
    case Block::YOGURT_1:
      bgra_edge = util::BGRA<GLfloat>(util::YOGURT_EDGE);
      break;
    case Block::ZYGOTE:
    case Block::ZYGOTE_1:
      bgra_edge = util::BGRA<GLfloat>(util::ZYGOTE_EDGE);
      break;
    case Block::ZYGOTE_SPAWN:
      bgra_edge = util::BGRA<GLfloat>(util::ZYGOTE_SPAWN_EDGE);
      break;
  }
  return bgra_edge;
}

std::string BlockUtils::getBlockTexture(Block block) {
  switch (block) {
    case Block::DESTROY:    return "bl_destroy.png";
    case Block::ELECTRO:    return "bl_electro.png";
    case Block::HYPER:      return "bl_hyper.png";
    case Block::KNOCK_VERTICAL:
    case Block::KNOCK_HORIZONTAL:  return "bl_knock.png";
    case Block::MAGIC:      return "bl_magic.png";
    case Block::NETWORK:    return "bl_network.png";
    case Block::ORIGIN:     return "bl_origin.png";
    case Block::QUICK_1:
    case Block::QUICK_2:
    case Block::QUICK:      return "bl_quick.png";
    case Block::ULTRA_1:
    case Block::ULTRA_2:
    case Block::ULTRA_3:
    case Block::ULTRA_4:
    case Block::ULTRA:      return "bl_ultra.png";
    case Block::TITAN:      return "bl_titan.png";
    case Block::INVUL:      return "bl_invul.png";
    case Block::EXTRA:      return "bl_extra.png";
    case Block::MIDAS:      return "bl_midas.png";
    case Block::ALUMINIUM:  return "bl_aluminium.png";
    case Block::BRICK:      return "bl_brick.png";
    case Block::CLAY:       return "bl_clay.png";
    case Block::FOG:        return "bl_fog.png";
    case Block::GLASS_1:
    case Block::GLASS:      return "bl_glass.png";
    case Block::IRON:       return "bl_iron.png";
    case Block::JELLY:      return "bl_jelly.png";
    case Block::STEEL:      return "bl_steel.png";
    case Block::PLUMBUM:    return "bl_plumbum.png";
    case Block::ROLLING:    return "bl_rolling.png";
    case Block::SIMPLE:     return "bl_simple.png";
    case Block::WATER:      return "bl_water.png";
    case Block::YOGURT_1:
    case Block::YOGURT:     return "bl_yogurt.png";
    case Block::ZYGOTE_1:
    case Block::ZYGOTE:     return "bl_zygote.png";
    case Block::ZYGOTE_SPAWN:  return "bl_zygote_spawn.png";
    default:
      break;
  }
  return "";
}

bool BlockUtils::cardinalityAffectingBlock(Block block) {
  return block != Block::ARTIFICAL &&
      block != Block::NONE &&
      block != Block::DESTROY &&
      block != Block::MIDAS &&
      block != Block::TITAN &&
      block != Block::INVUL &&
      block != Block::EXTRA;
}

bool BlockUtils::cardinalityNotAffectingVisibleBlock(Block block) {
  return block == Block::ARTIFICAL ||
      block == Block::DESTROY ||
      block == Block::MIDAS ||
      block == Block::TITAN ||
      block == Block::INVUL ||
      block == Block::EXTRA;
}

BlockGenerator::BlockGenerator()
  : m_generator(std::chrono::system_clock::now().time_since_epoch().count())
  , m_distribution(0, BlockUtils::totalOrdinaryBlocks - 1) {
}

Block BlockGenerator::generateBlock() {
  int value = BlockUtils::ordinaryBlockOffset + m_distribution(m_generator);
  return static_cast<Block>(value);
}

}
