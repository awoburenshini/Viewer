#pragma once

#include <Viewer.h>
#include <extensionViewer.h>
#include <memory>

class UserInteraction {
public:
    UserInteraction(MatrixXr* pV, MatrixXu* pF, Camera* pCamera);
    ~UserInteraction();
    
    // input NDC coordinates, return grabbed Vertex index
    // return -1 if nothing grabbed
    void grabPoint(const Vector2r& p);

    // input NDC coordinates, return grabbed Vertex index
    // return -1 if nothing grabbed
    void movePoint(const Vector2r& p);


private:
    int m_grabed;
    Camera* camera_ptr;
    MatrixXr* V_ptr;
    MatrixXu* F_ptr;
};
