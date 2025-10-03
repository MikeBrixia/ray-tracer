#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Graphics/Scene.h"

class Application
{

public:

    Application();
    
    std::vector<GLuint> VAO;
    std::vector<GLuint> VBO;
    std::vector<GLuint> EBO;
    
    GLuint ubo_transform;
    GLuint ubo_light;
    GLuint ubo_material;
    GLuint ubo_screen_info;
    
    GLuint raytracing_shader_program;
    GLuint phong_shader_program;
   
    GLFWwindow* window;
    const uint16_t WINDOW_WIDTH = 1400;
    const uint16_t WINDOW_HEIGHT = 1400;
    
    Scene scene;

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    
    void run();
    void terminate();

    void init_window();
    void init_shaders();
    void init_buffers();
    void create_vertex_buffer_for_mesh(Mesh mesh, GLuint vbo, GLuint ebo);
    void create_uniform_buffer(Mesh mesh);
    void create_scene();
    
    void game_loop();
    void process_input();
    void update();
    void render();

    void draw_raytracing_scene();
    void draw_rasterized_scene();
};
