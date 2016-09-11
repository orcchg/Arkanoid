package com.orcchg.arkanoid.surface;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.support.annotation.ColorInt;
import android.support.v4.app.FragmentActivity;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.View;
import android.view.animation.DecelerateInterpolator;
import android.widget.ImageView;
import android.widget.TextView;

import com.orcchg.arkanoid.R;
import com.orcchg.arkanoid.surface.Database.DatabaseException;
import com.orcchg.arkanoid.surface.Database.GameStat;

import java.io.IOException;
import java.lang.ref.WeakReference;

import timber.log.Timber;

public class MainActivity extends FragmentActivity {
  private static final int PLAYER_ID = 1;
  private static final int INITIAL_LIVES = 3;
  private static final int INITIAL_LEVEL = 0;
  private static final int INITIAL_SCORE = 0;
  private int currentLives = INITIAL_LIVES;
  private int currentLevel = INITIAL_LEVEL;
  private int currentScore = INITIAL_SCORE;
  
//  private static String mAlertDialogTitle;
//  private static String mCloseButtonLabel;
//  private static String mWarningMessage;
  private boolean dropStatFlag = false;
  
  static {
    System.loadLibrary("Arkanoid");
  }
  
  private AsyncContext mAsyncContext;
  private GameSurface mSurface;
  private NativeResources mNativeResources;
  private TextView mInfoTextView, mAddInfoTextView;
  private TextView mLifeMultiplierTextView, mCardinalityTextView;
  private ImageView[] mLifeViews;
  private View[] mEdgeViews;

  private EdgeColor mEdgeColor;
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    Timber.d("onCreate");
    setContentView(R.layout.activity_main);
    
    Resources res = getResources();
    mEdgeColor = new EdgeColor(res);
    final int FRAME_DELAY_NANOS = res.getInteger(R.integer.frame_delay_nanos);
//    mAlertDialogTitle = res.getString(R.string.internal_error);
//    mCloseButtonLabel = res.getString(R.string.close_button);
//    mWarningMessage = res.getString(R.string.internal_error_message);
    Timber.i("Frame delay is %s (nanos)", FRAME_DELAY_NANOS);
    
    dropStatFlag = getIntent().getBooleanExtra(InitActivity.bundleKey_dropStat, false);
    
    mAsyncContext = new AsyncContext(FRAME_DELAY_NANOS);
    mAsyncContext.setCoreEventListener(new CoreEventHandler(this));

    mSurface = (GameSurface) findViewById(R.id.surface_view);
    mInfoTextView = (TextView) findViewById(R.id.info_textview);
    mAddInfoTextView = (TextView) findViewById(R.id.add_info_textview);
    mLifeMultiplierTextView = (TextView) findViewById(R.id.life_multiplier_textview);
    mCardinalityTextView = (TextView) findViewById(R.id.cardinality_textview);
    mLifeViews = new ImageView[] {
      (ImageView) findViewById(R.id.life1_imageview),
      (ImageView) findViewById(R.id.life2_imageview),
      (ImageView) findViewById(R.id.life3_imageview),
      (ImageView) findViewById(R.id.life4_imageview),
      (ImageView) findViewById(R.id.life5_imageview)};
    mEdgeViews = new View[] {
            findViewById(R.id.top_edge), findViewById(R.id.bottom_edge),
            findViewById(R.id.left_edge), findViewById(R.id.right_edge)
    };
    
