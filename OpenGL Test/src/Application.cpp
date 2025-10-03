
#include "Application.h"
#include <iostream>
#include <stdexcept>
#include <ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Utils/FileHelper.h"
#include "../Utils/GeometryShapeBuilder.h"
#include "../Utils/GraphicsUtils.h"

Application::Application()
{
}

void Application::run()
{
    // Initialize application.
    init_window();
    init_shaders();
    create_scene();
    init_buffers();

    // Begin the game loop.
    game_loop();
}

void Application::init_window()
{
    /* Initialize the library */
    if (!glfwInit())
        return;
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("Failed to initialize glad!");

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Application::init_shaders()
{
    raytracing_shader_program = glCreateProgram();

    // If true, load ray tracing shaders
    std::string raytracing_vertex_data = FileHelper::load_shader_data("Shaders/RayTracer.vert");
    std::string raytracing_fragment_data = FileHelper::load_shader_data("Shaders/RayTracer.frag");

    GLuint raytracing_vertex_shader = GraphicsUtils::compile_shader(GL_VERTEX_SHADER, raytracing_vertex_data.c_str());
    GLuint raytracing_fragment_shader = GraphicsUtils::compile_shader(GL_FRAGMENT_SHADER, raytracing_fragment_data.c_str());

    glAttachShader(raytracing_shader_program, raytracing_vertex_shader);
    glAttachShader(raytracing_shader_program, raytracing_fragment_shader);
    glLinkProgram(raytracing_shader_program);

    // Get rid of the shader, we no longer need them.
    glDeleteShader(raytracing_vertex_shader);
    glDeleteShader(raytracing_fragment_shader);
    
    // Create the openGL program for shaders.
    phong_shader_program = glCreateProgram();
    
    // Otherwise,load default shaders.
    std::string phong_vertex_data = FileHelper::load_shader_data("Shaders/PhongShader.vert");
    std::string phong_fragment_data = FileHelper::load_shader_data("Shaders/PhongShader.frag");
    
    // Compile all the loaded shaders.
    GLuint phong_vertex_shader = GraphicsUtils::compile_shader(GL_VERTEX_SHADER, phong_vertex_data.c_str());
    GLuint phong_fragment_shader = GraphicsUtils::compile_shader(GL_FRAGMENT_SHADER, phong_fragment_data.c_str());
    
    // Attach shaders to the program and link it.
    glAttachShader(phong_shader_program, phong_vertex_shader);
    glAttachShader(phong_shader_program, phong_fragment_shader);
    glLinkProgram(phong_shader_program);
    
    // Get rid of the shader, we no longer need them.
    glDeleteShader(phong_vertex_shader);
    glDeleteShader(phong_fragment_shader);
}

void Application::init_buffers()
{
    GLsizei mesh_num = (GLsizei) scene.meshes.size();

    // Allocate vertex objs and buffers memory.
    VAO.resize(mesh_num);
    VBO.resize(mesh_num);
    EBO.resize(mesh_num);
    
    // Generate VAO, VBO AND EBO
    glGenVertexArrays(mesh_num, VAO.data());
    glGenBuffers(mesh_num, VBO.data());
    glGenBuffers(mesh_num, EBO.data());
    
    for (uint32_t i = 0; i < mesh_num; i++)
    {
        Mesh mesh = scene.meshes[i];
        GLuint vao_instance = VAO[i];
        GLuint vbo_instance = VBO[i];
        GLuint ebo_instance = EBO[i];
        
        glBindVertexArray(vao_instance);
        create_vertex_buffer_for_mesh(mesh, vbo_instance, ebo_instance);
    }
    create_uniform_buffer( scene.meshes[1]);
}

void Application::game_loop()
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        process_input();
        update();
        render();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    terminate();
}

