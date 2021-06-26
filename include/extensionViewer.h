#pragma once

#include <Viewer.h>

class learnOpenGLProgram:public GLProgram{
private:
    GLuint uOurColor;
    virtual void postLink() override{
        uOurColor = glGetUniformLocation(handle,"ourColor");
    }
public:
    learnOpenGLProgram():GLProgram(){

    }

    void setOurColor(const Vector4f& color){
        glUseProgram(*this);
        glUniform4f(uOurColor,color(0),color(1),color(2),color(3));
        glUseProgram(0);
    }
};