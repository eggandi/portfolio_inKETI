#include "share_memory.h"

#define DEBUG_1 //printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

static void f_v_VH_Data_SHM_Part_Callback_Install(void *part_ptr);
static void f_v_VH_Data_SHM_Setup_Part_Info(void *part_ptr);
static void f_v_VH_Data_SHM_Setup_Part_List(void *ptr);

extern void *F_i_VH_Data_SHM_Initial(key_t *shm_key)
{
	key_t key_value;DEBUG_1
	struct VH_SHM_Info_t *shm_info;DEBUG_1
	
	if(*shm_key != 0)
	{
		key_value = *shm_key;DEBUG_1
	}else{
		srand(time(NULL));DEBUG_1
		key_value = (DATA_SHA_MEMORY_DEFULT_KEY % 10000) * 10000 + (rand() % 10000 + 1);DEBUG_1
		*shm_key = key_value;DEBUG_1
	}
	
	int shm_id = shmget(key_value, DATA_SHA_MEMORY_DEFAULT_TOTAL_SIZE, 0777 | IPC_CREAT);DEBUG_1
	if(shm_id== -1)
	{
		return NULL;//(-1) * ((DOMAIN_CODE_DATA + DATA_LIST_CODE_SHA_MEMORY) + 01 + 01);DEBUG_1
	}
	shm_info = (struct VH_SHM_Info_t*)shmat(shm_id, NULL, 0);DEBUG_1
	if(shm_info == (void *)-1)
	{
		return NULL;//(-1) * ((DOMAIN_CODE_DATA + DATA_LIST_CODE_SHA_MEMORY) + 01 + 02);DEBUG_1
	}
	memset(shm_info , 0x00, DATA_SHA_MEMORY_DEFAULT_TOTAL_SIZE);DEBUG_1
	shm_info->shm_hdr_ptr = (struct VH_SHM_Info_t**)shm_info;DEBUG_1
	shm_info->shm_id = (int*)((char*)(shm_info->shm_hdr_ptr) + sizeof(struct VH_SHM_Info_t));DEBUG_1
	shm_info->shm_base_ptr = (void*)(shm_info->shm_id + 1);DEBUG_1
	for(int i = 0; i < DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM; i++)
	{
		if(i == 0)
		{
			shm_info->part_list[i] = (struct VH_SHM_Part_List_t*)(shm_info->shm_base_ptr + 1);DEBUG_1
			shm_info->part_state[i] = (enum VH_SHM_Part_State_e*)(shm_info->part_list[i] + 1);DEBUG_1
			shm_info->part_inuse_size[i] = (size_t*)(shm_info->part_state[i] + 1);DEBUG_1
		}else{
			shm_info->part_list[i] = (struct VH_SHM_Part_List_t*)((char*)shm_info->part_list[i - 1] + DATA_SHA_MEMORY_DEFAULT_PART_SIZE);DEBUG_1
			shm_info->part_state[i] = (enum VH_SHM_Part_State_e*)((char*)shm_info->part_state[i - 1] + DATA_SHA_MEMORY_DEFAULT_PART_SIZE);DEBUG_1
			shm_info->part_inuse_size[i] = (size_t*)((char*)shm_info->part_inuse_size[i - 1] + DATA_SHA_MEMORY_DEFAULT_PART_SIZE);DEBUG_1
		}

		shm_info->part_list[i]->part = (void**)(shm_info->part_list[i]);DEBUG_1
		shm_info->part_list[i]->part_num = (uint32_t*)((char*)(shm_info->part_list[i]->part + sizeof(struct VH_SHM_Part_List_t)));DEBUG_1
	}
	*shm_info->shm_hdr_ptr = (struct VH_SHM_Info_t*)shm_info->shm_hdr_ptr;DEBUG_1
	*shm_info->shm_id = shm_id;DEBUG_1
	*shm_info->shm_base_ptr = shm_info;DEBUG_1
	f_v_VH_Data_SHM_Setup_Part_List(shm_info);DEBUG_1
	
	return	(void*)shm_info;DEBUG_1
}

