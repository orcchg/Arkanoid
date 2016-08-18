package com.orcchg.arkanoid.surface;

import android.view.Surface;

class AsyncContext {
  private static final String TAG = "Arkanoid_AsyncContext";
  
  private final long descriptor;
  
  interface CoreEventListener {
    void onRefreshLives();
    void onRefreshLevel();
    void onRefreshScore();
    void onThrowBall();
    void onLostBall();
    void onLevelFinished();
    void onScoreUpdated(int score);
    void onAngleChanged(int angle);
    void onCardinalityChanged(int new_cardinality);
    void onPrizeCatch(Prize prize);
    void onErrorTextureLoad();
    void onErrorSoundLoad();
    void onDebugMessage(String message);
  }
  
  private CoreEventListener mListener;
  
  AsyncContext() {
    descriptor = init();
  }
  
  /* Package API */
  // --------------------------------------------------------------------------
  void start() { start(descriptor); }
  void stop() { stop(descriptor); }
  void destroy() { destroy(descriptor); }
  void setSurface(Surface surface) { setSurface(descriptor, surface); }
  void setResourcesPtr(long resources) { setResourcesPtr(descriptor, resources); }
  void loadResources() { loadResources(descriptor); }
  
  /* User actions */
  void shiftGamepad(float position) { shiftGamepad(descriptor, position); }
  void throwBall(float angle) { throwBall(descriptor, angle); }

  /* Tools */
  void loadLevel(final String[] level) { loadLevel(descriptor, level); }
  void setBonusBlocks(boolean flag) { setBonusBlocks(descriptor, flag); }
  
  String saveLevel() {
    String[] tokens = saveLevel(descriptor);
    StringBuilder builder = new StringBuilder();
    String delim = "";
    for (String token : tokens) {
      builder.append(delim);
      delim = "!";
      builder.append(token);
    }
    return builder.toString();
  }
  
  /* Events coming from native Core */
  void setCoreEventListener(CoreEventListener listener) {
    mListener = listener;
  }
  
  void fireJavaEvent_refreshLives() {
    if (mListener != null) {
      mListener.onRefreshLives();
    }
  }
  
  void fireJavaEvent_refreshLevel() {
    if (mListener != null) {
      mListener.onRefreshLevel();
    }
  }
  
  void fireJavaEvent_refreshScore() {
    if (mListener != null) {
      mListener.onRefreshScore();
    }
  }
  
  void fireJavaEvent_throwBall() {
    if (mListener != null) {
      mListener.onThrowBall();
    }
  }
  
  void fireJavaEvent_lostBall() {
    if (mListener != null) {
      mListener.onLostBall();
    }
  }
  
  void fireJavaEvent_levelFinished() {
    if (mListener != null) {
      mListener.onLevelFinished();
    }
  }
  
  void fireJavaEvent_onScoreUpdated(int score) {
    if (mListener != null) {
      mListener.onScoreUpdated(score);
    }
  }
  
  void fireJavaEvent_angleChanged(int angle) {
    if (mListener != null) {
      mListener.onAngleChanged(angle);
    }
  }
  
  void fireJavaEvent_cardinalityChanged(int new_cardinality) {
    if (mListener != null) {
      mListener.onCardinalityChanged(new_cardinality);
    }
  }
  
  void fireJavaEvent_prizeCatch(int prize) {
    if (mListener != null) {
      mListener.onPrizeCatch(Prize.fromInt(prize));
    }
  }
  
  void fireJavaEvent_errorTextureLoad() {
    if (mListener != null) {
      mListener.onErrorTextureLoad();
    }
  }
  
  void fireJavaEvent_errorSoundLoad() {
    if (mListener != null) {
      mListener.onErrorSoundLoad();
    }
  }
  
  void fireJavaEvent_debugMessage(String message) {
    if (mListener != null) {
      mListener.onDebugMessage(message);
    }
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private native long init();
  private native void start(long descriptor);
  private native void stop(long descriptor);
  private native void destroy(long descriptor);
  private native void setSurface(long descriptor, Surface surface);
  private native void setResourcesPtr(long descriptor, long resources);
  private native void loadResources(long descriptor);
  
  /* User actions */
  private native void shiftGamepad(long descriptor, float position);
  private native void throwBall(long descriptor, float angle);
  
  /* Tools */
  private native void loadLevel(long descriptor, String[] in_level);
  private native String[] saveLevel(long descriptor);
  private native void setBonusBlocks(long descriptor, boolean flag);
  private native void drop(long descriptor);
  private native int getScore(long descriptor);
}
