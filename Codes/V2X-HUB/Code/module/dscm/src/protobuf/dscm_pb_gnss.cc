/**
 * @file
 * @brief DSCMRedis () Protobuf GNSS 기능 구현
 * @date 2025-10-21
 * @author donggyu
 */

// 모듈 헤더파일
#include "dscm_pb.h"

extern "C" 
{
	using namespace std;
	using namespace DSCMDC;
	/**
	 * @brief VHMM 고빈도(GNSS) C 구조체 데이터를 C++ 구조체 V2cVehicleData의 v2xhub highFreqData 필드에 넣는다. 
	 * @param[in] V2cVehicleData V2cVehicleData 포인터
	 * @param[in] gnss_raw GNSS C 구조체 포인터
	 * @return 없음
	 */
	void DSCM_SetPBV2cVehicleDataFromGNSS_CStructure(DSCM_PBHandler_Ptr_V2cVehicleData h, KVHGNSSData *gnss_raw)
	{
		auto *V2cVehicleData = static_cast<DSCMDC::V2cVehicleData*>(h);
		// V2cVehicleData의 v2xhub 필드 참조
		auto *v2xhub = V2cVehicleData->mutable_v2xhub();
		// v2xhub의 highFreqData 필드 참조
		auto *highFreqData = v2xhub->mutable_highfreqdata();
			// highFreqData의 gnss 필드 참조
			auto *gnss = highFreqData->mutable_gnss();
				gnss->set_timestamp(gnss_raw->timestamp);
				gnss->set_lat(gnss_raw->lat);
				gnss->set_lon(gnss_raw->lon);
				gnss->set_elev(gnss_raw->elev);
				gnss->set_smajor_axis_acc(gnss_raw->smajor_axis_acc);
				gnss->set_sminor_axis_acc(gnss_raw->sminor_axis_acc);	
				gnss->set_smajor_axis_ori(gnss_raw->smajor_axis_ori);
				gnss->set_speed(gnss_raw->speed);
				gnss->set_heading(gnss_raw->heading);
				gnss->set_accel_lon(gnss_raw->accel_lon);
				gnss->set_accel_lat(gnss_raw->accel_lat);
				gnss->set_accel_vert(gnss_raw->accel_vert);
				gnss->set_yawrate(gnss_raw->yawrate);
				gnss->set_fix(static_cast<GnssFixType>(gnss_raw->fix));
				gnss->set_net_rtk_fix(static_cast<NetworkRtkFixType>(gnss_raw->net_rtk_fix));

		return;
	};

}  