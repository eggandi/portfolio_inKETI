import ctypes
import os
import platform
import struct
import random
import time
import sys
import os
import pefile


# 현재 OS 확인 (Windows or Linux)

is_windows = platform.system() == "Windows"
library_path = "./bin"
dll_dir = os.path.abspath(library_path)
os.add_dll_directory(dll_dir)

# 라이브러리 경로 설정 (경로 결합은 `os.path.join()` 사용)
lib_filename = "libsed_upload_keti.dll" if is_windows else "libsed_upload_keti.so"
dll_path = os.path.abspath(os.path.join(library_path, lib_filename))

print("Python Achteck:", struct.calcsize("P") * 8, "비트")
try:
    lib = ctypes.CDLL(dll_path)
    print(f"라이브러리 로드 성공: {dll_path}")
    sys.stdout.flush()
except OSError as e:
    print(f"라이브러리 로드 실패: {e}")
    
# 구조체 정의 (C 구조체 변환)

class SedBufferMessageType(ctypes.c_int):
    SED_BUFFER_MESSAGE_MSGID = 0
    SED_BUFFER_MESSAGE_TIMESTAMP = 1
    SED_BUFFER_MESSAGE_VEHICLE_INFO = 2
    SED_BUFFER_MESSAGE_RISKY_DRV_S = 3
    SED_BUFFER_MESSAGE_ROAD_EVENT_S = 4
    SED_BUFFER_MESSAGE_DMS_EVENT_S = 5

        
class VehicleInfo(ctypes.Structure):
    _fields_ = [
        ("vin", ctypes.c_char * 17),
        ("vehicle_type", ctypes.c_int),
        ("vehicle_lat", ctypes.c_int),
        ("vehicle_lng", ctypes.c_int)
    ]

class RiskyDrv(ctypes.Structure):
    _fields_ = [
        ("ex_speed", ctypes.c_uint32, 1),
        ("cont_ex_speed", ctypes.c_uint32, 1),
        ("rapid_acc", ctypes.c_uint32, 1),
        ("rapid_start", ctypes.c_uint32, 1),
        ("rapid_dec", ctypes.c_uint32, 1),
        ("rapid_braking", ctypes.c_uint32, 1),
        ("rapid_lane_change", ctypes.c_uint32, 1),
        ("sharp_passing", ctypes.c_uint32, 1),
        ("sharp_left_turn", ctypes.c_uint32, 1),
        ("sharp_right_turn", ctypes.c_uint32, 1),
        ("sharp_u_turn", ctypes.c_uint32, 1),
        ("reserved", ctypes.c_uint32, 21)  # 나머지 21비트 예약
    ]
    _pack_ = 1  # __attribute__((__packed__))에 해당하는 설
    
class RiskyDrvS(ctypes.Structure):
    _fields_ = [("risky_drv", RiskyDrv * 1)]

class RoadEvent(ctypes.Structure):
    _fields_ = [("road_cond", ctypes.c_int, 8),
                ("reserved", ctypes.c_uint32, 24)  # 나머지 21비트 예약
    ]
    _pack_ = 1

class RoadEventS(ctypes.Structure):
    _fields_ = [("road_event", RoadEvent * 1)]

class DmsEvent(ctypes.Structure):
    _fields_ = [
        ("drowsy_drv", ctypes.c_int, 8),
        ("driver_distraction", ctypes.c_int, 8),
        ("reserved", ctypes.c_uint32, 16)  # 나머지 21비트 예약
    ]
    _pack_ = 1

class DmsEventS(ctypes.Structure):
    _fields_ = [("dms_event", DmsEvent * 1)]

    
class AzHttpResponse(ctypes.Structure):
    _fields_ = [
        ("status_code", ctypes.c_int),
        ("content", ctypes.c_char_p)
    ]
    
# Azure Blob Client 초기화 함수
lib.SED_Azure_Blob_Client_Init.argtypes = [ctypes.c_char_p]
lib.SED_Azure_Blob_Client_Init.restype = ctypes.c_void_p  # 반환 타입이 void*

# 버퍼 초기화 함수
lib.SED_Buffer_Init.argtypes = [ctypes.POINTER(ctypes.c_int)]
lib.SED_Buffer_Init.restype = None

# 버퍼 만료 핸들러 활성화
lib.SED_Buffer_Signal_Expired_Active.argtypes = [ctypes.c_bool]
lib.SED_Buffer_Signal_Expired_Active.restype = None

# JSON 데이터 생성 함수
lib.SED_JSON_Print.argtypes = [ctypes.POINTER(ctypes.c_char_p)]
lib.SED_JSON_Print.restype = ctypes.c_int

# 데이터 푸시 함수
lib.SED_Buffer_Push_Data.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.c_int]
lib.SED_Buffer_Push_Data.restype = None

# `SED_Azure_Blob_Upload` 함수 정의
lib.SED_Azure_Blob_Upload.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.POINTER(AzHttpResponse)]
lib.SED_Azure_Blob_Upload.restype = ctypes.c_int



#Define the base SAS token URL (container level)
blob_name = "test_20250305"  # Name of the blob to upload to (e.g., "myfile.txt"). This is the name that the file will be saved with in the Azure container
sas_token = "sp=racwdl&st=2025-01-26T15:05:40Z&se=2026-01-01T23:05:40Z&spr=https&sv=2022-11-02&sr=c&sig=CXjVrtaipY6syiT71zEHzVI66RxkzHwbyCrh3KBhDsk%3D"

