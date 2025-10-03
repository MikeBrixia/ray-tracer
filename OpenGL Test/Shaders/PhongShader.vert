#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

out vec3 frag_world_position;
out vec3 frag_normal;

layout(std140, binding = 0) uniform matrices
{
    mat4 model;
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 1) uniform screen_info
{
    vec2 resolution;
};

void main() {
    // Transform position to world position.
    frag_world_position = vec3(model * vec4(inPosition, 1.0f));
    // transform normals to world space.
    frag_normal = normalize(mat3(transpose(inverse(model))) * inNormal);
    
    gl_Position = projection * view * vec4(frag_world_position, 1.0f);
}