extern INLINE_F bool F_b_VH_Data_SHM_Part_Callback_Data_Unuse(void *part_ptr)
{
	struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)part_ptr;
	switch(*part->part_state)
	{
		default:break;
		case SHM_PART_UNUSE:
		{
			return true;
		}
		case SHM_PART_INUSE:
		{
			return false;
		}
	}
	return false;
}

extern INLINE_F bool F_b_VH_Data_SHM_Part_Callback_Data_IsEmpty(void *part_ptr)
{
	struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*(((struct VH_SHM_Part_Info_t*)part_ptr)->part_hdr_ptr);DEBUG_1
	if(*part->data_num == 0)
	{
		if(*part->data_list != NULL || *part->data_list != 0)
		{
			(*part->Data_Flush)(part);DEBUG_1
		}
		return true;DEBUG_1
	}else{
		return false;DEBUG_1
	}
}

extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Push(void *part_ptr, void *ptr_input_data, bool *foraced)                    
{
	if(ptr_input_data && part_ptr)
	{
		struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)part_ptr;DEBUG_1
		if((*part->Data_IsEmpty)(part) && (*part->Data_Unuse)(part))
		{
			*part->part_state = SHM_PART_INUSE;DEBUG_1
			struct VH_SHM_Data_Send_Protocol_t *intput_data = (struct VH_SHM_Data_Send_Protocol_t *)ptr_input_data;DEBUG_1
			struct VH_SHM_Data_Info_t *data = (struct VH_SHM_Data_Info_t*)(part->Data_IsEmpty + 1);DEBUG_1
				data->data_hdr_ptr = (void**)data;DEBUG_1
				data->data_list_ptr = (struct VH_SHM_Data_List_t*)((char*)(data) + sizeof(struct VH_SHM_Data_Info_t));DEBUG_1
					struct VH_SHM_Data_List_t *data_list = (struct VH_SHM_Data_List_t*)data->data_list_ptr;DEBUG_1
						data_list->data = (void**)(data_list);DEBUG_1
						data_list->data_num = (uint32_t**)((char*)(data_list->data) + sizeof(struct VH_SHM_Data_List_t));DEBUG_1
						data_list->next = (struct VH_SHM_Data_List_t**)(data_list->data_num + 1);DEBUG_1
						data_list->prev = (struct VH_SHM_Data_List_t**)(data_list->next + 1);DEBUG_1
				
				data->data_type = (enum VH_SHM_Data_Type_e*)(data_list->prev + 1);DEBUG_1
				data->data_state = (enum VH_SHM_Data_State_e*)(data->data_type + 1);DEBUG_1
				data->data_len = (size_t*)(data->data_state + 1);DEBUG_1
				data->payload_ptr = (void*)(data->data_len + 1);DEBUG_1
			*data_list->data = (struct VH_SHM_Data_Info_t *)data->data_hdr_ptr;DEBUG_1
			
			*data_list->data_num = part->data_num;DEBUG_1
			*data_list->next = NULL;DEBUG_1
			*data_list->prev = NULL;DEBUG_1
			*part->inused_data_size = 0;DEBUG_1

			*data->data_type = intput_data->data_type;DEBUG_1
			*data->data_state = intput_data->data_state;DEBUG_1
			*data->data_len = intput_data->data_len;DEBUG_1
			memcpy(data->payload_ptr, intput_data->payload_ptr, *data->data_len);DEBUG_1
			
			*part->data_num = 1;DEBUG_1
			*part->inused_data_size += *data->data_len + (  sizeof(struct VH_SHM_Data_Info_t)  + \
															sizeof(struct VH_SHM_Data_List_t) + \
																sizeof(uint32_t*) + \
																sizeof(struct VH_SHM_Data_List_t*) + \
																sizeof(struct VH_SHM_Data_List_t*) + \
															sizeof(enum VH_SHM_Data_Type_e) + \
															sizeof(enum VH_SHM_Data_State_e) + \
															sizeof(size_t));		
			*part->data_list = data->data_list_ptr;DEBUG_1
			*part->part_state = SHM_PART_UNUSE;DEBUG_1

		}else if((*part->Data_Unuse)(part))
		{
			*part->part_state = SHM_PART_INUSE;DEBUG_1
			struct VH_SHM_Data_List_t *data_list = (struct VH_SHM_Data_List_t*)*part->data_list;DEBUG_1
			struct VH_SHM_Data_Send_Protocol_t *intput_data = (struct VH_SHM_Data_Send_Protocol_t *)ptr_input_data;DEBUG_1
			
			//Error Case
			if((DATA_SHA_MEMORY_DEFAULT_PART_SIZE - 172)  <= (*part->inused_data_size + intput_data->data_len))
			{
				if(*foraced)
				{
					while((DATA_SHA_MEMORY_DEFAULT_PART_SIZE - 172) >= (*part->inused_data_size + intput_data->data_len))
					{
						F_v_VH_Data_SHM_Part_Callback_Data_Pop(part_ptr, NULL, NULL);DEBUG_1
					}
				}else{
					printf("Share Momory Part Buffer is Full. start_ptr:%p, Data_Num:%d, Used/Size:%ld/%d\n", part, *part->data_num, *part->inused_data_size, DATA_SHA_MEMORY_DEFAULT_PART_SIZE);DEBUG_1
					//usleep(100 * 1000);DEBUG_1
					*part->part_state = SHM_PART_UNUSE;DEBUG_1
					return;DEBUG_1
				}
			}
			size_t shift_len = intput_data->data_len +  ( sizeof(struct VH_SHM_Data_Info_t)  + \
															sizeof(struct VH_SHM_Data_List_t) + \
																sizeof(uint32_t*) + \
																sizeof(struct VH_SHM_Data_List_t*) + \
																sizeof(struct VH_SHM_Data_List_t*) + \
															sizeof(enum VH_SHM_Data_Type_e) + \
															sizeof(enum VH_SHM_Data_State_e) + \
															sizeof(size_t));DEBUG_1
			uint32_t while_block_counter = 0;DEBUG_1

			while(*data_list->next != NULL)
			{
				if(while_block_counter > *part->data_num)
				{
					//Error Case
					(*part->Data_Flush)(part);DEBUG_1
				}
				data_list = *data_list->next;DEBUG_1
				while_block_counter++;DEBUG_1
			}	
					
			char *data_start_ptr = (char*)(part->Data_IsEmpty + 1);DEBUG_1
			memmove(data_start_ptr + shift_len, data_start_ptr , *part->inused_data_size);DEBUG_1
			memset(data_start_ptr, 0x00, shift_len);DEBUG_1

			struct VH_SHM_Data_List_t *next_data_list;DEBUG_1
			for(uint32_t i = 0; i < *part->data_num; i++)
			{
				//printf("[%d/%d]\n", i + 1, *part->data_num);DEBUG_1
				data_list = (struct VH_SHM_Data_List_t*)((char*)data_list + shift_len);DEBUG_1
				data_list->data = (void**)((char*)data_list->data + shift_len);DEBUG_1
				data_list->data_num = (uint32_t**)((char*)data_list->data_num + shift_len);DEBUG_1
				data_list->next = (struct VH_SHM_Data_List_t**)((char*)data_list->next + shift_len);DEBUG_1
				data_list->prev = (struct VH_SHM_Data_List_t**)((char*)data_list->prev + shift_len);DEBUG_1
				struct VH_SHM_Data_Info_t *shift_data = ((struct VH_SHM_Data_Info_t *)data_list->data);DEBUG_1
				shift_data->data_list_ptr = (struct VH_SHM_Data_List_t*)((char*)(shift_data->data_list_ptr) + (shift_len));DEBUG_1				
				shift_data->data_type = (enum VH_SHM_Data_Type_e*)((char*)(shift_data->data_type) + (shift_len));DEBUG_1
				shift_data->data_state = (enum VH_SHM_Data_State_e*)((char*)(shift_data->data_state) + (shift_len));DEBUG_1
				shift_data->data_len = (size_t*)((char*)(shift_data->data_len) + (shift_len));DEBUG_1
				shift_data->payload_ptr = (void*)((char*)(shift_data->data_type) + (shift_len));DEBUG_1
				shift_data->data_hdr_ptr = (void**)((char*)(shift_data->data_hdr_ptr) + (shift_len));DEBUG_1

				if(*data_list->prev != NULL)
				{	
					data_list = *data_list->prev;DEBUG_1
				}
				if(*shift_data->data_list_ptr->next != NULL)
				{
					*shift_data->data_list_ptr->next = (struct VH_SHM_Data_List_t*)((char*)(*shift_data->data_list_ptr->next) + (shift_len));DEBUG_1
				}		
				if(*shift_data->data_list_ptr->prev != NULL)
				{
					*shift_data->data_list_ptr->prev = (struct VH_SHM_Data_List_t*)((char*)(*shift_data->data_list_ptr->prev) + (shift_len));DEBUG_1
				}
				next_data_list = shift_data->data_list_ptr;	
			}
						
			struct VH_SHM_Data_Info_t *data = (struct VH_SHM_Data_Info_t*)(part->Data_IsEmpty + 1);DEBUG_1
				data->data_hdr_ptr = (void**)data;DEBUG_1
				data->data_list_ptr = (struct VH_SHM_Data_List_t*)((char*)(data) + sizeof(struct VH_SHM_Data_Info_t));DEBUG_1
				struct VH_SHM_Data_List_t *push_data_list = (struct VH_SHM_Data_List_t*)data->data_list_ptr;DEBUG_1
					push_data_list->data = (void**)(push_data_list);DEBUG_1
					push_data_list->data_num = (uint32_t**)((char*)(push_data_list->data) + sizeof(struct VH_SHM_Data_List_t));DEBUG_1
					push_data_list->next = (struct VH_SHM_Data_List_t**)(push_data_list->data_num + 1);DEBUG_1
					push_data_list->prev = (struct VH_SHM_Data_List_t**)(push_data_list->next + 1);DEBUG_1
				data->data_type = (enum VH_SHM_Data_Type_e*)(push_data_list->prev + 1);DEBUG_1
				data->data_state = (enum VH_SHM_Data_State_e*)(data->data_type + 1);DEBUG_1
				data->data_len = (size_t*)(data->data_state + 1);DEBUG_1
				data->payload_ptr = (void*)(data->data_len + 1);DEBUG_1
			*push_data_list->data = (struct VH_SHM_Data_Info_t *)data->data_hdr_ptr;DEBUG_1
			*push_data_list->data_num = part->data_num;DEBUG_1
			
			*push_data_list->next = next_data_list;DEBUG_1
			*push_data_list->prev = NULL;DEBUG_1
			
			*next_data_list->prev = (struct VH_SHM_Data_List_t*)data->data_list_ptr;DEBUG_1
			*data->data_type = intput_data->data_type;DEBUG_1
			*data->data_state = intput_data->data_state;DEBUG_1
			*data->data_len = intput_data->data_len;DEBUG_1
			memcpy(data->payload_ptr, intput_data->payload_ptr, *data->data_len);DEBUG_1
			*part->data_num += 1;DEBUG_1
			*push_data_list->data_num = *push_data_list->data_num;DEBUG_1
			*part->inused_data_size += *data->data_len + (  sizeof(struct VH_SHM_Data_Info_t)  + \
															sizeof(struct VH_SHM_Data_List_t) + \
																sizeof(uint32_t*) + \
																sizeof(struct VH_SHM_Data_List_t*) + \
																sizeof(struct VH_SHM_Data_List_t*) + \
															sizeof(enum VH_SHM_Data_Type_e) + \
															sizeof(enum VH_SHM_Data_State_e) + \
															sizeof(size_t));	
				
			*part->data_list = data->data_list_ptr;DEBUG_1
			*part->part_state = SHM_PART_UNUSE;DEBUG_1
		}
	}else{
		return;DEBUG_1
	}
	
}

extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Pop(void *part_ptr, void *pop_ptr, bool *only_read)
{
	if(part_ptr)
	{
		struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*(((struct VH_SHM_Part_Info_t*)part_ptr)->part_hdr_ptr);DEBUG_1
		if((*part->Data_IsEmpty)(part))
		{
			return;DEBUG_1
		}else if((*part->Data_Unuse)(part))
		{
			*part->part_state = SHM_PART_INUSE;DEBUG_1
			struct VH_SHM_Data_List_t *data_list = *part->data_list;DEBUG_1
			struct VH_SHM_Data_Send_Protocol_t *pop_data_ptr = (struct VH_SHM_Data_Send_Protocol_t*)pop_ptr;DEBUG_1
			uint32_t while_block_counter = 0;DEBUG_1
			while(*data_list->next != NULL)
			{
				if(while_block_counter > *part->data_num)
				{
					//Error
					(*part->Data_Flush)(part);DEBUG_1
					return;DEBUG_1
				}
				data_list = *data_list->next;DEBUG_1
				while_block_counter++;DEBUG_1
			}
			struct VH_SHM_Data_Info_t *data = (struct VH_SHM_Data_Info_t *)*(data_list->data);DEBUG_1
			if(pop_data_ptr)
			{
				pop_data_ptr->data_type = *data->data_type;DEBUG_1
				pop_data_ptr->data_state = *data->data_state;DEBUG_1
				pop_data_ptr->data_len = *data->data_len;DEBUG_1
				if(pop_data_ptr->payload_ptr == NULL)
				{
					pop_data_ptr->payload_ptr = (void*)malloc(sizeof(char*) * (*data->data_len));
				}
				memcpy(pop_data_ptr->payload_ptr, data->payload_ptr, *data->data_len);DEBUG_1
			}
			if(only_read)
			{

			}else{
				size_t pop_len = *data->data_len + (sizeof(struct VH_SHM_Part_Info_t)  + \
													sizeof(struct VH_SHM_Data_List_t) + \
														sizeof(uint32_t*) + \
														sizeof(struct VH_SHM_Data_List_t*) + \
														sizeof(struct VH_SHM_Data_List_t*) + \
													sizeof(enum VH_SHM_Data_Type_e) + \
													sizeof(enum VH_SHM_Data_State_e) + \
													sizeof(size_t));DEBUG_1
				memset(data, 0x00, pop_len);DEBUG_1
				*part->data_num -= 1;DEBUG_1
				*part->inused_data_size -= pop_len;DEBUG_1
				data_list = *data_list->prev;DEBUG_1
				(*data_list->next)->data_num = NULL;DEBUG_1
				*(*data_list->next)->data = NULL;DEBUG_1
				*(*data_list->next)->prev = NULL;DEBUG_1
				*data_list->next = NULL;DEBUG_1
			}
			while_block_counter = 0;DEBUG_1
			while(*data_list->prev != NULL)
			{
				if(while_block_counter > *part->data_num)
				{
					//Error
					(*part->Data_Flush)(part);DEBUG_1
				}
				data_list = *data_list->prev;DEBUG_1
				while_block_counter++;DEBUG_1
			}
			*part->part_state = SHM_PART_UNUSE;DEBUG_1
		}
		
	}else{
		return;DEBUG_1
	}
}

extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Read(void *part_ptr, bool *only_read)
{
	if(part_ptr)
	{
		F_v_VH_Data_SHM_Part_Callback_Data_Pop(part_ptr, NULL, only_read);DEBUG_1
	}
}

extern INLINE_F void F_v_VH_Data_SHM_Part_Callback_Data_Flush(void *part_ptr)
{
	if(part_ptr)
	{
		struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*(((struct VH_SHM_Part_Info_t*)part_ptr)->part_hdr_ptr);DEBUG_1
		*part->part_state = SHM_PART_INUSE;DEBUG_1
		char *flush_start_ptr = (char*)((char*)((*part->Data_IsEmpty)) + sizeof(bool*));DEBUG_1
		memset(flush_start_ptr, 0x00, *part->inused_data_size);DEBUG_1
		*part->data_num = 0;DEBUG_1
		*part->inused_data_size = 0;DEBUG_1
		*part->data_list = NULL;DEBUG_1
		*part->part_state = SHM_PART_UNUSE;DEBUG_1
	}
}

static void f_v_VH_Data_SHM_Part_Callback_Install(void *part_ptr)
{
	if(part_ptr)
	{	
		struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*(((struct VH_SHM_Part_Info_t*)part_ptr)->part_hdr_ptr);DEBUG_1
		(*part->Data_Push) = F_v_VH_Data_SHM_Part_Callback_Data_Push;DEBUG_1
		(*part->Data_Pop) = F_v_VH_Data_SHM_Part_Callback_Data_Pop;DEBUG_1
		(*part->Data_Read) = F_v_VH_Data_SHM_Part_Callback_Data_Read;DEBUG_1
		(*part->Data_Flush) = F_v_VH_Data_SHM_Part_Callback_Data_Flush;DEBUG_1
		(*part->Data_Unuse) = F_b_VH_Data_SHM_Part_Callback_Data_Unuse;DEBUG_1
		(*part->Data_IsEmpty) = F_b_VH_Data_SHM_Part_Callback_Data_IsEmpty;DEBUG_1
	}
}

