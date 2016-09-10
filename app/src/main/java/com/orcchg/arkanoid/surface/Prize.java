package com.orcchg.arkanoid.surface;

import android.support.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

public class Prize {
  public static final int NONE = 0;
  public static final int BLOCK = 1;
  public static final int CLIMB = 2;
  public static final int DESTROY = 3;
  public static final int DRAGON = 4;
  public static final int EASY = 5;
  public static final int EASY_T = 6;
  public static final int EVAPORATE = 7;
  public static final int EXPLODE = 8;
  public static final int EXTEND = 9;
  public static final int FAST = 10;
  public static final int FOG = 11;
  public static final int GOO = 12;
  public static final int HYPER = 13;
  public static final int INIT = 14;
  public static final int JUMP = 15;
  public static final int LASER = 16;
  public static final int MIRROR = 17;
  public static final int PIERCE = 18;
  public static final int PROTECT = 19;
  public static final int RANDOM = 20;
  public static final int SHORT = 21;
  public static final int SLOW = 22;
  public static final int UPGRADE = 23;
  public static final int DEGRADE = 24;
  public static final int VITALITY = 25;
  public static final int ZYGOTE = 26;
  public static final int SCORE_1 = 27;
  public static final int SCORE_2 = 28;
  public static final int SCORE_3 = 29;
  public static final int SCORE_4 = 30;
  public static final int SCORE_5 = 31;
  public static final int WIN = 32;

  @IntDef({
          NONE, BLOCK, CLIMB, DESTROY, DRAGON, EASY, EASY_T, EVAPORATE, EXPLODE, EXTEND,
          FAST, FOG, GOO, HYPER, INIT, JUMP, LASER, MIRROR, PIERCE, PROTECT, RANDOM,
          SHORT, SLOW, UPGRADE, DEGRADE, VITALITY, ZYGOTE, SCORE_1, SCORE_2, SCORE_3,
          SCORE_4, SCORE_5, WIN
  })
  @Retention(RetentionPolicy.SOURCE)
  public @interface Type {}
}
