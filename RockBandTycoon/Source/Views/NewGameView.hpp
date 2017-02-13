#pragma once

#include "GUI2D/View.hpp"
#include "GUI2D/TextFieldWidget.hpp"

class NewGameView : public gui::View
{
public:
    NewGameView(int slot);

    void OnStart();

private:
    gui::TextFieldWidget* mNameFld;

    int mSlot;
};