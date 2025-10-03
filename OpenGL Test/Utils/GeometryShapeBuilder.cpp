
#include "GeometryShapeBuilder.h"
#include <ext/scalar_constants.hpp>
#include "../src/Graphics/Mesh.h"

Mesh GeometryShapeBuilder::create_sphere(float radius, uint32_t latitude, uint32_t longitude,
                                         glm::vec3 center)
{
    Mesh mesh {};
    mesh.transform.set_position(center);
    mesh.transform.set_scale(radius);
    
    constexpr auto PI = glm::pi<float>();
    float sector_step = 2.f * PI / (float) longitude;
    float stack_step = PI / (float) latitude;
    
    for(uint32_t i = 0; i <= latitude; ++i)
    {
        // Rho is the stack_angle
        float rho =  PI/2 - (float)i * stack_step;
        // Project sphere point on the 2D circle.
        float xz = cosf(rho);
        // The height of the sphere vertex
        float y = sinf(rho);
        
        for(uint32_t j = 0; j <= longitude; ++j)
        {
            // Theta is the angle of the sector.
            float theta = (float)j * sector_step;

            // Use projection to scale x, z coordinates of the Circle,
            // thus getting sphere x, z coordinates.
            float x = xz * cosf(theta); 
            float z = xz * sinf(theta);
            
            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 normal = glm::normalize(position);
            Vertex vertex {position, normal, {1, 1 ,1}};
            mesh.vertices.push_back(vertex);
        }
    }

    // Indici: 2 triangoli per quad
    const uint32_t stride = longitude + 1;
    for (uint32_t i = 0; i < latitude; ++i)
    {
        for (uint32_t j = 0; j < longitude; ++j)
        {
            uint32_t k1 = i * stride + j;
            uint32_t k2 = k1 + stride;

            // First triangle
            mesh.indices.push_back(k1);
            mesh.indices.push_back(k2);
            mesh.indices.push_back(k1 + 1);

            // Second triangle.
            mesh.indices.push_back(k1 + 1);
            mesh.indices.push_back(k2);
            mesh.indices.push_back(k2 + 1);
        }
    }
    
    return mesh;
}
