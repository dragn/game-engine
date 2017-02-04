#include "Common.hpp"

#include "Widget.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using vh::App;
using vh::Renderer2D;

gui::Widget::~Widget()
{
    for (Widget* child : mChildren)
    {
        if (child) delete child;
    }
}

void gui::Widget::SetPos(int32_t x, int32_t y, eAnchor::Type anchor /*= eAnchor::TopLeft*/)
{
    mPosX = x;
    mPosY = y;
    mAnchor = anchor;
    mDirty = true;
}

void gui::Widget::GetSize(int32_t& width, int32_t& height) const
{
    width = mWidth;
    height = mHeight;
}

void gui::Widget::SetSize(int32_t width, int32_t height)
{
    mWidth = width;
    mHeight = height;
    mDirty = true;
}

void gui::Widget::Draw(int32_t x, int32_t y)
{
    // supposed to be implemented in child classes
}

void gui::Widget::AddChild(Widget* widget)
{
    widget->mParent = this;
    mChildren.push_back(widget);
}

bool gui::Widget::IsPointInside(int32_t x, int32_t y)
{
    return x >= mAbsPosX && x <= mAbsPosX + mWidth &&
        y >= mAbsPosY && y <= mAbsPosY + mHeight;
}

void gui::Widget::OnClick(int32_t x, int32_t y)
{
    // default behavior: propagate event to children
    for (Widget* child : mChildren)
    {
        if (child->IsPointInside(x, y))
        {
            child->OnClick(x, y);
        }
    }
}

void gui::Widget::SetDirty()
{
    mDirty = true;
    if (mParent) mParent->SetDirty();
}

void gui::Widget::Draw(Widget* parent)
{
    if (mDirty)
    {
        /* calculate screen-relative coordinates */
        CalcAbsPos(parent);

        /* draw this widget */
        Draw(mAbsPosX, mAbsPosY);

        /* draw all children */
        for (Widget* child : mChildren)
        {
            child->Draw(this);
        }

        /* reset dirty flag */
        mDirty = false;
    }
}

void gui::Widget::CalcAbsPos(Widget* parent)
{
    UpdateSize();

    /* base point */
    int32_t posX = 0;
    int32_t posY = 0;

    if (parent)
    {
        /* get base point from parent */
        if (mPosX == ePos::Center) posX = parent->mAbsPosX + parent->mWidth / 2;
        else if (mPosX == ePos::Right) posX = parent->mAbsPosX + parent->mWidth;
        else posX = parent->mAbsPosX;

        if (mPosY == ePos::Center) posY = parent->mAbsPosY + parent->mHeight / 2;
        else if (mPosY == ePos::Bottom) posY = parent->mAbsPosY + parent->mHeight;
        else posY = parent->mAbsPosY;
    }

    int32_t relPosX = mPosX < 0 ? 0 : mPosX;
    int32_t relPosY = mPosY < 0 ? 0 : mPosY;

    /* calc screen-relative coordinates based on anchor position */
    switch (mAnchor)
    {
    case gui::eAnchor::TopLeft:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY;
        break;
    case gui::eAnchor::TopCenter:
        mAbsPosX = posX + relPosX - mWidth / 2;
        mAbsPosY = posY + relPosY;
        break;
    case gui::eAnchor::TopRight:
        mAbsPosX = posX + relPosX - mWidth;
        mAbsPosY = posY + relPosY;
        break;
    case gui::eAnchor::CenterLeft:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY - mHeight / 2;
        break;
    case gui::eAnchor::CenterCenter:
        mAbsPosX = posX + relPosX - mWidth / 2;
        mAbsPosY = posY + relPosY - mHeight / 2;
        break;
    case gui::eAnchor::CenterRight:
        mAbsPosX = posX + relPosX - mWidth;
        mAbsPosY = posY + relPosY - mHeight / 2;
        break;
    case gui::eAnchor::BottomLeft:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY - mHeight;
        break;
    case gui::eAnchor::BottomCenter:
        mAbsPosX = posX + relPosX - mWidth / 2;
        mAbsPosY = posY + relPosY - mHeight;
        break;
    case gui::eAnchor::BottomRight:
        mAbsPosX = posX + relPosX - mWidth;
        mAbsPosY = posY + relPosY - mHeight;
        break;
    default:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY;
        break;
    }
}