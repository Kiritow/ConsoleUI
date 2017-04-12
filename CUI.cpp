#include "CUI.h"
#include <cstdio>

Selection::Selection()
{
    id=-1;
}

//virtual
void Selection::drawText()
{
    printf("%s",text.c_str());
}

//virtual
void Selection::drawInfo()
{
    printf("%s",info.c_str());
}

bool Selection::hasInfo()
{
    return !info.empty();
}

//virtual
int Selection::onClick()
{
    return 0;
}

Page::Page()
{

}

Page::~Page()
{

}

//virtual
void Page::draw()
{
    if(!title.empty())
    {
        printf("%s\n",title.c_str());
    }
    if(!text.empty())
    {
        printf("%s\n",text.c_str());
    }

    int id=1;
    for(std::list<Selection*>::iterator iter=_vec.begin();iter!=_vec.end();iter++)
    {
        printf("%d ",id);
        (*iter)->drawText();
        if((*iter)->hasInfo())
        {
            printf(" (");
            (*iter)->drawInfo();
            printf(")");
        }
        printf("\n");
        ++id;
    }
    printf("\n");
}

//virtual
void Page::onLoad()
{

}

//virtual
int Page::onUnload()
{
    return 0;
}

//virtual
void Page::onForeground()
{

}

//virtual
void Page::onBackground()
{

}

int Page::getSelectionSize()
{
    return _vec.size();
}

int Page::onActive(int id)
{
    int left=id;
    std::list<Selection*>::iterator iter=_vec.begin();
    while(left>0)
    {
        ++iter;
        --left;
    }
    return (*iter)->onClick();
}

void Page::setFrame(Frame* f)
{
    _pframe=f;
}

Frame* Page::getFrame()
{
    return _pframe;
}

void Page::add(Selection* p)
{
    _vec.push_back(p);
}

Frame::Frame()
{
    _page=nullptr;
    _cpage=nullptr;
    _npage=nullptr;
}

Frame::~Frame()
{

}

//protected
void Frame::clearScreen()
{
    system("cls");
}

//protected
void Frame::clearInput()
{
    fflush(stdin);
}

//protected
int Frame::onInvalidInput()
{
    clearInput();
    printf("Invalid Input.\n");
    return 0;
}

void Frame::setPage(Page* p)
{
    _page=p;
    p->setFrame(this);
}

Page* Frame::getPage()
{
    return _page;
}

void Frame::jumpTo(Page* p)
{
    _npage=p;
}

void Frame::run()
{
    /// No Page
    if(_page==nullptr) return;

    bool stop=false;
    _cpage=_page;

    _cpage->onLoad();

    while(!stop)
    {
        clearScreen();
        _cpage->draw();
        if(_cpage->getSelectionSize()>0)
        {
            int uinput;
            while(scanf("%d%*c",&uinput)!=1)
            {
                if(onInvalidInput()!=0)
                {
                    stop=true;
                    break;
                }
            }
            if(stop) continue;

            if(uinput<1||uinput>_cpage->getSelectionSize())
            {
                if(onInvalidInput()!=0)
                {
                    stop=true;
                    continue;
                }
            }

            int ret=_cpage->onActive(uinput-1);
            switch(ret)
            {
            case 2:
                stop=true;
                break;
            case 1:
                {
                    if(_stk.empty())
                    {
                        stop=true;
                    }
                    else
                    {
                        /// Unload Current Page
                        if(_cpage->onUnload()==0)
                        {
                            delete _cpage;
                        }
                        _cpage=_stk.top();
                        _stk.pop();
                        _cpage->onForeground();
                    }
                }
                break;
            case 0:
                {
                    if(_npage!=nullptr)
                    {
                        /// New Page.
                        _cpage->onBackground();
                        _stk.push(_cpage);
                        _cpage=_npage;
                        _npage=nullptr;
                        _cpage->onLoad();
                    }
                    else
                    {
                        /// continue running.
                    }
                }
                break;
            }
        }
        else
        {
            /// No Selection on page...
            stop=true;
        }
    }

    if(_cpage->onUnload()==0)
    {
        delete _cpage;
    }
    _cpage=nullptr;

    /// Unload all inactive pages.
    while(!_stk.empty())
    {
        _cpage=_stk.top();
        _stk.pop();
        if(_cpage->onUnload()==0)
        {
            delete _cpage;
        }
        _cpage=nullptr;
    }
}
