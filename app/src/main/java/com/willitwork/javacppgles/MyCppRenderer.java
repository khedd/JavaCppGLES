package com.willitwork.javacppgles;

//java interface for rendering using c++
class MyCppRenderer {

    void draw() {
        _draw();
    }

    void init(){
        _init();
    }


    private native void _init ();
    private native void _draw ();
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("crenderer");
    }

}
