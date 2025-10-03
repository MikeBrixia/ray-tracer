
#include "Camera.h"
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>

Camera::Camera(float aspect_ratio, float fov)
{
    projection = glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.f);
    view = glm::lookAt(
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
}

void Camera::set_projection_matrix(glm::mat4 projection_matrix)
{
    this->projection = projection_matrix;
}

void Camera::set_view_matrix(glm::mat4 view_matrix)
{
    this->view = view_matrix;
}

glm::mat4 Camera::get_projection_matrix() const
{
    return projection;
}

glm::mat4 Camera::get_view_matrix() const
{
    return view;
}

inline glm::mat4 Camera::get_camera_matrix() const
{
    return projection * view;
}

glm::vec3 Camera::get_forward()
{
    return glm::vec4(0, 0, -1, 0);
}

glm::vec3 Camera::get_up()
{
    return glm::vec4(0, 1, 0, 0);
}

glm::vec3 Camera::get_right()
{
    return glm::vec4(1, 0, 0, 0);
}
