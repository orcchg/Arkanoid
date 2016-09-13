package com.orcchg.arkanoid.surface;

import android.content.Context;
import android.support.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

public class Utility {
  static int dipToPixel(Context context, int dip) {
    float density = context.getResources().getDisplayMetrics().density;
    return (int) (dip * density + 0.5f);
  }

  static final int BALL_LOST_NONE = 0;
  static final int BALL_LOST_MISSING = 1;
  static final int BALL_LOST_DESTROY = 2;
  @IntDef({BALL_LOST_NONE, BALL_LOST_MISSING, BALL_LOST_DESTROY})
  @Retention(RetentionPolicy.SOURCE)
  @interface BallLost {}
}
