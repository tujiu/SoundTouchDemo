/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "codecDll_Tester/codecDll_Tester.h"
#include <android/log.h>


#define DEBUG 0

#if DEBUG
#include <android/log.h>
#  define  D(x...)  __android_log_print(ANDROID_LOG_INFO,"helloneon",x)
#else
#  define  D(...)  do {} while (0)
#endif

/* return current time in milliseconds */
static double
now_ms(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

#define LOOP_CNT (10000 )

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-neon/project/src/com/example/neon/HelloNeon.java
 */
 
jstring Java_com_example_soundtouchdemo_SilkDecoder_stringFromJNI( JNIEnv* env,  jobject thiz , jstring inputName, jstring outputName1, jstring outputName2)
{
    char*  str;
    uint64_t features;
    char buffer[512];
    char tryNeon = 0;
    double  t0, t1, time_c, time_neon;
    char* input;
    input = (char*)(*env)->GetStringUTFChars(env,inputName, NULL);
    char* output1;
    output1 = (char*)(*env)->GetStringUTFChars(env,outputName1, NULL);
    char* output2;
    output2 = (char*)(*env)->GetStringUTFChars(env,outputName2, NULL);

	time_c = 0.0;
	
	__android_log_print(ANDROID_LOG_INFO, "silk", "before silkdemo...\r\n");
	silkdemo(input, output1, output2);
	__android_log_print(ANDROID_LOG_INFO, "silk", "after sikldemo... \r\n ");

    asprintf(&str, "FIR Filter benchmark:\nC version          : %g ms\n", time_c);
    strlcpy(buffer, str, sizeof buffer);
    free(str);

    strlcat(buffer, "Neon version   : ", sizeof buffer);

    

EXIT:
    return (*env)->NewStringUTF(env, buffer);
	
}


