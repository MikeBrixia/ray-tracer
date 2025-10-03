#pragma once
#include <glad/glad.h>

class GraphicsUtils
{
    
public:

    static GLuint compile_shader(GLenum type, const char* source);
};
