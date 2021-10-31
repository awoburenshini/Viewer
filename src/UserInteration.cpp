#include <UserInteraction.h>

UserInteraction::UserInteraction(MatrixXr *pV, MatrixXu *pF, Camera *pCamera) :
    camera_ptr(pCamera), V_ptr(pV), F_ptr(pF) {
}

UserInteraction::~UserInteraction() {
    //alert don't delete pointer camera_ptr,V_ptr,F_ptr
}

int UserInteraction::grabPoint(const Vector2r &p) const {
    // todo
    return -1;
}
