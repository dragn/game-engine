#pragma once

#include "Modules/GUI2D/View.hpp"
#include "Modules/Renderer2D/MRenderer2D.hpp"
#include "Modules/ResourceSystem/ResourceTypes/RFont.hpp"
#include "Modules/standard.hpp"

namespace gui
{

    enum class eTransitionMode
    {
        NONE,
        FADE
    };

    struct GUI2DOptions
    {
        const char* fontPath = "<font path is not set>";
        int fontSize = 0;
        const char* hdr1FontPath = "<font path is not set>";
        int hdr1FontSize = 0;
        const char* hdr2FontPath = "<font path is not set>";
        int hdr2FontSize = 0;
    };

    /*
          //       \\
        MMMM GUI2D MMMM
          \\       //

        User-interface engine based on MRenderer2D
    */
    class MGUI2D : public vh::Module
    {
        VH_MODULE(MGUI2D);

    public:
        MGUI2D(const GUI2DOptions& opts)
            : vh::Module(vh::TickFrequency::NORMAL)
            , mView(nullptr)
            , mNextView(nullptr)
            , mNextModalView(nullptr)
            , mScale(1)
            , mOptions(opts)
            , mFocused(nullptr)
            , mArrowCursor(nullptr)
            , mBeamCursor(nullptr)
            , mHandCursor(nullptr)
            , mGoBack(false)
            , mGoBackToMain(false)
            , mTransition(-1)
        {}

        virtual vh::Ret TickInit(uint32_t delta) override;
        virtual vh::Ret TickRun(uint32_t delta) override;
        virtual vh::Ret TickClose(uint32_t delta) override;

        void SetView(View* view, bool withFade = true);
        View* GetView() const { return mView; }

        void SetModalView(View* view, eTransitionMode transition = eTransitionMode::FADE);
        View* GetModalView() const { return mModalViewStack.back(); }

        void Back(eTransitionMode mode = eTransitionMode::FADE); // go back to previous view in stack
        void BackToMain(eTransitionMode mode = eTransitionMode::FADE); // clear the view stack and go back to main view

        virtual void HandleEvent(SDL_Event* event) override;

        TTF_Font* GetFont() const;
        TTF_Font* GetHdr1Font() const;
        TTF_Font* GetHdr2Font() const;

        /* Calculate text size rendered with default font */
        void CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight);

        void SetFocus(Widget* widget);

        SDL_Cursor* GetArrowCursor();
        SDL_Cursor* GetHandCursor();
        SDL_Cursor* GetBeamCursor();

        template<typename T, typename... Args> T* MakeModal(eTransitionMode mode, Args... args)
        {
            T* view = new T(args...);
            SetModalView(view, mode);
            return view;
        }

    private:
        View* mView;
        std::list<View*> mModalViewStack;

        View* mNextView;
        View* mNextModalView;
        uint32_t mScale;

        GUI2DOptions mOptions;

        Widget* mFocused;

        SDL_Cursor* mArrowCursor;
        SDL_Cursor* mBeamCursor;
        SDL_Cursor* mHandCursor;

        bool mGoBack;
        bool mGoBackToMain;

        void FocusedDestroyed(Widget* widget);

        int16_t mTransition;

        vh::MRenderer2D* mRenderer;

        void ApplyTransition();
        bool WaitForTransition();
        bool IsInTransition();
        void StartTransition();
    };

} // namespace gui
