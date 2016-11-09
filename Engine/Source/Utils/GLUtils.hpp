#pragma once

#include "Common.hpp"

#include <string>

namespace vh {

namespace Utils {

struct CubeMap {
    std::string pos_x;
    std::string neg_x;
    std::string pos_y;
    std::string neg_y;
    std::string pos_z;
    std::string neg_z;
};

GLuint BufferData(GLsizeiptr size, const GLfloat *data);
GLuint BufferElementArray(GLsizeiptr size, const GLuint *data);
GLuint BufferTexture2D(SDL_Surface*);
SDL_Surface *LoadImage(const std::string &filename);
GLuint LoadCubeMapTexture(const CubeMap &skyBox);

void PutUniformMat4(GLuint programID, const std::string &name, M4 &data);
void PutUniformVec3(GLuint programID, const std::string &name, V3 &data);
void PutUniformVec4(GLuint programID, const std::string &name, V4 &data);
void PutUniformFloat(GLuint programID, const std::string &name, float data);

// Get shader program specified by key (compiles on first access)
GLuint GetShaderProgram(const std::string &key);

} // namespace Utils

} // namespace vh
