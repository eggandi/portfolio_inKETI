#ifdef __cplusplus
extern "C" {
#endif

// FileData 구조체를 C++의 FileData 객체로 사용
typedef struct _FileData FileData;

// 함수 프로토타입 정의

void protobuf_filedata_set_content_1(FileData* file_data, const char* data, size_t length);
void protobuf_filedata_set_content_2(FileData* file_data, const char* data, size_t length);
//void protobuf_filedata_set_content_3(FileData* file_data, const char* data, size_t length);
//#void protobuf_filedata_set_content_4(FileData* file_data, const char* data, size_t length);
//void protobuf_filedata_set_content_5(FileData* file_data, const char* data, size_t length);
void protobuf_savetofile(const char* filename, FileData* file_data);
#ifdef __cplusplus
}
#endif