static void f_v_VH_Data_SHM_Setup_Part_Info(void *part_ptr)
{
	struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t *)part_ptr;DEBUG_1
	part->part_hdr_ptr = (void*)part;DEBUG_1
	part->part_type = (enum VH_SHM_Part_Type_e*)((char*)(part->part_hdr_ptr) + sizeof(struct VH_SHM_Part_Info_t));DEBUG_1
	part->part_state = (enum VH_SHM_Part_State_e*)(part->part_type + 1);DEBUG_1
	part->data_num = (uint32_t*)(part->part_state + 1);DEBUG_1
	part->inused_data_size = (size_t*)(part->data_num + 1);DEBUG_1
	part->data_list = (struct VH_SHM_Data_List_t**)(part->inused_data_size + 1);DEBUG_1

	part->Data_Push = (void(**)())((char*)(part->data_list) + sizeof(struct VH_SHM_Data_List_t*));DEBUG_1
	part->Data_Pop = (void(**)())(part->Data_Push + 1);DEBUG_1
	part->Data_Read = (void(**)())(part->Data_Pop + 1);DEBUG_1
	part->Data_Flush = (void(**)())(part->Data_Read + 1);DEBUG_1
	part->Data_Unuse = (bool(**)())(part->Data_Flush + 1);DEBUG_1
	part->Data_IsEmpty = (bool(**)())(part->Data_Unuse + 1);DEBUG_1
	f_v_VH_Data_SHM_Part_Callback_Install(part_ptr);DEBUG_1
	*part->part_state = SHM_PART_UNUSE;DEBUG_1
	*part->data_list = NULL;DEBUG_1
}

static void f_v_VH_Data_SHM_Setup_Part_List(void *ptr)
{
	struct VH_SHM_Info_t *shm_info_ptr = (struct VH_SHM_Info_t*)ptr;DEBUG_1
	struct VH_SHM_Part_List_t **part_list = shm_info_ptr->part_list;DEBUG_1
	enum VH_SHM_Part_State_e **part_state = shm_info_ptr->part_state;DEBUG_1
	size_t **part_inuse_size = shm_info_ptr->part_inuse_size;DEBUG_1
	for(int i = 0; i < DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM; i++)
	{
		*(part_list[i]->part_num) = DATA_SHA_MEMORY_DEFAULT_TOTAL_PART_NUM;DEBUG_1

		f_v_VH_Data_SHM_Setup_Part_Info(*(part_list[i]->part));DEBUG_1
		struct VH_SHM_Part_Info_t *part_hdr_ptr = *(part_list[i]->part);DEBUG_1
		part_state[i] = part_hdr_ptr->part_state;DEBUG_1
		part_inuse_size[i] = part_hdr_ptr->inused_data_size;DEBUG_1
	}

}
