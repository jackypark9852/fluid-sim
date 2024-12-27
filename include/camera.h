#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// A perspective projection camera
class Camera
{
public:
    float fovy;
    float aspect;
    float nearClip;     // Near clip plane distance
    float farClip;      // Far clip plane distance

    //Computed attributes

    glm::vec3 eye,      // The position of the camera in world space
        target,         // The point in world space towards which the camera is pointing
        forward,        // The normalized vector from eye to ref. Is also known as the camera's "forward" vector.
        up,             // The normalized vector pointing upwards IN CAMERA SPACE. This vector is perpendicular to LOOK and RIGHT.
        right;          // The normalized vector pointing rightwards IN CAMERA SPACE. It is perpendicular to UP and LOOK.

public:
    Camera(unsigned int w, unsigned int h);

    void recomputeAspectRatio(unsigned int w, unsigned int h);
    
    /// <summary>
    /// Projection * View matrix
    /// </summary>
    /// <returns></returns>
    glm::mat4 getViewProj();
    glm::mat4 getView();
    glm::mat4 getProj();

    /// <summary>
    /// Rotates the camera's eye position about its target point by the 
    /// input degrees around the vector (0,1,0). 
    /// Also rotates the camera's forward, right, and up vectors 
    /// by the same amount.
    /// </summary>
    /// <param name="deg"> The degrees to rotate by. </param>
    void RotateAboutGlobalUp(float deg);
    /// <summary>
    /// Rotates the camera's eye position about its target point by the 
    /// input degrees around the camera's right vector.
    /// Also rotates the camera's forward and up vectors
    /// by the same amount.
    /// </summary>
    /// <param name="deg"> The degrees to rotate by. </param>
    void RotateAboutLocalRight(float deg);

    /// <summary>
    /// Translates the camera's eye position and target point along
    /// the camera's right vector by the input amount.
    /// </summary>
    /// <param name="amt"> Pan amount. </param>
    void PanAlongRight(float amt);
    /// <summary>
    /// Translates the camera's eye position and target point along
    /// the camera's up vector by the input amount.
    /// </summary>
    /// <param name="amt"> Pan amount. </param>
    void PanAlongUp(float amt);

    /// <summary>
    /// Translates the camera's eye position along its forward
    /// vector by the input amount.
    /// </summary>
    /// <param name="amt"> Zoom amount. </param>
    void Zoom(float amt);
};
