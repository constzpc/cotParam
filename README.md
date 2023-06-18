# 轻量级参数管理框架(C语言)

## 介绍

1. 采用表驱动方式统一管理所有参数，包括缺省值、最小值和最大值等

   > - 支持定义普通参数，无缺省值、最小值和最大值限制
   > - 支持定义普通参数，有缺省值，但无最小值和最大值限制
   > - 支持定义普通参数，有缺省值，最小值和最大值限制
   >
2. 采用宏定义快速对参数进行定义、注册和管理
3. 支持已定义变量做为参数进行管理，如全局变量或者结构体成员变量
4. 支持基本类型参数和字符串参数
5. 支持序列化和反序列化操作，可在本地储存设备保存/读取二进制数据

   > - 支持键值对的方式储存，即使后期版本迭代对参数表删除/插入数据时也能向下兼容
   > - 支持非键值对的方式储存，适合小容量的储存设备，序列化后的数据内容少，但是后期版本迭代对参数表删除或插入数据时不能向下兼容，只有通过在参数表后添加参数才能向下兼容
   > - 通过多次读写储存设备分别加载参数和保存参数，更兼容小内存的平台使用（多次调用回调函数处理）
   > - 支持在数据加载或保存时当参数当前值不合法（超出范围）触发错误处理回调函数，有上层应用程序决定如何处理（可以恢复默认值）
   >
6. 支持功能配置裁剪

   > - 根据不同的平台，可以对部分功能裁剪，或者修改配置适用于不同容量的芯片开发
   > - 键值对的方式储存：向下兼容较好
   > - 可以选择只支持基本类型的参数储存功能，如字符串类型参数和64位长度的参数可裁剪
   >

## 软件架构

## 使用说明

定义参数表

```c

PARAM_DEFINE_DAT (g_test, PARAM_INT16, 10);
PARAM_DEFINE_DAT_DEF (g_test_2, PARAM_UINT16, 20);
PARAM_DEFINE_DAT_RANGE (g_test_3, PARAM_DOUBLE, 3.15, -10, 10);
PARAM_DEFINE_STR_RANGE (g_test_str, 10, "abcdef", 5);
PARAM_DEFINE_DAT_RANGE (g_test_4, PARAM_INT8, 8, -10, 10);
PARAM_DEFINE_DAT_RANGE (g_test_5, PARAM_UINT32, 620, 500, 10000);
PARAM_DEFINE_DAT_RANGE (g_test_6, PARAM_UINT8, 45, 5, 100);
PARAM_DEFINE_DAT_RANGE (g_test_7, PARAM_INT64, 5, -542, 5450);
PARAM_DEFINE_BIND_DAT_RANGE(sg_tTest_test1, PARAM_UINT16, 20, 10, 2000); // 为即将绑定的变量定义相关参数信息
PARAM_DEFINE_BIND_DAT(sg_tTest_test2, PARAM_FLOAT); // 为即将绑定的变量定义相关参数信息，初值为sg_tTest 变量定义时的初值
PARAM_DEFINE_BIND_STR_RANGE(sg_tTest_str, sizeof(sg_tTest.str), "const-zpc", 6);// 为即将绑定的变量定义相关参数信息，初值为sg_tTest 变量定义时的初值

ParamInfo_t sg_ParamTable[] = {
    PARAM_ITEM_DAT(1, g_test, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_DEF(2, g_test_2, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(3, g_test_3, PARAM_ATTR_WR),
    PARAM_ITEM_STR_RANGE(4, g_test_str, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(5, g_test_4, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(6, g_test_5, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(7, g_test_6, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(8, g_test_7, PARAM_ATTR_READ), // 只读
    PARAM_ITEM_DAT_RANGE_BIND(9, sg_tTest_test1, sg_tTest.test1, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_BIND(10, sg_tTest_test2, sg_tTest.test2, PARAM_ATTR_WR),
    PARAM_ITEM_STR_RANGE_BIND(11, sg_tTest_str, sg_tTest.str, PARAM_ATTR_WR),
};

```

通过宏去操作参数示例片段代码

```c
// 首先需要在头文件声明
PARAM_EXTERN_DAT(g_test, PARAM_INT16);
PARAM_EXTERN_DAT(g_test_2, PARAM_UINT16);
PARAM_EXTERN_DAT(g_test_3, PARAM_DOUBLE);
PARAM_EXTERN_STR(g_test_str, 10);
PARAM_EXTERN_DAT(g_test_4, PARAM_INT8);
PARAM_EXTERN_DAT(g_test_5, PARAM_UINT32);
PARAM_EXTERN_DAT(g_test_6, PARAM_UINT8);
PARAM_EXTERN_DAT(g_test_7, PARAM_INT64);


//函数中使用
int main()
{
//方法一：
    // 对参数g_test_6设置新的值50，如果超出限定范围则不处理
    PARAM_DAT_SET_NEW_VALUE(g_test_6, 50, PARAM_NONE);

    // 对参数g_test_6设置新的值50，如果超出限定范围则重置为默认值
    PARAM_DAT_SET_NEW_VALUE(g_test_6, 50, PARAM_DEF);

    // 对参数g_test_6设置新的值150，如果超出限定范围则参数小于最小值则为最小值, 参数大于最大值则为最大值
    PARAM_DAT_SET_NEW_VALUE(g_test_6, 150, PARAM_MIN_MAX);


//方法二：
    // 对参数g_test_6设置新的值50，如果超出限定范围则不处理
    g_test_6 = 50;
    PARAM_DAT_CHECK_RANGE(g_test_6, PARAM_NONE);

    // 对参数g_test_6设置新的值50，如果超出限定范围则重置为默认值
    g_test_6 = 50;
    PARAM_DAT_CHECK_RANGE(g_test_6, PARAM_DEF);

    // 对参数g_test_6设置新的值150，如果超出限定范围则参数小于最小值则为最小值, 参数大于最大值则为最大值
    g_test_6 = 150;
    PARAM_DAT_CHECK_RANGE(g_test_6, PARAM_MIN_MAX);
}
```

