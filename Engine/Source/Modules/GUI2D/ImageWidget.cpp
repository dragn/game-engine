#include "Modules/VhModules_PCH.hpp"
#include "ImageWidget.hpp"

#include "Modules/Renderer2D/MRenderer2D.hpp"

using namespace vh;
using namespace gui;

gui::ImageWidget::ImageWidget(const char* filename)
    : mSurf(nullptr)
{
    mSurf = IMG_Load(filename);
    if (mSurf == nullptr)
    {
        LOG(WARNING) << "Could not load image: " << filename;
    }
}

gui::ImageWidget::~ImageWidget()
{
    if (mSurf != nullptr)
    {
        SDL_FreeSurface(mSurf);
        mSurf = nullptr;
    }
}

gui::ImageWidget::ImageWidget()
    : mSurf(nullptr)
{
}

gui::ImageWidget::ImageWidget(SDL_Surface* surf)
    : mSurf(surf)
{
}

void gui::ImageWidget::Draw(int32_t x, int32_t y)
{
    if (mSurf == nullptr) return;

    MRenderer2D* render = App::Get<MRenderer2D>();
    CHECK(render);

    render->DrawImage(mSurf, x, y, mSurf->w, mSurf->h);
}

void gui::ImageWidget::UpdateSize()
{
    if (mSurf == nullptr)
    {
        SetSize(0, 0);
    }
    else
    {
        SetSize(mSurf->w, mSurf->h);
    }
}
