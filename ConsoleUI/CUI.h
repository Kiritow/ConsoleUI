#pragma once
#include <list>
#include <string>
#include <stack>

/// Fwd DECL
class Frame;

class Selection
{
public:
    Selection();
    int id;
    std::string text;
    std::string info;

    virtual void drawText();
    virtual void drawInfo();
    bool hasInfo();
    Frame* getFrame();
    void setFrame();

    /// return 2 to exit.
    /// return 1 to return last page.(If no page to return, exit the program)
    /// return 0 to continue running.(If you want to change page, you should return 0)
    virtual int onClick();
private:
    Frame* _pframe;
};

class Page
{
public:
    Page();
    virtual ~Page();
    std::string title;
    std::string text;

    void add(Selection*);
    int del(Selection*);
    int getSelectionSize();

    Frame* getFrame();
    void setFrame(Frame*);

    virtual void draw();

    virtual void onLoad();
    virtual void onForeground();
    virtual void onBackground();
    /// return 0 tell frame to delete page, return 1 tell frame not to delete
    virtual int onUnload();

    virtual int onActive(int id);
private:
    std::list<Selection*> _vec;
    Frame* _pframe;
};

class Frame
{
public:
    Frame();
    ~Frame();
    void run();
    Page* getPage();
    void setPage(Page*);

    void jumpTo(Page*);

protected:
    void clearScreen();
    void clearInput();

    /// return 1 if error cannot be solve. return 0 on solved problem.
    int onInvalidInput();
private:
    /// First Page.
    Page* _page;
    /// Current Processing Page (Internal Variable)
    Page* _cpage;
    /// Next Page to be processed (Internal Variable)
    Page* _npage;
    /// Inactive Page Stack (Internal Variable)
    std::stack<Page*> _stk;
};

