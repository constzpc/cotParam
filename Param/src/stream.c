#include "stream.h"
#include "param_cfg.h"
#include <string.h>

typedef union
{
    uint32_t data;
    float    fdata;
}unFloat_t;

typedef union
{
    uint64_t data;
    double   fdata;
}unDouble_t;

static uint8_t *VarintEncoded(uint8_t *pbuf, uint64_t val)
{
    uint64_t tmpVal = val;
    uint8_t *ptmpPtr = pbuf;

    do
    {
        *ptmpPtr++ = (uint8_t)tmpVal | 0x80;
        tmpVal >>= 7;
    } while (tmpVal >= 0x80);

    if (val >= 0x80)
    {
        *ptmpPtr++ = (uint8_t)tmpVal;
    }
    else
    {
        pbuf[0] &= ~0x80;
    }
    
    return ptmpPtr;
}

static uint8_t *VarintDecode(uint8_t *pbuf, uint64_t *pVal)
{
    uint8_t offset = 0;
    uint64_t result = 0;
    
    do
    {
        result |= (uint64_t)((*pbuf) & ~0x80) << offset;
        offset += 7;
    } while (((*pbuf++) & 0x80) != 0);
    
    *pVal = result;
    
    return pbuf;
}

static uint8_t *ZigzagEncoded(uint8_t *pbuf, int64_t val)
{
    return VarintEncoded(pbuf, (uint64_t)((val<<1)^(val>>63)));
}

static uint8_t *ZigzagDecode(uint8_t *pbuf, int64_t *pVal)
{
    uint64_t u64val;
    uint8_t *ptr = VarintDecode(pbuf, &u64val);
    
    *pVal = (int64_t)((u64val>>1) ^ - (u64val&1));

    return ptr;
}

static uint8_t *sMemPack(uint8_t *ptr, int64_t val, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        *ptr++ = val >> (i * 8);
    }

    return ptr;
}

static uint8_t *sMemUnPack(uint8_t *ptr, int64_t *pVal, uint8_t size)
{
    uint64_t result = 0;

    for (int i = 0; i < size; i++)
    {
        result |= ((int64_t)*ptr) << (i * 8);
        ptr++;
    }

    *pVal = result;

    return ptr;
}

static uint8_t *uMemPack(uint8_t *ptr, uint64_t val, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        *ptr++ = val >> (i * 8);
    }

    return ptr;
}

static uint8_t *uMemUnPack(uint8_t *ptr, uint64_t *pVal, uint8_t size)
{
    uint64_t result = 0;

    for (int i = 0; i < size; i++)
    {
        result |= ((uint64_t)*ptr) << (i * 8);
        ptr++;
    }

    *pVal = result;

    return ptr;
}

static uint8_t *PackFloat(uint8_t *ptr, float val)
{
    unFloat_t t;

    t.fdata = val;

    for (int i = 0; i < 4; i++)
    {
        *ptr++ = t.data >> (i * 8);
    }

    return ptr;
}

static uint8_t *UnPackFloat(uint8_t *ptr, float *pVal)
{
    unFloat_t t;
    t.data = 0;

    for (int i = 0; i < 4; i++)
    {
        t.data |= ((uint32_t)(*ptr) << (i * 8));
        ptr++;
    }

    *pVal = t.fdata;
    return ptr;
}

static uint8_t *PackDouble(uint8_t *ptr, double val)
{
    unDouble_t t;

    t.fdata = val;

    for (int i = 0; i < 8; i++)
    {
        *ptr++ = t.data >> (i * 8);
    }

    return ptr;
}

static uint8_t *UnPackDouble(uint8_t *ptr, double *pVal)
{
    unDouble_t t;
    t.data = 0;

    for (int i = 0; i < 8; i++)
    {
        t.data |= ((uint64_t)(*ptr) << (i * 8));
        ptr++;
    }

    *pVal = t.fdata;
    return ptr;
}

static uint8_t *PackTag(uint8_t *pbuf, const ParamInfo_t *pParam)
{
    uint8_t *ptr = pbuf;
    uint16_t tag = (pParam->id << 4) | pParam->type;

    pbuf[0] = tag & 0xFF;
    pbuf[1] = (tag >> 8) & 0xFF;
    ptr += 2;

    return ptr;
}

