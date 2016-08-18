package com.orcchg.arkanoid.surface;

import android.content.res.AssetManager;

public class NativeResources {
  private static final String TAG = "Arkanoid_NativeResources";
  
  private final long descriptor;
  
  NativeResources(AssetManager assets, String internal_storage) {
    descriptor = init(assets, internal_storage);
  }
  
  long getPtr() { return descriptor; }
  
  /* Package API */
  // --------------------------------------------------------------------------
  boolean readTexture(String filename) { return readTexture(descriptor, filename); }
  boolean readSound(String filename) { return readSound(descriptor, filename); }
  void release() { release(descriptor); }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private native long init(AssetManager assets, String internal_storage);
  private native boolean readTexture(long descriptor, String filename);
  private native boolean readSound(long descriptor, String filename);
  private native void release(long descriptor);
}
