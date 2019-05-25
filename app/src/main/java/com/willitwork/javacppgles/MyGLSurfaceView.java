package com.willitwork.javacppgles;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class MyGLSurfaceView extends GLSurfaceView {
    private final MyGLRenderer mGLRenderer;
    public MyGLSurfaceView(Context context) {
        super(context);

//        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
//        setEGLContextClientVersion(3);
        setEGLContextClientVersion(2);
        mGLRenderer = new MyGLRenderer();

        setRenderer( mGLRenderer);
    }
}
