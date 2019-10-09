#pragma once

#include "Modules/Console/ConsoleEngine.hpp"
#include "Modules/Console/ConsoleCommands.hpp"
#include "Modules/Renderer/Renderable/Overlay.hpp"
#include "Modules/Renderer/Renderer.hpp"
#include "Modules/standard.hpp"

namespace vh
{

// number of lines in console output
const size_t NUM_LINES = 15;

// font size for console output
const size_t FONT_SIZE = 12;

// spacing between console output
const size_t LINE_SPACE = 6;

// number of history records
const size_t NUM_HISTORY = 10;

typedef void(*CmdHandler)(const std::vector<std::string>&);

class Console : public Module, public google::LogSink
{
    VH_MODULE(Console);

public:
    Console()
        : Module(TickFrequency::NEVER)
        , mShowConsole(false)
        , mFont(nullptr)
        , mMessages(NUM_LINES)
        , mMsgIdx(0)
        , mHistory(NUM_HISTORY)
        , mHistoryIdx(0)
        , mCurrHistoryIdx(0)
        , mSurf(nullptr)
        , mEngine(nullptr)
        , mRenderer(nullptr)
    {
    }

    /* Component interface */
    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;

    virtual void HandleEvent(SDL_Event* event);

    void Register(const std::string& name, CmdHandler handler);

    void PrintMessage(const std::string& msg);
    void ToggleConsole();

    bool IsShown() const
    {
        return mShowConsole;
    }

private:
    bool mShowConsole;
    TTF_Font* mFont;

    std::string mInput;

    cs::CritSection mMsgCS;
    std::vector<std::string> mMessages;
    size_t mMsgIdx;

    std::vector<std::string> mHistory;
    size_t mHistoryIdx;
    size_t mCurrHistoryIdx;

    SDL_Surface* mSurf;
    Overlay mOverlay;

    ConsoleEngine* mEngine;
    Renderer* mRenderer;

    void _Redraw();

public:
    virtual void send(google::LogSeverity severity, const char* full_filename,
        const char* base_filename, int line, const struct ::tm* tm_time,
        const char* message, size_t message_len) override;
};

} // namespace vh
