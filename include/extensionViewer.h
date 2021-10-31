#pragma once

#include <Viewer.h>
class Camera {
private:
    Vector3r m_center; // camera position (world coordinates)
    Vector3r m_front;  // camera front
    Vector3r m_up;     // camera up
    Vector3r m_right;  // camera right
private:
    Matrix4r m_view;
    Matrix4r m_projective;

public:
    void setProjective(Real left, Real right, Real bottom, Real top, Real nearVal, Real farVal) {
        m_projective.setZero();
        m_projective(0, 0) = (2.0f * nearVal) / (right - left);
        m_projective(1, 1) = (2.0f * nearVal) / (top - bottom);
        m_projective(0, 2) = (right + left) / (right - left);
        m_projective(1, 2) = (top + bottom) / (top - bottom);
        m_projective(2, 2) = -(farVal + nearVal) / (farVal - nearVal);
        m_projective(3, 2) = -1.0f;
        m_projective(2, 3) = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    };

    void setLookAt(const Vector3r &origin, const Vector3r &target, const Vector3r &up) {
        Vector3r f = (target - origin).normalized();
        Vector3r s = f.cross(up).normalized();
        Vector3r u = s.cross(f);

        m_front  = -f;
        m_center = origin;
        m_right  = s;
        m_up     = u;

        m_view       = Matrix4r::Identity();
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

    void setOrtho(Real left, Real right, Real bottom,
                  Real top, Real nearVal, Real farVal) {
        m_projective.setZero();
        m_projective(0, 0) = 2.0f / (right - left);
        m_projective(1, 1) = 2.0f / (top - bottom);
        m_projective(2, 2) = -2.0f / (farVal - nearVal);
        m_projective(0, 3) = -(right + left) / (right - left);
        m_projective(1, 3) = -(top + bottom) / (top - bottom);
        m_projective(2, 3) = -(farVal + nearVal) / (farVal - nearVal);
    }
    const Matrix4r &getViewMatrix() const {
        return m_view;
    }
    const Matrix4r &getProjectiveMatrix() const {
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

    void setOurColor(const Vector4r &color) {
        glUseProgram(*this);
#ifdef SINGLE_PRECISION
        glUniform4f(uOurColor, color(0), color(1), color(2), color(3));
#else
        glUniform4d(uOurColor, color(0), color(1), color(2), color(3));
#endif
        glUseProgram(0);
    }

    void setView(const Matrix4r &view) {
        glUseProgram(*this);
#ifdef SINGLE_PRECISION
        glUniformMatrix4fv(uView, 1, GL_FALSE, view.data());
#else
        glUniformMatrix4dv(uView, 1, GL_FALSE, view.data());
#endif
        glUseProgram(0);
    }

    void setProjective(const Matrix4r &proj) {
        glUseProgram(*this);
#ifdef SINGLE_PRECISION
        glUniformMatrix4fv(uProj, 1, GL_FALSE, proj.data());
#else
        glUniformMatrix4dv(uProj, 1, GL_FALSE, proj.data());
#endif
        glUseProgram(0);
    }
};