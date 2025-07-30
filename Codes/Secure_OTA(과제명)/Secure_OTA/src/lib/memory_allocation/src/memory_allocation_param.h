#include <./memory_allocation_include.h>

#define MAX_MEMORY_SIZE 0x0FFFFFFF
#define MEMORY_USED_DATA_LIST_SIZE 1024
#define MEMORY_USED_INFO_LIST_SIZE 128

enum Data_Type_e
{
    INT_32,
    UINT_8,
    UINT_15,
    OCTET_STRING
};
struct Memory_Used_Data_Info_t
{
    pthread_mutex_t mtx;
    enum Data_Type_e type;
    uint8_t *p_start;
    uint8_t *p_end;
    uint8_t *p_now; 
    
    uint8_t *Data_Pointer_List[MEMORY_USED_DATA_LIST_SIZE];
    size_t Data_Size_List[MEMORY_USED_DATA_LIST_SIZE];
    size_t *Data_Count;
    #if 0 //To Be Making
        bool Used_Index;
        int Index_List[Queue_Index_Size][1024];
    #endif

};

struct Memory_Used_Info_t
{
    pthread_mutex_t mtx;
    uint8_t *Memroy_Full_Start;
    uint8_t *Memroy_Full_Now;
    size_t Memroy_Full_Size;
    int Memroy_Used_List_Num;
    struct Memory_Used_Data_Info_t *Memory_Used_Data_Info_List[MEMORY_USED_INFO_LIST_SIZE]; 
    
};