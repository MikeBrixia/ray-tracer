#include "FileHelper.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>

std::vector<uint32_t> FileHelper::load_SPIRV_binary(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if(!file) throw std::runtime_error(filepath + "not found, unable to load SPRI-V file");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if(size % 4 != 0) throw std::runtime_error(filepath + "SPRI-V invalid file lenght");

    std::vector<uint32_t> buffer(size / 4);
    file.read(reinterpret_cast<char*>(buffer.data()), size);

    return buffer;
}

std::string FileHelper::load_shader_data(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file) throw std::runtime_error(filepath + " not found, unable to load shader file!");

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