static uint8_t *UnPackTag(uint8_t *pbuf, uint16_t *pId, uint8_t *pType)
{
    uint8_t *ptr = pbuf;
    uint16_t tag = ((uint16_t)pbuf[1] << 8) | pbuf[0];

    *pId = tag >> 4;
    *pType = tag & 0x0f;

    ptr += 2;

    return ptr;
}

static ParamInfo_t *FindTag(uint16_t id, uint8_t type, const ParamInfo_t *pParamTab, uint16_t num)
{
    ParamInfo_t *pParam = NULL;

    for (uint16_t i = 0; i < num; i++)
    {
        if (id == pParamTab[i].id && type == pParamTab[i].type)
        {
            
            pParam = (ParamInfo_t *)&pParamTab[i];
            break;
        }
    }

    return pParam;
}

static uint8_t *StructPack(uint8_t *pbuf, const ParamInfo_t *pParam)
{
    uint8_t *ptr = pbuf;

    switch (pParam->type)
    {
    case PARAM_INT8:
        ptr = sMemPack(ptr, (*(int8_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_INT16:
        ptr = sMemPack(ptr, (*(int16_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_INT32:
        ptr = sMemPack(ptr, (*(int32_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_INT64:
        ptr = sMemPack(ptr, (*(int64_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_UINT8:
        ptr = uMemPack(ptr, (*(uint8_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_UINT16:
        ptr = uMemPack(ptr, (*(uint16_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_UINT32:
        ptr = uMemPack(ptr, (*(uint32_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_UINT64:
        ptr = uMemPack(ptr, (*(uint64_t *)pParam->pCurValue), pParam->length);
        break;
    case PARAM_FLOAT:
        ptr = PackFloat(ptr, *(float *)pParam->pCurValue);
        break;
    case PARAM_DOUBLE:
        ptr = PackDouble(ptr, *(double *)pParam->pCurValue);
        break;
    case PARAM_STARING:
        memcpy(ptr, pParam->pCurValue,  pParam->length);
        ptr += pParam->length;
        break;
    
    default:
        break;
    }

    return ptr;
}

static uint8_t *StructUnPack(uint8_t *pbuf, uint8_t type, const ParamInfo_t *pParam)
{
    uint8_t *ptr = pbuf;
    uint64_t u64Val;
    int64_t s64Val;
    float fVal;
    double dVal;

    switch (type)
    {
    case PARAM_INT8:
        ptr = sMemUnPack(ptr, &s64Val, pParam->length);
        if (pParam != NULL)
            (*(int8_t *)pParam->pCurValue) = (int8_t)s64Val;
        break;
    case PARAM_INT16:   
        ptr = sMemUnPack(ptr, &s64Val, pParam->length);
        if (pParam != NULL)
            (*(int16_t *)pParam->pCurValue) = (int16_t)s64Val;
        break;
    case PARAM_INT32:
        ptr = sMemUnPack(ptr, &s64Val, pParam->length);
        if (pParam != NULL)
            (*(int32_t *)pParam->pCurValue) = (int32_t)s64Val;
        break;
    case PARAM_INT64:
        ptr = sMemUnPack(ptr, &s64Val, pParam->length);
        if (pParam != NULL)
            (*(int64_t *)pParam->pCurValue) = s64Val;
        break;
    case PARAM_UINT8:
        ptr = uMemUnPack(ptr, &u64Val, pParam->length);
        if (pParam != NULL)
            (*(uint8_t *)pParam->pCurValue) = (uint8_t)u64Val;
        break;
    case PARAM_UINT16:
        ptr = uMemUnPack(ptr, &u64Val, pParam->length);
        if (pParam != NULL)
            (*(uint16_t *)pParam->pCurValue) = (uint16_t)u64Val;
        break;
    case PARAM_UINT32:
        ptr = uMemUnPack(ptr, &u64Val, pParam->length);
        if (pParam != NULL)
            (*(uint32_t *)pParam->pCurValue) = (uint32_t)u64Val;
        break;
    case PARAM_UINT64:
        ptr = uMemUnPack(ptr, &u64Val, pParam->length);
        if (pParam != NULL)
            (*(uint64_t *)pParam->pCurValue) = u64Val;
        break;
    case PARAM_FLOAT:
        ptr = UnPackFloat(ptr, &fVal);
        if (pParam != NULL)
            (*(float *)pParam->pCurValue) = fVal;
        break;
    case PARAM_DOUBLE:
        ptr = UnPackDouble(ptr, &dVal);
        if (pParam != NULL)
            (*(double *)pParam->pCurValue) = dVal;
        break;
    case PARAM_STARING:
        if (pParam != NULL)
            memcpy((void *)pParam->pCurValue, ptr, pParam->length);
        ptr += pParam->length;
        break;
    
    default:
        break;
    }

    return ptr;
}

static uint8_t *Encoded(uint8_t *pbuf, const ParamInfo_t *pParam)
{
    uint8_t *ptr = pbuf;

    switch (pParam->type)
    {
    case PARAM_INT8:
        *ptr++ = (uint8_t)(*(int8_t *)pParam->pCurValue);
        // ptr = ZigzagEncoded(ptr, (*(int8_t *)pParam->pCurValue));
        break;
    case PARAM_INT16:
        ptr = ZigzagEncoded(ptr, (*(int16_t *)pParam->pCurValue));
        break;
    case PARAM_INT32:
        ptr = ZigzagEncoded(ptr, (*(int32_t *)pParam->pCurValue));
        break;
    case PARAM_INT64:
        ptr = ZigzagEncoded(ptr, (*(int64_t *)pParam->pCurValue));
        break;
    case PARAM_UINT8:
        *ptr++ = (*(uint8_t *)pParam->pCurValue);
        // ptr = VarintEncoded(ptr, (*(uint8_t *)pParam->pCurValue));
        break;
    case PARAM_UINT16:
        ptr = VarintEncoded(ptr, (*(uint16_t *)pParam->pCurValue));
        break;
    case PARAM_UINT32:
        ptr = VarintEncoded(ptr, (*(uint32_t *)pParam->pCurValue));
        break;
    case PARAM_UINT64:
        ptr = VarintEncoded(ptr, (*(uint64_t *)pParam->pCurValue));
        break;

    case PARAM_FLOAT:
        ptr = PackFloat(ptr, *(float *)pParam->pCurValue);
        break;
    case PARAM_DOUBLE:
        ptr = PackDouble(ptr, *(double *)pParam->pCurValue);
        break;
    case PARAM_STARING:
    {
        uint16_t length = strlen((char *)pParam->pCurValue) > pParam->length ? pParam->length : strlen((char *)pParam->pCurValue);
        for (int i = 0; i < PARAM_STRING_SIZE; i++)
        {
            *ptr++ = (length >> (i * 8)) & 0xff;
        }
        memcpy(ptr, pParam->pCurValue,  length);
        ptr += length;
    }
        break;
    
    default:
        break;
    }

    return ptr;
}

static uint8_t *Decode(uint8_t *pbuf, uint8_t type, ParamInfo_t *pParam)
{
    uint8_t *ptr = pbuf;
    uint64_t u64Val;
    int64_t s64Val;
    float fVal;
    double dVal;

    switch (type)
    {
    case PARAM_INT8:
        if (pParam != NULL)
            (*(int8_t *)pParam->pCurValue) = *(int8_t *)ptr;
        ptr++;
        // ptr = ZigzagDecode(ptr, &s64Val);
        // (*(int8_t *)pParam->pCurValue) = (int8_t)s64Val;
        break;
    case PARAM_INT16:   
        ptr = ZigzagDecode(ptr, &s64Val);
        if (pParam != NULL)
            (*(int16_t *)pParam->pCurValue) = (int16_t)s64Val;
        break;
    case PARAM_INT32:
        ptr = ZigzagDecode(ptr, &s64Val);
        if (pParam != NULL)
            (*(int32_t *)pParam->pCurValue) = (int32_t)s64Val;
        break;
    case PARAM_INT64:
        ptr = ZigzagDecode(ptr, &s64Val);
        if (pParam != NULL)
            (*(int64_t *)pParam->pCurValue) = s64Val;
        break;
    case PARAM_UINT8:
        if (pParam != NULL)
            (*(uint8_t *)pParam->pCurValue) = *(uint8_t *)ptr;
        ptr++;
        // ptr = VarintDecode(ptr, &u64Val);
        // (*(uint8_t *)pParam->pCurValue) = (uint8_t)u64Val;
        break;
    case PARAM_UINT16:
        ptr = VarintDecode(ptr, &u64Val);
        if (pParam != NULL)
            (*(uint16_t *)pParam->pCurValue) = (uint16_t)u64Val;
        break;
    case PARAM_UINT32:
        ptr = VarintDecode(ptr, &u64Val);
        if (pParam != NULL)
            (*(uint32_t *)pParam->pCurValue) = (uint32_t)u64Val;
        break;
    case PARAM_UINT64:
        ptr = VarintDecode(ptr, &u64Val);
        if (pParam != NULL)
            (*(uint64_t *)pParam->pCurValue) = u64Val;
        break;
    case PARAM_FLOAT:
        ptr = UnPackFloat(ptr, &fVal);
        if (pParam != NULL)
            (*(float *)pParam->pCurValue) = fVal;
        break;
    case PARAM_DOUBLE:
        ptr = UnPackDouble(ptr, &dVal);
        if (pParam != NULL)
            (*(double *)pParam->pCurValue) = dVal;
        break;
    case PARAM_STARING:
    {
        uint16_t length = 0;
        for (int i = 0; i < PARAM_STRING_SIZE; i++)
        {
            length |= ((uint16_t)*ptr) << (i * 8);
            ptr++;
        }
        if (pParam != NULL)
            memcpy((void *)pParam->pCurValue, ptr, length);
        ptr += length;
    }
        break;
    
    default:
        break;
    }

    return ptr;
}

/**
  * @brief      将数据信息序列化为二进制数据流
  * 
  * @param[out] pbuf        序列化后的二进制数据流
  * @param[in]  pParamTab   参数表
  * @param[in]  num         参数表元素数目
  * @return     序列化后的二进制数据流长度 
  */
uint32_t StreamToBin(uint8_t *pbuf, const ParamInfo_t *pParamTab, uint16_t num)
{
    uint8_t *ptr = pbuf;
    
#if ((PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_STRUCT) || (PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_ENCODED))
    uint16_t cnt = num;

    for (uint16_t id = 0; id < 0xfff && cnt > 0; id++)
    {
        for (uint16_t j = 0; j < num; j++)
        {
            if (id == pParamTab[j].id)
            {
#if (PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_ENCODED)
                ptr = Encoded(ptr, &pParamTab[j]);
#else
                ptr = StructPack(ptr, &pParamTab[j]);
#endif
                cnt--;
                break;
            }
        }
    }
#else
    for (uint16_t i = 0; i < num; i++)
    {
        ptr = PackTag(ptr, &pParamTab[i]);
#if (PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_TAG_ENCODED)
        ptr = Encoded(ptr, &pParamTab[i]);
#else
        ptr = StructPack(ptr, &pParamTab[i]);
#endif
    }
#endif 

    return ptr - pbuf;
}

/**
  * @brief      将二进制数据流反序列化为数据信息
  * 
  * @param[in]  pbuf      需要反序列化后的二进制数据流
  * @param[in]  length    需要反序列化后的二进制数据流长度 
  * @param[in,out]  pParamTab 参数表
  * @param[in]  num       参数表元素数目 
  */
void StreamFromBin(uint8_t *pbuf, uint32_t length, ParamInfo_t *pParamTab, uint16_t num)
{
    uint8_t *ptr = pbuf;
    uint16_t id;
    uint8_t type;

#if ((PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_STRUCT) || (PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_ENCODED))
    uint16_t cnt = num;

    for (uint16_t id = 0; id < 0xfff && cnt > 0; id++)
    {
        for (uint16_t j = 0; j < num; j++)
        {
            if (id == pParamTab[j].id)
            {
#if (PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_ENCODED)
                ptr = Decode(ptr, pParamTab[j].type, &pParamTab[j]);
#else
                ptr = StructUnPack(ptr, pParamTab[j].type, &pParamTab[j]);
#endif
                cnt--;
            }
        }
    }
#else
    while (ptr - pbuf < length)
    {
        ptr = UnPackTag(ptr, &id, &type);
#if (PARAM_SERIALIZE_MODE == PARAM_SERIALIZE_TAG_ENCODED)
        ptr = Decode(ptr, type, FindTag(id, type, pParamTab, num));
#else
        ptr = StructUnPack(ptr, type, FindTag(id, type, pParamTab, num));
#endif
        
        
    }
#endif 
}
