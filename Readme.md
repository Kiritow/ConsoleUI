#Console UI  
一个简单的控制台UI抽象框架.  
说起来你可能不信,但这是我第一次感觉到一丝MVC的气息...(也有可能是MFC...)  

## 类说明
### Selection
`virtual int onClick()` 处理选中事件  
> 返回值  
0 不进行页面切换(如果设置了jump页面则跳转到下一页面)  
1 返回上一页面  
2 退出程序  

`virtual void drawText()` 绘制选项文字  
`virtual void drawInfo()` 绘制选项提示  

### Page
`virtual void onLoad()` 当页面被加载时执行  
`virtual void onBackground()` 当页面进入后台时执行  
`virtual void onForeground()` 当页面从后台被载入到前台时执行
`virtual int onUnload()` 当页面被卸载时执行
> 返回值  
0 由对应的Frame进行资源释放(delete)  
1 Frame不进行资源释放

