#ifndef __TODDEFINITION_H__
#define __TODDEFINITION_H__

#include <string>
#include "TodList.h"
#include "Reanimator.h"
#include "TodParticle.h"

enum class DefFieldType : int
{
    DT_INVALID,
    DT_INT,
    DT_FLOAT,
    DT_STRING,
    DT_ENUM,
    DT_VECTOR2,
    DT_ARRAY,
    DT_TRACK_FLOAT,
    DT_FLAGS,
    DT_IMAGE,
    DT_FONT
};

/*
    [??????????????]
        “????????????”????????????? _DefClass??????? _DefClass ?????? _Class?
        ???ReanimatorDefinition ?? Reanimation ?????????????TodParticleDefinition ?? TodParticleSystem ????????????????
*/

// ====================================================================================================
// ? ??????
// ----------------------------------------------------------------------------------------------------
// ??????????/??????????? DefSymbol ???????????/?????????
// ====================================================================================================
class DefSymbol
{
public:
    int                 mSymbolValue;                   //+0x0??????????????????? -1 ????????
    const char* mSymbolName;                    //+0x4????????????????????????????????????
};
//extern DefSymbol gParticleFlagSymbols[];  //0x69E290
//extern DefSymbol gEmitterTypeSymbols[];  //0x69E260
//extern DefSymbol gParticleTypeSymbols[];  //0x69E200

// ====================================================================================================
// ? ??????
// ----------------------------------------------------------------------------------------------------
// ????????????????????_MemVar???????????????
// ====================================================================================================
class DefField
{
public:
    const char* mFieldName;                     //+0x0??? _MemVar ??????????????????????????????
    int                 mFieldOffset;                   //+0x4?_MemVar ?????????????????
    DefFieldType        mFieldType;                     //+0x8?*_MemVar ?????????????????????????
    void* mExtraData;                     //+0xC????????? *_MemVar ??????????????
    // ? _MemVar ????????????????? mExtraData ??? _MemVar ???????????????
    // ? _MemVar ????????????? mExtraData ?????????? DefSymbol ?????????mExtraData ?????
    // ?????? _DefClass ????????????????????? _DefClass ??????????????
    // ?? _DefClass ???????????????????????????????????????????????
    // ????????????????????????????????“??”??????????????????“??”?????????????
};

// ====================================================================================================
// ? ???????
// ----------------------------------------------------------------------------------------------------
// ????????????????_DefClass????????????????????“_DefDefClass”?
// ====================================================================================================
class DefMap
{
public:
    DefField* mMapFields;                     //+0x0??????????? _DefClass ????????? _DefClass ??????????????
    int                 mDefSize;                       //+0x4??? _DefClass ????????????????????????????? sizeof(_DefClass)
    void* (*mConstructorFunc)(void*);     //+0x8?_DefClass ????????????
};

void* TodParticleDefinitionConstructor(void* thePointer); //0x5155A0
void* TodEmitterDefinitionConstructor(void* thePointer);  //0x5155C0
void* ParticleFieldConstructor(void* thePointer);         //0x515620
void* TrailDefinitionConstructor(void* thePointer);       //0x51B7F0
void* ReanimatorTransformConstructor(void* thePointer);   //0x471570
void* ReanimatorTrackConstructor(void* thePointer);       //0x4715B0
void* ReanimatorDefinitionConstructor(void* thePointer);  //0x4715D0

//extern DefField gParticleFieldDefFields[];  //0x69E2F8
extern DefMap gParticleFieldDefMap;  //0x69E338
//
//extern DefField gEmitterDefFields[];  //0x69E350
extern DefMap gEmitterDefMap;  //0x69E344
//
//extern DefField gParticleDefFields[];  //0x69E670
extern DefMap gParticleDefMap;  //0x69E690
//
extern DefMap gTrailDefMap;  //0x69D98C
//
//extern DefField gReanimatorTransformDefFields[];  //0x69F088
extern DefMap gReanimatorTransformDefMap;  //0x69F07C
//
//extern DefField gReanimatorTrackDefFields[];  //0x69F148
extern DefMap gReanimatorTrackDefMap;  //0x69F178
//
//extern DefField gReanimatorDefFields[];  //0x69F184
extern DefMap gReanimatorDefMap;  //0x69F1B4

// ====================================================================================================
// ? ??????
// ----------------------------------------------------------------------------------------------------
// ??????????????????????
// ====================================================================================================
class DefinitionArrayDef
{
public:
    void* mArrayData;                     //+0x0??????????????????????????????“??”??
    int                 mArrayCount;                    //+0x4???????????????“??”???????????“???”??
    // ?????????“?????? + ??”?????????? DefField ???? DefinitionArrayDef ??
    // ?? TodParticleDefinition ?? *mEmitterDefs ? mEmitterDefCount??? TodEmitterDefinition ?? *mParticleFields ? mParticleFieldCount ??
    // ??????? mArrayCount ????????????????????????????????????? mArrayData ????????????
};

// ====================================================================================================
// ? ?????????
// ----------------------------------------------------------------------------------------------------
// ???????????????????????????????????????????????
// ====================================================================================================
class CompressedDefinitionHeader
{
public:
    unsigned int        mCookie;                        //+0x0???????????
    unsigned int        mUncompressedSize;              //+0x4?????????
};

// ====================================================================================================
// ? ??????
// ----------------------------------------------------------------------------------------------------
// ?????????????????????????????????
// ====================================================================================================
class DefLoadResPath
{
public:
    const char* mPrefix;                        //+0x0????????“IMAGE_"
    const char* mDirectory;                     //+0x4???????????????“images\”
};

