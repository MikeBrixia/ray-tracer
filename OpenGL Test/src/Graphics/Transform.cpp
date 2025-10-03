
#include "Transform.h"
#include <ext/matrix_transform.hpp>

Transform::Transform(): rotation(), position(), scale(), model()
{
    this->translation_matrix = glm::mat4(1.0f);
    this->rotation_matrix = glm::mat4(1.0f);
    this->scale_matrix = glm::mat4(1.0f);
    update_model_matrix();
}

Transform::Transform(glm::mat4& translation, glm::mat4& rotation, glm::mat4& scale): rotation(), position(), scale(),
    model()
{
    this->translation_matrix = translation;
    this->rotation_matrix = rotation;
    this->scale_matrix = scale;
    update_model_matrix();
}

void Transform::set_position(glm::vec3 new_position)
{
    this->position = new_position;
    translation_matrix = glm::translate(glm::mat4(1.0f), new_position);
    update_model_matrix();
}

void Transform::set_rotation(glm::quat new_rotation)
{
    this->rotation = new_rotation;
    rotation_matrix = glm::mat4_cast(new_rotation);
    update_model_matrix();
}

void Transform::set_scale(glm::vec3 new_scale)
{
    this->scale = new_scale;
    scale_matrix = glm::scale(glm::mat4(1.0f), new_scale);
    update_model_matrix();
}

void Transform::set_scale(float new_scale)
{
    set_scale({new_scale, new_scale, new_scale});
}

glm::mat4& Transform::get_model_matrix()
{
    return model;  
}

glm::vec3 Transform::get_local_position() const
{
    return position; 
}

glm::vec3 Transform::get_world_position() const
{
    return model * glm::vec4(position, 0.f);
}

glm::vec3 Transform::get_scale() const
{
    return scale;
}

glm::quat Transform::get_rotation() const
{
    return rotation;
}

glm::quat Transform::get_world_rotation() const
{
    return rotation * glm::quat_cast(rotation_matrix);
}

void Transform::update_model_matrix()
{
    model = translation_matrix * rotation_matrix * scale_matrix; 
}
