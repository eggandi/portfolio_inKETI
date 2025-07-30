#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#else
#endif

#ifdef _WIN32  // Windows 환경
    #include <windows.h>  // Windows API
#else  // POSIX 환경 (Linux/macOS)
    #include <unistd.h>    // sleep, usleep
#endif

#ifdef TRANSPORT_CURL
    #include <curl/curl.h>
#endif

#include <azure/storage/az_storage_blobs.h>

#ifndef _DSED_LIB_BUILD_AZURE 
#define _DSED_LIB_BUILD_AZURE 1

#ifdef _WIN32  // Windows 환경
    #define EXPORT __declspec(dllexport)
#else  // POSIX 환경 (Linux/macOS)
    #define EXPORT EXPORT
#endif

// 모듈 헤더파일

#endif // !_DSED_LIB_BUILD_AZURE

EXPORT void *SED_Azure_Blob_Client_Init(char* const URL_WITH_SAS);
EXPORT int SED_Azure_Blob_Upload(void *client, const char* content, az_http_response* out_response);

#ifdef __cplusplus
}
#endif