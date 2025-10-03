#pragma once

#include <vec3.hpp>

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float normal_irradiance;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct AmbientLight
{
    glm::vec3 color;
    float intensity;
};


