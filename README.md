# 轻量级参数管理框架(C语言)

嵌入式软件中的系统数据参数是指在嵌入式系统中用于实现系统功能和控制的各种数据，如用户参数、状态、配置信息等；那么如何管理这些数据对于嵌入式系统的正确运行和维护非常重要。

该参数管理框架代码就是如何统一管理软件中的各类系统数据参数。

> 该参数管理并不涉及数据是如何储存的。因为有些系统数据并不需要储存起来，只需要进行管理而已。

## 介绍

### 参数管理

* [X] 通过将已定义变量（**全局变量**）添加到参数表进行参数的统一管理，单个参数包括了当前值、缺省值、最小值、最大值、参数名和属性等信息。

  > - 当前值：已定义的变量
  > - 缺省值：默认值，并非变量初值
  > - 最小值：该参数的最小值
  > - 最大值：该参数的最大值
  > - 参数名：对该参数的描述
  > - 属性：有多种属性信息，方便后续功能扩展
  >   - 读/写权限：参数模块中无具体作用，可用于UI或者其他方式显示时使用
  >   - 重置权限：设置默认值后则存在该属性
  >   - 校验权限：设置最大最小值后则存在该属性
  >
* [X] 根据不同的场景管理不同的参数

  > - 无缺省值、最小值和最大值限制，适合于记录类型的参数，比如状态数据或历史数据等
  > - 有缺省值，但无最小值和最大值限制，适合于配置类型的参数
  > - 有缺省值，最小值和最大值限制，适合于关键性类型的参数，比如用户参数或者关键的状态数据等
  >
* [X] 同时若单个参数表无法满足参数数目或者参数分类管理，可定义多张参数表

  > - 每张参数表中的参数ID唯一，不可重复；
  > - 不同参数表ID可以重复定义
  >

### 参数类型

* [X] 数值类型参数

  > `int`、`float`、`double` 等基本类型的参数
  >
* [X] 字符串类型参数

  > `char` 定义用来储存字符串的数组
  >

### 参数校验

为了更好的统一管理参数，可以对参数设置的范围进行校验，防止参数设置超出预期范围，导致程序不可控。

* [X] 范围校验

  > 根据参数的最大和最小值进行判断，数值类型的参数则根据数值超出范围判断。而字符串则是根据字符串长度超出范围判断。
  >
* [X] 自定义校验

  > 提供回调函数，每个参数可设置自定义的校验方式，比如某个参数需要设置为多少的倍数，或者根据其他参数决定当前参数的取值范围等。
  >

上述两种校验方式均需要参数设置缺省值，最小值和最大值后才有效。

### 兼容性

* [X] 提供了参数表的序列化和反序列化操作。

  > - 方便在本地储存设备（如flash、eeprom等）保存/读取二进制数据，甚至还可以跨设备传输使用
  > - 提供了两种方式：
  >   - 第一种：只需要提供参数数据保存/加载的回调函数，调用相关接口函数完成参数的序列化保存和反序列化加载；该方式会多次触发回调函数保存数据和读取数据，适用于小内存的平台使用（不需要申请内存）
  >   - 第二种：需要提前申请内存用来保存参数表序列化的数据或者读取即将反序列化的数据；一次性完成操作（需要申请较大的内存完成）
  >
* [X] 支持启用键值对功能

  > - 每个参数都需要指定唯一的ID，在后期版本迭代对参数表删除、插入或添加参数时也能向下兼容，不会影响其他参数。
  > - 即使更新了参数表的最大容纳范围，也能保证兼容，比如上个版本配置参数只支持16个，当前版本配置支持了256个，也能正确处理。
  > - 启用键值对后序列化的数据长度也会比较大，因为每个参数序列化时包含了ID和长度信息。
  >

### 可裁剪

根据不同的平台，可以对部分功能裁剪，或者修改配置适用于不同容量的芯片进行开发。

| 配置选项                        | 描述                                                                                                                                                                                                                                                                                                                                                   |
| ------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `COT_PARAM_USE_KEY_VALUE`     | 是否采用键值对方式序列化和反序列化参数数据                                                                                                                                                                                                                                                                                                             |
| `COT_PARAM_USE_CUSTOM_CHECK`  | 是否启用参数自定义校验功能                                                                                                                                                                                                                                                                                                                             |
| `COT_PARAM_USE_STRING_TYPE`   | 是否启用字符串参数类型                                                                                                                                                                                                                                                                                                                                 |
| `COT_PARAM_USE_64_BIT_LENGTH` | 是否启用64bit的参数类型                                                                                                                                                                                                                                                                                                                                |
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
    uint16_t usValue;
    uint8_t ucValue;
    uint32_t uiValue;
    float fValue;
    char szString_1[12];
    double dValue;
    int16_t sValue;
    int8_t cValue;
    int32_t iValue;
    char szString_2[10];
}ParamDemo_t;

