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

static uint8_t *PackInt8(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        *ptr++ = (uint8_t)(*(int8_t *)pVal);
    }
    else
    {
        ptr = sMemPack(ptr, (*(int8_t *)pVal), sizeof(int8_t));
    }

    return ptr;
}

static uint8_t *PackInt16(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        ptr = ZigzagEncoded(ptr, (*(int16_t *)pVal));
    }
    else
    {
        ptr = sMemPack(ptr, (*(int16_t *)pVal), sizeof(int16_t));
    }

    return ptr;
}

static uint8_t *PackInt32(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        ptr = ZigzagEncoded(ptr, (*(int32_t *)pVal));
    }
    else
    {
        ptr = sMemPack(ptr, (*(int32_t *)pVal), sizeof(int32_t));
    }

    return ptr;
}

static uint8_t *PackInt64(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        ptr = ZigzagEncoded(ptr, (*(int64_t *)pVal));
    }
    else
    {
        ptr = sMemPack(ptr, (*(int64_t *)pVal), sizeof(int64_t));
    }

    return ptr;
}

static uint8_t *PackUint8(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        *ptr++ = (*(uint8_t *)pVal);
    }
    else
    {
        ptr = uMemPack(ptr, (*(uint8_t *)pVal), sizeof(uint8_t));
    }

    return ptr;
}

static uint8_t *PackUint16(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        ptr = VarintEncoded(ptr, (*(uint16_t *)pVal));
    }
    else
    {
        ptr = uMemPack(ptr, (*(uint16_t *)pVal), sizeof(uint16_t));
    }

    return ptr;
}

static uint8_t *PackUint32(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        ptr = VarintEncoded(ptr, (*(uint32_t *)pVal));
    }
    else
    {
        ptr = uMemPack(ptr, (*(uint32_t *)pVal), sizeof(uint32_t));
    }

    return ptr;
}

static uint8_t *PackUint64(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    if (isEncoded)
    {
        ptr = VarintEncoded(ptr, (*(uint64_t *)pVal));
    }
    else
    {
        ptr = uMemPack(ptr, (*(uint64_t *)pVal), sizeof(uint64_t));
    }

    return ptr;
}

static uint8_t *PackFloat(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    unFloat_t t;

    t.fdata = *(float *)pVal;

    for (int i = 0; i < 4; i++)
    {
        *ptr++ = t.data >> (i * 8);
    }

    return ptr;
}

static uint8_t *PackDouble(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    unFloat_t t;

    t.fdata = *(double *)pVal;

    for (int i = 0; i < 8; i++)
    {
        *ptr++ = t.data >> (i * 8);
    }

    return ptr;
}

static uint8_t *PackString(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded)
{
    // if (isEncoded)
    // {
    //     length = strlen((char *)pVal) > length ? length : strlen((char *)pVal);
    // }

    for (int i = 0; i < PARAM_STRING_SIZE; i++)
    {
        *ptr++ = (length >> (i * 8)) & 0xff;
    }

    memcpy(ptr, pVal,  length);
    ptr += length;

    return ptr;
}

typedef uint8_t *(*PackFun_t)(uint8_t *ptr, const void *pVal, uint16_t length, uint8_t isEncoded);

PackFun_t packFunTable[]={
    PackInt8,
    PackInt16,
    PackInt32,
    PackInt64,
    PackUint8,
    PackUint16,
    PackUint32,
    PackUint64,
    PackFloat,
    PackDouble,
    PackString
};

static uint8_t *PackData(uint8_t *pbuf, uint8_t type, void *pVal, uint16_t length, uint8_t isEncoded)
{
    return packFunTable[type](pbuf, pVal, length, isEncoded);
}

uint8_t *PackParam(uint8_t *pBuf, const ParamInfo_t *pParam, uint8_t isUseEncoded, uint8_t opt)
{
    pBuf = PackData(pBuf, pParam->type, pParam->pCurValue, pParam->length, isUseEncoded);

    if (opt & PARAM_PACK_NAME)
    {
        uint16_t length = strlen(pParam->pszName) >= PARAM_NAME_MAX_LENGTH ? PARAM_NAME_MAX_LENGTH - 1 : strlen(pParam->pszName);
        memcpy(pBuf, pParam->pszName, length + 1);
        pBuf += (length + 1);
    }

    if (opt & PARAM_PACK_ATTR)
    {
        pBuf = PackUint8(pBuf, &pParam->attr, 1, 0);
    }

    if (opt & PARAM_PACK_DEF_VAL)
    {
        pBuf = PackData(pBuf, pParam->type, pParam->pDefValue, pParam->length, isUseEncoded);
    }

    if ((opt & PARAM_PACK_MIN_VAL) && pParam->type != PARAM_STARING)
    {
        pBuf = PackData(pBuf, pParam->type, pParam->pMinValue, pParam->length, isUseEncoded);
    }

    if ((opt & PARAM_PACK_MAX_VAL) && pParam->type != PARAM_STARING)
    {
        pBuf = PackData(pBuf, pParam->type, pParam->pMaxValue, pParam->length, isUseEncoded);
    }

    return pBuf;
}

