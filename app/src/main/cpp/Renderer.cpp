// example adapted from
// https://github.com/googlesamples/android-ndk/blob/master/gles3jni/app/src/main/cpp/gles3jni.cpp
#include "Renderer.h"
#include <vector>
#include <malloc.h>
#include <android/log.h>

//datas to drawn
//no need to import whole glm for simple example
struct Vertex{
    float x, y, z;
};

const Vertex QUAD[4] = {
        // Square with diagonal < 2 so that it fits in a [-1 .. 1]^2 square
        // regardless of rotation.
        {-0.0f, -0.0f, 0.0f},
        {0.7f, -0.0f, 0.0f},
        {-0.0f,  0.7f, 0.0f},
        {0.7f,  0.7f, 0.0f}
};

const unsigned int ORDER[6] = {0, 1, 2, 3, 2, 1};

//for shader loading
#define LOG_TAG "GLES C++"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

//renderer
void Renderer::draw() {
    glUseProgram(mProgram);
//
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glEnableVertexAttribArray(mVertexAttribPos);
    glVertexAttribPointer(mVertexAttribPos, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    //unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::init (){

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    std::vector<Vertex> testVertices (QUAD, QUAD + 4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * testVertices.size(), &testVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    std::vector<unsigned int> testOrder ( ORDER, ORDER + 6);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * testOrder.size(), &testOrder[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    static const char VERTEX_SHADER[] =
            "precision highp float;\n"
            "attribute vec4 inPosition;\n"
            "void main(){\n"
            "   gl_Position = inPosition;\n"
            "}\n";

    static const char FRAGMENT_SHADER[] =
            "precision highp float;\n"
            "void main() {\n"
            "    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
            "}\n";

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);

    mVertexAttribPos = glGetAttribLocation(mProgram, "inPosition");
}