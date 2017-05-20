#pragma once

#include <vector>
#include <forward_list>
#include "Renderable/Renderable.hpp"
#include "Lights.hpp"
#include "App/Component.hpp"

namespace vh {

const static size_t MAX_POINT_LIGHTS = 10;
const static size_t MAX_AMBIENT_LIGHTS = 1;
const static size_t MAX_DIRECTION_LIGHTS = 10;

/**
 * Renderer options
 */
struct RendererOptions {
    const char *resourceRoot = ".";
    size_t screenWidth = 1024;
    size_t screenHeight = 768;
    bool borderless = true;

    enum
    {
        AA_OFF = 0, // Antialiasing turned off
        AA_2X,      // MSAAx2
        AA_4X       // MSAAx4
    } antialias = AA_OFF;

    enum
    {
        MON_FIRST = 0,
        MON_SECOND
    } monitor = MON_FIRST;
};

namespace RenderFlags
{
enum Type
{
    DRAW_WIREFRAMES = 1,
    DRAW_LABELS = 1 << 1,
};
}

/**
 * The Renderer
 **/
class Renderer : public vh::Component {
    COMPONENT_NAME(Renderer);

public:

    Renderer() : Component(eTickFrequency::NORMAL) {}
    Renderer(const RendererOptions& opts) : Component(eTickFrequency::NORMAL), mOptions(opts) {}
    virtual ~Renderer() {}

    virtual void TickInit(uint32_t delta) override;
    virtual void TickClose(uint32_t delta) override;

    virtual void StartFrame() override;
    virtual void EndFrame() override;

    /**
     * Set up light sources
     */
    void AddLight(const PointLight* light);
    void RemoveLight(const PointLight* light);
    const std::vector<const PointLight*> GetPointLights() const;
;
    /**
     * Sets up a projection matrix
     */
    void SetProjection(glm::mat4 projection) { mProjection = projection; };
    const M4& GetProjection() const { return mProjection; }

    /**
     * Sets up view matrix (camera)
     */
    void SetView(glm::mat4 view) { mView = view; };
    const M4& GetView() const { return mView; }

    const RendererOptions& GetOptions() const
    {
        return mOptions;
    }

    uint8_t GetFlags() const
    {
        return mFlags;
    }

    bool IsOn(RenderFlags::Type flag) const
    {
        return mFlags && flag;
    }

    void Toggle(RenderFlags::Type flag)
    {
        mFlags ^= flag;
    }

private:
    RendererOptions mOptions;

    uint8_t mFlags = 0;

    SDL_GLContext mGLContext;
    SDL_Window *mWindow;
    uint32_t mWindowID;

    std::vector<const PointLight*> mLights;

    glm::mat4 mProjection;
    glm::mat4 mView;
};

}
