/**
 * @addtogroup cvcoctci_lib cvcoctci library API
 * @{
 *
 * @file
 * @brief
 * @date 2020-08-08
 * @author gyun
 */


#ifndef V2X_SW_CVCOCTCI_DEFINES_H
#define V2X_SW_CVCOCTCI_DEFINES_H


/// @brief TCI 메시지 최대 크기
#define TCI_MSG_MAX_SIZE  3000

/// @brief TemporaryID 길이
#define CVCOCTCI_TEMPORARY_ID_LEN 4

/// @brief PDU 최대 길이
#define MAX_PDU_SIZE 2304

/// @brief HashedID8 길이
#define HASHED_ID8_LEN 8

#ifndef MAC_ALEN
#define MAC_ALEN (6) ///< MAC 주소 길이
#endif

/// @brief WSA에 수납될 수 있는 Service Info 최대 개수 (표준 상의 기본값)
#define WSA_SERVICE_INFO_MAX_NUM (31)

/// @brief WSA에 수납될 수 있는 Channel Info 최대 개수 (표준 상의 기본값)
#define WSA_CHAN_INFO_MAX_NUM (31)

/// @brief MAC 주소 초기화 매크로
#define CVCOCTCI_MAC_ADDR_INITIALIZER { 0,0,0,0,0,0 }

#ifndef IPv6_ALEN
#define IPv6_ALEN (16) ///< IPv6 주소 길이
#endif

/// @brief IPv6 주소 초기화 매크로
#define CVCOCTCI_IPv6_ADDR_INITIALIZER { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }


#endif //V2X_SW_CVCOCTCI_DEFINES_H

/**
 * @}
 */
