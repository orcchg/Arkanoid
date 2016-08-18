package com.orcchg.arkanoid.surface;

import android.content.Context;

public class Utility {
  public static int dipToPixel(Context context, int dip) {
    float density = context.getResources().getDisplayMetrics().density;
    return (int) (dip * density + 0.5f);
  }
}
