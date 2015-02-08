#pragma once

#include "../Renderer/GLUtils.hpp"
#include "RenderingObject.hpp"

namespace Vhagar {

  class SkyBox : public RenderingObject {

    public:
      SkyBox(CubeMap cubeMap) : cubeMap(cubeMap) {};

    private:

      CubeMap cubeMap;

      GLBufferInfo glInfo;
      GLuint skyTexture;
      GLuint programID;

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

      virtual void render(glm::mat4 projection, glm::mat4 view, LightSource lightSource) {} // nothing to do here
  };

}
