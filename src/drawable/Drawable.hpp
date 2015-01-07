#pragma once

#include "../core/Object.hpp"
#include "../core/Common.hpp"

class Drawable : public Object {

  friend class GL3Renderer;

  protected:
  GLsizeiptr _vertexDataSize = 0;
  uptr<GLfloat[]> _vertexData;

  GLsizeiptr _aColorDataSize = 0;
  uptr<GLfloat[]> _aColorData;

  GLsizeiptr _dColorDataSize = 0;
  uptr<GLfloat[]> _dColorData;

  GLsizeiptr _sColorDataSize = 0;
  uptr<GLfloat[]> _sColorData;

  GLsizeiptr _normalDataSize = 0;
  uptr<GLfloat[]> _normalData;

  GLsizei _indexDataSize = 0;
  uptr<GLuint[]> _indexData;

  GLsizei _materialIndexDataSize = 0;
  uptr<GLsizei[]> _materialIndexData;
};