通过函数去操作参数示例片段代码

```c
//函数中使用
int main()
{
    PARAM_UINT16_T tmp;
  
//方法一：
    // 对参数g_test_6设置新的值50，如果超出限定范围则不处理
    tmp = 50;
    Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_NONE);

    // 对参数g_test_6设置新的值50，如果超出限定范围则重置为默认值
    tmp = 50;
    Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_DEF);

    // 对参数g_test_6设置新的值150，如果超出限定范围则参数小于最小值则为最小值, 参数大于最大值则为最大值
    tmp = 150;
    Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_MIN_MAX);


//方法二：
    // 对参数g_test_6设置新的值50，如果超出限定范围则不处理
    g_test_6 = 50;
    Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_NONE);

    // 对参数g_test_6设置新的值50，如果超出限定范围则重置为默认值
    g_test_6 = 50;
    Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_DEF);

    // 对参数g_test_6设置新的值150，如果超出限定范围则参数小于最小值则为最小值, 参数大于最大值则为最大值
    g_test_6 = 150;
    Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_MIN_MAX);
}
```

序列化 Demo 的结果展示

```base
# 配置最多支持 15 个参数时加载参数时分多次读取/保存操作打印日志
load param：
        read: [21] -> 12 0a 00 22 14 00 38 33 33 33 33 33 33 09 40 4a 61 62 63 64 65
        read: [21] -> 66 00 00 00 00 51 08 64 6c 02 00 00 71 2d 88 05 00 00 00 00 00 
save param：
        save: [3] -> 12 0a 00
        save: [3] -> 22 14 00
        save: [9] -> 38 33 33 33 33 33 33 09 40
        save: [11] -> 4a 61 62 63 64 65 66 00 00 00 00
        save: [2] -> 51 08
        save: [5] -> 64 6c 02 00 00
        save: [2] -> 71 2d
        save: [9] -> 88 05 00 00 00 00 00 00 00

# 配置最多支持 15 个参数时的序列化内容及长度
"键值对序列化": [44] -> 12 0a 00 22 14 00 38 33 33 33 33 33 33 09 40 4a 61 62 63 64 65 66 00 00 00 00 51 08 64 6c 02 00 00 71 2d 88 05 00 00 00 00 00 00 00
"序列化": [36] -> 50 00 14 00 33 33 33 33 33 33 09 40 67 5f 74 65 73 74 5f 36 00 00 00 6c 02 00 00 2d 05 00 00 00 00 00 00 00 

# 配置最多支持 256 个参数时的序列化内容及长度
"键值对序列化": [52] -> 02 01 0a 00 02 02 14 00 08 03 33 33 33 33 33 33 09 40 0a 04 61 62 63 64 65 66 00 00 00 00 01 05 08 04 06 6c 02 00 00 01 07 2d 08 08 05 00 00 00 00 00 00 00
"序列化": [36] -> 50 00 14 00 33 33 33 33 33 33 09 40 67 5f 74 65 73 74 5f 36 00 00 00 6c 02 00 00 2d 05 00 00 00 00 00 00 00 

# 配置最多支持 4096 个参数时的序列化内容及长度
"键值对序列化": [60] -> 02 10 00 0a 00 02 20 00 14 00 08 30 00 33 33 33 33 33 33 09 40 0a 40 00 61 62 63 64 65 66 00 00 00 00 01 50 00 08 04 60 00 6c 02 00 00 01 70 00 2d 08 80 00 05 00 00 00 00 00 00 00
"序列化": [36] -> 50 00 14 00 33 33 33 33 33 33 09 40 67 5f 74 65 73 74 5f 36 00 00 00 6c 02 00 00 2d 05 00 00 00 00 00 00 00 

```

## demo样式

博客：

[轻量级参数管理框架（C语言）](https://blog.csdn.net/qq_24130227/article/details/129233836?spm=1001.2014.3001.5501)

## 关于作者

1. CSDN 博客 [大橙子疯](https://blog.csdn.net/qq_24130227?spm=1010.2135.3001.5343)
2. 联系邮箱 const_zpc@163.com
3. 了解更多可关注微信公众号

![大橙子疯嵌入式](微信公众号.jpg)
