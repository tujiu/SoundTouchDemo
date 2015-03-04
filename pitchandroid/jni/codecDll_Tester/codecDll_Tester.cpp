// codecDll_Tester.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "codecDll_Tester.h"
#include "CodecDll.h"


#define tag "siklandroiddemo"

int testCodecDllEncode(char* input, char* output1, char* output2);
int testCodecDllDecode(char* input, char* output1, char* output2);

int silkdemo(char* input, char* output1, char* output2)
{	
	__android_log_print(ANDROID_LOG_INFO,  tag, "Encode...");
	testCodecDllEncode(input, output1, output2);
	
	__android_log_print(ANDROID_LOG_INFO,  tag, "Decode...\r\n");
	testCodecDllDecode(input, output1, output2);

	__android_log_print(ANDROID_LOG_INFO,  tag, "over...\r\n");
	
	return 0;
	
}

int testCodecDllEncode(char* input, char* output1, char* output2)
{
	FILE* pfPcmFile = NULL;
	FILE* pfEncodedFile = NULL ;
	void* pEncodec = NULL ;
	pfPcmFile     = fopen( input, "rb" );
	pfEncodedFile = fopen( output1, "wb" );
	pEncodec = CreateCodecInstance( "silk", eEncode );
	int nSampleRate = 16000 ;
	int nBitRate    = 40000 ;
	SetValue(pEncodec,EncodeSampleRate,(void*)&nSampleRate,sizeof(int));
	//SetValue(pEncodec,EncodeBitRate,(void*)&nBitRate,sizeof(int));
	char  ppcmBuf[16000*80*2*20/1000] = {0};
	int pcmbufLen = 16000*20*2/1000;
	char  pencodedBuf[16000*80*2*20/1000] = {0};
	int encodebufLen = 16000*20*2/1000;
	char* pcmBuf = ppcmBuf;
	char* encodedBuf = pencodedBuf;
	int count = 0;
	if( pfPcmFile && pfEncodedFile )
	{
		while( fread( pcmBuf,pcmbufLen,1,pfPcmFile) == 1  )
		{
			int ret = Encode( pEncodec,(char*)pcmBuf,pcmbufLen,encodedBuf,&encodebufLen);
			if( ret )
			{
				//printf("Encode Error\n");
				__android_log_print(ANDROID_LOG_INFO,  tag, "Encode Error:%d \r\n", ret);
				break;
			}
			count++;
			fwrite(&encodebufLen,sizeof(encodebufLen),1,pfEncodedFile);
			fwrite(encodedBuf,encodebufLen,1,pfEncodedFile);
			encodebufLen = pcmbufLen;
		}
		__android_log_print(ANDROID_LOG_INFO,  tag, "count = %d\r\n", count);
	}


	DestroyCodecInstance(pEncodec);
	fclose( pfPcmFile );
	fclose( pfEncodedFile );
	__android_log_print(ANDROID_LOG_INFO,  tag, "encoded file = %s\r\n", output1);
	return 0;

}

int testCodecDllDecode(char* input, char* output1, char* output2)
{
	FILE* pfPcmFile = NULL;
	FILE* pfEncodedFile = NULL ;
	void* pDecodec = NULL ;
	pfEncodedFile = fopen( output1, "rb" );
	pfPcmFile     = fopen( output2, "wb" );
	
	pDecodec = CreateCodecInstance( "silk", eDecode );

	char  pencodedBuf[16000*80*2*20/1000] = {0};
	int encodebufLen = 0;

	char  ppcmBuf[16000*80*2*20/1000] = {0};
	int   pcmbufLen = 16000*20*2/1000;

	char* pcmBuf = ppcmBuf;
	char* encodedBuf = pencodedBuf;
	if( pfPcmFile && pfPcmFile )
	{
		while( fread( &encodebufLen,sizeof(encodebufLen),1,pfEncodedFile) == 1  )
		{
			if( fread( encodedBuf,encodebufLen,1,pfEncodedFile) == 1 )
			{
				int ret  = Decode( pDecodec, encodedBuf,encodebufLen,pcmBuf,&pcmbufLen);
				if ( ret )
				{
					//printf("Deocode Error\n");
					__android_log_print(ANDROID_LOG_INFO,  tag, "Deocode Error: %d\r\n", ret);
					break;
				}
				fwrite(pcmBuf,pcmbufLen,1,pfPcmFile);
			}
			else
				break;
			
		}

	}


	DestroyCodecInstance(pDecodec);
	fclose( pfPcmFile );
	fclose( pfEncodedFile );

	__android_log_print(ANDROID_LOG_INFO,  tag, "decoded file = %s\r\n", output2);
	return 0;

}
