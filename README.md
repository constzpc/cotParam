# 轻量级参数管理框架(C语言)

## 介绍
1.  采用表驱动方式统一管理所有参数，包括缺省值、最小值和最大值等

2.  采用宏定义快速注册和添加参数进行管理

3.  支持基本类型参数和字符串参数

4.  支持两种方式序列化和反序列化操作，便于选择在本地保存二进制数据或者跨设备通信使用
    > - 其中本地化序列化方式又可以选择“键值对”或者“值”保存，“键值对”方式更有利于向下兼容，即使迭代中删除数据也能正常获取；
    > - 本地化序列化又可以采用“编码”压缩的方式，最大限度压缩数据，因此采用“编码”时如果参数数值不同，序列化后的数据长度也有可能不同
    > - 跨设备序列化数据默认采用“键值对”和“编码”压缩的方式，同时增加了其他便于解析的信息，方便其他设备使用同样的代码进行反序列化

## 软件架构

定义参数表

```c
PARAM_DEFINE (test, PARAM_INT16, 10, -100, 100);
PARAM_DEFINE (test_2, PARAM_UINT16, 20, 0, 100);
PARAM_DEFINE (test_3, PARAM_DOUBLE, 3.15, -10, 10);
PARAM_DEFINE_STR (test_str, 10, "abcdef");
PARAM_DEFINE (test_4, PARAM_INT8, 8, -10, 10);
PARAM_DEFINE (test_5, PARAM_UINT32, 620, 500, 10000);
PARAM_DEFINE (test_6, PARAM_UINT8, 4, 5, 100);
PARAM_DEFINE (test_7, PARAM_INT64, 5, -542, 5450);

ParamInfo_t sg_ParamTable[] = {
    PARAM_REG(1, test, PARAM_INT16, PARAM_ATTR_ALL),
    PARAM_REG(2, test_2, PARAM_UINT16, PARAM_ATTR_READ),
    PARAM_REG(3, test_3, PARAM_DOUBLE, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_STR_REG(4, test_str, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_REG(5, test_4, PARAM_INT8, PARAM_ATTR_READ),
    PARAM_REG(6, test_5, PARAM_UINT32, PARAM_ATTR_READ),
    PARAM_REG(7, test_6, PARAM_UINT8, PARAM_ATTR_READ),
    PARAM_REG(8, test_7, PARAM_INT64, PARAM_ATTR_WRITE),
};
```

Demo 的结果展示（具体编译demo后使用）
```c
Init:
  id    name            type     length   attr     val        def        min        max
 1      test            int16_t    2      wr       10         10         -100       100
 2      test_2          uint16_t   2      r        20         20         0          100
 3      test_3          double     8      wr       3.150000   3.150000   -10.000000 10.000000
 4      test_str        string     10     wr       abcdef     abcdef
 5      test_4          int8_t     1      r        8          8          -10        10
 6      test_5          uint32_t   4      r        620        620        500        10000
 7      test_6          uint8_t    1      r        4          4          5          100
 8      test_7          int64_t    8      w        5          0          5          0

"普通序列化": [37] -> 0a 00 14 00 9a 99 49 40 9a 99 49 40 0a 61 62 63 64 65 66 00 00 00 00 08 6c 02 00 00 04 05 00 00 00 00 00 00 00
"编码序列化": [26] -> 14 14 9a 99 49 40 9a 99 49 40 0a 61 62 63 64 65 66 00 00 00 00 08 ec 04 04 0a
"增加键值序列化": [53] -> 11 00 0a 00 25 00 14 00 39 00 9a 99 49 40 9a 99 49 40 4a 00 0a 61 62 63 64 65 66 00 00 00 00 50 00 08 66 00 6c 02 00 00 74 00 04 83 00 05 00 00 00 00 00 00 00 
"增加键值并编码序列化": [42] -> 11 00 14 25 00 14 39 00 9a 99 49 40 9a 99 49 40 4a 00 0a 61 62 63 64 65 66 00 00 00 00 50 00 08 66 00 ec 04 74 00 04 83 00 0a


"跨设备的序列化（携带基础信息）": [50] -> 11 00 80 14 25 00 80 14 39 00 80 9a 99 49 40 9a 99 49 40 4a 00 80 0a 61 62 63 64 65 66 00 00 00 00 50 00 80 08 66 00 80 ec 04 74 00 80 04 83 00 80 0a

"跨设备的序列化（携带所有信息）": [174] -> 11 00 ff 14 74 65 73 74 00 ff 14 c7 01 c8 01 25 00 ff 14 74 65 73 74 5f 32 00 01 14 00 64 39 00 ff 9a 99 49 40 9a 99 49 40 74 65 73 74 5f 33 00 03 9a 99 49 40 9a 99 49 40 00 00 20 c1 00 00 20 c1 00 00 20 41 00 00 20 41 4a 00 ff 0a 61 62 63 64 65 66 00 00 00 00 74 65 73 74 5f 73 74 72 00 03 0a 61 62 63 64 65 66 00 08 f6 0a 50 00 ff 08 74 65 73 74 5f 34 00 01 08 f6 0a 66 00 ff ec 04 74 65 73 74 5f 35 00 01 ec 04 f4 03 90 4e 74 00 ff 04 74 65 73 74 5f 36 00 01 04 05 64 83 00 ff 0a 74 65 73 74 5f 37 00 02 0a bb 08 94 55

```

## 使用说明
1.  通过函数 Param_ModifyById、Param_ModifyByName 限制修改参数
2.  调用函数 Param_Serialize 对参数序列化, 保存至储存芯片中
3.  从储存芯片读取数据后，调用 Param_Parse 反序列化得到参数，同时进行参数范围校验

## demo样式
博客：

[轻量级参数管理框架（C语言）](https://blog.csdn.net/qq_24130227/article/details/129233836?spm=1001.2014.3001.5501)

## 关于作者
1.  CSDN 博客 [大橙子疯](https://blog.csdn.net/qq_24130227?spm=1010.2135.3001.5343)
2.  联系邮箱 const_zpc@163.com
3.  了解更多可关注微信公众号

![大橙子疯嵌入式](微信公众号.jpg)