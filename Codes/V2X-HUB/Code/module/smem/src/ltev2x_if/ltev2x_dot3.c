/**
 * @file
 * @brief CSM(C-ITS Service Module)의 V2X 서비스 기능 구현
 * @date 2025-08-28
 * @author Dong
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief Set the PSID for Dot3
 * @param[in] psid Dot3 PSID
 * @retval Dot3PSID
 */
Dot3PSID LTEV2X_SetParameterDot3PSID(Dot3PSID psid)
{
	int ret;
	switch (psid) {
		case kDot3PSID_Min:
		case kDot3PSID_Max:
		case kDot3PSID_IPv6Routing:
		case kDot3PSID_WSA:
		case kDot3PSID_NA:
			ret = psid;
			break;
		default :
			LTEV2X_MarcoJ2735RangeCheck(ret, psid, kDot3PSID_Max, kDot3PSID_Min);
			break;
	}
	return (Dot3PSID)ret;
}


/**
 * @brief Set Dot3 Parameter ChannelNumber
 * @param[in] chan_num ChannelNumber 포인터
 * @retval Dot3ChannelNumber
 */
Dot3ChannelNumber LTEV2X_SetParameterDot3ChannelNumber(Dot3ChannelNumber *chan_num)
{
	int ret;
	if (chan_num == NULL) {
		ret = kDot3ChannelNumber_NA;
	} else {
		switch (*chan_num) {
			case kDot3ChannelNumber_Min:
			case kDot3ChannelNumber_CCH:
			case kDot3ChannelNumber_Max:
			case kDot3ChannelNumber_NA:
			case kDot3ChannelNumber_KoreaV2XMin:
			case kDot3ChannelNumber_KoreaV2XCCH:
			case kDot3ChannelNumber_KoreaV2XMax:
				ret = *chan_num;
			break;
			default :
				LTEV2X_MarcoJ2735RangeCheck(ret, *chan_num, kDot3ChannelNumber_Max, kDot3ChannelNumber_Min);
				break;
		}
	}
	return (Dot3ChannelNumber)ret;
}


/**
 * @brief Set Dot3 Parameter DataRate
 * @param[in] datarate DataRate 포인터	
 * @retval Dot3DataRate
 */
Dot3DataRate LTEV2X_SetParameterDot3DataRate(Dot3DataRate *datarate)
{
	int ret;
	if (datarate == NULL) {
		ret = kDot3DataRate_NA;
	} else {
		switch (*datarate) {
			case kDot3DataRate_3Mbps: /// = kDot3DataRate_10MHzMin, kDot3DataRate_Min
			case kDot3DataRate_4p5Mbps:
			case kDot3DataRate_6Mbps: /// = kDot3DataRate_20MHzMin, kDot3DataRate_TxDefault
			case kDot3DataRate_9Mbps:
			case kDot3DataRate_12Mbps:
			case kDot3DataRate_18Mbps:
			case kDot3DataRate_24Mbps:
			case kDot3DataRate_27Mbps: /// = kDot3DataRate_10MHzMax, kDot3DataRate_Max
			case kDot3DataRate_36Mbps:
			case kDot3DataRate_48Mbps:
			case kDot3DataRate_54Mbps: /// = kDot3DataRate_20MHzMax
			case kDot3DataRate_NA:
				ret = *datarate;
			break;
			default :
				ret = *datarate * (-1);
				break;
		}
	}
	return (Dot3DataRate)ret;
}


/**
 * @brief Set Dot3 Parameter TransmitPower
 * @param[in] transmit_power TransmitPower 포인터
 * @retval Dot3Power
 */
Dot3Power LTEV2X_SetParameterDot3TransmitPower(Dot3Power *transmit_power)
{
	int ret;
	if (transmit_power == NULL) {
		ret = kDot3Power_NA;
	} else {
		switch (*transmit_power) {
			case kDot3Power_Min:
			case kDot3Power_Max:
			case kDot3Power_MaxInClassC: /// = kDot3Power_TxDefault
			case kDot3Power_MaxEIRPInClassC:
			case kDot3Power_NA:
				ret = *transmit_power;
			break;
			default :
				LTEV2X_MarcoJ2735RangeCheck(ret, *transmit_power, kDot3Power_Max, kDot3Power_Min);
				break;
		}
	}
	return (Dot3Power)ret;
}


/**
 * @brief WSMP 파라메터 설정
 * @param[in] params WSMP 입력 파라메터 입력 받을 포인터
 * @param[in] version WSMP 헤더에 수납되는 프로토콜 버전 (생성 시에는 값을 입력할 필요 없음)
 * @param[in] psid WSMP 헤더에 수납되는 PSID
 * @param[in] chan_num WSMP 헤더 확장필드에 수납되는 채널번호 (NA 일 경우 수납 X)
 * @param[in] datarate WSMP 헤더 확장필드에 수납되는 데이터레이트 (NA 일 경우 수납 X)
 * @param[in] transmit_power WSMP 헤더 확장필드에 수납되는 전송파워 (NA 일 경우 수납 X)
 * @retval 0: 성공
 * @retval -1: 실패, -입력값(범위 오류)
 */
int LTEV2X_SetParameterDot3(LTEV2XDot3WSMParameters *params,  Dot3ProtocolVersion version,
																															Dot3PSID psid,
																															Dot3ChannelNumber *chan_num,
																															Dot3DataRate *datarate,
																															Dot3Power *transmit_power)
{
	if (!params) {
		return -1;
	}
	// version 입력값 체크 후 입력
	LTEV2X_MarcoJ2735RangeCheck(params->version, version, kDot3ProtocolVersion_Current, kDot3ProtocolVersion_Current);
	// PSID 입력값 체크 후 입력
	if ((int)(params->psid = LTEV2X_SetParameterDot3PSID(psid)) < 0)
	{
		return params->psid;
	}
	// 채널번호 입력값 체크 후 입력
	if ((int)(params->chan_num = LTEV2X_SetParameterDot3ChannelNumber(chan_num)) < 0)
	{
		return params->chan_num;
	}
	// 데이터레이트 입력값 체크 후 입력
	if ((int)(params->datarate = LTEV2X_SetParameterDot3DataRate(datarate)) < 0)
	{
		return params->datarate;
	}
	// 전송파워 입력값 체크 후 입력
	if ((int)(params->transmit_power = LTEV2X_SetParameterDot3TransmitPower(transmit_power)) < 0)
	{
		return params->transmit_power;
	}
	
	return 0;
}

