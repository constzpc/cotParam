# 轻量级参数管理框架(C语言)

#### 介绍
1.  采用表驱动方式统一管理所有参数，包括缺省值、最小值和最大值等

2.  采用宏定义快速注册和添加参数进行管理

3.  支持基本类型参数和字符串参数

4.  支持序列化和反序列化，便于保存和读取校验

5.  可通过串口协议或其他协议调用接口修改参数

#### 软件架构
软件架构说明

#### 使用说明
1.  通过函数 Param_ModifyById、Param_ModifyByName 限制修改参数
2.  调用函数 Param_Serialize 对参数序列化, 保存至储存芯片中
3.  从储存芯片读取数据后，调用 Param_Parse 反序列化得到参数，同时进行参数范围校验

#### demo样式
博客：

[轻量级参数管理框架（C语言）](https://blog.csdn.net/qq_24130227/article/details/129233836?spm=1001.2014.3001.5501)

#### 关于作者
1.  CSDN 博客 [大橙子疯](https://blog.csdn.net/qq_24130227?spm=1010.2135.3001.5343)
2.  联系邮箱 const_zpc@163.com
3.  了解更多可关注微信公众号

![大橙子疯嵌入式](微信公众号.jpg)