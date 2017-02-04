#pragma once

#include "App/Component.hpp"
#include "View.hpp"
#include "SDL_ttf.h"

namespace gui
{

struct GUI2DOptions
{
    const char* fontPath = "<font path is not set>";
    int fontSize = 12;
};

class GUI2D : public vh::Component
{
    COMPONENT_NAME(GUI2D);

public:
    GUI2D(const GUI2DOptions& opts)
        : vh::Component(vh::eTickFrequency::NORMAL)
        , mActiveView(nullptr)
        , mFont(nullptr)
        , mOptions(opts)
    {}

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    void SetView(View* view);

    virtual void HandleEvent(SDL_Event* event);

    TTF_Font* GetFont() const { return mFont; }

    /* Calculate text size rendered with default font */
    void CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight);
    void DrawText(const char* text, int32_t x, int32_t y);

private:
    View* mActiveView;
    uint32_t mScale;

    TTF_Font* mFont;
    GUI2DOptions mOptions;
};

}