# Construct the full blob URL by appending the blob name to the SAS URL (before the `?`)
blob_url = f"https://sliceduploadtest.blob.core.windows.net/sliced-data/{blob_name}?{sas_token}".encode("utf-8")
blob_url_c = ctypes.create_string_buffer(blob_url)

# Main 함수 변환
if __name__ == "__main__":

    expried_time = ctypes.c_int(3)
    client = lib.SED_Azure_Blob_Client_Init(blob_url_c)  # 1) Init Azure Blob client
    if client:
        print("Azure Blob Client 초기화 성공")
    else:
        print("Azure Blob Client 초기화 실패")
    lib.SED_Buffer_Init(ctypes.byref(ctypes.c_int(1)))  # 2) Init Buffer
    #lib.SED_Buffer_Signal_Expired_Active(True)  # 3) Set Active Expired Signal Handler
    
    # 5) Push Data to Buffer
    item_count = ctypes.c_int(1)
    vehicle_info = VehicleInfo()
    risky_drv_s = RiskyDrvS()
    road_event_s = RoadEventS()
    dms_event_s = DmsEventS()
    
    #Fill the Filed !!!!

    msgid = random.randint(1, int("0xffffffff", 16)) 

    timestamp = b"250111111511" #str size Max 12
    vehicle_info.vin = b"12345678901234567" #str size Max 17
    vehicle_info.vehicle_type = random.randint(0, 3) * 16
    vehicle_info.vehicle_lat = 12331
    vehicle_info.vehicle_lng = 33453

    risky_drv_s.risky_drv[0].ex_speed = 1                       
    risky_drv_s.risky_drv[0].cont_ex_speed = 1
    risky_drv_s.risky_drv[0].rapid_acc = 1
    risky_drv_s.risky_drv[0].rapid_start = 0
    risky_drv_s.risky_drv[0].rapid_dec = random.randint(0, 1)
    risky_drv_s.risky_drv[0].rapid_braking = random.randint(0, 1)
    risky_drv_s.risky_drv[0].rapid_lane_change = random.randint(0, 1)
    risky_drv_s.risky_drv[0].sharp_passing = random.randint(0, 1)
    risky_drv_s.risky_drv[0].sharp_left_turn = random.randint(0, 1)
    risky_drv_s.risky_drv[0].sharp_right_turn = random.randint(0, 1)
    risky_drv_s.risky_drv[0].sharp_u_turn = random.randint(0, 1)
    
    road_event_s.road_event[0].road_cond = random.randint(0, 1)
    
    dms_event_s.dms_event[0].drowsy_drv = random.randint(0, 1)
    dms_event_s.dms_event[0].driver_distraction = random.randint(0, 1)


    msgid_c_int = ctypes.byref(ctypes.c_int(msgid))
    timestamp_c_str = ctypes.byref(ctypes.create_string_buffer(timestamp))
    vehicle_info_c_stu = ctypes.byref(vehicle_info)
    risky_drv_s_c_stu = ctypes.byref(risky_drv_s)
    road_event_s_c_stu = ctypes.byref(road_event_s)
    dms_event_s_c_stu = ctypes.byref(dms_event_s)
    
    lib.SED_Buffer_Push_Data(SedBufferMessageType.SED_BUFFER_MESSAGE_MSGID,         msgid_c_int, 0)
    lib.SED_Buffer_Push_Data(SedBufferMessageType.SED_BUFFER_MESSAGE_TIMESTAMP,     timestamp_c_str, 0)
    lib.SED_Buffer_Push_Data(SedBufferMessageType.SED_BUFFER_MESSAGE_VEHICLE_INFO,  vehicle_info_c_stu, 0)
    lib.SED_Buffer_Push_Data(SedBufferMessageType.SED_BUFFER_MESSAGE_RISKY_DRV_S,   risky_drv_s_c_stu, 1)
    lib.SED_Buffer_Push_Data(SedBufferMessageType.SED_BUFFER_MESSAGE_ROAD_EVENT_S,  road_event_s_c_stu, 1)
    lib.SED_Buffer_Push_Data(SedBufferMessageType.SED_BUFFER_MESSAGE_DMS_EVENT_S,   dms_event_s_c_stu, 1)
    

    #6) Parsing JSON Data
    out_buffer = ctypes.c_char_p()
    ret = lib.SED_JSON_Print(ctypes.byref(out_buffer))
    # JSON 데이터 출력
    if ret == 0 and out_buffer.value:
        print("SED_JSON_Print 성공:", out_buffer.value.decode())
    else:
        print("SED_JSON_Print 실패")

    client_ptr = None  # `NULL` 전달 (Python에서는 `None` 사용 가능)
    out_response = AzHttpResponse()
    ret = lib.SED_Azure_Blob_Upload(None, out_buffer, ctypes.byref(out_response))

    # 업로드 결과 출력
    if ret == 0:
        print(f"Azure Blob Upload 성공, 응답 코드: {ret}")
        if ret == -4:
            print("Running sample with no_op HTTP implementation.\nRecompile az_core with an HTTP client "
                  "implementation like CURL to see sample sending network requests.\n\n"
                  "i.e. cmake -DTRANSPORT_CURL=ON")
        if out_response.content:
            print(f"응답 내용: {out_response.content.decode()}")
    else:
        print("Azure Blob Upload 실패")
        
    n = 0
    while( n < 10):
        time.sleep(1)
        n = n + 1
