#include "Mesh.h"

#include <ext/matrix_transform.hpp>

Mesh::Mesh(): material()
{
}

Mesh::Mesh(const Mesh& mesh)
{
    this->vertices = mesh.vertices;
    this->indices = mesh.indices;
    this->material = mesh.material;
    this->transform = mesh.transform;
}
