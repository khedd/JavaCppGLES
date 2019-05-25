//
// Created by stadmin on 20.07.2017.
//

#ifndef JAVACOPENGL_RENDERER_H
#define JAVACOPENGL_RENDERER_H
//is it because of only gl2?
#include <GLES2/gl2.h>

class Renderer {
public:
    // initializes shaders and gl buffers
    void init();

    //renders the openg buffers using the shader
    void draw();
private:
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;

    GLuint mProgram;
    GLint mVertexAttribPos;

    unsigned int mElementCount = 0;
};


#endif //JAVACOPENGL_RENDERER_H
