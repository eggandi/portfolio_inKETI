#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf-c/protobuf-c.h>
#include "protobuf.pb-c.h"
#define _DEBUG_LOG printf("[DEBUG][%s][%d]\n", __func__, __LINE__);


// 바이너리 데이터를 설정하는 함수 (C 인터페이스)
/* extern "C" */
void protobuf_filedata_set_content_1(FileData* file_data, const char* data, size_t length) {
    
    file_data->content_1.data = data;
    printf("file_data->content_1.data:%p\n", file_data->content_1.data);
    file_data->content_1.len = length;
}
/* extern "C" */
void protobuf_filedata_set_content_2(FileData* file_data, const char* data, size_t length) {
    file_data->content_2.data = data;
    printf("file_data->content_2.data:%p\n", file_data->content_2.data);
    file_data->content_2.len = length;
}
/* extern "C" */
void protobuf_filedata_set_content_3(FileData* file_data, const char* data, size_t length) {
    file_data->content_3.data = data;
    printf("file_data->content_3.data:%p\n", file_data->content_3.data);
    file_data->content_3.len = length;
}

/* extern "C" */

typedef struct {
    ProtobufCBuffer base;
    FILE *fp;
    const void *content_1_ptr;
    const void *content_2_ptr;
    const void *content_3_ptr;
} FileOutputBuffer;

#define CHUNK_SIZE 10240

void file_write(ProtobufCBuffer *buffer, size_t len, const uint8_t *data) {
    FileOutputBuffer *fb = (FileOutputBuffer *)buffer;
    uint8_t temp[CHUNK_SIZE];
    size_t read_len = 0;
    size_t written = 0;
    if (data == fb->content_1_ptr || (data == fb->content_2_ptr) ) 
    {
        size_t total_read_len = 0;
        while ((read_len = fread(temp, 1, CHUNK_SIZE, (FILE*)data)) > 0) {
            if(read_len > 0) {
                total_read_len += read_len;
                written += fwrite(temp, 1, read_len, fb->fp);
                //printf("read_len: %lu/%lu, write_len:%lu/%lu\n", total_read_len, len, written, len);
            }
        }
        if (written != len) {
            perror("파일 쓰기 실패");
        }
    } else if (data == fb->content_3_ptr) {
        // content_3에 대한 처리 (예: 다른 파일에 쓰기 등)
        written += fwrite(data, 1, len, fb->fp);
        if (written != len) {
            perror("파일 쓰기 실패");
        }
    } else {
    }
        
    // fflush(fb->fp); // Uncomment if you want to flush after each write
    // printf("file_write: %s\n", data);_DEBUG_LOG
}

size_t protobuf_savetofile(const char* filename, FileData* file_data) 
{    
    size_t ret;
    FILE *fp = fopen(filename, "wb");_DEBUG_LOG
    if (!fp) 
    {
        perror("파일 열기 실패");_DEBUG_LOG
        return;
    }

    FileOutputBuffer out = {.base = { file_write },
                            .fp = fp,
                            .content_1_ptr = file_data->content_1.data,
                            .content_2_ptr = file_data->content_2.data,
                            .content_3_ptr = file_data->content_3.data, 
                        };
   
    file_data__pack_to_buffer(file_data, (ProtobufCBuffer *)&out); 
    fclose(fp);

    // 💡 여기부터 다시 읽어서 decode 시도
    fp = fopen(filename, "rb");
    if (!fp) {
        perror("파일 다시 열기 실패");
        return;
    }
    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    rewind(fp);
    ret = fsize;
    printf("file 재열기: %s, size:%lu\n", filename, fsize);
    uint8_t *buf = malloc(fsize);
    if (buf == NULL) {
        perror("메모리 할당 실패");
        fclose(fp);
        return;
    }
    FileData *fdata = file_data__unpack(NULL, fp, buf);
    fclose(fp);
    if (fdata == NULL) {
        printf("protobuf 데이터 아님 또는 파싱 실패\n");
    }else{

        file_data__free_unpacked(fdata, NULL);
        free(buf);
    }
    return ret;
}
