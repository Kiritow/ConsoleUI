#include "ColorUI.h"
#include <windows.h>

namespace _ColorUI_Internal_Namespace
{

class _auto_init_console_info_class
{
public:
    _auto_init_console_info_class()
    {
        hout=GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO tinfo;
        GetConsoleScreenBufferInfo(hout,&tInfo);
        color=tinfo.wAttributes;
    }
    HANDLE hout;
    WORD color;
}_auto_init_console_info_obj;

void cprint(ConsoleColor FrontColor,ConsoleColor BackColor)
{
    int iFront=0;
    int iBack=0;
    switch(FrontColor)
    {
    case ConsoleColor::black:
        iFront=0;break;
    case ConsoleColor::blue:
        iFront=FOREGROUND_BLUE;break;
    case ConsoleColor::lightblue:
        iFront=FOREGROUND_GREEN | FOREGROUND_BLUE;break;
    case ConsoleColor::green:
        iFront=FOREGROUND_GREEN;break;
    case ConsoleColor::purple:
        iFront=FOREGROUND_RED |FOREGROUND_BLUE;break;
    case ConsoleColor::red:
        iFront=FOREGROUND_RED;break;
    case ConsoleColor::white:
        iFront=FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; break;
    case ConsoleColor::yellow:
        iFront=FOREGROUND_RED | FOREGROUND_GREEN;break;
    }
    switch(BackColor)
    {
    case ConsoleColor::black:
        iBack=0;break;
    case ConsoleColor::blue:
        iBack=BACKGROUND_BLUE;break;
    case ConsoleColor::lightblue:
        iBack=BACKGROUND_GREEN | BACKGROUND_BLUE;break;
    case ConsoleColor::green:
        iFront=BACKGROUND_GREEN;break;
    case ConsoleColor::purple:
        iBack=BACKGROUND_RED | BACKGROUND_BLUE;break;
    case ConsoleColor::red:
        iBack=BACKGROUND_RED;break;
    case ConsoleColor::white:
        iBack=BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; break;
    case ConsoleColor::yellow:
        iBack=BACKGROUND_RED | BACKGROUND_GREEN;break;
    }
    if(iFront!=0) iFront=iFront | FOREGROUND_INTENSITY;
    if(iBack!=0) iBack=iBack| BACKGROUND_INTENSITY;
    SetConsoleTextAttribute(_auto_init_console_info_obj.hout,iFront | iBack );
}

void cprint()
{
    SetConsoleTextAttribute(_auto_init_console_info_obj.hout,_auto_init_console_info_obj.color);
}

}/// End of namespace _ColorUI_Internal_Namespace

ColorSelection::ColorSelection()
{
    frontColor=ConsoleColor::white;
    backColor=ConsoleColor::black;
}
