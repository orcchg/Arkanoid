#ifndef __ARKANOID_PRIZE__H__
#define __ARKANOID_PRIZE__H__

#include <random>

namespace game {

enum class Prize : int {
  NONE = 0,
  BLOCK = 1,      //! produces BRICK block (0 cardinality)    { level }
  CLIMB = 2,      // [ t ] climbs up the bite
  DESTROY = 3,    //! lose ball
  DRAGON = 4,     // [ t ] summons dragon
  EASY = 5,       //! [ 1 ] for all ordinary / action blocks  { ball }
  EASY_T = 6,     //! [ t ] same                              { ball }
  EVAPORATE = 7,  // transform all WATER to FOG              { level }
  EXPLODE = 8,    //! [ 1 ] explodes blocks nearest to ball   { ball }
  EXTEND = 9,     //! [ t ] extends bite
  FAST = 10,      //! [ t ] speed-up ball
  FOG = 11,       // produces FOG blocks instead NONE (0 cardinality)   { level }
  GOO = 12,       //! [ t ] glues ball and bite               { ball }
  HYPER = 13,     //! teleports ball to random place
  INIT = 14,      //! reloads current level
  JUMP = 15,      //! [ t ] explodes blocks nearest to ball { ball }
  LASER = 16,     //! [ t ] laser beam from bite
  MIRROR = 17,    //! [ t ] all collisions from bite are mirrored  { ball }
  PIERCE = 18,    //! [ 1 ] explodes blocks behind                  { ball }
  PROTECT = 19,   //! [ t ] add protection blocks                  { level }
  RANDOM = 20,    //! [ t ] random angle for any bite collision    { ball }
  SHORT = 21,     //! [ t ] shortens bite
  SLOW = 22,      //! [ t ] slow down ball
  UPGRADE = 23,   //! improves collided ordinary blocks (+ cardinality)  { ball }
  DEGRADE = 24,   //! decreases collided ordinary blocks (- cardinality) { ball }
  VITALITY = 25,  //! additional live
  ZYGOTE = 26,    // multiplies ball                              { ball }
  SCORE_1 = 27,
  SCORE_2 = 28,
  SCORE_3 = 29,
  SCORE_4 = 30,
  SCORE_5 = 31,
  WIN = 32,       //! win level
};

class PrizeUtils {
public:
  constexpr static int totalPrizes = 32;  // WIN not included
  constexpr static double prizeProbability = 0.415;
  constexpr static double winProbability = 0.0025;
};

class PrizeGenerator {
public:
  PrizeGenerator();
  Prize generatePrize();  //!< Generates random prize of any type

  inline void setBonusBlocks(bool flag) { m_bonus_blocks = flag; }

private:
  bool m_bonus_blocks;
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;
  std::bernoulli_distribution m_success_distribution;
  std::bernoulli_distribution m_win_distribution;
};

}

#endif  // __ARKANOID_PRIZE__H__