    // ------------------------------------------
    mNativeResources = new NativeResources(getAssets(), getFilesDir().getAbsolutePath());
    try {
      String[] texture_resources = getAssets().list("texture");
      for (String texture : texture_resources) {
//        Timber.d(TAG, "Texture asset: " + texture);
        mNativeResources.readTexture(texture);
      }
      String[] sound_resources = getAssets().list("sound");
      for (String sound : sound_resources) {
//        Timber.d(TAG, "Sound asset: " + sound);
        mNativeResources.readSound(sound);
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
    mAsyncContext.setResourcesPtr(mNativeResources.getPtr());
    
    // ------------------------------------------
    ArkanoidApplication app = (ArkanoidApplication) getApplication();
    if (!app.DATABASE.updatePlayer(PLAYER_ID, "Player")) {
      try {
        app.DATABASE.insertPlayer("Player");
      } catch (DatabaseException e) {
        e.printStackTrace();
      }
    }
  }
  
  @Override
  protected void onResume() {
    Timber.d("onResume");
    mAsyncContext.start();
    mSurface.setAsyncContext(mAsyncContext);
    mAsyncContext.loadResources();
    
    ArkanoidApplication app = (ArkanoidApplication) getApplication();
    GameStat game_stat = app.DATABASE.getStat(PLAYER_ID);
    String level_state = "";
    if (!dropStatFlag && game_stat != null) {
      setLives(game_stat.lives);
      setLevel(game_stat.level);
      setScore(game_stat.score);
      level_state = game_stat.state;
    } else {
      setLives(INITIAL_LIVES);
      setLevel(INITIAL_LEVEL);
      setScore(INITIAL_SCORE);
      mAsyncContext.fireJavaEvent_refreshLives();
      mAsyncContext.fireJavaEvent_refreshLevel();
      mAsyncContext.fireJavaEvent_refreshScore();
    }
    mAsyncContext.loadLevel(Levels.get(currentLevel, level_state));
    setBonusPrizes();
    super.onResume();
  }
  
  @Override
  protected void onPause() {
    Timber.d("onPause");
    setStat(PLAYER_ID, currentLives, currentLevel, currentScore);
    mAsyncContext.stop();
    finish();
    super.onPause();
  }
  
  @Override
  protected void onDestroy() {
    Timber.d("onDestroy");
    mAsyncContext.destroy();
    mNativeResources.release();
    releaseAllViews();
    System.gc();
    super.onDestroy();
  }
  
  @Override
  public boolean onKeyDown(int keyCode, KeyEvent event) {
    switch (keyCode) {
      case KeyEvent.KEYCODE_BACK:
        Timber.d("onKeyDown( back )");
        finish();
        break;
    }
    return super.onKeyDown(keyCode, event);
  }
  
  @Override
  public boolean onCreateOptionsMenu(android.view.Menu menu) {
    getMenuInflater().inflate(R.menu.arkanoid_menu, menu);
    return true;
  }
  
  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    switch (item.getItemId()) {
      case R.id.throwBall:
        mAsyncContext.throwBall((float) Math.PI / 6);
        break;
      case R.id.nextLevel:
        mAsyncContext.fireJavaEvent_levelFinished();
        mAsyncContext.fireJavaEvent_onScoreUpdated(-10 * (int) Math.pow(currentLevel + 1, 2));
        break;
      case R.id.dropStat:
        ArkanoidApplication app = (ArkanoidApplication) getApplication();
        app.DATABASE.clearStat(PLAYER_ID);
        setLives(INITIAL_LIVES);
        setLevel(INITIAL_LEVEL);
        setScore(INITIAL_SCORE);
        mAsyncContext.fireJavaEvent_refreshLives();
        mAsyncContext.fireJavaEvent_refreshLevel();
        mAsyncContext.fireJavaEvent_refreshScore();
        mAsyncContext.loadLevel(Levels.get(currentLevel));
        setBonusPrizes();
        break;
    }
    return true;
  }
  
  /* Gameplay methods */
  // --------------------------------------------------------------------------
  void lostGame() {
    Timber.i("Game is lost!");
    setLives(INITIAL_LIVES);
    mAsyncContext.fireJavaEvent_refreshLives();
    mAsyncContext.loadLevel(Levels.get(currentLevel, ""));
    setBonusPrizes();
  }
  
  /* Support methods */
  // --------------------------------------------------------------------------
  AsyncContext getAsyncContext() { return mAsyncContext; }
  
  GameStat getStat(long player_id) {
    ArkanoidApplication app = (ArkanoidApplication) getApplication();
    return app.DATABASE.getStat(player_id);
  }
  
