#include "camera.h"

Camera::Camera(unsigned int w, unsigned int h)
    : fovy(55), aspect(w / (float)h), nearClip(0.1), farClip(1000),
    eye(0, 0, 10), target(0, 0, 0),
    forward(0, 0, -1), up(0, 1, 0), right(glm::cross(forward, up))
{}

void Camera::recomputeAspectRatio(unsigned int w, unsigned int h) {
    aspect = w / (float)h;
}


glm::mat4 Camera::getViewProj() {
    return getProj() * getView();
}

glm::mat4 Camera::getView() {
    return glm::lookAt(eye, target, up);
}

glm::mat4 Camera::getProj() {
    return glm::perspective(glm::radians(fovy), aspect, nearClip, farClip);
}


void Camera::RotateAboutGlobalUp(float deg) {
    float rad = glm::radians(deg);
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(1), rad, glm::vec3(0, 1, 0)));
    eye = rot * eye;
    forward = rot * forward;
    right = rot * right;
    up = rot * up;
}

void Camera::RotateAboutLocalRight(float deg) {
    float rad = glm::radians(deg);
    glm::mat3 rot = glm::mat3(glm::rotate(glm::mat4(1), rad, right));
    eye = rot * eye;
    forward = rot * forward;
    up = rot * up;
}

void Camera::Zoom(float amt) {
    glm::vec3 fTrans = forward * amt;
    eye += fTrans;
}

void Camera::PanAlongRight(float amt) {
    glm::vec3 rTrans = right * amt;
    eye += rTrans;
    target += rTrans;
}

void Camera::PanAlongUp(float amt) {
    glm::vec3 uTrans = up * amt;
    eye += uTrans;
    target += uTrans;
}