#define URL "http://192.168.0.250/download/program/"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
 
/* Auxiliary function that waits on the socket. */
static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
  struct timeval tv;
  fd_set infd, outfd, errfd;
  int res;
 
  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (int)(timeout_ms % 1000) * 1000;
 
  FD_ZERO(&infd);
  FD_ZERO(&outfd);
  FD_ZERO(&errfd);
 
  FD_SET(sockfd, &errfd); /* always check for error */
 
  if(for_recv) {
    FD_SET(sockfd, &infd);
  }
  else {
    FD_SET(sockfd, &outfd);
  }
 
  /* select() returns the number of signalled sockets or -1 */
  res = select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
  return res;
}
 
int main(void)
{
    CURL *curl;

    char data[] = {0x44, 0x01, 0x41, 0x42, 0x43, 0x44, 0x31, 0x32, 0x33, 0x34, 0x45, 0x46, 0x47, 0x48, 0x35, 0x36, 0x37, 0x38, 0xAA};

    char *request = malloc(sizeof(char) * 10240);
    sprintf(request, "%s %s %s\r\n", "POST", URL, "HTTP/1.0");

    sprintf(request, "%s%s: %s\r\n",request , "Host", "http://192.168.0.250");
    sprintf(request, "%s%s: %s\r\n",request , "Accept", "*/*");

    sprintf(request, "%s%s: %s\r\n",request , "Content-Type", "Application/octet-stream");
    sprintf(request, "%s%s: %d\r\n",request , "Content-Length", sizeof(data));
    sprintf(request, "%s\r\n", request);
    size_t request_len = strlen(request);
    memcpy(request + strlen(request), data, sizeof(data));
    request_len += sizeof(data);

    curl = curl_easy_init();
    if(curl) {
    CURLcode res;
    curl_socket_t sockfd;
    size_t nsent_total = 0;

    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        printf("Error: %s\n", curl_easy_strerror(res));
        return 1;
    }
    
    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
    if(res != CURLE_OK) {
        printf("Error: %s\n", curl_easy_strerror(res));
        return 1;
    }

    printf("Sending request.\n");

    do 
    {
        size_t nsent;
        do {
        nsent = 0;
        
        res = curl_easy_send(curl, request + nsent_total, request_len - nsent_total, &nsent);
        nsent_total += nsent;

        if(res == CURLE_AGAIN && !wait_on_socket(sockfd, 0, 60000L)) {
            printf("Error: timeout.\n");
            return 1;
        }
        } while(res == CURLE_AGAIN);

        if(res != CURLE_OK) {
        printf("Error: %s\n", curl_easy_strerror(res));
        return 1;
        }

        printf("Sent %lu bytes.\n", (unsigned long)nsent);

    } while(nsent_total < request_len);

    printf("Reading response.\n");

    for(;;) 
    {
        /* Warning: This example program may loop indefinitely (see above). */
        char buf[1024];
        size_t nread;
        do {
        nread = 0;
        res = curl_easy_recv(curl, buf, sizeof(buf), &nread);

        if(res == CURLE_AGAIN && !wait_on_socket(sockfd, 1, 60000L)) {
            printf("Error: timeout.\n");
            return 1;
        }
        } while(res == CURLE_AGAIN);

        if(res != CURLE_OK) {
        printf("Error: %s\n", curl_easy_strerror(res));
        break;
        }

        if(nread == 0) {
        /* end of the response */
        break;
        }
        
        printf("\nReceived %lu bytes.\n", (unsigned long)nread);
        printf("\n%s\n\n", buf);
        int count = 0;
        while(buf[count])
        {
            printf("%02X", buf[count]); 
            count++;
        }
        printf("\n");


        char *ptr = strstr(buf, "\r\n\r\n");
        ptr = ptr + 4;
        int http_body_len = nread - (ptr - &buf[0]) - 2;
        char http_body[http_body_len];
        memcpy(http_body, ptr, http_body_len);//strlen(ptr));
        //ptr = strtok(ptr, "\n");
        
        count = 0;
        while(count < http_body_len)
        {
            printf("%02X", http_body[count]); 
            count++;
        }
        printf("\n");
    }
   
        /* always cleanup */
        curl_easy_cleanup(curl);

    }
    return 0;
}
