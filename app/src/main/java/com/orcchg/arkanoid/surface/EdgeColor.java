package com.orcchg.arkanoid.surface;

import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.support.annotation.ColorInt;

import com.orcchg.arkanoid.R;

class EdgeColor {
    final @ColorInt int DESTROY;
    final @ColorInt int HYPER;
    final @ColorInt int INIT;
    final @ColorInt int VITALITY;
    final @ColorInt int WIN;

    GradientDrawable[] destroy;
    GradientDrawable[] hyper;
    GradientDrawable[] init;
    GradientDrawable[] vitality;
    GradientDrawable[] win;

    EdgeColor(Resources resources) {
        DESTROY = resources.getColor(R.color.edge_color_destroy);
        HYPER = resources.getColor(R.color.edge_color_hyper);
        INIT = resources.getColor(R.color.edge_color_init);
        VITALITY = resources.getColor(R.color.edge_color_vitality);
        WIN = resources.getColor(R.color.edge_color_win);

        {  // destroy
            int[] colors = {Color.TRANSPARENT, DESTROY};
            destroy = new GradientDrawable[4];
            destroy[0] = new GradientDrawable(GradientDrawable.Orientation.BOTTOM_TOP, colors);
            destroy[1] = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
            destroy[2] = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
            destroy[3] = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
        }

        {  // hyper
            int[] colors = {Color.TRANSPARENT, HYPER};
            hyper = new GradientDrawable[4];
            hyper[0] = new GradientDrawable(GradientDrawable.Orientation.BOTTOM_TOP, colors);
            hyper[1] = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
            hyper[2] = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
            hyper[3] = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
        }

        {  // init
            int[] colors = {Color.TRANSPARENT, INIT};
            init = new GradientDrawable[4];
            init[0] = new GradientDrawable(GradientDrawable.Orientation.BOTTOM_TOP, colors);
            init[1] = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
            init[2] = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
            init[3] = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
        }

        {  // vitality
            int[] colors = {Color.TRANSPARENT, VITALITY};
            vitality = new GradientDrawable[4];
            vitality[0] = new GradientDrawable(GradientDrawable.Orientation.BOTTOM_TOP, colors);
            vitality[1] = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
            vitality[2] = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
            vitality[3] = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
        }

        {  // win
            int[] colors = {Color.TRANSPARENT, WIN};
            win = new GradientDrawable[4];
            win[0] = new GradientDrawable(GradientDrawable.Orientation.BOTTOM_TOP, colors);
            win[1] = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
            win[2] = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
            win[3] = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);
        }
    }
}
