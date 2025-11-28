/**
 * @file
 * @brief DSCMRedis () Protobuf IVDCT 기능 구현
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
	 * @brief IVDCT C 구조체 데이터를 C++ 구조체 V2cVehicleData의 IVDCT 필드에 넣는다. 
	 * @param[in] V2cVehicleData V2cVehicleData 포인터
	 * @param[in] ivdct_raw IVDCT C 구조체 포인터
	 * @return 없음
	 */
	void DSCM_SetPBV2cVehicleDataFromIVDCT_CStructure(DSCM_PBHandler_Ptr_V2cVehicleData h, KVHIVDCTData *ivdct_raw)
	{
		auto *V2cVehicleData = static_cast<DSCMDC::V2cVehicleData*>(h);
		// V2cVehicleData의 ivdct 필드 참조
		auto *ivdct = V2cVehicleData->mutable_ivdct();

		auto *system = ivdct->mutable_system();
			system->set_check_time(ivdct_raw->system.status_ct);
			system->set_status(static_cast<HealthStatus>(ivdct_raw->system.status));

		auto *driving = ivdct->mutable_driving();
			auto *speed = driving->mutable_speed();
				speed->set_wheel_speed(static_cast<double>(ivdct_raw->driving.speed.wheel_speed));
				speed->set_yawrate(static_cast<double>(ivdct_raw->driving.speed.yawrate));
				speed->set_accel_pedal_pos(static_cast<double>(ivdct_raw->driving.speed.accel_pedal_pos));
			auto *engine = driving->mutable_engine();
				double truque = static_cast<double>(ivdct_raw->driving.engine.torque_1) + static_cast<double>(ivdct_raw->driving.engine.torque_2 * 0.125);
				engine->set_torque(truque);
				engine->set_rpm(static_cast<double>(ivdct_raw->driving.engine.rpm));

			auto *gear = driving->mutable_gear(); 
				gear->set_ratio(ivdct_raw->driving.gear.ratio);
				auto *gear_current = gear->mutable_current();
				if (ivdct_raw->driving.gear.current < 0) {
					if (ivdct_raw->driving.gear.current == -5) {
						gear_current->set_mode(static_cast<DSCMDC::TransmissionState>(4));
					} else{
						gear_current->set_mode(static_cast<DSCMDC::TransmissionState>(3));
					}
					gear_current->set_gear(static_cast<int32_t>(ivdct_raw->driving.gear.current * -1));
				} else {
					if (ivdct_raw->driving.gear.current == 0) {
						gear_current->set_mode(static_cast<DSCMDC::TransmissionState>(5));
					} else if (ivdct_raw->driving.gear.current == 126) {
						gear_current->set_mode(static_cast<DSCMDC::TransmissionState>(0));
					} else {
						gear_current->set_mode(static_cast<DSCMDC::TransmissionState>(1));
					}
					gear_current->set_gear(static_cast<int32_t>(ivdct_raw->driving.gear.current));
				}
			
			auto *steering_wheel = driving->mutable_steering_wheel();
				steering_wheel->set_angle(static_cast<double>(ivdct_raw->driving.steering_wheel.angle));

			auto *brake = driving->mutable_brake();
				brake->set_pedal_pos(static_cast<double>(ivdct_raw->driving.brake.pedal_pos));
				brake->set_temp_warning(static_cast<DSCMDC::DeviceSystemStatus>(ivdct_raw->driving.brake.temp_warning));
				brake->set_tcs(static_cast<DSCMDC::DeviceSystemStatus>(ivdct_raw->driving.brake.tcs));
				brake->set_abs(static_cast<DSCMDC::DeviceSystemStatus>(ivdct_raw->driving.brake.abs));

		auto *diag = ivdct->mutable_diag();
				auto *battery = diag->mutable_battery();
					battery->set_voltage(static_cast<double>(ivdct_raw->diag.battery.voltage));
				auto *diag_engine = diag->mutable_coolant_temp();
					diag_engine->set_coolant_temp(static_cast<double>(ivdct_raw->diag.engine.coolant_temp));
				auto *fuel = diag->mutable_fuel();
					fuel->set_economy(static_cast<double>(ivdct_raw->diag.fuel.economy));

		auto *lights = ivdct->mutable_lights();
				lights->set_hazard(static_cast<DSCMDC::OnOffType>(ivdct_raw->lights.hazard_sig));

		auto *weights = ivdct->mutable_weights();
				for(int i = 0; i < KVH_IVDCT_AXLE_WEIGHT_MAX_NUM; ++i) {
					int checksum = 	ivdct_raw->weight.axle_weight[i].location_ct +
													ivdct_raw->weight.axle_weight[i].location +
													ivdct_raw->weight.axle_weight[i].weight_ct +
													ivdct_raw->weight.axle_weight[i].weight;
					if(checksum != 0) {
						auto *axle_weight = weights->add_axle_weight();
						axle_weight->set_location(static_cast<DSCMDC::AxleLocation>(ivdct_raw->weight.axle_weight[i].location));
						axle_weight->set_weight(static_cast<double>(ivdct_raw->weight.axle_weight[i].weight));
					}	else {
						break;
					}
				}
				
		return;
	};


 
}  