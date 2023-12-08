# 轻量级参数管理框架(C语言)

## 介绍

### 管理方式

通过将已定义变量添加到参数表进行参数的统一管理，包括缺省值、最小值和最大值等，方便进行参数校验纠正处理。

> 已定义变量也包括结构体的成员变量，均为**全局变量**。

- 支持将已定义变量绑定为参数，无缺省值、最小值和最大值限制，适合于记录类型的参数
- 支持将已定义变量绑定为参数，有缺省值，但无最小值和最大值限制，适合于配置类型的参数
- 支持将已定义变量绑定为参数，有缺省值，最小值和最大值限制，适合于用户设置或者关键性类型的参数

除此之外，还有参数名、属性等，方便配合UI或者参数的权限管理。

同时若单个参数表无法满足参数数目或者参数分类管理，可定义多张参数表

> 每张参数表中的参数ID唯一，不可重复；但不同参数表ID可以相同

### 参数类型

支持数值和字符串两种类型参数。

> - 数值类型：`int`、`float`、`double` 等基本类型的参数
> - 字符串类型：`char` 定义用来储存字符串的数组



### 兼容性

* [X] 提供了参数表的序列化和反序列化操作。

> - 方便在本地储存设备（如flash、eeprom等）保存/读取二进制数据，甚至还可以跨设备传输
> - 提供了两种方式：
>   - 保存/加载：提供参数实际保存/加载的回调函数，通过多次触发回调函数完成参数的序列化保存、加载反序列化功能；适用于小内存的平台使用（不需要申请内存处理）
>   - 序列化和反序列化：需要提前申请内存用来保存参数表序列化的数据或者读取即将反序列化的数据；一次性完成操作（需要申请较大的内存完成）

* [X] 支持启用键值对功能

> - 每个参数都需要指定唯一的ID，在后期版本迭代对参数表删除、插入或添加参数时也能向下兼容，不会影响其他参数。
> - 启用键值对后序列化的数据长度也会比较大，因为每个参数序列化时包含了ID和长度信息


### 可裁剪

根据不同的平台，可以对部分功能裁剪，或者修改配置适用于不同容量的芯片进行开发。

| 配置选项                        | 描述                                                                                                                                                                                                                                                                                                                                                   |
| ------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `COT_PARAM_USE_KEY_VALUE`     | 是否采用键值对方式序列化和反序列化参数数据                                                                                                                                                                                                                                                                                                             |
| `COT_PARAM_USE_STRING_TYPE`   | 启用字符串参数类型                                                                                                                                                                                                                                                                                                                                     |
| `COT_PARAM_USE_64_BIT_LENGTH` | 启用64bit的参数类型                                                                                                                                                                                                                                                                                                                                    |
| `COT_PARAM_NAME_MAX_LENGTH`   | 参数名字最大定义长度，小于或等于1则禁用参数名功能                                                                                                                                                                                                                                                                                                      |
| `COT_PARAM_STRING_MAX_LENGTH` | 字符串类型的参数取值最大定义长度（包括结束符），需启用 `COT_PARAM_USE_STRING_TYPE`                                                                                                                                                                                                                                                                   |
| `COT_PARAM_SUPPORT_NUM`       | 单张参数表最多添加多少个参数，需启用 `COT_PARAM_USE_KEY_VALUE`，可选:<br />`COT_PARAM_SUPPORT_16`：ID取值范围0-15，即最多16个参数<br />`COT_PARAM_SUPPORT_256`：ID取值范围0-255，即最多256个参数<br /> `COT_PARAM_SUPPORT_4096`：ID取值范围0-4095，即最多4096个参数<br /><br />注：若没有启用 `COT_PARAM_USE_KEY_VALUE` 键值对方式，则无限制 |


## 软件设计

略


## 使用说明

### 参数表定义


