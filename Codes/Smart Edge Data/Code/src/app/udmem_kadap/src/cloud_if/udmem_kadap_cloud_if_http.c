/**
 * @file
 * @brief
 * @date 2024-12-01
 * @author user
 */


// 의존 헤더 파일
#include "curl/curl.h"

// 모듈 헤더파일
#include "udmem_kadap.h"


#define UDMEM_KADAP_HTTP_CODE_OK (200)
#define UDMEM_KADAP_HTTP_CODE_BAD_DATA (400) ///< 클라이언트의 잘못된 데이터(문법)
#define UDMEM_KADAP_HTTP_POST_HDR "content-type: application/json"


/**
 * @brief 서버로부터 수신된 HTTP response 메시지가 저장되는 메모리 정보 구조체
 */
typedef struct
{
  uint8_t *octs; ///< 메시지 바이트열
  size_t len; ///< 메시지 바이트열의 길이
} UDMEM_KADAP_HTTP_RESPONSE;


/**
 * @brief HTTPS POST/GET Response 메시지 수신 시 호출되는 콜백함수. 수신된 데이터를 userp(=Dot2HTTPSData)에 저장한다.
 * @param[in] contents 수신 데이터
 * @param[in] size 수신 데이터 블록 크기
 * @param[in] nmemb 수신 데이터 블록 개수
 * @param[in] userp 사용자 private 데이터(=Dot2HTTPSData). 수신된 Response 데이터를 저장한다.
 * @return 처리한 데이터길이
 * @retval 0: 실패
 *
 */
static size_t UDMEM_KADAP_HTTP_ResponseCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t ret = 0, realsize = size * nmemb;
  UDMEM_KADAP_HTTP_RESPONSE *resp = (UDMEM_KADAP_HTTP_RESPONSE *)userp;
  resp->octs = realloc(resp->octs, resp->len + realsize + 1);
  if (resp->octs) {
    memcpy(resp->octs + resp->len, contents, realsize);
    resp->len += realsize;
    resp->octs[resp->len] = 0;
    ret = realsize;
  }
  return ret;
}


/**
 * @brief 서버로 HTTPS POST request를 송신한다.
 * @param[in] url request를 전송할 URL
 * @param[in] contents request body에 수납될 컨텐츠
 * @retval 0: 성공
 * @retval -1: 실패
 */
int UDMEM_KADAP_HTTP_POST(const char *url, const char *contents)
{
  int ret;
  CURLcode res;
  LOG(kKVHLOGLevel_InOut, "HTTP post(%s)\n", url);
  struct curl_slist *header_list = NULL;

#if 1
  /*
   * CURL을 초기화한다.
   */
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL *curl = curl_easy_init();
  if (!curl) {
    return -1;
  }

  /*
   * HTTP 연결할 서버의 URL을 지정한다.
   */
  res = curl_easy_setopt(curl, CURLOPT_URL, url);
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_URL) failed: %s\n", url, curl_easy_strerror(res));
    ret = -1;
    goto out;
  }

  /**
   * HTTP가 POST method를 사용하도록 한다
   */
  res = curl_easy_setopt(curl, CURLOPT_POST, 1L);
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_POST) failed: %s\n", url, curl_easy_strerror(res));
    ret = -1;
    goto out;
  }

  /*
   * HTTP POST request header 및 body를 설정한다.
   */
  header_list = curl_slist_append(header_list, UDMEM_KADAP_HTTP_POST_HDR);
  res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list); // 헤더 완성
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_HTTPHEADER) failed: %s\n", url, curl_easy_strerror(res));
    ret = -1;
    goto out;
  }
  res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(contents)); // body 설정
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_POSTFIELDSIZE) failed: %s\n", url, curl_easy_strerror(res));
    ret = -1;
    goto out;
  }
  res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, contents); // body 설정
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_POSTFIELDS) failed: %s\n", url, curl_easy_strerror(res));
    ret = -1;
    goto out;
  }

  /*
   * HTTP POST response 수신 설정한다 -> 이 부분을 실행하지 않으면, 수신된 response 메시지가 stderr에 출력된다.
   * 이 부분을 실행함으로써 stderr로 출력되는 메시지를 메모리에 저장되도록 변경할 수 있다.
   */
  res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UDMEM_KADAP_HTTP_ResponseCallback); // response 메시지를 처리할 콜백함수 등록
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_WRITEFUNCTION) failed: %s\n", curl_easy_strerror(res));
    ret = -1;
    goto out;
  }
  UDMEM_KADAP_HTTP_RESPONSE resp;
  resp.octs = malloc(1);
  resp.len = 0;
  if (!resp.octs) {
    ret = -1;
    goto out;
  }
  res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_setopt(CURLOPT_WRITEDATA) failed: %s\n", curl_easy_strerror(res));
    ret = -1;
    goto out;
  }

  /*
   * HTTP POST를 실행한다.
   */
  long http_code;
  res = curl_easy_perform(curl);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  if (res != CURLE_OK) {
    ERR("Fail to HTTP post(%s) - curl_easy_perform() failed: %s, code: %d\n", url, curl_easy_strerror(res), http_code);
    ret = -1;
    goto out;
  }

  if (http_code != UDMEM_KADAP_HTTP_CODE_OK) {
    ERR("Fail to HTTP post(%s) - http code(%d) is not OK\n", url, http_code);
    if (http_code == UDMEM_KADAP_HTTP_CODE_BAD_DATA) {
      ERR("%s", contents);
    }
    ret = -1;
    goto out;
  }
  LOG(kKVHLOGLevel_InOut, "Success to HTTP post(%s)\n", url);
  ret = 0;

out:
  if (resp.octs) {
    free(resp.octs);
  }
  curl_easy_cleanup(curl);
  curl_slist_free_all(header_list);
  curl_global_cleanup();
  return ret;
#else
  LOG(kKVHLOGLevel_InOut, "Success to HTTP post(%s)\n", url);
  ret = 0;
#endif
}


