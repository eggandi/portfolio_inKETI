#include <./memory_allocation_include.h>
#include <./memory_allocation_api.h>
#include <./memory_allocation_param.h>

/* 
    @brief:전체 메모리를 할당
    Using_Size:할당한 총 메모리 크기 
*/
struct Memory_Used_Info_t F_s_Memory_Initial(size_t Using_Size)
{   
    struct Memory_Used_Info_t Used_Info;
    if(Using_Size > 0){
        pthread_mutex_init(&Used_Info.mtx, NULL);
        Used_Info.Memroy_Full_Size = Using_Size;
        Used_Info.Memroy_Full_Start = malloc(sizeof(uint8_t) * Used_Info.Memroy_Full_Size);
        Used_Info.Memroy_Full_Now = Used_Info.Memroy_Full_Start;
        Used_Info.Memroy_Used_List_Num = 0;
    }
    return Used_Info;
}

/* 
    @brief:할당된 메모리 중 데이터를 저장할 영역을 분배 받는다. 
    Used_Info:할당된 메모리에 대한 정보
    Using_Size:분배 받을 메모리 크기 
*/
struct Memory_Used_Data_Info_t F_s_Memory_Data_Distributtion(struct Memory_Used_Info_t Used_Info, size_t Using_Size, int *err)
{   
    
    size_t Memory_Left = Used_Info.Memroy_Full_Start  + Used_Info.Memroy_Full_Size - Used_Info.Memroy_Full_Now;
    struct Memory_Used_Data_Info_t data;
    if(Memory_Left < 0)
    {   
        *err = -1;
        return data;
    }else if(Memory_Left > Using_Size){
       
        pthread_mutex_init(&data.mtx, NULL);
        pthread_mutex_lock(&data.mtx);
        data.type = 0;
        data.p_start = Used_Info.Memroy_Full_Now;
        data.p_now = data.p_start;
        data.p_end = data.p_start + Using_Size;
        Used_Info.Memroy_Full_Now = data.p_end;
        data.Data_Count = malloc(sizeof(size_t));
        pthread_mutex_unlock(&data.mtx);
        Used_Info.Memory_Used_Data_Info_List[Used_Info.Memroy_Used_List_Num] = &data;
        Used_Info.Memroy_Used_List_Num++;
        *err = 0;
        return data;
    }else{
        *err = -1;
        return data;
    }
}
/* 
    @brief 
    할당된 메모리 중 데이터 저장을 위해 분배된 영역에 데이터를 저장
    A:데이터 저장 영역의 정보
    Data:저장할 데이터
    Data_size:저장할 데이터의 크기(가변 크기의 데이터일때는 입력 필수)
*/
size_t F_i_Memory_Data_Push(struct Memory_Used_Data_Info_t *Data_Info,  void *Data, size_t Data_size)
{   
    if(Data)
    {
        if(F_Memory_Data_isAvaialbe(Data_Info, Data_size) == 0)
        {
            return -(Data_Info->p_end - Data_Info->p_now);
        }else{
            pthread_mutex_lock(&Data_Info->mtx);
            memcpy(Data_Info->p_now, (uint8_t *)Data, Data_size);
            Data_Info->Data_Pointer_List[*Data_Info->Data_Count] = Data_Info->p_now;
            Data_Info->Data_Size_List[*Data_Info->Data_Count] = Data_size;
            *Data_Info->Data_Count = *Data_Info->Data_Count + 1;
            Data_Info->p_now = Data_Info->p_now + Data_size;
            switch(Data_Info->type)
            {
                case INT_32: //int32
                    break;
                case UINT_8: //uint8 or char
                    break;
                case OCTET_STRING: //OCTET_STRING
                    break;
                default: break;
            }
            pthread_mutex_unlock(&Data_Info->mtx);
            
            return (Data_Info->p_end - Data_Info->p_now);
        }
    }else{

        return 0;
    }
}

/**
 * @brief 
 * 할당된 메모리 중 데이터 저장을 위해 분배된 영역에 첫번째 데이터를 반환
 * Data_Info:데이터 저장 영역의 정보
 * Return:데이터 타입에 따라 값을 저장해서 void 포인터로 반환
 */
