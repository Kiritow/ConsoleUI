#include "ColorUI.h"

#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#include <conio.h> // getch
#else
#include <cstdio> 
/// Fix getch() without conio.h
#include <termios.h>

struct termios _old, _new;
/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &_old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &_new); /* use these new terminal i/o settings now */
}
/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}
/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}
/* Read 1 character without echo */
char getch(void)
{
	return getch_(0);
}
/* Read 1 character with echo */
char getche(void)
{
	return getch_(1);
}
#endif

namespace _cns
{
#ifdef _WIN32 /// Windows Platform
	class _auto_init_console_info_class
	{
	public:
		_auto_init_console_info_class()
		{
			hout = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO tInfo;
			GetConsoleScreenBufferInfo(hout, &tInfo);
			color = tInfo.wAttributes;
		}
		HANDLE hout;
		WORD color;
	}_auto_init_console_info_obj;

	inline ConsoleColor _winColor2ConsoleColor(int winColor)
	{
		switch (winColor)
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
		switch (conColor)
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
			return FOREGROUND_RED | FOREGROUND_BLUE;
		case ConsoleColor::yellow:
			return FOREGROUND_RED | FOREGROUND_GREEN;
		case ConsoleColor::white:
			return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		default:
			return 0;
		}
	}

	inline int _ConsoleColor2winBackColor(ConsoleColor conColor)
	{
		switch (conColor)
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
		default:
			return 0;
		}
	}

	inline void cprint(ConsoleColor FrontColor, ConsoleColor BackColor, bool withIntensity = true)
	{
		int iFront = _ConsoleColor2winForeColor(FrontColor);
		int iBack = _ConsoleColor2winBackColor(BackColor);
		if (withIntensity)
		{
			if (iFront != 0) iFront = iFront | FOREGROUND_INTENSITY;
			if (iBack != 0) iBack = iBack | BACKGROUND_INTENSITY;
		}
		SetConsoleTextAttribute(_auto_init_console_info_obj.hout, iFront | iBack);
	}

	inline void cprint()
	{
		SetConsoleTextAttribute(_auto_init_console_info_obj.hout, _auto_init_console_info_obj.color);
	}
#else /// Linix-like 
	inline void cprint(ConsoleColor FrontColor, ConsoleColor BackColor, bool withIntensity = true)
	{
		/// Ignore intensity option?
		printf("\033[%d;%dm", (int)FrontColor + 30, (int)BackColor + 40);
	}
	inline void cprint()
	{
		printf("\033[0m");
	}
