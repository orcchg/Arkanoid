package com.orcchg.arkanoid.surface;

import android.app.Dialog;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.Toast;

import com.orcchg.arkanoid.R;

import java.lang.ref.WeakReference;

import timber.log.Timber;

public class InitActivity extends FragmentActivity {
  private static final String TAG = "Arkanoid_InitActivity";
  static final String bundleKey_dropStat = "bundleKey_dropStat";
  
//  private static String mAlertDialogTitle, mAboutDialogTitle;
//  private static String mOKButtonLabel, mCancelButtonLabel, mCloseButtonLabel;
//  private static String mWarningMessage, mAboutMessage;
  private boolean databaseExists = false;
  
  private Button mContinueGameButton;
//  private Button mNewGameButton;
//  private Button mAboutGameButton;
//  private Button mQuitGameButton;
  
  private MediaPlayer mMediaPlayer;
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    Timber.d(TAG, "onCreate");
    setContentView(R.layout.activity_init);
    
//    Resources res = getResources();
//    mAlertDialogTitle = res.getString(R.string.new_game_button);
//    mAboutDialogTitle = res.getString(R.string.about_title);
//    mOKButtonLabel = res.getString(R.string.ok_button);
//    mCancelButtonLabel = res.getString(R.string.cancel_button);
//    mCloseButtonLabel = res.getString(R.string.close_button);
//    mWarningMessage = res.getString(R.string.warning_message);
//    mAboutMessage = res.getString(R.string.about_message);
    
//    mNewGameButton = (Button) findViewById(R.id.new_game_button);
//    mAboutGameButton = (Button) findViewById(R.id.about_game_button);
//    mQuitGameButton = (Button) findViewById(R.id.quit_game_button);
    
    mMediaPlayer = MediaPlayer.create(this, R.raw.button_click);
  }
  
  public void onClickContinueGame(View view) {
    mMediaPlayer.start();
    Intent intent = new Intent(this, MainActivity.class);
    intent.putExtra(bundleKey_dropStat, false);
    startActivity(intent);
  }

  public void onClickNewGame(View view) {
    mMediaPlayer.start();
    if (databaseExists) {
      warningDialog();
    } else {
      Intent intent = new Intent(this, MainActivity.class);
      intent.putExtra(bundleKey_dropStat, true);
      startActivity(intent);
    }
  }
  
  public void onClickAboutGame(View view) {
    mMediaPlayer.start();
    aboutDialog();
  }
  
  public void onClickQuitGame(View view) {
    mMediaPlayer.start();
    finish();
  }
  
  @Override
  protected void onResume() {
    super.onResume();
    try {
      mContinueGameButton = (Button) findViewById(R.id.continue_game_button);
    } catch (Exception e) {
      // Button was not found
      // It means, your button doesn't exist on the "current" view
      // It was freed from the memory, therefore stop of activity was performed
      // In this case application to be restarted
      Intent i = new Intent();
      i.setClass(getApplicationContext(), InitActivity.class);
      i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
      startActivity(i);
      // Show toast to the user
      Toast.makeText(getApplicationContext(), "Data lost due to excess use of other apps", Toast.LENGTH_LONG).show();
      return;
    }
    databaseExists = checkDatabase();
    mContinueGameButton.setEnabled(databaseExists);
  }
  
  @Override
  protected void onDestroy() {
    mContinueGameButton = null;
    mMediaPlayer.release();
    mMediaPlayer = null;
    System.gc();
    super.onDestroy();
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private boolean checkDatabase() {
//    SQLiteDatabase checkDB = null;
//    try {
//      String path = getDatabasePath(Database.databaseName).getAbsolutePath();
//      Timber.i(TAG, "Checking database existence: " + path);
//      checkDB = SQLiteDatabase.openDatabase(path, null, SQLiteDatabase.OPEN_READONLY);
//    } catch (SQLiteException e) {
//      // database doesn't exist yet
//      Timber.w(TAG, "Database doesn't exist yet");
//    }
//    return checkDB != null;
    ArkanoidApplication app = (ArkanoidApplication) getApplication();
    return (app.DATABASE.totalPlayers() + app.DATABASE.totalStatRecords()) != 0;
  }
  
  private void warningDialog() {
//    final FragmentActivity activity = this;
//    new AlertDialog.Builder(activity)
//        .setTitle(mAlertDialogTitle)
//        .setMessage(mWarningMessage)
//        .setPositiveButton(mOKButtonLabel, new DialogInterface.OnClickListener() {
//          @Override
//          public void onClick(DialogInterface dialog, int which) {
//            Intent intent = new Intent(activity, MainActivity.class);
//            intent.putExtra(bundleKey_dropStat, true);
//            startActivity(intent);
//          }
//        })
//        .setNegativeButton(mCancelButtonLabel, new DialogInterface.OnClickListener() {
//          @Override
//          public void onClick(DialogInterface dialog, int which) {
//            // no-op
//          }
//        }).show();
    new WarningDialog(this).show();
  }
  
  private void aboutDialog() {
//    new AlertDialog.Builder(this)
//        .setTitle(mAboutDialogTitle)
//        .setMessage(mAboutMessage)
//        .setPositiveButton(mCloseButtonLabel, new DialogInterface.OnClickListener() {
//          @Override
//          public void onClick(DialogInterface dialog, int which) {
//            // no-op
//          }
//        }).show();
    new AboutDialog(this).show();
  }
  
  /* Dialogs */
  // --------------------------------------------------------------------------
  private static class AboutDialog extends Dialog implements View.OnClickListener {
    private WeakReference<InitActivity> activityRef;
    
    public AboutDialog(InitActivity activity) {
      super(activity);
      activityRef = new WeakReference<InitActivity>(activity);
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      requestWindowFeature(Window.FEATURE_NO_TITLE);
      setContentView(R.layout.dialog_one_button);
      Button close_button = (Button) findViewById(R.id.dialog_one_button_button);
      close_button.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
      InitActivity activity = activityRef.get();
      if (activity != null) {
        activity.mMediaPlayer.start();
      }
      dismiss();
    }
  }
  
  private static class WarningDialog extends Dialog implements View.OnClickListener {
    private WeakReference<InitActivity> activityRef;
    
    public WarningDialog(InitActivity activity) {
      super(activity);
      activityRef = new WeakReference<InitActivity>(activity);
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      requestWindowFeature(Window.FEATURE_NO_TITLE);
      setContentView(R.layout.dialog_two_buttons);
      Button ok_button = (Button) findViewById(R.id.dialog_two_buttons_first_button);
      Button cancel_button = (Button) findViewById(R.id.dialog_two_buttons_second_button);
      ok_button.setOnClickListener(this);
      cancel_button.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
      InitActivity activity = activityRef.get();
      if (activity != null) {
        activity.mMediaPlayer.start();
      }
      switch (v.getId()) {
        case R.id.dialog_two_buttons_first_button:
          if (activity != null) {
            Intent intent = new Intent(activity, MainActivity.class);
            intent.putExtra(bundleKey_dropStat, true);
            activity.startActivity(intent);
          }
          break;
        case R.id.dialog_two_buttons_second_button:
          // no-op
          break;
      }
      dismiss();
    }
  }
}
