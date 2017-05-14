# Console UI  
一个简单的控制台UI抽象框架.  
说起来你可能不信,但这是我第一次感觉到一丝MVC的气息...(也有可能是MFC...)  

## ColorUI编程参考
参见[Header](ConsoleUI/ColorUI.h) , [Source](ConsoleUI/ColorUI.cpp)  
**注意** ColorUI与CUI互不兼容.  
### ColorSelection
`ConsoleColor frontColorNormal,backColorNormal,frontColorActivate,backColorActivate` 选项的前景色与背景色(激活与未激活)  
`string text` 选项内容  
`string info` 选项说明(可为空)  
`virtual void drawText()` 绘制选项文字  
`virtual void drawInfo()` 绘制选项提示  
`virtual void onActivate()` 被激活时调用  
`virtual void onDeActivate()` 被取消激活时调用  
`virtual int onClick()` 处理选中事件  
> 返回值  
0 不进行页面切换(如果设置了jump页面则跳转到下一页面)  
1 返回上一页面  
2 退出程序  

`virtual int onDelete()` 选项所在页面被卸载时调用  
> 返回值  
0 此选项将被页面删除  
其他 此选项将不会被页面删除

### ColorPage
`ConsoleColor titleFrontColor,titleBackColor` 标题颜色  
`ConsoleColor textFrontColor,textBackColor` 内容颜色  
`void add(ColorSelection*)` 添加选项  
`int del(ColorSelection*)` 删除选项  
`virtual void onLoad()` 当页面被加载时执行  
`virtual void onBackground()` 当页面进入后台时执行  
`virtual void onForeground()` 当页面从后台被载入到前台时执行  
`virtual int onUnload()` 当页面被卸载时执行
> 返回值  
0 由对应的Frame进行资源释放(delete)  
1 Frame不进行资源释放

### ColorFrame
`void setHomePage(ColorPage*)` 设置启动页面  
`void run()` 启动程序  
`void jumpTo(ColorPage*)` 设置跳转页面  
**注意:** 这个跳转页面不能是已经存在的页面(已进入后台的). 否则会引起内存错误.(但是这不是设计上的错误)  
`void enterInputMode()` 进入自由输入模式  
`void exitInputMode()` 退出自由输入模式  
**注意:** 在自定义的页面中,务必使用以上两个方法以保证页面运行前和框架在调用页面后,框架能够正确运行.

### ColorInputModeGuard
`ColorInputModeGuard(ColorFrame*)` 使用ColorFrame对Guard进行初始化并进入自由输入模式.析构时将自动退出自由输入模式.  



## CUI编程参考
参见[Header](ConsoleUI/CUI.h) , [Source](ConsoleUI/CUI.cpp)  
### Selection
`string text` 选项内容  
`string info` 选项说明(可为空)  
`virtual int onClick()` 处理选中事件  
> 返回值  
0 不进行页面切换(如果设置了jump页面则跳转到下一页面)  
1 返回上一页面  
2 退出程序  

`virtual void drawText()` 绘制选项文字  
`virtual void drawInfo()` 绘制选项提示  

### Page
`string title` 页面标题  
`string text` 页面内容  
`virtual void onLoad()` 当页面被加载时执行  
`virtual void onBackground()` 当页面进入后台时执行  
`virtual void onForeground()` 当页面从后台被载入到前台时执行  
`virtual int onUnload()` 当页面被卸载时执行
> 返回值  
0 由对应的Frame进行资源释放(delete)  
1 Frame不进行资源释放

### Frame  
`virtual void setPage(Page*)` 设置首页  
`virtual void getPage()` 获取首页  
`virtual void run()` 启动程序  
`virtual void jumpTo()` 由其他控件调用,设置跳转的下一页面  

## ConsoleUI 设计理念 
在ConsoleUI(包括CUI和ColorUI)的设计中，页面(Page,ColorPage)为内容承载主体. 框架(Frame,ColorFrame)对内容的调度是基于页面的. 选项(Selection,ColorSelection)作为交互式操作的一部分,嵌入在页面中并接受框架管理.  
一个页面的生命周期如下:  构造->onLoad()->onBackground()<->onForeground()->onUnload()->析构  
