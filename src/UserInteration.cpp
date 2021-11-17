#include <UserInteraction.h>

UserInteraction::UserInteraction(MatrixXr *pV, MatrixXu *pF, Camera *pCamera) :
    camera_ptr(pCamera), V_ptr(pV), F_ptr(pF) {
}

UserInteraction::~UserInteraction() {
    //alert don't delete pointer camera_ptr,V_ptr,F_ptr
}

void UserInteraction::grabPoint(const Vector2r &p) {
    // todo
    int num_vertex = V_ptr->cols();
    float maxDis = -1.;
    unsigned int ind = 0;
    return;
    for(int i = 0; i < num_vertex; ++i){
        
    }
}

void UserInteraction::movePoint(const Vector2r& p){

}