#version 450

#define PI 3.1415926538

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
    // Near = -1 in NDC, Far = 1
    vec2 ndc = (gl_FragCoord.xy / resolution) * 2.0 - 1.0;
    vec4 near_clip = vec4(ndc, -1.0, 1.0);
    vec4 near_world = inverse(projection * view) * near_clip;
    near_world /= near_world.w;
    
    // 3D position of the center of our sphere.
    const vec3 center = vec3(model * vec4(0, 0, 0, 1));
    // camera position in world space
    vec3 eye = vec3(inverse(view)[3]);
    // ray direction in world space
    vec3 ray_dir = normalize(near_world.xyz - eye);
    
    // Direction from eye to center.
    vec3 e_c = eye - center;
    // Prepare parameters A, B AND C to solve quadratic equation.
    // The equation will be solved in object space and then transformed,
    // later, to world space.
    float a = dot(ray_dir, ray_dir);
    float b = 2 * dot(ray_dir, e_c);
    float c = dot(e_c, e_c) - 0.25*0.25;
    
    // compute the discriminant of the quadric equation.
    float discriminant = b*b - 4*a*c;
    // Equation has solutions iff discriminant in non-negative.
    if(discriminant >= 0)
    {
        // Find t1 and t2 solutions of the equation.
        float discriminant_sqrt = sqrt(discriminant);
        float t1 = (-b - discriminant_sqrt) / (2.0 * a);
        float t2 = (-b + discriminant_sqrt) / (2.0 * a);
        // Determine the first intersection point of the rays.
        float t = (t1 > 0.0) ? t1 : ((t2 > 0.0) ? t2 : -1.0);
        
        // if intersection point is not behind the camera, apply phong shading.
        if(t > 0.0)
        {
            float light_dir_intensity = light_dir.w;
            float normal_irradiance = directional_light_color.w;
            float ambient_light = ambient_light_color.w;

            // Intersection point in world space
            vec3 hit_point = eye + ray_dir * t;
            // normal of the hit point in world space
            vec3 normal = normalize(hit_point - center);
            
            vec3 view_direction = normalize(eye - hit_point);
            vec3 reflection = reflect(-light_dir.xyz, normal);
            
            float light_normal_dot = max(dot(normal, light_dir.xyz), 0);
            
            float irradiance = normal_irradiance * light_normal_dot;
            float diffuse =  albedo/PI * irradiance;
            float specular = specularity * pow(max(dot(view_direction, reflection), 0), shininess);
            
            float R = (directional_light_color.x * (diffuse + specular + ambient_light));
            float G = (directional_light_color.y * (specular));
            float B = (directional_light_color.z * (specular));
            
            outColor = vec4(R, G, B, 1.0) * light_dir_intensity;
        }
    }
}
