#pragma once

#pragma once

#include <fwd.hpp>
#include <vec3.hpp>
#include <vector>

struct Mesh;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    
    Vertex(glm::vec3 position): normal(), color()
    {
        this->position = position;
    }

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color)
    {
        this->position = position;
        this->normal = normal;
        this->color = color;
    }

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::mat4* model): color(color)
    {
        this->position = position;
        this->normal = normal;
    }
};

struct Sphere
{
    std::vector<Vertex> vertices;
    float radius;
    glm::vec3 center;
};

class GeometryShapeBuilder
{
    
public:
    static Mesh create_sphere(float radius, uint32_t latitude, uint32_t longitude, glm::vec3 center);
};
