#include "ConsoleUI/ColorUI.h"
#include <cstdio>
using namespace std;

class ReturnButton : public ColorSelection
{
public:
    ReturnButton()
    {
        text="Return";
        info="Return to previous page.";
    }
    virtual int onClick() override
    {
        return 1;
    }
};

class ExitButton : public ColorSelection
{
public:
    ExitButton()
    {
        text="Exit";
        info="Exit This Program";
    }
    virtual int onClick() override
    {
        return 2;
    }
};

class EmptyButton : public ColorSelection
{
public:
    EmptyButton()
    {
        text="EmptyButton";
        info="This is an empty button. (without action)";
    }
};

class NewPage : public ColorPage
{
public:
    NewPage()
    {
        title="This is a new page";
        text="As you can see this page is different from the previous page. Now that page has gone to background and has been stored in a stack. ";
        add(new ReturnButton);
    }
};

class LinkToPageButton : public ColorSelection
{
public:
    LinkToPageButton()
    {
        text="Link To New Page";
        info="Select this, and a new page will be opened.";
    }
    virtual int onClick() override
    {
        getFrame()->jumpTo(new NewPage);
        return 0;
    }
};

class PlainPage : public ColorPage
{
public:
    PlainPage()
    {
        title="New Page";
        text="You may never see this page";
    }
    virtual void draw() override
    {
        printf("This is a plain page. As you can see there's no title or selection here. \n"
               "You can override ColorPage::draw() in your derived page class. \n"
               "Then you can do something instead of printing in this method. \n"
               "For Example, now you can read a integer from keyboard (use scanf). \n"
               "However, You MUST call enterInputMode() before that and exitInputMode() after that. \n"
               " (A ColorInputModeGuard works too) \n"
               "\n"
               "Now Input A Integer. (If you input a invalid value, you have to try again.)\n");
        ColorInputModeGuard guard(getFrame());
        int input;
        while(scanf("%d%*c",&input)!=1)
        {
            printf("Invalid Input.\n");
			scanf("%*[^\n]%*c");
        }
        printf("Well Done! You inputed: %d\n",input);
        printf("You may have understand it. Now Press ENTER To Continue.\n");
        char buff[1024];
		fgets(buff, 1024, stdin);
    }
};

class LinkToPlainPageButton : public ColorSelection
{
public:
    LinkToPlainPageButton()
    {
        text="Link to Plain Page";
        info="This button will guide you to a plain page";
    }
    virtual int onClick() override
    {
        getFrame()->jumpTo(new PlainPage);
        return 0;
    }
};

class MainPage : public ColorPage
{
public:
    MainPage()
    {
        title="Hello, World!";
        text="This page is an example page used ColorUI Framework. As you can see there are four selections below. Try select them.";
        add(new EmptyButton);
        add(new LinkToPageButton);
        add(new LinkToPlainPageButton);
        add(new ExitButton);
    }
};

int main()
{
    ColorFrame f;
    f.setHomePage(new MainPage);
    f.run();
    return 0;
}
