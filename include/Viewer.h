#pragma once
#include <fstream>
#include <glad/glad.h>
#include <common.h>
#include <iostream>

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

class GLData : public NonCopyable {
protected:
    GLuint handle; // VAO
    GLData();
    ~GLData();
    void bufferData(GLuint vbo, void *buff, size_t size);
public:

    operator GLuint() const;
};

class GLModelData : public GLData {
protected:
    GLuint vbo[2]; // vertex buffer object handles : position index
public:
    GLModelData(); // will create a intact Vertex Array pointer
    void setPositionData(Real *buff, unsigned int len);
    void setIndexData(unsigned int *buff, unsigned int len);
    const GLuint &getVertexVBO() {
        return vbo[0];
    }

    ~GLModelData();
};

class GLBackground : public NonCopyable {
private:
    class GLBackgroundData {
    public:
        GLBackgroundData();

        void uploadData(Real *data, size_t size);

        void uploadIndice(unsigned int *data, size_t size);

        operator const GLuint() const;

    private:
        GLuint handle;
        GLuint vbo[2];
    };

private:
    GLProgram g_program;
    GLBackgroundData g_data;

    void make_program(const std::string &vs, const std::string &fs);

private:
    Eigen::Matrix<Real, 6, 4, Eigen::ColMajor> m_data;           // four points
    Eigen::Matrix<unsigned int, 3, 2, Eigen::ColMajor> m_indice; // two triangles make a quad;

public:
    GLBackground() = delete;
    GLBackground(const std::string &vs, const std::string &fs);
    void draw();
};