#endif /// End of platform

	int _last_keyval;
	int _GetUserInputKey_Real()
	{
		return _last_keyval = getch();///defined in conio.h
	}

	enum class KEY { UNDEF = 0, UP, DOWN, LEFT, RIGHT, CONFIRM, ESC, HOME, END };
	int _last_isSpecial;

	KEY GetUserInputKey()
	{
		int a = _GetUserInputKey_Real();
		if (a == 224 || a == 0)
		{
			_last_isSpecial = 1;
			int b = _GetUserInputKey_Real();
			switch (b)
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
			_last_isSpecial = 0;
			switch (a)
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

	int GetAction(int& cid, int MinVal, int MaxVal, int EscapeVal)
	{
		KEY t = GetUserInputKey();
		switch (t)
		{
		case KEY::UP:
		{
			cid = max(min(cid - 1, MaxVal), MinVal);
			return 0;
		}
		case KEY::DOWN:
		{
			cid = max(min(cid + 1, MaxVal), MinVal);
			return 0;
		}
		case KEY::ESC:
		{
			cid = EscapeVal;
			return 0;
		}
		case KEY::CONFIRM:
			return 1;
		case KEY::HOME:
		{
			cid = MinVal;
			return 0;
		}
		case KEY::END:
		{
			cid = MaxVal;
			return 0;
		}
		default:
			return 0;
		}
	}

	static void ClearInputBuffer()
	{
		scanf("%*[^\n]%*c");
	}

}/// End of namespace _cns

ColorSelection::ColorSelection()
{
	frontColorNormal = ConsoleColor::white;
	backColorNormal = ConsoleColor::black;
	frontColorActivate = ConsoleColor::black;
	backColorActivate = ConsoleColor::yellow;
	_is_active = false;
	_pframe = nullptr;
}

//virtual
ColorSelection::~ColorSelection()
{

}

//virtual
void ColorSelection::drawText()
{
	if (_is_active)
	{
		_cns::cprint(frontColorActivate, backColorActivate);
	}
	else
	{
		_cns::cprint(frontColorNormal, backColorNormal);
	}

	printf("%s", text.c_str());
}

//virtual
void ColorSelection::drawInfo()
{
	printf("%s", info.c_str());
}

bool ColorSelection::hasInfo()
{
	return !info.empty();
}

//virtual
void ColorSelection::onActivate()
{
	_is_active = true;
}

//virtual
void ColorSelection::onDeActivate()
{
	_is_active = false;
}

ColorFrame* ColorSelection::getFrame()
{
	return _pframe;
}

void ColorSelection::setFrame(ColorFrame* pFrame)
{
	_pframe = pFrame;
}

//virtual
int ColorSelection::onClick()
{
	return 0;
}

//virtual
int ColorSelection::onDelete()
{
	return 0;
}

ColorPage::ColorPage()
{
	titleFrontColor = ConsoleColor::black;
	titleBackColor = ConsoleColor::lightblue;
	textFrontColor = ConsoleColor::white;
	textBackColor = ConsoleColor::black;
	_curActive = 0;
	_pframe = nullptr;
}

//virtual
ColorPage::~ColorPage()
{

}

void ColorPage::add(ColorSelection* p)
{
	p->setFrame(getFrame());
	_vec.push_back(p);
}

int ColorPage::del(ColorSelection* p)
{
	auto iter = std::find(_vec.begin(), _vec.end(), p);
	if (iter == _vec.end())
	{
		/// Object to delete is Not Found
		return 1;
	}

	if (_curActive == iter - _vec.begin() + 1)
	{
		/// Current Activated Selection is to be deleted.
		_curActive = 0;

		/// Delete Selection
		if ((*iter)->onDelete() == 0)
		{
			(*iter)->setFrame(nullptr);
			delete (*iter);
		}
		else
		{
			(*iter)->setFrame(nullptr);
		}

		/// Delete From Record.
		_vec.erase(iter);
	}
	else
	{
		/// Get current activated selection.
		ColorSelection* _curActive_ptr = _vec.at(_curActive - 1);

		/// Delete Target Selection
		if ((*iter)->onDelete() == 0)
		{
			(*iter)->setFrame(nullptr);
			delete (*iter);
		}
		else
		{
			(*iter)->setFrame(nullptr);
		}

		/// Delete Target From Record.
		_vec.erase(iter);

		/// Re-Calculate active id.
		_curActive = std::find(_vec.begin(), _vec.end(), _curActive_ptr) - _vec.begin() + 1;
	}

	return 0;
}

int ColorPage::getSelectionSize() const
{
	return _vec.size();
}

int ColorPage::getCurrentActive() const
{
	return _curActive;
}


ColorFrame* ColorPage::getFrame()
{
	return _pframe;
}

void ColorPage::setFrame(ColorFrame* pFrame)
{
	_pframe = pFrame;
	for (auto& ptr : _vec)
	{
		ptr->setFrame(getFrame());
	}
}

//virtual
void ColorPage::draw()
{
	if (!title.empty())
	{
		_cns::cprint(titleFrontColor, titleBackColor);
		printf("%s\n", title.c_str());
	}
	if (!text.empty())
	{
		_cns::cprint(textFrontColor, textBackColor, false);
		printf("%s\n", text.c_str());
	}
	if (!_curActive) // _curActive==0
	{
		if (!_vec.empty())
		{
			_curActive = 1;
			onSelectionOver(_curActive);
		}
	}
	for (auto iter = _vec.begin(); iter != _vec.end(); ++iter)
	{
		if (!(*iter)->text.empty())
		{
			(*iter)->drawText();
		}
		else
		{
			_cns::cprint();
			printf("(Empty Selection)");
		}
		printf("\n");
	}
	_cns::cprint();
	printf("---------------\n");
	if (_vec.at(_curActive - 1)->hasInfo())
	{
		_vec.at(_curActive - 1)->drawInfo();
		printf("\n");
	}
	_cns::cprint();
}

//virtual
int ColorPage::onLoad()
{
	return 0;
}

//virtual
void ColorPage::onBackground()
{

}

//virtual
void ColorPage::onForeground()
{

}

//virtual
int ColorPage::onUnload()
{
	for (auto& ptr : _vec)
	{
		if (!ptr->onDelete())
		{
			delete ptr;
		}
	}
	_vec.clear();
	_curActive = 0;
	return 0;
}

//virtual
void ColorPage::onSelectionOver(int id)
{
	_vec.at(_curActive - 1)->onDeActivate();
	_vec.at(id - 1)->onActivate();
	_curActive = id;
}

//virtual
int ColorPage::onActive(int id)
{
	return _vec.at(id - 1)->onClick();
}

//protected
std::vector<ColorSelection*>& ColorPage::_getvec()
{
	return _vec;
}

ColorFrame::ColorFrame()
{
	_home_page = nullptr;
	_cur_page = nullptr;
	_next_page = nullptr;
	_delete_home_page_on_dtor = true;
	_has_started = false;
}

ColorFrame::~ColorFrame()
{

}

void ColorFrame::setHomePage(ColorPage* pPage, bool deleteOnRemove)
{
	if (_has_started) return;
	_home_page = pPage;
	_delete_home_page_on_dtor = deleteOnRemove;
	pPage->setFrame(this);
}

ColorPage* ColorFrame::getHomePage()
{
	return _home_page;
}

void ColorFrame::jumpTo(ColorPage* nextPage)
{
	_next_page = nextPage;
}

void ColorFrame::clearScreen()
{
	_cns::cprint();
	system("cls");
}

void ColorFrame::clearInput()
{
	fflush(stdin);
}

void ColorFrame::enterInputMode()
{
	clearInput();
}

void ColorFrame::exitInputMode()
{
	clearInput();
}

void ColorFrame::run()
{
	/// A Frame can only be started once.
	if (_has_started) return;

	_has_started = true;

	/// No Home Page
	if (_home_page == nullptr) return;
	_cur_page = _home_page;

	_cur_page->onLoad();

	bool stop = false;
	int cid = 0;
	while (!stop)
	{
		clearScreen();
		_cur_page->draw();
		if (_cur_page->getSelectionSize() > 0)
		{
			cid = _cur_page->getCurrentActive();
			int oldcid = cid;
			if (_cns::GetAction(cid, 1, _cur_page->getSelectionSize(), _cur_page->getSelectionSize()))
			{
				/// Confirmed Key Pressed.
				switch (_cur_page->onActive(cid))
				{
				case 0:
				{
					if (_next_page != nullptr)
					{
						/// New Page!
						/// Bring Current Page to Background
						_cur_page->onBackground();
						_stk.push(_cur_page);

						/// Load New Page
						_cur_page = _next_page;
						_next_page = nullptr;
						_cur_page->setFrame(this);
						_cur_page->onLoad();
					}
					else
					{
						/// No New Page. Go Straight.
					}
				}
				break;
				case 1:
				{
					/// Unload Current Page
					if (_cur_page->onUnload() == 0)
					{
						_cur_page->setFrame(nullptr);
						delete _cur_page;
					}
					else
					{
						_cur_page->setFrame(nullptr);
					}
					_cur_page = nullptr;

					/// Bring Previous Page to Foreground.
					if (_stk.empty())
					{
						stop = true;
					}
					else
					{
						_cur_page = _stk.top();
						_stk.pop();
						_cur_page->onForeground();
					}
				}
				break;
				case 2:
				default:
					stop = true;
					break;
				}
			}
			else
			{
				/// If not confirmed , just go ahead.
				if (oldcid != cid)
				{
					_cur_page->onSelectionOver(cid);
				}
			}
		}
		else
		{
			/// Unload Current Page
			if (_cur_page->onUnload() == 0)
			{
				_cur_page->setFrame(nullptr);
				delete _cur_page;
			}
			else
			{
				_cur_page->setFrame(nullptr);
			}
			_cur_page = nullptr;

			/// Bring Previous Page to Foreground.
			if (_stk.empty())
			{
				stop = true;
			}
			else
			{
				_cur_page = _stk.top();
				_stk.pop();
				_cur_page->onForeground();
			}
		}
	}

	if (_cur_page)
	{
		if (_cur_page->onUnload() == 0)
		{
			_cur_page->setFrame(nullptr);
			delete _cur_page;
		}
		else
		{
			_cur_page->setFrame(nullptr);
		}
		_cur_page = nullptr;
	}
	while (!_stk.empty())
	{
		_cur_page = _stk.top();
		_cur_page->onForeground();
		if (_cur_page->onUnload() == 0)
		{
			_cur_page->setFrame(nullptr);
			delete _cur_page;
		}
		else
		{
			_cur_page->setFrame(nullptr);
		}
	}
	_cur_page = nullptr;
}

ColorInputModeGuard::ColorInputModeGuard(ColorFrame* f)
{
	_pframe = f;
	if (_pframe)
	{
		_pframe->enterInputMode();
	}
}

ColorInputModeGuard::~ColorInputModeGuard()
{
	if (_pframe)
	{
		_pframe->exitInputMode();
	}
}
