#pragma once
#include <string>
#include <list>
#include <stack>

enum class ConsoleColor { black=0,red=1,green=2,yellow=3,blue=4,purple=5,lightblue=6,deepgreen=6,white=7 };

/// Fwd DECL
class ColorFrame;

class ColorSelection
{
public:
    ConsoleColor frontColorNormal,backColorNormal,frontColorActivate,backColorActivate;
    std::string text,info;

    ColorSelection();
    virtual ~ColorSelection();

    virtual void drawText();
    virtual void drawInfo();
    bool hasInfo();

    void onActivate();
    void onDeActivate();

    ColorFrame* getFrame();
    void setFrame(ColorFrame*);

    /// return 2 to exit.
    /// return 1 to return last page.(If no page to return, exit the program)
    /// return 0 to continue running.(If you want to change page, you should return 0) (default)
    virtual int onClick();
private:
    bool _is_active;
    ColorFrame* _pframe;
};

class ColorPage
{
public:
    ConsoleColor titleFrontColor,titleBackColor,textFrontColor,textBackColor;
    std::string title,text;

    ColorPage();
    virtual ~ColorPage();

    /// Add and delete ColorSelections.
    void add(ColorSelection* p);
    int del(ColorSelection* p);

    /// Called by ColorFrame (mostly). Let Frame know how many selections have been added.
    int getSelectionSize();

    ColorFrame* getFrame();
    void setFrame(ColorFrame*);

    virtual void draw();

    /// Called when the page is load. Return a non-zero value means loading error.
    virtual int onLoad();

    /// Called when the page is entering background or foreground.
    virtual void onBackground();
    virtual void onForeground();

    /// Called when the page is unload. Return 0 tells the frame to delete this page.
    virtual int onUnload();

    /// Called when a selection is selected or unselected.
    virtual void onSelectionOver(int id);
    virtual void onSelectionOut(int id);

    /// Returns ColorSelection::onClick()
    virtual int onActive(int id);
protected:
    /// Get internal selection list, for derived class use.
    std::list<ColorSelection*>& _getlst();
private:
    std::list<ColorSelection*> _lst;
    int _curActive;
};

class ColorFrame
{
public:
    ColorFrame();
    ~ColorFrame();

    void setHomePage(ColorPage*);
    ColorPage* getHomePage();

    void run();

    /// Called By ColorSelections. (Call this function to set next page).
    /// Notice: If this function is called more than once while a selection is running, only the last call will be valid.
    void jumpTo(ColorPage*);

    /// Called By ColorPages.
    /// NOTICE: You MUST call 'enterInputMode' before you accept liner input from console. After all input is done, you must call exitInputMode.
    void enterInputMode();
    void exitInputMode();


private:
    ColorPage* _home_page;
    ColorPage* _cur_page;
    ColorPage* _next_page;
    std::stack<ColorPage*> _stk;
};

class ColorInputModeGuard
{
public:
    ColorInputModeGuard(ColorFrame* f);
    ~ColorInputModeGuard();
};
