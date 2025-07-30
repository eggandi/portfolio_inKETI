#include <./memory_allocation_include.h>

struct Memory_Used_Info_t F_s_Memory_Initial(size_t Using_Size);
struct Memory_Used_Data_Info_t F_s_Memory_Data_Distributtion(struct Memory_Used_Info_t Used_Info, size_t Using_Size, int *err);
size_t F_i_Memory_Data_Push(struct Memory_Used_Data_Info_t *Data_Info,  void *Data, size_t Data_size);
void *F_v_Memory_Data_Pop(struct Memory_Used_Data_Info_t *Data_Info, size_t *out_size);
void F_Memory_Data_Free(struct Memory_Used_Info_t Used_Info, struct Memory_Used_Data_Info_t *Data_Info);
bool F_Memory_Data_isAvaialbe(struct Memory_Used_Data_Info_t *Data_Info, size_t Data_Size);
bool F_Memory_Data_isEmpty(struct Memory_Used_Data_Info_t *Data_Info);
bool F_Memory_Data_isFull(struct Memory_Used_Data_Info_t *Data_Info);
