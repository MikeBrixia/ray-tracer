#pragma once
#include <string>
#include <vector>

class FileHelper
{
public:

    static std::vector<uint32_t> load_SPIRV_binary(const std::string& filepath);
    static std::string load_shader_data(const std::string& filepath);
};
