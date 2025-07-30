#include "common_include.h"
#include <sys/shm.h>

#define INLINE_F inline
#define DATA_LIST_CODE_SHA_MEMORY 0x00220000

#define DATA_SHA_MEMORY_DEFULT_KEY 1835

#define DATA_SHA_MEMORY_PART_DEFAULT_INFOMATION_SIZE 128
#define DATA_SHA_MEMORY_DEFAULT_BLOCK_SIZE 1024

#define DATA_SHA_MEMORY_DEFAULT_TOTAL_SIZE DATA_SHA_MEMORY_PART_DEFAULT_INFOMATION_SIZE + (DATA_SHA_MEMORY_DEFAULT_BLOCK_SIZE * DATA_SHA_MEMORY_DEFAULT_TOTAL_BLOCK_NUM)
#define DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM 10
#define DATA_SHA_MEMORY_DEFAULT_PART_BLOCK_NUM 20
#define DATA_SHA_MEMORY_DEFAULT_PART_SIZE DATA_SHA_MEMORY_DEFAULT_BLOCK_SIZE * DATA_SHA_MEMORY_DEFAULT_PART_BLOCK_NUM
#define DATA_SHA_MEMORY_DEFAULT_TOTAL_BLOCK_NUM DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM * DATA_SHA_MEMORY_DEFAULT_PART_BLOCK_NUM

#define DATA_SHA_MEMORY_ZERO_MASK 0x00000000
#define DATA_SHA_MEMORY_PART_INFOMATION_POSITION DATA_SHA_MEMORY_ZERO_MASK

#define DATA_SHA_MEMORY_PART_HEADER_SIZE 24

enum VH_SHM_Data_Type_e
{
	SHM_PART_TYPE_READ_ONLY = 0,
	SHM_PART_TYPE_READ_WRITE, 
	SHM_PART_TYPE_SAVE_TO_FIlE,
	SHM_PART_TYPE_SAVE_TO_BUFF,
	SHM_PART_TYPE_SAVE_TO_QUEU,
};
enum VH_SHM_Data_State_e
{
	SHM_DATA_UNUSE = 0,
	SHM_DATA_INUSE,
};

struct VH_SHM_Data_Send_Protocol_t
{
	enum VH_SHM_Data_Type_e data_type;
	enum VH_SHM_Data_State_e data_state;

	size_t data_len;
	void *payload_ptr;
};

struct VH_SHM_Data_Info_t 
{
	void **data_hdr_ptr;

	struct VH_SHM_Data_List_t *data_list_ptr;
	enum VH_SHM_Data_Type_e *data_type;
	enum VH_SHM_Data_State_e *data_state;
	size_t *data_len;

	void *payload_ptr;
};

struct VH_SHM_Data_List_t
{
	void **data;

	uint32_t **data_num;
	struct VH_SHM_Data_List_t **next;
	struct VH_SHM_Data_List_t **prev;
};

enum VH_SHM_Part_Type_e
{
	SHM_PART_TYPE_MODULE_NAME_0 = 0,
	SHM_PART_TYPE_MODULE_DTG_0,
	SHM_PART_TYPE_MODULE_DMS_0,
	SHM_PART_TYPE_MODULE_ADS_0,
	SHM_PART_TYPE_MODULE_HMI_0,
	SHM_PART_TYPE_MODULE_NAME_5,
	SHM_PART_TYPE_MODULE_NAME_6,
	SHM_PART_TYPE_MODULE_NAME_7,
	SHM_PART_TYPE_MODULE_NAME_8,
	SHM_PART_TYPE_MODULE_NAME_9,
};
enum VH_SHM_Part_State_e
{
	SHM_PART_UNUSE = 0,
	SHM_PART_INUSE,
};

struct VH_SHM_Part_Info_t
{
	void **part_hdr_ptr;

	enum VH_SHM_Part_Type_e *part_type;
	enum VH_SHM_Part_State_e *part_state;
	uint32_t *data_num;
	size_t *inused_data_size;
	struct VH_SHM_Data_List_t **data_list;

	void (**Data_Push)();
	void (**Data_Pop)();
	void (**Data_Read)();
	void (**Data_Flush)();
	bool (**Data_Unuse)();
	bool (**Data_IsEmpty)();
};
struct VH_SHM_Part_List_t{
	void **part;

	uint32_t *part_num;
};

struct VH_SHM_Info_t
{
	struct VH_SHM_Info_t **shm_hdr_ptr;

	int *shm_id;
	void **shm_base_ptr;

	struct VH_SHM_Part_List_t *part_list[DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM];
	enum VH_SHM_Part_State_e *part_state[DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM];
	size_t *part_inuse_size[DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM];

};

extern void *F_i_VH_Data_SHM_Initial(key_t *shm_key);
extern INLINE_F bool F_b_VH_Data_SHM_Part_Callback_Data_Unuse(void *part_ptr);
extern INLINE_F bool F_b_VH_Data_SHM_Part_Callback_Data_IsEmpty(void *part_ptr);
extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Push(void *part_ptr, void *ptr_input_data, bool *foraced);               
extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Pop(void *part_ptr, void *pop_data_ptr, bool *only_read);
extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Read(void *part_ptr, bool *only_read);
extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Flush(void *part_ptr);
