package com.orcchg.arkanoid.surface;

import androidx.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

class Prize {
  static final int NONE = 0;
  static final int BLOCK = 1;
  static final int CLIMB = 2;
  static final int DESTROY = 3;
  static final int DRAGON = 4;
  static final int EASY = 5;
  static final int EASY_T = 6;
  static final int EVAPORATE = 7;
  static final int EXPLODE = 8;
  static final int EXTEND = 9;
  static final int FAST = 10;
  static final int FOG = 11;
  static final int GOO = 12;
  static final int HYPER = 13;
  static final int INIT = 14;
  static final int JUMP = 15;
  static final int LASER = 16;
  static final int MIRROR = 17;
  static final int PIERCE = 18;
  static final int PROTECT = 19;
  static final int RANDOM = 20;
  static final int SHORT = 21;
  static final int SLOW = 22;
  static final int UPGRADE = 23;
  static final int DEGRADE = 24;
  static final int VITALITY = 25;
  static final int ZYGOTE = 26;
  static final int SCORE_1 = 27;
  static final int SCORE_2 = 28;
  static final int SCORE_3 = 29;
  static final int SCORE_4 = 30;
  static final int SCORE_5 = 31;
  static final int WIN = 32;

  @IntDef({
          NONE, BLOCK, CLIMB, DESTROY, DRAGON, EASY, EASY_T, EVAPORATE, EXPLODE, EXTEND,
          FAST, FOG, GOO, HYPER, INIT, JUMP, LASER, MIRROR, PIERCE, PROTECT, RANDOM,
          SHORT, SLOW, UPGRADE, DEGRADE, VITALITY, ZYGOTE, SCORE_1, SCORE_2, SCORE_3,
          SCORE_4, SCORE_5, WIN
  })
  @Retention(RetentionPolicy.SOURCE)
  @interface Type {}
}
