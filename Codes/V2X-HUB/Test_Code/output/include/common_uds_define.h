#define _D_LIB_COMMON_LIST 1

#define RAMDISK_DEVICE_PATH "/dev/ram0"
#define RAMDISK_DEFAULT_SIZE 2 //Megabyte

struct VH_Common_List_t{
    void *list_data;

    uint32_t *list_num;
    struct VH_Common_List_t *next;
    struct VH_Common_List_t *prev;
};

#ifdef _D_LIB_COMMON_LIST
    extern void F_v_VH_Common_List_Flush(struct VH_Common_List_t *list);
    extern void F_v_VH_Common_List_Del(struct VH_Common_List_t *list, void *data);
    extern void F_v_VH_Common_List_Add(struct VH_Common_List_t *list, void *data);
#endif