ParamDemo_t g_tTestVal = {
    .usValue = 20,
    .ucValue = 10,
    .uiValue = 1000,
    .fValue = 3.14,
    .szString_1 = "abcd",
    .dValue = 5.12,
    .sValue = -100,
    .cValue = -2,
    .iValue = 300,
    .szString_2 = "12234",
};

int8_t g_cTest = 50;
char g_szString[10] = "qwer";

static int CheckSValue(const void *pCurParam);

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_tTestVal.usValue, COT_PARAM_UINT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(2, g_tTestVal.ucValue, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_tTestVal.uiValue, COT_PARAM_UINT32, COT_PARAM_ATTR_WR, 1000, 1000, 10000),
    COT_PARAM_ITEM_BIND(4, g_tTestVal.fValue, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR, 10, -10.5, 10.5),
    COT_PARAM_ITEM_BIND(5, g_tTestVal.szString_1, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcd", 3, sizeof(g_tTestVal.szString_1)),
    COT_PARAM_ITEM_BIND(6, g_tTestVal.dValue, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 0, -90.10, 100.10),
    COT_PARAM_ITEM_BIND(7, g_tTestVal.sValue, COT_PARAM_INT16, COT_PARAM_ATTR_WR, 100, -200, 200, CheckSValue), // 添加自定义校验
    COT_PARAM_ITEM_BIND_WITH_NAME(8, "g_cTest", g_cTest, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 50, -100, 100), // 另取参数名
    COT_PARAM_ITEM_BIND(9, g_szString, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "XXX", 3, 6),
};

static int CheckSValue(const void *pCurParam)
{
    const int16_t *p_sValue = (const int16_t *)pCurParam;

    if ((*p_sValue) % 2 != 0)
    {
        return -1;
    }

    return 0;
}


int mian()
{
    cotParam_Init(&sg_tParamManager, sg_ParamTable, COT_PARAM_TABLE_SIZE(sg_ParamTable));
}


```

### 参数保存/加载

将参数表的部分信息（包括当前值）进行序列化，方便进行数据保存，等下次开机再读取数据后反序列化，还原成参数表的这部分信息。

> 甚至将这部分数据传输给另一台设备，进行参数数据备份或同步等功能

将参数表序列化完成后，可以对数据进行二次封装，比如可以增加头信息，crc校验等再进行数据保存，同样在加载时也需要解析，将额外添加的信息去除后才能进行反序列化。

##### 第一种

提供参数数据保存/加载的回调函数，调用相关接口函数完成参数的序列化保存和反序列化加载；该方式会多次触发回调函数保存数据和读取数据，适用于小内存的平台使用（不需要额外申请内存）

```c

// 所有参数校验出错时恢复默认处理
int OnCheckErrorResetHandle(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult)
{
    cotParam_SingleParamResetDefValue(pParamInfo);
    return 0;
}

// 从储存空间读取数据
int OnLoadCallback(uint8_t *pBuf, uint16_t bufSize, uint16_t *pLength)
{
    uint16_t length;
    static uint32_t s_offset = 0;

    if (s_offset < sg_length)
    {
        if (s_offset + bufSize <= sg_length)
        {
            length = bufSize;
            memcpy(pBuf, &sg_buf[s_offset], length);
            s_offset += length;
        }
        else
        {
            length = sg_length - s_offset;
            memcpy(pBuf, &sg_buf[s_offset], length);
            s_offset += length;
        }
    }
    else
    {
        length = 0;
        s_offset = 0;
    }

    *pLength = length;
    return 0;
}

// 写数据至储存空间
int OnSaveCallback(const uint8_t *pBuf, uint16_t len)
{
    static uint32_t s_offset = 0;

    if (len > 0)
    {
        memcpy(&sg_buf[s_offset], pBuf, len);
        s_offset += len;
        sg_length = s_offset;
    }
    else
    {
        s_offset = 0;
    }

    return 0;
}

// 加载参数
void LoadParam(void)
{
    cotParam_Load(&sg_tParamManager, OnLoadCallback);
  
    // 加载后全部参数进行校验（可选）
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
}

// 保存参数
void SaveParam(void)
{
    // 保存前全部参数进行校验（可选）
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    cotParam_Save(&sg_tParamManager, OnSaveCallback);
}

```

#### 第二种

需要提前申请内存用来保存参数表序列化的数据或者读取即将反序列化的数据。

1. 定义buf

```c
// 加载参数
void LoadParam(void)
{
    uint8_t buf[500];
    uint32_t length;

    length = read(buf);
    cotParam_Deserialization(&sg_tParamManager, buf, length);
  
    // 加载后全部参数进行校验（可选）
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
}