```c
typedef struct
{
    uint16_t test1;
    float test2;
    char str[12];
}ParamDemo_t;

static ParamDemo_t sg_tTest = {
    .str = "sdf",
    .test1 = 20,
    .test2 = 567.4
};

int16_t g_test = 50;
uint16_t g_test_2 = 20;
double g_test_3 = 3.15;
COT_PARAM_INT8_T g_test_4 = 8;
COT_PARAM_UINT32_T g_test_5 = 620;
COT_PARAM_UINT8_T g_test_6 = 45;
COT_PARAM_INT64_T g_test_7 = 5;
COT_PARAM_INT8_T g_sd = 2;
COT_PARAM_INT64_T g_test_88 = 80;
char g_test_str[15] = "abcdef";
char g_str_des[13] = "EERR";
char g_str_des_2[15] = "sdRR";
char g_str_des_3[15] = "ewRR";

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_test, COT_PARAM_INT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND_WITH_NAME(2, "g_test_2", g_test_2, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_test_3, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 3.15, -2.15, 5.12),
    COT_PARAM_ITEM_BIND(4, g_test_str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcdef", 0, sizeof(g_test_str)),
    COT_PARAM_ITEM_BIND(5, g_test_4, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 8, -10, 10),
    COT_PARAM_ITEM_BIND(6, g_test_5, COT_PARAM_UINT32, COT_PARAM_ATTR_WR, 620, 500, 10000),
    COT_PARAM_ITEM_BIND(7, g_test_6, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 45, 5, 100),
    COT_PARAM_ITEM_BIND(8, g_test_7, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 5, -542, 5450),
    COT_PARAM_ITEM_BIND(9, sg_tTest.test1, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20, 10, 2000),
    COT_PARAM_ITEM_BIND(10, sg_tTest.test2, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(11, sg_tTest.str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "const-zpc", 6, sizeof(sg_tTest.str)),
    COT_PARAM_ITEM_BIND(12, g_test_88, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 5, -542, 5450),
    COT_PARAM_ITEM_BIND(13, g_str_des, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "WER45", 10, sizeof(g_str_des)),
    COT_PARAM_ITEM_BIND(14, g_str_des_2, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "WTG"),
    COT_PARAM_ITEM_BIND(15, g_str_des_3, COT_PARAM_STRING, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(17, g_sd, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 5),
};

static cotParamManager_t sg_tParamManager;

int mian()
{
    cotParam_Init(&sg_tParamManager, sg_ParamTable, COT_PARAM_TABLE_SIZE(sg_ParamTable));
}


```



### 参数保存/加载

1. 保存/加载方式（函数内部完成序列化和反序列化，逐步写入保存/读取加载）

```c

// 所有参数校验出错时恢复默认处理
int OnCheckErrorResetHandle(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult)
{
    cotParam_SingleParamResetDefValue(pParamInfo);
    return 0;
}

// 从储存设备多次读取
int OnLoadCallback(uint8_t *pBuf, uint16_t *len, bool *pisFinish)
{
    uint16_t needReadLen = *len;
    static uint32_t s_already_read_length = 0;

    if (sg_length == s_offset)
    {
        *len = 0;
        s_already_read_length = 0;
        *pisFinish = true;
        return 0;
    }
    else
    {
        *pisFinish = false;
    }

    if (sg_length - s_already_read_length < needReadLen)
    {
        needReadLen = sg_length - s_already_read_length ;
    }

    *len = read(pBuf, needReadLen);
    s_already_read_length += (*len);

    return 0;
}
//函数中使用
int LoadParam()
{
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
        cotParam_Load(&sg_tParamManager, OnLoadCallback);
}
```



2. 序列化/反序列化方式（一次性写入保存/读取加载）

```c
//函数中使用
int main()
{
#if 0
    uint8_t buf[500];
  
    uint32_t length = cotParam_Serialize(&sg_tParamManager, buf);
    write(buf, length);
#else
    uint8_t *p_buf = (uint8_t *)malloc(cotParam_GetSerializeSize(&sg_tParamManager));
  
    uint32_t length = cotParam_Serialize(&sg_tParamManager, p_buf);
    write(p_buf, length);
    free(p_buf);
    p_buf = NULL;
#endif
}
```

### 校验处理

```c
// 对某个变量参数进行范围校验，得到校验结果
cotParam_SingleParamCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), &eCheckResult);

// 对某个变量参数变更后（当前值已经变化）进行校验处理，若超出范围则恢复默认
g_test_3 = 1000;
cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), COT_PARAM_RESET_DEF);

// 对某个变量参数在需要变更前（当前值没有变化）进行校验处理，若新的值超出范围则不更新变量参数当前的值
double tmp = 1000;
cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), &tmp, COT_PARAM_RESET_NONE)
```

## demo样式

博客：

[轻量级参数管理框架（C语言）](https://blog.csdn.net/qq_24130227/article/details/129233836?spm=1001.2014.3001.5501)

## 关于作者

1. CSDN 博客 [大橙子疯](https://blog.csdn.net/qq_24130227?spm=1010.2135.3001.5343)
2. 联系邮箱 const_zpc@163.com
3. 了解更多可关注微信公众号

![大橙子疯嵌入式](微信公众号.jpg)
