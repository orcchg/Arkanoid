package com.orcchg.arkanoid.surface;

import java.lang.ref.WeakReference;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class GameSurface extends SurfaceView implements SurfaceHolder.Callback {
  private static final String TAG = "Arkanoid_GameSurface";
  private static final int VERTICAL_SWIPE_THRESHOLD_DP = 80;  // in dp
  
  private static int mDesiredWidth = 512;   // in dp
  private static int mDesiredHeight = 512;  // in dp
  private int mWidth;
  private int mHeight;
  private float mHalfWidth;
  private float touchCurrentX = 0.0f;
  private float touchCurrentY = 0.0f;
  private float VERTICAL_SWIPE_THRESHOLD;
  
  private WeakReference<AsyncContext> mAsyncContextRef;
  
  public GameSurface(Context context) {
    this(context, null);
  }
  
  public GameSurface(Context context, AttributeSet attrs) {
    this(context, attrs, 0 /* custom default style */);
  }
  
  public GameSurface(Context context, AttributeSet attrs, int defStyleAttr) {
//    this(context, attrs, defStyleAttr, 0);
    super(context, attrs, defStyleAttr);
    VERTICAL_SWIPE_THRESHOLD = Utility.dipToPixel(context, VERTICAL_SWIPE_THRESHOLD_DP);
    getHolder().addCallback(this);
  }
  
//  public GameSurface(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
//    super(context, attrs, defStyleAttr, defStyleRes);
//    getHolder().addCallback(this);
//  }

  @Override
  public void surfaceCreated(SurfaceHolder holder) {
    Log.d(TAG, "Game surface created");
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    Log.d(TAG, "Game surface changed");
    if (mAsyncContextRef != null) {
      AsyncContext acontext = mAsyncContextRef.get();
      if (acontext != null) {
        acontext.setSurface(holder.getSurface());
      }
    }
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder) {
    Log.d(TAG, "Game surface destroyed");
    if (mAsyncContextRef != null) {
      AsyncContext acontext = mAsyncContextRef.get();
      if (acontext != null) {
        acontext.setSurface(null);
      }
    }
  }

  @Override
  public boolean onTouchEvent(MotionEvent event) {
    switch (event.getAction()) {
      case MotionEvent.ACTION_DOWN:
        touchCurrentX = event.getX() * event.getXPrecision();
        touchCurrentY = event.getY() * event.getYPrecision();
        break;
      case MotionEvent.ACTION_MOVE:
        float position = event.getX() * event.getXPrecision() - mHalfWidth;
        if (mAsyncContextRef != null) {
          AsyncContext acontext = mAsyncContextRef.get();
          if (acontext != null) {
            acontext.shiftGamepad(position / mHalfWidth);
          }
        }
        break;
      case MotionEvent.ACTION_UP:
        float pointerTwoTouchXdiff = event.getX() * event.getXPrecision() - touchCurrentX;
        float pointerTwoTouchYdiff = Math.abs(event.getY() * event.getYPrecision() - touchCurrentY);
        if (pointerTwoTouchYdiff >= VERTICAL_SWIPE_THRESHOLD) {
          float ratio = pointerTwoTouchYdiff / pointerTwoTouchXdiff;
          float angle = (float) Math.atan(Math.abs(ratio));
          angle = ratio >= 0 ? angle : (float) (Math.PI - angle);
          if (mAsyncContextRef != null) {
            AsyncContext acontext = mAsyncContextRef.get();
            if (acontext != null) {
              acontext.throwBall(angle);
            }
          }
        }
        performClick();
      case MotionEvent.ACTION_CANCEL:
        touchCurrentX = 0.0f;
        touchCurrentY = 0.0f;
        break;
    }
    return true;
  }
  
  @Override
  public boolean performClick() {
    return super.performClick();
  }
  
  /* Internal methods */
  // --------------------------------------------------------------------------
  void setAsyncContext(final AsyncContext acontext) {
    mAsyncContextRef = new WeakReference<AsyncContext>(acontext);
  }
  
  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
  }
  
  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    int widthMode = MeasureSpec.getMode(widthMeasureSpec);
    int widthSize = MeasureSpec.getSize(widthMeasureSpec);
    int heightMode = MeasureSpec.getMode(heightMeasureSpec);
    int heightSize = MeasureSpec.getSize(heightMeasureSpec);

    if (widthMode == MeasureSpec.EXACTLY) {
      mWidth = widthSize;
    } else if (widthMode == MeasureSpec.AT_MOST) {
      mWidth = Math.min(mDesiredWidth, widthSize);
    } else {
      mWidth = mDesiredWidth;
    }
    mHalfWidth = mWidth * 0.5f;

    if (heightMode == MeasureSpec.EXACTLY) {
      mHeight = heightSize;
    } else if (heightMode == MeasureSpec.AT_MOST) {
      mHeight = Math.min(mDesiredHeight, heightSize);
    } else {
      mHeight = mDesiredHeight;
    }

    setMeasuredDimension(mWidth, mHeight);
  }
}
