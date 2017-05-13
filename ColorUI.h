#pragma once
#include "CUI.h"

enum class ConsoleColor { black=0,red=1,green=2,yellow=3,blue=4,purple=5,lightblue=6,deepgreen=6,white=7 };

class ColorSelection : public Selection
{
public:
    ConsoleColor frontColor,backColor;

    ColorSelection();
    virtual void drawText() override;
    virtual void drawInfo() override;

    void onActivate();
    void onDeActivate();
private:
    bool _activate;
};

class ColorPage : public Page
{
public:
    virtual void draw() override;

    virtual int onActive(int id) override;
    virtual void onActivate(int id);
    virtual void onDeactivate(int id);
};

class ColorFrame : public Frame
{
public:
    void run(); /// overwritten
};