void Application::create_vertex_buffer_for_mesh(Mesh mesh, GLuint vbo, GLuint ebo)
{
    // Bind vertices data inside the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const size_t VBO_size = mesh.vertices.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) VBO_size, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data());
    
    // Bind sphere indices inside the element buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    const size_t EBO_size = mesh.indices.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) EBO_size, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data());

    // Unbind the vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Application::create_uniform_buffer(Mesh mesh)
{
    glm::vec2 res = glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    glGenBuffers(1, &ubo_screen_info);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_screen_info);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &res, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_screen_info);

    Camera camera = scene.cameras[0];

    constexpr glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 projection = camera.get_projection_matrix();
    const glm::mat4 view = camera.get_view_matrix();
    
    glGenBuffers(1, &ubo_transform);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_transform);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_transform);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &model);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &view);

    DirectionalLight directional_light = scene.directional_lights[0];
    AmbientLight ambient_light = scene.ambient_light;

    // Light direction and intensity.
    glm::vec4 light_dir_and_intensity {directional_light.direction, directional_light.intensity};
    glm::vec4 directional_light_color_and_normal_irradiance {directional_light.color, directional_light.normal_irradiance};
    glm::vec4 amb_light_color_and_intensity {ambient_light.color, ambient_light.intensity};
    
    glGenBuffers(1, &ubo_light);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_light);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo_light);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light_dir_and_intensity);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &directional_light_color_and_normal_irradiance);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec4), &amb_light_color_and_intensity);
    
    glGenBuffers(1, &ubo_material);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_material);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 4, &mesh.material, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 4, ubo_material);
}

void Application::create_scene()
{
    const float aspect_ratio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
    Camera camera {aspect_ratio, 90.f};
    camera.transform.set_position(glm::vec3(0.0f, 0.0f, 3.0f));
    scene.model = glm::mat4(1.0f);
    scene.main_camera = &camera;
    scene.cameras.push_back(camera);

    // Create the left-side sphere.
    Mesh sphere = GeometryShapeBuilder::create_sphere(.25, 160, 160,
        {-0.6,0, 0});
    sphere.material.shininess = 10;
    sphere.material.albedo = 1.0;
    sphere.material.specularity  = 1.0f;
    sphere.material.emissive = 0.0f;
    scene.meshes.push_back(sphere);

    // Create the right-side sphere.
    Mesh sphere1 = Mesh(sphere);
    sphere1.transform.set_scale(.25);
    sphere1.transform.set_position(glm::vec3(.6, 0, 0));
    scene.meshes.push_back(sphere1);
    
    DirectionalLight directional_light;
    directional_light.color = glm::vec3(1.0, 1.0, 1.0);
    directional_light.direction = normalize(glm::vec3(1.0, 1.0, 1.0));
    directional_light.intensity = 1.0;
    directional_light.normal_irradiance = 1.0f;

    AmbientLight ambient_light;
    ambient_light.color = glm::vec3(1.0, 1.0, 1.0);
    ambient_light.intensity = 0.1f;

    scene.directional_lights.push_back(directional_light);
    scene.ambient_light = ambient_light;
}

void Application::process_input()
{
    Camera* camera = &scene.cameras[0];

    glm::vec3 camera_position = camera->transform.get_local_position();
    glm::vec3 camera_front = camera->get_forward();
    glm::vec3 camera_up = camera->get_up();
    glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

    float current_frame = (float) glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    float camera_speed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    camera->transform.set_position(camera_position);
    
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_transform);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &view);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Application::update()
{
}

void Application::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_raytracing_scene();
    draw_rasterized_scene();
}

void Application::draw_raytracing_scene()
{
    Mesh sphere = scene.meshes[0];
    
    glUseProgram(raytracing_shader_program);
    glDisable(GL_DEPTH_TEST);
        
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_transform);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
        &sphere.transform.get_model_matrix());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    glEnable(GL_DEPTH_TEST);
    // Unbind buffers.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Application::draw_rasterized_scene()
{
    glUseProgram(phong_shader_program);
    Mesh sphere = scene.meshes[1];
    
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    
    // Send vertex attributes to the vertex shader.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // use the second sphere transform inside the UBO.
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_transform);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
        &sphere.transform.get_model_matrix());
    glDrawElements(GL_TRIANGLES, (GLsizeiptr) sphere.indices.size(), GL_UNSIGNED_INT, nullptr);
    // Unbind buffers.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Application::terminate()
{
    glDeleteVertexArrays(VAO.size(), VAO.data());
    glDeleteBuffers(VBO.size(), VBO.data());
    glDeleteBuffers(EBO.size(), EBO.data());
    glfwTerminate();
}


