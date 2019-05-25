package com.willitwork.javacppgles;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    private MyGLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mGLSurfaceView = new MyGLSurfaceView(this);
        setContentView( mGLSurfaceView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLSurfaceView.onResume();
    }
}