uint8_t *PackTagAndParam(uint8_t *pBuf, const ParamInfo_t *pParam, uint8_t isUseEncoded, uint8_t opt)
{
    isUseEncoded = isUseEncoded > 0 ? 1 : 0;
    pBuf = PackTag(pBuf, pParam);
    pBuf = PackParam(pBuf, pParam, isUseEncoded, opt);

    return pBuf;
}

uint8_t *PackTagAndOptAndParam(uint8_t *pBuf, const ParamInfo_t *pParam, uint8_t isUseEncoded, uint8_t opt)
{
    isUseEncoded = isUseEncoded > 0 ? 1 : 0;
    pBuf = PackTag(pBuf, pParam);
    *pBuf++ = (isUseEncoded << 7) |  (opt & 0x7f);

    pBuf = PackParam(pBuf, pParam, isUseEncoded, opt);

    return pBuf;
}

static uint8_t *UnPackInt8(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    int64_t s64Val;

    if (isDecode)
    {
        s64Val = *(int8_t *)ptr;
        ptr++;
    }
    else
    {
        ptr = sMemUnPack(ptr, &s64Val, sizeof(int8_t));
    }

    if (pVal != NULL)
    {
        *(int8_t *)pVal = (int8_t)s64Val;
    }

    *pLength = sizeof(int8_t);

    return ptr;
}

static uint8_t *UnPackInt16(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    int64_t s64Val;

    if (isDecode)
    {
        ptr = ZigzagDecode(ptr, &s64Val);
    }
    else
    {
        ptr = sMemUnPack(ptr, &s64Val, sizeof(int16_t));
    }

    if (pVal != NULL)
    {
        *(int16_t *)pVal = (int16_t)s64Val;
    }

    *pLength = sizeof(int16_t);

    return ptr;
}

static uint8_t *UnPackInt32(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    int64_t s64Val;

    if (isDecode)
    {
        ptr = ZigzagDecode(ptr, &s64Val);
    }
    else
    {
        ptr = sMemUnPack(ptr, &s64Val, sizeof(int32_t));
    }

    if (pVal != NULL)
    {
        *(int32_t *)pVal = (int32_t)s64Val;
    }

    *pLength = sizeof(int32_t);

    return ptr;
}

static uint8_t *UnPackInt64(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    int64_t s64Val;

    if (isDecode)
    {
        ptr = ZigzagDecode(ptr, &s64Val);
    }
    else
    {
        ptr = sMemUnPack(ptr, &s64Val, sizeof(int64_t));
    }

    if (pVal != NULL)
    {
        *(int64_t *)pVal = (int64_t)s64Val;
    }

    *pLength = sizeof(int64_t);

    return ptr;
}



static uint8_t *UnPackUint8(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    uint64_t u64Val;

    if (isDecode)
    {
        u64Val = *(uint8_t *)ptr;
        ptr++;
    }
    else
    {
        ptr = uMemUnPack(ptr, &u64Val, sizeof(uint8_t));
    }

    if (pVal != NULL)
    {
        *(uint8_t *)pVal = (uint8_t)u64Val;
    }

    *pLength = sizeof(uint8_t);

    return ptr;
}

static uint8_t *UnPackUint16(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    uint64_t u64Val;

    if (isDecode)
    {
        ptr = VarintDecode(ptr, &u64Val);
    }
    else
    {
        ptr = uMemUnPack(ptr, &u64Val, sizeof(uint16_t));
    }

    if (pVal != NULL)
    {
        *(uint16_t *)pVal = (uint16_t)u64Val;
    }

    *pLength = sizeof(uint16_t);

    return ptr;
}

static uint8_t *UnPackUint32(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    uint64_t u64Val;

    if (isDecode)
    {
        ptr = VarintDecode(ptr, &u64Val);
    }
    else
    {
        ptr = uMemUnPack(ptr, &u64Val, sizeof(uint32_t));
    }

    if (pVal != NULL)
    {
        *(uint32_t *)pVal = (uint32_t)u64Val;
    }

    *pLength = sizeof(uint32_t);

    return ptr;
}

