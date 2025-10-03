#pragma once

#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/quaternion.hpp>

struct Transform
{
    
private:
    glm::mat4 translation_matrix;
    glm::mat4 rotation_matrix;
    glm::mat4 scale_matrix;

    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 scale;

    glm::mat4 model;

public:
    Transform();
    Transform(glm::mat4& translation, glm::mat4& rotation, glm::mat4& scale);

    void set_position(glm::vec3 new_position);
    void set_rotation(glm::quat new_rotation);
    void set_scale(glm::vec3 new_scale);
    void set_scale(float new_scale);
    
    glm::mat4& get_model_matrix();
    glm::vec3 get_local_position() const;
    glm::vec3 get_world_position() const;
    glm::vec3 get_scale() const;
    glm::quat get_rotation() const;
    glm::quat get_world_rotation() const;
    
private:
    void update_model_matrix();
};