  void setStat(long player_id, int lives, int level, int score) {
    ArkanoidApplication app = (ArkanoidApplication) getApplication();
    Timber.i("Stat to be stored: [%s, %s, %s]", lives, level, score);
    String levelState = mAsyncContext.saveLevel();
    Timber.i("Level: %s", levelState);
    if (!app.DATABASE.updateStat(player_id, lives, level, score, levelState)) {
      try {
        app.DATABASE.insertStat(player_id, lives, level, score, levelState);
      } catch (DatabaseException e) {
        e.printStackTrace();
      }
    }
  }
  
  void setLives(int lives) {
    int auxLives = lives;
    if (lives > mLifeViews.length) {
      mLifeMultiplierTextView.setText("+" + Integer.toString(lives - mLifeViews.length));
      auxLives = mLifeViews.length;
    } else if (lives < 0) {
      lives = 0;
      lostGame();
      return;
    } else {
      mLifeMultiplierTextView.setText("");
    }
    for (int i = 0; i < auxLives; ++i) {
      mLifeViews[i].setVisibility(View.VISIBLE);
    }
    for (int i = auxLives; i < mLifeViews.length; ++i) {
      mLifeViews[i].setVisibility(View.INVISIBLE);
    }
    currentLives = lives;
  }
  
  void setLevel(int level) {
    currentLevel = level;
  }
  
  void setScore(int score) {
    currentScore = score;
    mInfoTextView.setText(Integer.toString(score));
  }
  
  void setScoreUpdate(int score) {
    if (score >= 0) {
      mAddInfoTextView.setTextColor(Color.GREEN);
      mAddInfoTextView.setText("+" + Integer.toString(score));
    } else {
      mAddInfoTextView.setTextColor(Color.RED);
      mAddInfoTextView.setText(Integer.toString(score));
    }
  }
  
  void setAngleValue(int angle) {
    mInfoTextView.setText(Integer.toString(angle));
  }
  
  void setCardinalityValue(int new_cardinality) {
    mCardinalityTextView.setText(Integer.toString(new_cardinality));
  }
  
  void setBonusPrizes() {
    @Prize.Type int prizeType = Prize.NONE;
    switch (currentLevel) {
      case 3:
      case 16:
      case 25:
      case 39:
      case 51:
        prizeType = Prize.BLOCK;
        break;
      case 54:
        prizeType = Prize.VITALITY;
        break;
      case 55:
      case 61:
        prizeType = Prize.DESTROY;
        break;
      case 56:
        prizeType = Prize.PIERCE;
        break;
    }
    mAsyncContext.setBonusPrizes(prizeType);
  }
  
  void levelFinishedAdditional() {
    setStat(PLAYER_ID, currentLives, currentLevel, currentScore);
  }
  
  private void warningDialog() {
//    new AlertDialog.Builder(this)
//        .setTitle(mAlertDialogTitle)
//        .setMessage(mWarningMessage)
//        .setPositiveButton(mCloseButtonLabel, new DialogInterface.OnClickListener() {
//          @Override
//          public void onClick(DialogInterface dialog, int which) {
//            finish();
//          }
//        }).show();
    finish();
  }
  
  private void debugDialog(String message) {
    if (currentLevel != 0) {
      return;
    }
    new AlertDialog.Builder(this)
        .setTitle("Debug")
        .setMessage(message)
        .setPositiveButton("Close", new DialogInterface.OnClickListener() {
          @Override
          public void onClick(DialogInterface dialog, int which) {
            // no-op
          }
        }).show();
  }
  
  private void releaseAllViews() {
    mSurface = null;
    mInfoTextView = null;
    mAddInfoTextView = null;
    mLifeMultiplierTextView = null;
    mCardinalityTextView = null;
    for (int i = 0; i < mLifeViews.length; ++i) {
      mLifeViews[i] = null;
    }
  }
  
  /* Core event listeners */
  // --------------------------------------------------------------------------
  private static class CoreEventHandler implements AsyncContext.CoreEventListener {
    private WeakReference<MainActivity> activityRef;
    private int currentLives = INITIAL_LIVES;
    private int currentLevel = INITIAL_LEVEL;
    private int currentScore = INITIAL_SCORE;
    