static uint8_t *UnPackUint64(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    uint64_t u64Val;

    if (isDecode)
    {
        ptr = VarintDecode(ptr, &u64Val);
    }
    else
    {
        ptr = uMemUnPack(ptr, &u64Val, sizeof(uint64_t));
    }

    if (pVal != NULL)
    {
        *(uint64_t *)pVal = (uint64_t)u64Val;
    }

    *pLength = sizeof(uint64_t);

    return ptr;
}

static uint8_t *UnPackFloat(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    unFloat_t t;
    t.data = 0;

    for (int i = 0; i < 4; i++)
    {
        t.data |= ((uint32_t)(*ptr) << (i * 8));
        ptr++;
    }

    if (pVal != NULL)
    {
        *(float *)pVal = t.fdata;
    }

    *pLength = 4;
    
    return ptr;
}

static uint8_t *UnPackDouble(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    unFloat_t t;
    t.data = 0;

    for (int i = 0; i < 8; i++)
    {
        t.data |= ((uint64_t)(*ptr) << (i * 8));
        ptr++;
    }

    if (pVal != NULL)
    {
        *(double *)pVal = t.fdata;
    }

    *pLength = 8;

    return ptr;
}

static uint8_t *UnPackString(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    uint16_t length = 0;

    for (int i = 0; i < PARAM_STRING_SIZE; i++)
    {
        length |= ((uint16_t)*ptr) << (i * 8);
        ptr++;
    }

    // if (isDecode)
    {
        *pLength = length;
    }

    if (pVal != NULL)
    {
        memcpy(pVal, ptr, length);
    }
    
    ptr += length;
    return ptr;
}

typedef uint8_t *(*UnPackFun_t)(uint8_t *ptr, void *pVal, uint16_t *pLength, uint8_t isDecode);

UnPackFun_t unpackFunTable[]={
    UnPackInt8,
    UnPackInt16,
    UnPackInt32,
    UnPackInt64,
    UnPackUint8,
    UnPackUint16,
    UnPackUint32,
    UnPackUint64,
    UnPackFloat,
    UnPackDouble,
    UnPackString
};

static uint8_t *UnPackData(uint8_t *pbuf, uint8_t type, void *pVal, uint16_t *pLength, uint8_t isDecode)
{
    return unpackFunTable[type](pbuf, pVal, pLength, isDecode);
}

uint8_t *UnPackParam(uint8_t *pBuf, ParamInfo_t *pParam, uint8_t isUseDecode, uint8_t opt)
{
    uint16_t length;
    pBuf = UnPackData(pBuf, pParam->type, pParam->pCurValue, &pParam->length, isUseDecode);

    if (opt & PARAM_PACK_NAME)
    {
        length = strlen(pBuf) >= PARAM_NAME_MAX_LENGTH ? PARAM_NAME_MAX_LENGTH - 1 : strlen(pBuf);
        memcpy(pParam->pszName, pBuf, length + 1);
        pBuf[length] = '\0';
        pBuf += (length + 1);
    }

    if (opt & PARAM_PACK_ATTR)
    {
        pBuf = UnPackUint8(pBuf, &pParam->attr, &length, 0);
    }

    if (opt & PARAM_PACK_DEF_VAL)
    {
        pBuf = UnPackData(pBuf, pParam->type, pParam->pDefValue, &length, isUseDecode);
    }

    if ((opt & PARAM_PACK_MIN_VAL) && pParam->type != PARAM_STARING)
    {
        pBuf = UnPackData(pBuf, pParam->type, pParam->pMinValue, &length, isUseDecode);
    }

    if ((opt & PARAM_PACK_MAX_VAL) && pParam->type != PARAM_STARING)
    {
        pBuf = UnPackData(pBuf, pParam->type, pParam->pMaxValue, &length, isUseDecode);
    }

    return pBuf;
}

uint8_t *UnPackTagAndParam(uint8_t *pBuf, ParamInfo_t *pParam, uint8_t isUseDecode, uint8_t opt)
{
    pBuf = UnPackTag(pBuf, &pParam->id, &pParam->type);
    pBuf = UnPackParam(pBuf, pParam, isUseDecode, opt);

    return pBuf;
}

uint8_t *UnPackTagAndOptAndParam(uint8_t *pBuf, ParamInfo_t *pParam)
{
    uint8_t isUseDecode;
    uint8_t opt;

    pBuf = UnPackTag(pBuf, &pParam->id, &pParam->type);

    isUseDecode = (*pBuf >> 7) & 0x01;
    opt = (*pBuf & 0x7f);
    pBuf++;

    pBuf = UnPackParam(pBuf, pParam, isUseDecode, opt);

    return pBuf;
}