// 保存参数
void SaveParam(void)
{
    uint8_t buf[500];
    uint32_t length;

    // 保存前全部参数进行校验（可选）
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    length = cotParam_Serialize(&sg_tParamManager, buf);
    write(buf, length);
}

```

2. buf通过内存申请

```c
// 加载参数
void LoadParam(void)
{
    uint8_t pBuf = (uint8_t *)malloc(500);  // 申请足够大的内存
    uint32_t length;

    length = read(pBuf, length); // 获取数据的实际长度
    cotParam_Deserialization(&sg_tParamManager, pBuf, length);
  
    // 加载后全部参数进行校验（可选）
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
}

// 保存参数
void SaveParam(void)
{
    uint8_t pBuf = (uint8_t *)malloc(cotParam_GetSerializeSize(&sg_tParamManager));
    uint32_t length;

    // 保存前全部参数进行校验（可选）
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    length = cotParam_Serialize(&sg_tParamManager, pBuf);
    write(buf, length);
}

int main()
{
    // 保存
    uint8_t *p_wbuf = (uint8_t *)malloc(cotParam_GetSerializeSize(&sg_tParamManager));
  
    uint32_t length = cotParam_Serialize(&sg_tParamManager, p_wbuf);
    write(p_wbuf, length);
    free(p_wbuf);
    p_wbuf = NULL;

    // 加载
    uint8_t *p_rbuf = (uint8_t *)malloc(cotParam_GetSerializeSize(&sg_tParamManager));
  
    uint32_t length = read(r_buf);
    cotParam_Deserialization(&sg_tParamManager, p_rbuf, length);
    write(p_rbuf, length);
    free(p_rbuf);
    p_rbuf = NULL;
}
```

### 校验方式

校验需要提前在参数表中设置缺省值，最小值和最大值后才有效。

#### 范围校验

根据参数设置范围进行校验。

```c
// 对某个变量当前参数进行范围校验，得到校验结果后自行处理
cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), &eCheckResult);

if (eCheckResult != COT_PARAM_CHECK_OK) // 修改后检查
{
    cotParam_SingleParamResetDefValue(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3)); // 如果校验失败，则恢复为默认值
}


// 对某个变量参数变更后（当前值已经变化）进行校验处理，若超出范围则恢复默认
g_test_3 = 1000;
cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), COT_PARAM_RESET_DEF);

// 对某个变量参数在需要变更前（当前值没有变化）进行校验处理，得到校验结果后自行处理
double tmp = 1000;
cotParam_SingleParamCheckInput(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), &tmp, &eCheckResult);

if (eCheckResult == COT_PARAM_CHECK_OK)
{
    g_test_3 = tmp;// 如果校验成功，则修改
}

// 对某个变量参数在需要变更前（当前值没有变化）进行校验处理，若新的值超出范围则不更新变量参数当前的值
double tmp = 1000;
cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_3), &tmp, COT_PARAM_RESET_NONE)

```

#### 自定义校验

在符合参数范围内，根据参数设置的自定义校验函数进行校验。

```c
static int CheckTestS16(const void *pCurParam)
{
    const int16_t *p_test_s16 = (const int16_t *)pCurParam;

    /* 需要校验 g_test_s16 为2的倍数（建议不能直接使用 g_test_s16 去判断，因为回调函数入参不一定时该值，
       虽然类型是一样的）*/
    if ((*p_test_s16) % 2 != 0)  
    {
        return -1;
    }

    return 0; // 0表示自定义校验成功，其他表示失败
}

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_test_1, COT_PARAM_INT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(2, g_test_2, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_test_float, COT_PARAM_FLOAT, COT_PARAM_ATTR_READ, 3.15, -10, 10),
    COT_PARAM_ITEM_BIND(6, g_test_s8, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 10, -10, 15),
    COT_PARAM_ITEM_BIND(7, g_test_s16, COT_PARAM_INT16, COT_PARAM_ATTR_WR, 100, -100, 3000, CheckTestS16), // 设置自定义校验
    COT_PARAM_ITEM_BIND(8, g_test_s32, COT_PARAM_INT32, COT_PARAM_ATTR_WR, 1000, -900, 10000),
}

```


## demo样式

博客：

[轻量级参数管理框架（C语言）](https://blog.csdn.net/qq_24130227/article/details/129233836?spm=1001.2014.3001.5501)

## 关于作者

1. CSDN 博客 [大橙子疯](https://blog.csdn.net/qq_24130227?spm=1010.2135.3001.5343)
2. 联系邮箱 const_zpc@163.com
3. 了解更多可关注微信公众号

![大橙子疯嵌入式](微信公众号.jpg)

如果对你有帮助，可以多多支持一下，感谢！

![收款码](收款码.jpg)
