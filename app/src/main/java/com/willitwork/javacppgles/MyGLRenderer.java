package com.willitwork.javacppgles;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class MyGLRenderer implements GLSurfaceView.Renderer {
    final private Triangle mTriangle;
    final private MyCppRenderer mCppRenderer;



    MyGLRenderer(){
        mTriangle = new Triangle();
        mCppRenderer = new MyCppRenderer();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        gl.glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        //init java triangle
        mTriangle.init();
        //init c quad
        mCppRenderer.init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        mTriangle.draw();
        mCppRenderer.draw ();
    }

    static int loadShader(int type, String shaderCode){

        // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
        // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
        int shader = GLES20.glCreateShader(type);

        // add the source code to the shader and compile it
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }

    //Simple triangle class from
    //https://developer.android.com/training/graphics/opengl/shapes.html
    private static class Triangle{
        private FloatBuffer vertexBuffer;
        private int mProgram;
        private int mPositionHandle;
        private int mColorHandle;

        private final int vertexCount = triangleCoords.length / COORDS_PER_VERTEX;
        private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex


        private final String vertexShaderCode =
                "attribute vec4 vPosition;" +
                "void main() {" +
                "  gl_Position = vPosition;" +
                "}";

        private final String fragmentShaderCode =
                "precision mediump float;" +
                "uniform vec4 vColor;" +
                "void main() {" +
                "  gl_FragColor = vColor;" +
                "}";

        // number of coordinates per vertex in this array
        static final int COORDS_PER_VERTEX = 3;
        static float triangleCoords[] = {   // in counterclockwise order:
                0.0f,  0.122008459f, 0.0f, // top
                -0.7f, -0.711004243f, 0.0f, // bottom left
                0.1f, -0.711004243f, 0.0f  // bottom right
        };

        // Set color with red, green, blue and alpha (opacity) values
        float color[] = { 0.63671875f, 0.76953125f, 0.22265625f, 1.0f };

        public Triangle() {
            // initialize vertex byte buffer for shape coordinates
            ByteBuffer bb = ByteBuffer.allocateDirect(
                    // (number of coordinate values * 4 bytes per float)
                    triangleCoords.length * 4);
            // use the device hardware's native byte order
            bb.order(ByteOrder.nativeOrder());

            // create a floating point buffer from the ByteBuffer
            vertexBuffer = bb.asFloatBuffer();
            // add the coordinates to the FloatBuffer
            vertexBuffer.put(triangleCoords);
            // set the buffer to read the first coordinate
            vertexBuffer.position(0);
        }

        public void init (){
            int vertexShader = MyGLRenderer.loadShader(GLES20.GL_VERTEX_SHADER,
                    vertexShaderCode);
            int fragmentShader = MyGLRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER,
                    fragmentShaderCode);

            // create empty OpenGL ES Program
            mProgram = GLES20.glCreateProgram();
            // add the vertex shader to program
            GLES20.glAttachShader(mProgram, vertexShader);
            // add the fragment shader to program
            GLES20.glAttachShader(mProgram, fragmentShader);
            // creates OpenGL ES program executables
            GLES20.glLinkProgram(mProgram);

        }

        public void draw() {
            // Add program to OpenGL ES environment
            GLES20.glUseProgram(mProgram);
            // get handle to vertex shader's vPosition member
            mPositionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
            // Enable a handle to the triangle vertices
            GLES20.glEnableVertexAttribArray(mPositionHandle);
            // Prepare the triangle coordinate data
            GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
                    GLES20.GL_FLOAT, false,
                    vertexStride, vertexBuffer);

            // get handle to fragment shader's vColor member
            mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
            // Set color for drawing the triangle
            GLES20.glUniform4fv(mColorHandle, 1, color, 0);
            // Draw the triangle
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
            // Disable vertex array
            GLES20.glDisableVertexAttribArray(mPositionHandle);
        }

    }
}