    CoreEventHandler(final MainActivity activity) {
      activityRef = new WeakReference<>(activity);
      GameStat game_stat = activity.getStat(PLAYER_ID);
      if (game_stat != null) {
        currentLives = game_stat.lives;
        currentLevel = game_stat.level;
        currentScore = game_stat.score;
      }
    }
    
    @Override
    public void onRefreshLives() {
      currentLives = INITIAL_LIVES;
    }
    
    @Override
    public void onRefreshLevel() {
      currentLevel = INITIAL_LEVEL;
    }
    
    @Override
    public void onRefreshScore() {
      currentScore = INITIAL_SCORE;
    }
    
    @Override
    public void onThrowBall() {
      Timber.i("Ball has been thrown!");
    }
    
    @Override
    public void onLostBall() {
      edgeEffect(Prize.DESTROY);
      --currentLives;
      Timber.i("Ball has been lost! Lives rest: %s", currentLives);
      updateLives();
      onScoreUpdated(-2 * (int) Math.pow(currentLevel + 1, 2));  // lost ball decreases score
    }

    @Override
    public void onLevelFinished() {
      Timber.i("Level finished!");
      final MainActivity activity = activityRef.get();
      if (activity != null) {
        edgeEffect(Prize.WIN);
        ++currentLevel;
        if (currentLevel >= Levels.TOTAL_LEVELS) {
          currentLevel = INITIAL_LEVEL;
        }
        activity.setLevel(currentLevel);
        activity.mAsyncContext.loadLevel(Levels.get(currentLevel));
        activity.setBonusPrizes();
        activity.levelFinishedAdditional();
      }
    }
    
    @Override
    public void onScoreUpdated(final int score) {
      currentScore += score;
      if (currentScore < 0) {
        currentScore = 0;
      }
      final MainActivity activity = activityRef.get();
      if (activity != null) {
        activity.runOnUiThread(new Runnable() {
        @Override
        public void run() {
          activity.setScore(currentScore);
          activity.setScoreUpdate(score);
        }});
      }
    }
    
    @Override
    public void onAngleChanged(final int angle) {
//      final MainActivity activity = activityRef.get();
//      if (activity != null) {
//        activity.runOnUiThread(new Runnable() {
//          @Override
//          public void run() {
//            activity.setAngleValue(angle);
//          }});
//      }
    }
    
