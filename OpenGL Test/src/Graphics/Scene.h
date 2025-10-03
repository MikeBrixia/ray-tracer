#pragma once

#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

struct Scene
{
    glm::mat4 model;
    
    std::vector<Camera> cameras;
    std::vector<DirectionalLight> directional_lights;
    std::vector<PointLight> point_lights;
    std::vector<Mesh> meshes;

    AmbientLight ambient_light;
    Camera* main_camera;
};
