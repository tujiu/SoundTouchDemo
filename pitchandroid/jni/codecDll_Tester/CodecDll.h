#pragma once
#ifndef LIB_CODEC_H
#define LIB_CODEC_H

#ifdef __cplusplus
extern "C" {
#endif

#define EncodeSampleRate            0x0001     /* Int, KeyValue Must be in 8000,12000,16000,24000,32000,44100,48000 */   
#define EncodeBitRate               0x0002     /* Int, KeyValue Must be in 5000~100000 */
	/********************************/
	/* SampleRate| avg BitRate(kpbs)*/
	/*------------------------- ----*/
	/*   8000    |   5 - 20			*/
	/*  12000    |   7 - 25			*/
	/*  16000    |   8 - 30			*/
	/*  24000    |  20 - 40			*/
	/********************************/
#define EncodeMaxInternalSampleRate 0x0004    /* Int, KeyValue Must be in 8000,12000,16000,24000 */
#define EncodeComplexity            0x0008    /* Int, KeyValue Must be in 0,1,2 */
#define EncodePacketSize            0x0010    /* Int, KeyValue Must be 20ms,40ms,60ms,80ms,100ms samples of SampleRate*/
	/* eg   16000 , 20ms  PacketSize, keyvalue is 16000 * 20 / 1000 = 320  */
#define EncodePacketLossPercentage  0x0020    /* Int, KeyValue Must be in 0~100 */
#define EncodeEnableInBandFEC       0x0040    /* Int, KeyValue Must be in 0, 1 */
#define EncodeEnableDTX             0x0080    /* Int, KeyValue Must be in 0, 1 */

#define DecodeSampleRate            0x0001    /* Int, KeyValue Must be in 8000,12000,16000,24000,32000,44100,48000 */

	//silk default
	/*
	void *enc = CreateCodecInstance(eEncode);
	int res = SetValue(enc,EncodeSampleRate,8000);

	*/
	// Encode
	// SampleRate            16000
	// BitRate               40000
	// MaxInternalSampleRate 24000
	// Complexity            2
	// PacketSize            16000 * 20 / 1000
	// PacketLossPercentage  0
	// EnableInBandFEC       0
	// EnableDTX             0

	//Decode
	// SampleRate            16000


typedef enum
{
	ERR_SUCCESS = 0,                  // no error
	ERR_CODEC_INTERNAL_ERROR = -1,    // codec internal error
	ERR_INVALID_ARG = -2,             // invalid arg
	ERR_OUT_OF_MEMROY = -3,           // memroy error
	ERR_INVALID_KEY  = -4,            // invalid key, not in keyvaluemacros.h
	ERR_INVALID_KEY_VALUE = -5,       // invalid keyvalue, not in range 

	ERR_ENC_INPUT_LEN = -6,             // encode input length error
	ERR_ENC_PAYLOAD_BUF_TOO_SHORT = -7, // encode output len error
	ERR_ENC_INVALID_CODEC_NAME = -8,    // codec not support

	ERR_DEC_PAYLOAD_TOO_LARGE = -9,     // decode input length error
	ERR_DEC_OUTPUT_LEN    = -10         // decode output len error


}ERROR_CODE;


typedef enum
{    
	eEncode = 0,
	eDecode = 1
}EMask;

void*  CreateCodecInstance(const char* codecName,int flag);
int    DestroyCodecInstance(void* pCodecInstance);
int    SetValue(void* pCodecInstance,int key,void* keyValue,int keyBytesSize);
int    GetValue(void* pCodecInstance,int key,void* keyValue,int keyBytesSize);
int    Encode(void* pCodecInstance,char* pPcmInputBuf,int pcmBufBytesSize,char* pEncodedBuf,int* pEncodedBufBytesSize);
int    Decode(void* pCodecInstance,char* pEncodedBuf,int EncodedBufBytesSize,char* pPcmDecodedBuf,int* pcmBufBytesSize);



#ifdef __cplusplus
}
#endif
#endif