    @Override
    public void onCardinalityChanged(final int new_cardinality) {
      final MainActivity activity = activityRef.get();
      if (activity != null) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            activity.setCardinalityValue(new_cardinality);
          }});
      }
    }
    
    @Override
    public void onPrizeCatch(final @Prize.Type int prize) {
      int score = 0;
      edgeEffect(prize);
      switch (prize) {
        case Prize.BLOCK:
        case Prize.CLIMB:  // effect not implemented
          score += 35;
          break;
        case Prize.DESTROY:
          --currentLives;
          updateLives();
          break;
        case Prize.DRAGON:  // effect not implemented
          score += 90;
          break;
        case Prize.EASY:
        case Prize.EASY_T:
        case Prize.EVAPORATE:  // effect not implemented
        case Prize.EXPLODE:
        case Prize.EXTEND:
        case Prize.FAST:
        case Prize.FOG:  // effect not implemented
        case Prize.GOO:
        case Prize.HYPER:
          score += 35;
          break;
        case Prize.INIT:
          final MainActivity activity = activityRef.get();
          if (activity != null) {
            activity.mAsyncContext.loadLevel(Levels.get(currentLevel, ""));
            activity.setBonusPrizes();
          }
          break;
        case Prize.JUMP:
        case Prize.LASER:
        case Prize.MIRROR:
        case Prize.PIERCE:
        case Prize.PROTECT:
        case Prize.RANDOM:
        case Prize.SHORT:
        case Prize.SLOW:
        case Prize.UPGRADE:
        case Prize.DEGRADE:
          score += 35;
          break;
        case Prize.VITALITY:
          ++currentLives;
          score += 45;
          updateLives();
          break;
        case Prize.WIN:
          score += 400;
          break;
        case Prize.ZYGOTE:  // effect not implemented
          score += 15;
          break;
        case Prize.SCORE_5:
          score += 95;
        case Prize.SCORE_4:
          score += 75;
        case Prize.SCORE_3:
          score += 50;
        case Prize.SCORE_2:
          score += 35;
        case Prize.SCORE_1:
          score += 20;
        case Prize.NONE:
        default:
          break;
      }
      onScoreUpdated(score);
    }

    private void edgeEffect(@Prize.Type int prize) {
      MainActivity activity = activityRef.get();
      if (activity != null) {
        @ColorInt int color = 0;
        switch (prize) {
          case Prize.DESTROY:
            color = activity.mEdgeColor.DESTROY;
            break;
          case Prize.HYPER:
            color = activity.mEdgeColor.HYPER;
            break;
          case Prize.INIT:
            color = activity.mEdgeColor.INIT;
            break;
          case Prize.VITALITY:
            color = activity.mEdgeColor.VITALITY;
            break;
          case Prize.WIN:
            color = activity.mEdgeColor.WIN;
            break;
          default:
            color = 0;  // no edge effect
            break;
        }
        if (color != 0) {
          activity.edgeEffect(color);
        }
      }
    }
    
    @Override
    public void onErrorTextureLoad() {
      warningDialog();
    }
    
    @Override
    public void onErrorSoundLoad() {
      warningDialog();
    }
    
    @Override
    public void onDebugMessage(String message) {
      debugDialog(message);
    }
    
    // ------------------------------------------
    private void updateLives() {
      final MainActivity activity = activityRef.get();
      if (activity != null) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            activity.setLives(currentLives);
          }
        });
      }
    }
    
    private void warningDialog() {
      final MainActivity activity = activityRef.get();
      if (activity != null) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            activity.warningDialog();
          }
        });
      }
    }
    
    private void debugDialog(final String message) {
      final MainActivity activity = activityRef.get();
      if (activity != null) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            activity.debugDialog(message);
          }
        });
      }
    }
  }  // end of inner class

  // --------------------------------------------------------------------------
  private void edgeEffect(final @ColorInt int color) {
    Timber.v("Edge effect %s", color);
    runOnUiThread(new Runnable() {
      @Override
      public void run() {
        int[] colors = {Color.TRANSPARENT, color};
        GradientDrawable top = new GradientDrawable(GradientDrawable.Orientation.BOTTOM_TOP, colors);
        GradientDrawable bottom = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM, colors);
        GradientDrawable left = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
        GradientDrawable right = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, colors);

        animateEdgeView(0, top);
        animateEdgeView(1, bottom);
        animateEdgeView(2, left);
        animateEdgeView(3, right);
      }
    });
  }

  private void animateEdgeView(int index, Drawable drawable) {
    mEdgeViews[index].setBackgroundDrawable(drawable);
    ObjectAnimator fadeOut = ObjectAnimator.ofFloat(mEdgeViews[index], "alpha", 1.0f, 0.0f);
    fadeOut.addUpdateListener(createAnimatorUpdateListener(mEdgeViews[index]));
    fadeOut.addListener(createAnimatorListener(mEdgeViews[index]));
    fadeOut.setInterpolator(new DecelerateInterpolator());
    fadeOut.setDuration(1000);
    fadeOut.start();
  }

  private Animator.AnimatorListener createAnimatorListener(final View view) {
    return new Animator.AnimatorListener() {
      @Override
      public void onAnimationStart(Animator animator) {
        view.setVisibility(View.VISIBLE);
      }

      @Override
      public void onAnimationEnd(Animator animator) {
        view.setVisibility(View.INVISIBLE);
      }

      @Override
      public void onAnimationCancel(Animator animator) {
        view.setVisibility(View.INVISIBLE);
      }

      @Override
      public void onAnimationRepeat(Animator animator) {
      }
    };
  }

  private ValueAnimator.AnimatorUpdateListener createAnimatorUpdateListener(final View view) {
    return new ValueAnimator.AnimatorUpdateListener() {
      @Override
      public void onAnimationUpdate(ValueAnimator valueAnimator) {
        view.setAlpha((float) valueAnimator.getAnimatedValue());
      }
    };
  }
}
