#include "ColorUI.h"
#include <windows.h>
#include <conio.h>

namespace _ColorUI_Internal_Namespace
{

class _auto_init_console_info_class
{
public:
    _auto_init_console_info_class()
    {
        hout=GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO tInfo;
        GetConsoleScreenBufferInfo(hout,&tInfo);
        color=tInfo.wAttributes;
    }
    HANDLE hout;
    WORD color;
}_auto_init_console_info_obj;

inline ConsoleColor _winColor2ConsoleColor(int winColor)
{
    switch(winColor)
    {
    case 0:
        return ConsoleColor::black;
    case FOREGROUND_BLUE:
    case BACKGROUND_BLUE:
        return ConsoleColor::blue;
    case FOREGROUND_GREEN:
    case BACKGROUND_GREEN:
        return ConsoleColor::green;
    case FOREGROUND_RED:
    case BACKGROUND_RED:
        return ConsoleColor::red;
    case FOREGROUND_GREEN | FOREGROUND_BLUE:
    case BACKGROUND_GREEN | BACKGROUND_BLUE:
        return ConsoleColor::lightblue;
    case FOREGROUND_RED | FOREGROUND_BLUE:
    case BACKGROUND_RED | BACKGROUND_BLUE:
        return ConsoleColor::purple;
    case FOREGROUND_RED | FOREGROUND_GREEN:
    case BACKGROUND_RED | BACKGROUND_GREEN:
        return ConsoleColor::yellow;
    case FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED:
    case BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED:
        return ConsoleColor::white;
    }
}

inline int _ConsoleColor2winForeColor(ConsoleColor conColor)
{
    switch(conColor)
    {
    case ConsoleColor::black:
        return 0;
    case ConsoleColor::blue:
        return FOREGROUND_BLUE;
    case ConsoleColor::green:
        return FOREGROUND_GREEN;
    case ConsoleColor::red:
        return FOREGROUND_RED;
    case ConsoleColor::lightblue:
        return FOREGROUND_GREEN | FOREGROUND_BLUE;
    case ConsoleColor::purple:
        return FOREGROUND_RED |FOREGROUND_BLUE;
    case ConsoleColor::yellow:
        return FOREGROUND_RED | FOREGROUND_GREEN;
    case ConsoleColor::white:
        return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
}

inline int _ConsoleColor2winBackColor(ConsoleColor conColor)
{
    switch(conColor)
    {
    case ConsoleColor::black:
        return 0;
    case ConsoleColor::blue:
        return BACKGROUND_BLUE;
    case ConsoleColor::green:
        return BACKGROUND_GREEN;
    case ConsoleColor::red:
        return BACKGROUND_RED;
    case ConsoleColor::lightblue:
        return BACKGROUND_GREEN | BACKGROUND_BLUE;
    case ConsoleColor::purple:
        return BACKGROUND_RED | BACKGROUND_BLUE;
    case ConsoleColor::yellow:
        return BACKGROUND_RED | BACKGROUND_GREEN;
    case ConsoleColor::white:
        return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    }
}

inline void cprint(ConsoleColor FrontColor,ConsoleColor BackColor)
{
    int iFront=_ConsoleColor2winBackColor(FrontColor);
    int iBack=_ConsoleColor2winBackColor(BackColor);
    if(iFront!=0) iFront=iFront | FOREGROUND_INTENSITY;
    if(iBack!=0) iBack=iBack| BACKGROUND_INTENSITY;
    SetConsoleTextAttribute(_auto_init_console_info_obj.hout,iFront | iBack );
}

inline void cprint()
{
    SetConsoleTextAttribute(_auto_init_console_info_obj.hout,_auto_init_console_info_obj.color);
}

int _last_keyval;
int _GetUserInputKey_Real()
{
    return _last_keyval=getch();///defined in conio.h
}

enum class KEY { UNDEF=0, UP, DOWN, LEFT, RIGHT, CONFIRM, ESC, HOME, END };
int _last_isSpecial;

KEY GetUserInputKey()
{
    int a=_GetUserInputKey_Real();
    if(a==224||a==0)
    {
        _last_isSpecial=1;
        int b=_GetUserInputKey_Real();
        switch(b)
        {
            case 72:return KEY::UP;
            case 80:return KEY::DOWN;
            case 75:return KEY::LEFT;
            case 77:return KEY::RIGHT;
            case 71:return KEY::HOME;
            case 79:return KEY::END;
            default:return KEY::UNDEF;
        }
    }
    else
    {
        _last_isSpecial=0;
        switch(a)
        {
            case ' ':case '\r':case '\n':case '5':
            return KEY::CONFIRM;
            case 'w':case 'W':case '8':
            return KEY::UP;
            case 's':case 'S':case '2':
            return KEY::DOWN;
            case 'a':case 'A':case '4':
            return KEY::LEFT;
            case 'd':case 'D':case '6':
            return KEY::RIGHT;
            case 27:
            return KEY::ESC;

            default:
            return KEY::UNDEF;
        }
    }
}

int GetAction(int& cid,int MinVal,int MaxVal,int EscapeVal=MaxVal)
{
    KEY t=GetUserInputKey();
    switch(t)
    {
    case KEY::UP:
        {
            cid=std::max(std::min(cid-1,MaxVal),MinVal);
            return 0;
        }
    case KEY::DOWN:
        {
            cid=std::max(std::min(cid+1,MaxVal),MinVal);
            return 0;
        }
    case KEY::ESC:
        {
            cid=EscapeVal;
            return 0;
        }
    case KEY::CONFIRM:
        return 1;
    case KEY::HOME:
        {
            cid=MinVal;
            return 0;
        }
    case KEY::END:
        {
            cid=MaxVal;
            return 0;
        }
    default:
        return 0;
    }
}

}/// End of namespace _ColorUI_Internal_Namespace

ColorSelection::ColorSelection()
{

}
