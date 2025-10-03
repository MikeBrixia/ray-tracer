#pragma once

#include <glm.hpp>
#include <vector>
#include "Material.h"
#include "Transform.h"
#include "../../Utils/GeometryShapeBuilder.h"

struct Mesh
{
    Mesh();
    Mesh(const Mesh& mesh);

    Transform transform;
    Material material;
    
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