void *F_v_Memory_Data_Pop(struct Memory_Used_Data_Info_t *Data_Info, size_t *out_size)
{   
    
    size_t data_size = 0;
    void *out_data;
    if(F_Memory_Data_isEmpty(Data_Info))
    {
        pthread_mutex_lock(&Data_Info->mtx);
        data_size = data_size;
        pthread_mutex_unlock(&Data_Info->mtx);
        return NULL;
    }else{
        pthread_mutex_lock(&Data_Info->mtx);
        if(*Data_Info->Data_Count > 0)
        {
            *out_size = Data_Info->Data_Size_List[0];
            out_data = malloc(sizeof(uint8_t) * (*out_size));
            memcpy(out_data, Data_Info->p_start, *out_size);
            memmove(Data_Info->p_start, Data_Info->p_start + *out_size, (Data_Info->p_end - Data_Info->p_start) - *out_size);
            memmove(&Data_Info->Data_Pointer_List[0], &Data_Info->Data_Pointer_List[1], MEMORY_USED_DATA_LIST_SIZE - 1);
            memmove(&Data_Info->Data_Size_List[0], &Data_Info->Data_Size_List[1], MEMORY_USED_DATA_LIST_SIZE - 1);
            *Data_Info->Data_Count = *Data_Info->Data_Count - 1;
            Data_Info->p_now = Data_Info->p_now - *out_size;
            switch(Data_Info->type)
            {
                case INT_32: //int32
                {         
                    break;
                }
                case UINT_8: //uint8 or char
                {   
                    break;
                }
                case OCTET_STRING: //OCTET_STRING
                {
                    break;
                }
                default: break;
            }
        }else{

        }
        pthread_mutex_unlock(&Data_Info->mtx);
        
        return (void *)out_data;
    }
}
/**
 * @brief 
 * 
 * Data_Info:데이터 저장 영역의 정보
 * Return:데이터 타입에 따라 값을 저장해서 void 포인터로 반환
 */
int F_i_Memory_Data_Reading(struct Memory_Used_Data_Info_t *Data_Info, uint8_t *out_data)
{   
    size_t data_size = 0;
    if(F_Memory_Data_isEmpty(Data_Info))
    {
        return -1;
    }else{
        out_data = (uint8_t*)F_v_Memory_Data_Pop(Data_Info, &data_size);
        F_i_Memory_Data_Push(Data_Info, out_data, data_size);
        return 0;
    }
}

/**
 * @brief 
 * 할당된 메모리 중 데이터를 저장하기 위한 영역을 반환
 * Used_Info:할당된 메모리에 대한 정보
 * Data_Info:데이터 저장 영역의 정보
 * Return:
 */
void F_Memory_Data_Free(struct Memory_Used_Info_t Used_Info, struct Memory_Used_Data_Info_t *Data_Info)
{
    pthread_mutex_lock(&Data_Info->mtx);
    memset(Data_Info->p_start, 0x00, (Data_Info->p_end - Data_Info->p_start));
    if(Data_Info->p_end == Used_Info.Memroy_Full_Start + Used_Info.Memroy_Full_Size)
    {
    }else
    {
        memmove(Data_Info->p_start, Data_Info->p_end, (Used_Info.Memroy_Full_Start + Used_Info.Memroy_Full_Size) - Data_Info->p_end);
        pthread_mutex_destroy(&Data_Info->mtx);
        free(Data_Info->Data_Count);
    }
    pthread_mutex_unlock(&Data_Info->mtx);
   
    return;
}
/**
 * @brief 
 * 사용 가능 여부 확인
 * Data_Info:데이터 저장 영역의 정보
 * Data_Size:
 * Return:
 */
bool F_Memory_Data_isAvaialbe(struct Memory_Used_Data_Info_t *Data_Info, size_t Data_Size)
{

    pthread_mutex_lock(&Data_Info->mtx);
   
    bool ret;
    if((Data_Info->p_end - Data_Info->p_now) >= Data_Size)
    {
        ret = true;
    }else{
        ret = false;
    }
    pthread_mutex_unlock(&Data_Info->mtx);
    return ret;
}
/**
 * @brief 
 * 사용 가능 여부 확인
 * Data_Info:데이터 저장 영역의 정보
 * Return:
 */
bool F_Memory_Data_isEmpty(struct Memory_Used_Data_Info_t *Data_Info)
{
    pthread_mutex_lock(&Data_Info->mtx);
    bool ret;
    if((Data_Info->p_now - Data_Info->p_start) == 0)
    {
        ret = true;
    }else{
        ret = false;
    }
    pthread_mutex_unlock(&Data_Info->mtx);
    return ret;
}
/**
 * @brief 
 * 사용 가능 여부 확인
 * Data_Info:데이터 저장 영역의 정보
 * Return:
 */
bool F_Memory_Data_isFull(struct Memory_Used_Data_Info_t *Data_Info)
{
    pthread_mutex_lock(&Data_Info->mtx);
    bool ret;
    size_t Data_Size;
    switch(Data_Info->type)
    {
        case INT_32: Data_Size = sizeof(int);break;
        case UINT_8: Data_Size = sizeof(uint8_t);break;
        default: break;
    }
    if(Data_Info->p_now > Data_Info->p_end - Data_Size)
    {
        ret = true;
    }else{
        ret = false;
    }
    pthread_mutex_unlock(&Data_Info->mtx);
    return ret;
}
