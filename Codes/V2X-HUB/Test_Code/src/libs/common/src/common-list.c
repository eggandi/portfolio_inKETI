#include "common_include.h"
#include "common_uds_define.h"


static bool f_b_VH_Common_List_IsEmpty(struct VH_Common_List_t *list)
{

    if(!list){
        list = (struct VH_Common_List_t*)malloc(sizeof(struct VH_Common_List_t));
        list->list_num = malloc(sizeof(uint32_t));
        *list->list_num = 0;
    }
    if(list->list_num == NULL){

        list->list_num = malloc(sizeof(uint32_t));
        *list->list_num = 0;
    }

    if(*list->list_num == 0)
    {
        while(list->prev)
        {
            list = list->prev;
        }
        while(list->next)
        {
            if(list->list_data)
            {   
                F_v_VH_Common_List_Flush(list);
            }
            list = list->next;
        }
        return true;
    }
    if(list->list_data == NULL)
    {
        return true;
    }
    
    return false;
}

extern void F_v_VH_Common_List_Flush(struct VH_Common_List_t *list)
{
    while(list->next)
    {
        list = list->next;
    }
    while(list->prev)
    {
        if(list->list_data)
        {
            free(list->list_data);
            (*list->list_num)--;
        }
        list = list->prev;
    }
}

extern void F_v_VH_Common_List_Add(struct VH_Common_List_t *list, void *data)
{
    while(list->next)
    {
        list = list->next;
    }
    if(f_b_VH_Common_List_IsEmpty(list))
    {
        list->list_data = data;
        list->next = NULL;
        (*list->list_num)++;
    }else{
        list->next = (struct VH_Common_List_t*)malloc(sizeof(struct VH_Common_List_t));
        list->next->prev = list;
        list->next->list_num = list->list_num;
        list = list->next;
        list->list_data = data;
        list->next = NULL;
        (*list->list_num)++;
    }
}

extern void F_v_VH_Common_List_Del(struct VH_Common_List_t *list, void *data)
{
    struct VH_Common_List_t *temp_list;
    
    if(data)
    {
        while(list->prev)
        {
            list = list->prev;
        }

    }else{
        while(list->next)
        {
            list = list->next;
        }
        while(list->prev)
        {
            if(list->list_data == data)
            {
                break;
            }
            list = list->prev;
        }
    }
    if(f_b_VH_Common_List_IsEmpty(list))
    {

    }else{
        free(list->list_data);
        list->next->prev = NULL;
        temp_list = list;
        temp_list->next = NULL;
        temp_list->list_num = NULL;
        list = list->next;
        (*list->list_num)--;
    }
}
