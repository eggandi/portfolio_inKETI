#include "buffer.h"
#define _DEBUG_LOG printf("[DEBUG][%d][%s]\n", __LINE__, __func__); 

static bool Pro_Buffer_Array_Isempty(struct pro_buffer_t *buffer); 
static void Pro_Buffer_Array_Flush(struct pro_buffer_t *buffer); 

//* Define - 준_전역변수 *//
struct pro_buffer_t *g_buffer; 

/**
 * @brief 버퍼 초기화
 * @param setup 버퍼 설정
 * @return 성공시 버퍼 구조체 반환, 실패시 NULL 반환
 */
extern void *Pro_Buffer_Array_Init(struct buffer_array_setup_t *setup) {

    if(setup->buffer_ptr != NULL) {
        g_buffer = setup->buffer_ptr;
    }else{
        if (g_buffer != NULL) {
            Pro_Buffer_Array_Flush(g_buffer);
        }
        g_buffer = malloc(sizeof(struct pro_buffer_t));
    }

    if (setup == NULL) {
        g_buffer->status.buffer_size_max = PRO_DEFAULT_BUFFER_ARRAY_SIZE;
    } else {
        g_buffer->status.buffer_size_max = setup->buffer_size;
    }

    if (g_buffer == NULL) {
        return NULL;
    }
    
    g_buffer->status.buffer_size_now = 0;
    g_buffer->status.buffer_pop_flag = false;
    g_buffer->status.buffer_push_flag = false;
    g_buffer->front = NULL;
    g_buffer->array = NULL;

    return g_buffer;
}

/**
 * @brief 버퍼에 데이터 추가
 * @param data 데이터
 * @param size 데이터 크기
 * @return 성공시 저장된 데이터 개수, 실패시 -1 반환
 */
extern int Pro_Buffer_Array_Push(void *data, size_t size, int *id, struct pro_buffer_t *user_buffer) 
{
    struct pro_buffer_t *buffer_tmp;
    if(user_buffer != NULL) {
        buffer_tmp = user_buffer;
    }else{
        buffer_tmp = g_buffer;
    }
    if (buffer_tmp == NULL || data == NULL) {
        return -1;
    }
    struct pro_buffer_status_t *status = &buffer_tmp->status;
    while(status->buffer_push_flag == true)
    {
        usleep(1);
    }
    status->buffer_push_flag = true;

    if (buffer_tmp->status.buffer_size_now == buffer_tmp->status.buffer_size_max) {
        struct pro_default_buffer_array_t *front_array = buffer_tmp->front;
        free(front_array->data);
        buffer_tmp->front = front_array->next;
        if (buffer_tmp->front != NULL) {
            buffer_tmp->front->prev = NULL;
        }
        free(front_array);
        buffer_tmp->status.buffer_size_now--;
    }

    struct pro_default_buffer_array_t *array = malloc(sizeof(struct pro_default_buffer_array_t));
    if (array == NULL) {
        return buffer_tmp->status.buffer_size_now;;
    }
    array->data = malloc(size);
    if (array->data == NULL) {
        free(array);
        return buffer_tmp->status.buffer_size_now;
    }
    memcpy(array->data, data, size);
    array->size = size;
    if(id != NULL) {
        array->id = *id;
    }
    array->next = NULL;

    if (buffer_tmp->status.buffer_size_now == 0) {
        buffer_tmp->front = array;
        array->prev = NULL;
    } else {
        array->prev = buffer_tmp->array;
        buffer_tmp->array->next = array;
    }

    buffer_tmp->array = array;
    buffer_tmp->status.buffer_size_now++;
    status->buffer_push_flag = false;
    return buffer_tmp->status.buffer_size_now;;
}

/**
 * @brief 버퍼에서 데이터 가져오기 FIFO
 * @param out_buffer 가져올 데이터를 저장할 버퍼
 * @param out_size 가져올 데이터 크기
 * @return 현재 버퍼 크기
 */
extern int Pro_Buffer_Array_Pop(struct pro_default_buffer_array_t *out_buffer, struct pro_buffer_t *user_buffer) 
{
    int ret;
    struct pro_buffer_t *buffer_tmp;
    if(user_buffer != NULL) {
        buffer_tmp = user_buffer;
    }else{
        buffer_tmp = g_buffer;
    }
    if (buffer_tmp == NULL || Pro_Buffer_Array_Isempty(buffer_tmp)) {
        return 0;
    }
    struct pro_buffer_status_t *status = &buffer_tmp->status;
    while(status->buffer_pop_flag == true)
    {
        usleep(1);
    }
    status->buffer_pop_flag = true;
    struct pro_default_buffer_array_t *array = buffer_tmp->front;
    if (out_buffer->data == NULL) {
        out_buffer->data = malloc(sizeof(char) * array->size);
        memcpy(out_buffer->data, array->data, array->size);
        out_buffer->size = array->size;
        out_buffer->id = array->id;
    }
    buffer_tmp->front = array->next;
    if (buffer_tmp->front != NULL) {
        buffer_tmp->front->prev = NULL;
    } else {
        buffer_tmp->array = NULL;
    }

    free(array->data);
    free(array);
    ret = buffer_tmp->status.buffer_size_now;
    buffer_tmp->status.buffer_size_now--;

    status->buffer_pop_flag = false;
    return ret;
}

/**
 * @brief 버퍼가 비어 있는지 확인
 * @return 비어 있으면 true, 아니면 false
 */
static bool Pro_Buffer_Array_Isempty(struct pro_buffer_t *buffer) 
{
    return (buffer == NULL || buffer->status.buffer_size_now == 0);
}

/**
 * @brief 버퍼 정리
 */
static void Pro_Buffer_Array_Flush(struct pro_buffer_t *buffer) {
    if (buffer == NULL) {
        return;
    }
    
    while (Pro_Buffer_Array_Pop(NULL, buffer) != 0) {
        continue;
    }

    free(buffer);
    buffer = NULL;
}
