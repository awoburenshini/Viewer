#pragma onces
#include <common.h>

struct CameraParameters
{
    Real zoom = 1.0f, viewAngle = 45.0f;

    Real dnear = 0.05f, dfar = 100.0f;

    Vector3r eye = Vector3r(0.0f, 0.0f, 3.0f); // Camera position in 3D (world space)

    Vector3r center = Vector3r(0.0f, 0.0f, 0.0f); // Point that the camera is looking at (world space)

    Vector3r up = Vector3r(0.0f, 1.0f, 0.0f); // Orientation of the camera, (world space)

    Real modelZoom = 1.0f;
};

// https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
class ArcballCamera
{
public:
    ArcballCamera()
    {
    }

    CameraParameters CamParam;

    // input NDC pos
    void MouseRotate(const Vector2r &m_pos)
    {
        // Vector4r position(
        //     CamParam.eye.x(),
        //     CamParam.eye.y(),
        //     CamParam.eye.z(),
        //     1.
        //     );

        // Vector4r pivot(
        //     CamParam.center.x(),
        //     CamParam.center.y(),
        //     CamParam.center.z(),
        //     1.
        // );
        
        Real xAngle = (last.x() - m_pos.x()) * 2 * PI;
        Real yAngle = (last.y() - m_pos.y()) * PI;

        Vector3r CamDir = (CamParam.center - CamParam.eye).normalized();

        Real cosAngle = CamDir.dot(CamParam.up);

        Matrix3r rotationX = Eigen::AngleAxis(xAngle, CamParam.up).matrix();
        

        return;
    }

private:
    Vector2r last;
};