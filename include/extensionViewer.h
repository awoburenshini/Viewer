#pragma once

#include <Viewer.h>
class Camera {
private:
    Vector3f m_center; // camera position (world coordinates)
    Vector3f m_front;  // camera front
    Vector3f m_up;     // camera up
    Vector3f m_right;  // camera right
private:
    Matrix4f m_view;
    Matrix4f m_projective;

public:
    void setProjective(Float left, Float right, Float bottom, Float top, Float nearVal, Float farVal) {
        m_projective.setZero();
        m_projective(0, 0) = (2.0f * nearVal) / (right - left);
        m_projective(1, 1) = (2.0f * nearVal) / (top - bottom);
        m_projective(0, 2) = (right + left) / (right - left);
        m_projective(1, 2) = (top + bottom) / (top - bottom);
        m_projective(2, 2) = -(farVal + nearVal) / (farVal - nearVal);
        m_projective(3, 2) = -1.0f;
        m_projective(2, 3) = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    };

    void setLookAt(const Vector3f &origin, const Vector3f &target, const Vector3f &up) {
        Eigen::Vector3f f = (target - origin).normalized();
        Eigen::Vector3f s = f.cross(up).normalized();
        Eigen::Vector3f u = s.cross(f);

        m_front  = -f;
        m_center = origin;
        m_right  = s;
        m_up     = u;

        m_view       = Matrix4f::Identity();
        m_view(0, 0) = s(0);
        m_view(0, 1) = s(1);
        m_view(0, 2) = s(2);
        m_view(1, 0) = u(0);
        m_view(1, 1) = u(1);
        m_view(1, 2) = u(2);
        m_view(2, 0) = -f(0);
        m_view(2, 1) = -f(1);
        m_view(2, 2) = -f(2);
        m_view(0, 3) = -s.transpose() * origin;
        m_view(1, 3) = -u.transpose() * origin;
        m_view(2, 3) = f.transpose() * origin;
    };

    void setOrtho(Float left, Float right, Float bottom,
                  Float top, Float nearVal, Float farVal) {
        m_projective.setZero();
        m_projective(0, 0) = 2.0f / (right - left);
        m_projective(1, 1) = 2.0f / (top - bottom);
        m_projective(2, 2) = -2.0f / (farVal - nearVal);
        m_projective(0, 3) = -(right + left) / (right - left);
        m_projective(1, 3) = -(top + bottom) / (top - bottom);
        m_projective(2, 3) = -(farVal + nearVal) / (farVal - nearVal);
    }
    const Matrix4f &getViewMatrix() const {
        return m_view;
    }
    const Matrix4f &getProjectiveMatrix() const {
        return m_projective;
    }
};

class learnOpenGLProgram : public GLProgram {
private:
    GLuint uOurColor;
    GLuint uProj;
    GLuint uView;

    virtual void postLink() override final {
        uOurColor = glGetUniformLocation(handle, "ourColor");
        uProj     = glGetUniformLocation(handle, "proj");
        uView     = glGetUniformLocation(handle, "view");
    }

public:
    learnOpenGLProgram() :
        GLProgram() {
    }

    void setOurColor(const Vector4f &color) {
        glUseProgram(*this);
        glUniform4f(uOurColor, color(0), color(1), color(2), color(3));
        glUseProgram(0);
    }

    void setView(const Matrix4f &view) {
        glUseProgram(*this);
        glUniformMatrix4fv(uView, 1, GL_FALSE, view.data());
        glUseProgram(0);
    }

    void setProjective(const Matrix4f &proj) {
        glUseProgram(*this);
        glUniformMatrix4fv(uProj, 1, GL_FALSE, proj.data());
        glUseProgram(0);
    }
};