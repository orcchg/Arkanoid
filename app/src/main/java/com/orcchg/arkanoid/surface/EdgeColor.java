package com.orcchg.arkanoid.surface;

import android.content.res.Resources;
import android.support.annotation.ColorInt;

import com.orcchg.arkanoid.R;

class EdgeColor {
    final @ColorInt int DESTROY;
    final @ColorInt int HYPER;
    final @ColorInt int INIT;
    final @ColorInt int VITALITY;
    final @ColorInt int WIN;

    EdgeColor(Resources resources) {
        DESTROY = resources.getColor(R.color.edge_color_destroy);
        HYPER = resources.getColor(R.color.edge_color_hyper);
        INIT = resources.getColor(R.color.edge_color_init);
        VITALITY = resources.getColor(R.color.edge_color_vitality);
        WIN = resources.getColor(R.color.edge_color_win);
    }
}
