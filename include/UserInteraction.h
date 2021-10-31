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
    int grabPoint(const Vector2r& p)const;
private:
    Camera* camera_ptr;
    MatrixXr* V_ptr;
    MatrixXu* F_ptr;
};
