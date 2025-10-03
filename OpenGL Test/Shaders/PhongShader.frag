#version 450

#define PI 3.1415926538

in vec3 frag_world_position;
in vec3 frag_normal;

layout(location = 0) out vec4 outColor;

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

layout(std140, binding = 2) uniform directional_light
{
    // x, y, z are the direction, w is the intensity 
    // of the directional light.
    vec4 light_dir;

    // x, y, z are the color values of the directional
    // light, w is the normal irradiance.
    vec4 directional_light_color;

    // x, y, z are used for the ambient light color, 
    // w is used for the intensity.
    vec4 ambient_light_color;
};

layout(std140, binding = 4) uniform material_info
{
    float specularity;
    float shininess;
    float albedo;
    float emissive;
};

void main() {
    vec3 eye = vec3(inverse(view)[3]); // camera position
    
    float light_dir_intensity = light_dir.w;
    float normal_irradiance = directional_light_color.w;
    float ambient_light = ambient_light_color.w;
    
    vec3 normal = normalize(frag_normal);
    vec3 view_direction = normalize(eye - frag_world_position);
    vec3 reflectance = reflect(-light_dir.xyz, normal);

    float light_normal_dot = max(dot(normal, light_dir.xyz), 0);
    
    float irradiance = normal_irradiance * light_normal_dot;
    float diffuse =  albedo/PI * irradiance;
    float specular = specularity * pow(max(dot(view_direction, reflectance), 0), shininess);
    
    float R = (directional_light_color.x * (diffuse + ambient_light + specular));
    float G = (directional_light_color.y * (specular));
    float B = (directional_light_color.z * (specular));

    outColor = vec4(R, G, B, 1.0) * light_dir_intensity;
}