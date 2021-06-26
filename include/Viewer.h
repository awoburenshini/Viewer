#pragma once
#include <fstream>
#include <glad/glad.h>
#include <common.h>

class NonCopyable {
private:
    NonCopyable(const NonCopyable &other) = delete;
    NonCopyable &operator=(const NonCopyable &other) = delete;

public:
    NonCopyable() {
    }
};

class GLShader : public NonCopyable {
private:
    GLuint handle;

public:
    GLShader(GLenum shaderType);
    ~GLShader();
    void compile(const char *source);
    void compile(std::ifstream &source);

    operator const GLuint() const; // declare how to change GLShader to GLuint (For the convenience of OpenGL functions)
};

class GLProgram : public NonCopyable {
protected:
    GLuint handle;
    virtual void postLink();
public:
    GLProgram();
    virtual void link(const GLShader &vshader, const GLShader &fshader);
    operator const GLuint() const;
    ~GLProgram();
};

class GLProgramData : public NonCopyable {
protected:
    GLuint handle; // VAO
    GLuint vbo[2]; // vertex buffer object handles : position index
    void bufferData(unsigned int index, void *buff, size_t size);

public:
    GLProgramData(); // will create a intact Vertex Array pointer
    void setPositionData(Float *buff, unsigned int len);
    void setIndexData(unsigned int *buff, unsigned int len);

    operator GLuint() const;
    ~GLProgramData();
};