SexyString /**/  DefinitionGetCompiledFilePathFromXMLFilePath(const SexyString& theXMLFilePath);
bool                    IsFileInPakFile(const SexyString& theFilePath);
bool                    DefinitionIsCompiled(const SexyString& theXMLFilePath);
bool                    DefinitionReadCompiledFile(const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition);
void                    DefinitionFillWithDefaults(DefMap* theDefMap, void* theDefinition);
void                    DefinitionXmlError(XMLParser* theXmlParser, char* theFormat, ...);
bool                    DefSymbolValueFromString(DefSymbol* theSymbolMap, const char* theName, int* theResultValue);
bool                    DefinitionReadXMLString(XMLParser* theXmlParser, SexyString& theValue);
bool                    DefinitionReadIntField(XMLParser* theXmlParser, int* theValue);
bool                    DefinitionReadFloatField(XMLParser* theXmlParser, float* theValue);
bool                    DefinitionReadStringField(XMLParser* theXmlParser, const char** theValue);
bool                    DefinitionReadEnumField(XMLParser* theXmlParser, int* theValue, DefSymbol* theSymbolMap);
bool                    DefinitionReadVector2Field(XMLParser* theXmlParser, SexyVector2* theValue);
bool                    DefinitionReadArrayField(XMLParser* theXmlParser, DefinitionArrayDef* theArray, DefField* theField);
bool                    DefinitionReadFloatTrackField(XMLParser* theXmlParser, FloatParameterTrack* theTrack);
bool                    DefinitionReadFlagField(XMLParser* theXmlParser, const SexyString& theElementName, uint* theResultValue, DefSymbol* theSymbolMap);
bool                    DefinitionReadImageField(XMLParser* theXmlParser, Image** theImage);
bool                    DefinitionReadFontField(XMLParser* theXmlParser, _Font** theFont);
bool                    DefinitionReadField(XMLParser* theXmlParser, DefMap* theDefMap, void* theDefinition, bool* theDone);
bool                    DefinitionWriteCompiledFile(const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionCompileFile(const SexyString theXMLFilePath, const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition);

void                    DefMapWriteToCache(void*& theWritePtr, DefMap* theDefMap, void* theDefinition);
void                    DefWriteToCacheString(void*& theWritePtr, char** theValue);
void                    DefWriteToCacheArray(void*& theWritePtr, DefinitionArrayDef* theValue, DefMap* theDefMap);
void                    DefWriteToCacheFloatTrack(void*& theWritePtr, FloatParameterTrack* theValue);
void                    DefWriteToCacheImage(void*& theWritePtr, Image** theValue);
void                    DefWriteToCacheFont(void*& theWritePtr, _Font** theValue);

void* DefinitionCompressCompiledBuffer(void* theBuffer, unsigned int theBufferSize, unsigned int* theResultSize);

/*inline*/ unsigned int DefGetSizeString(char** theValue);
/*inline*/ unsigned int DefinitionGetArraySize(DefinitionArrayDef* theValue, DefMap* theDefMap);
/*inline*/ unsigned int DefGetSizeFloatTrack(FloatParameterTrack* theValue);
/*inline*/ unsigned int DefGetSizeImage(Image** theValue);
/*inline*/ unsigned int DefGetSizeFont(_Font** theValue);

/*inline*/ unsigned int DefinitionGetDeepSize(DefMap* theDefMap, void* theDefinition);
/*inline*/ unsigned int DefinitionGetSize(DefMap* theDefMap, void* theDefinition);
/*inline*/ void* DefinitionAlloc(int theSize);
void* DefinitionUncompressCompiledBuffer(void* theCompressedBuffer, size_t theCompressedBufferSize, size_t& theUncompressedSize, const SexyString& theCompiledFilePath);
uint /**/        DefinitionCalcHashSymbolMap(int aSchemaHash, DefSymbol* theSymbolMap);
uint /**/        DefinitionCalcHashDefMap(int aSchemaHash, DefMap* theDefMap, TodList<DefMap*>& theProgressMaps);
uint /**/        DefinitionCalcHash(DefMap* theDefMap);
inline bool             DefReadFromCacheString(void*& theReadPtr, char** theString);
inline bool             DefReadFromCacheArray(void*& theReadPtr, DefinitionArrayDef* theArray, DefMap* theDefMap);
inline bool             DefReadFromCacheImage(void*& theReadPtr, Image** theImage);
inline bool             DefReadFromCacheFont(void*& theReadPtr, _Font** theFont);
inline bool             DefReadFromCacheFloatTrack(void*& theReadPtr, FloatParameterTrack* theTrack);
bool                    DefMapReadFromCache(void*& theReadPtr, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionCompileAndLoad(const SexyString& theXMLFilePath, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionLoadMap(XMLParser* theXmlParser, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionLoadImage(Image** theImage, const SexyString& theName);
bool                    DefinitionLoadFont(_Font** theFont, const SexyString& theName);
bool                    DefinitionLoadXML(const SexyString& theFilename, DefMap* theDefMap, void* theDefinition);
void                    DefinitionFreeArrayField(DefinitionArrayDef* theArray, DefMap* theDefMap);
void                    DefinitionFreeMap(DefMap* theDefMap, void* theDefinition);

/*inline*/ bool         FloatTrackIsSet(const FloatParameterTrack& theTrack);
/*inline*/ void         FloatTrackSetDefault(FloatParameterTrack& theTrack, float theValue);
float                   FloatTrackEvaluate(FloatParameterTrack& theTrack, float theTimeValue, float theInterp);
float                   FloatTrackEvaluateFromLastTime(FloatParameterTrack& theTrack, float theTimeValue, float theInterp);
/*inline*/ bool         FloatTrackIsConstantZero(FloatParameterTrack& theTrack);

#endif
