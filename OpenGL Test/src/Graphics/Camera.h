#pragma once

#include <glm.hpp>
#include <fwd.hpp>
#include "Transform.h"

struct Camera
{
    Camera(float aspect_ratio, float fov);
    
private:
    glm::mat4 projection;
    glm::mat4 view;

public:
    Transform transform;
    
    void set_projection_matrix(glm::mat4 projection_matrix);
    void set_view_matrix(glm::mat4 view_matrix);
    
    glm::mat4 get_projection_matrix() const;
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_camera_matrix() const;

    glm::vec3 get_forward();
    glm::vec3 get_up();
    glm::vec3 get_right();
};
