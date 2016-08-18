package com.orcchg.arkanoid.surface;

import android.app.Application;

public class ArkanoidApplication extends Application {
  Database DATABASE;
  
  @Override
  public void onCreate() {
    super.onCreate();
    DATABASE = new Database(getApplicationContext());
  }
}
