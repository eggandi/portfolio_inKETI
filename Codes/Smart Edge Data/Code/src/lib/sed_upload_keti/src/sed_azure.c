/**
 * @file
 * @brief
 * @date 2025-02-06
 * @author user
 */

// 의존 헤더 파일
#include "sed_azure.h"
#include "sed_json.h"

#define _DEBUG_LOG printf("[DEBUG][%d][%s]\n", __LINE__, __func__); 

az_storage_blobs_blob_client g_sed_azure_client; 
bool g_sed_azure_upload_active = false; 

#ifdef TRANSPORT_CURL
    #define SED_DEFAULT_CURL_ENABLE TRANSPORT_CURL
#else
    #define SED_DEFAULT_CURL_ENABLE 0
    #define CURL_GLOBAL_ALL 0 
    #define CURLE_OK 0 
    int curl_global_init(int empty){
        empty = empty;
        return 0;
    }
    void curl_global_cleanup(){
        return;
    }
#endif

static int SED_Azure_CURL_Init();
/**
 * @brief Azure Blob Storage 클라이언트(전역변수)를 초기화하는 함수
 * @param URL Azure Blob Storage URL
 * @return 성공 시 0, 실패 -1
 */
static int SED_Azure_CURL_Init()
{
    if(SED_DEFAULT_CURL_ENABLE)
    {
        // If running with libcurl, call global init. See project Readme for more info
        if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
        {
        printf("Couldn't init libcurl.\n");
        return -1;
        }
        // Set up libcurl cleaning callback as to be called before ending program
        atexit(curl_global_cleanup);
    }
    return 0;
}

/**
 * @brief Azure Blob Storage 클라이언트(전역변수)를 초기화하는 함수
 * @param URL Azure Blob Storage URL
 * @return 성공 시 0, 실패 -1
 */
EXPORT void *SED_Azure_Blob_Client_Init(char* const URL_WITH_SAS)
{
    // 1) Init client.
    // Example expects AZURE_BLOB_URL_WITH_SAS in env to be a URL w/ SAS token
    if(g_sed_azure_upload_active == true)
    {
        return (void*)NULL;
    }
    if(SED_DEFAULT_CURL_ENABLE)
    {
        SED_Azure_CURL_Init();
    }
    memset(&g_sed_azure_client, 0, sizeof(az_storage_blobs_blob_client));

    if(URL_WITH_SAS != NULL){
        char* const blob_url = (char* const)URL_WITH_SAS;//getenv(URL_WITH_SAS);
        printf("Blob URL: %s\n", blob_url);
        if (blob_url == NULL)
        {
            printf("Blob URL environment variable " "%s" " not set.\n", URL_WITH_SAS);
            return (void*)-1;
        }else{
            printf("Blob URL: %s\n", blob_url);
        }
#ifdef _DEBUG_LOG_AZURE
        else{
            if(_DEBUG_LOG_AZURE)
                printf("Blob URL: %s\n", blob_url);
        }
#endif
        if (az_result_failed(az_storage_blobs_blob_client_init(&g_sed_azure_client, az_span_create_from_str(blob_url), AZ_CREDENTIAL_ANONYMOUS, NULL)))
        {
            printf("Failed to init blob client.\n");
            return (void*)-2;
        }
    }

    g_sed_azure_upload_active = true;
    
    return (void*)&g_sed_azure_client;
}
/**
 * @brief Azure Blob Storage에 파일을 업로드하는 함수
 * @param client Azure Blob Storage 클라이언트
 * @param content 업로드할 파일 내용
 * @param out_response HTTP 응답 데이터
 * @return 성공 시 0, 실패 -1, -2, -3
 */
EXPORT int SED_Azure_Blob_Upload(void *client, const char* content, az_http_response* out_response)
{
// 만약 임의의 client를 사용하려면 client 정보를 넣어야 함
    if(g_sed_azure_upload_active == false)
    {
#ifdef _DEBUG_LOG_AZURE
        printf("SED_Azure_Blob_Client_Init() is not called or work\n");
#endif
        return -1;
    }
    az_storage_blobs_blob_client *p_client = NULL;
    if(client != NULL)
    {
        p_client = (az_storage_blobs_blob_client*)client;

    }else{
        p_client = &g_sed_azure_client;

    }

/******* 1) Create a buffer for response (will be reused for all requests)   *****/
    uint8_t response_buffer[1024 * 4] = { 0 };
    az_http_response *http_response;
// 만약 HTTP 응답데이터(response)를 반환 받고 싶으면 out_response에 allocated된 메모리 주소를 넣어야 함.
    if(out_response != NULL)
    {        
        http_response = out_response;
    }else {
        http_response = malloc(sizeof(az_http_response));
    }
    
    if (az_result_failed(az_http_response_init(http_response, AZ_SPAN_FROM_BUFFER(response_buffer))))
    {
      printf("\nFailed to init http response.\n");
      return -2;
    }

// 3) fill upload buffer
    az_span content_to_upload;
    if(content == NULL)
    {
        char *cjson_out = NULL;
        int ret = SED_JSON_Print(&cjson_out);
        if(ret < 0)
        {
            return -3;
        }else{
            content_to_upload._internal.ptr = (uint8_t*)(cjson_out);
            content_to_upload._internal.size = strlen(cjson_out);
        }
    }else{
        content_to_upload._internal.ptr = (uint8_t*)(content);
        content_to_upload._internal.size = strlen(content);

        #if 0
        if(content[strlen(content)] != '\0')
        {
            content_to_upload._internal.size = strlen(content) + 1;
        }else{
            content_to_upload._internal.size = strlen(content);
        }
        #endif
    }

// 4) upload content
    az_result blob_upload_result = az_storage_blobs_blob_upload(p_client, NULL, content_to_upload, NULL, http_response);

// This validation is only for the first time SDK client is used. API will return not implemented
// if samples were built with no_http lib.
    if (blob_upload_result == AZ_ERROR_DEPENDENCY_NOT_PROVIDED)
    {
        printf("Running sample with no_op HTTP implementation.\nRecompile az_core with an HTTP client "
                "implementation like CURL to see sample sending network requests.\n\n"
                "i.e. cmake -DTRANSPORT_CURL=ON ..\n\n");

        return -4;
    }else if (az_result_failed(blob_upload_result)) // Any other error would terminate sample
    {
        printf("\nFailed to upload blob.\n");
        return -5;
    }

    // 5) get response and parse it
    az_http_status_code status_code = az_http_response_get_status_code(http_response);
    printf("Status Code: %d (%s)\n", status_code, status_code == AZ_HTTP_STATUS_CODE_CREATED ? "success" : "error");
    if(out_response == NULL)
    {        
        free(http_response);
    }
    return 0;
}

