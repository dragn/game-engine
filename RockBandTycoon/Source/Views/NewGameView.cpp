#include "PCH.hpp"

#include "NewGameView.hpp"
#include "MenuView.hpp"

using namespace gui;

NewGameView::NewGameView(int slot)
{
    SetBackground("Assets/Images/menu_background.png");

    TextWidget* enterTxt = new TextWidget("Enter band name");
    enterTxt->SetPos(ePos::Center, 80, eAnchor::BottomCenter);
    AddWidget(enterTxt);

    TextFieldWidget* nameFld = new TextFieldWidget();
    nameFld->SetPos(ePos::Center, 82, eAnchor::TopCenter);
    nameFld->SetSize(120, 20);
    AddWidget(nameFld);

    ButtonWidget* backBtn = new ButtonWidget("Back");
    backBtn->SetSize(40, 20);
    backBtn->SetPos(239, 124, eAnchor::TopRight);
    backBtn->SetOnClickHandler([] ()
    {
        GUI2D* gui = vh::App::Get<GUI2D>();
        gui->SetView(new MenuView());
    });
    AddWidget(backBtn);

    ButtonWidget* startBtn = new ButtonWidget("Start");
    startBtn->SetSize(40, 20);
    startBtn->SetPos(241, 124, eAnchor::TopLeft);
    AddWidget(startBtn);
}
