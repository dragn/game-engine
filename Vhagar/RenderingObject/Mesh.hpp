#pragma once

#include "../Common.hpp"
#include "RenderingObject.hpp"

namespace Vhagar {

  struct GLBufferInfo {
    /**
     * The number of vertex attribute arrays  
     */
    GLuint attribCount = 0;

    /**
     * Vertex attributes buffer id and size
     */
    GLuint attribBuffer = 0;
    GLsizei attribBufferSize = 0;

    /**
     * Index array buffer id and size
     */
    GLuint indexBuffer = 0;
    GLsizei indexBufferSize = 0;
  };

  class Mesh : public RenderingObject {

    friend class SkyBox;

    public:
      /**
       * Sets Model matrix for this object
       */
      void setModel(glm::mat4 _model) { model = _model; }


    protected:
      // FIXME fix inconsistent private member naming

      glm::mat4 model;

      GLsizei _attribSize;
      GLsizei _indexSize;

      uptr<GLfloat[]> _vertexData;
      uptr<GLfloat[]> _normalData;
      uptr<GLfloat[]> _aColorData;
      uptr<GLfloat[]> _dColorData;
      uptr<GLfloat[]> _sColorData;

      uptr<GLuint[]> _indexData;

      GLuint programID;
      GLBufferInfo glInfo;

    private:
      /**
       * Called by Renderer when this object is added to the rendered world.
       * Should alocate OpenGL buffers.
       */
      virtual void beforeRender();

      /**
       * Called by Renderer when this object is removed from rendered world.
       * Should deallocate any OpenGL buffers.
       */
      virtual void afterRender();

      /**
       * Handles actual rendering of this object (called from Renderer::render())
       */
      virtual void render(glm::mat4 projection, glm::mat4 view);
  };

  class ObjMesh : public Mesh {
    public:
      ObjMesh(const char *filename);
  };

}
