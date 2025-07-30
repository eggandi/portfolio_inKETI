
/**
 * @brief IVDCT 프로토콜 버퍼 핸들러를 생성한다.
 */
void IVDCTDCM_PrintfIVDCTCTypeData(KVHIVDCTData* d)
{
	if (d == NULL) {
		ERR("IVDCT data is NULL\n");
		return;
	}
	printf("IVDCT->system.status_ct: %lu\n", d->system.status_ct);
	printf("IVDCT->system.status: %lu\n", d->system.status);

	printf("IVDCT->driving.speed.wheel_speed_ct: %lu\n", d->driving.speed.wheel_speed_ct);
	printf("IVDCT->driving.speed.wheel_speed: %ld\n", d->driving.speed.wheel_speed);
	printf("IVDCT->driving.speed.yawrate_ct: %lu\n", d->driving.speed.yawrate_ct);
	printf("IVDCT->driving.speed.yawrate: %ld\n", d->driving.speed.yawrate);
	printf("IVDCT->driving.speed.accel_pedal_pos_ct: %lu\n", d->driving.speed.accel_pedal_pos_ct);
	printf("IVDCT->driving.speed.accel_pedal_pos: %ld\n", d->driving.speed.accel_pedal_pos);
	
	printf("IVDCT->driving.engine.torque_1_ct: %lu\n", d->driving.engine.torque_1_ct);
	printf("IVDCT->driving.engine.torque_1: %ld\n", d->driving.engine.torque_1);
	printf("IVDCT->driving.engine.torque_2_ct: %lu\n", d->driving.engine.torque_2_ct);
	printf("IVDCT->driving.engine.torque_2: %ld\n", d->driving.engine.torque_2);
	printf("IVDCT->driving.engine.rpm_ct: %lu\n", d->driving.engine.rpm_ct);
	printf("IVDCT->driving.engine.rpm: %ld\n", d->driving.engine.rpm);
	
	printf("IVDCT->driving.gear.ratio_ct: %lu\n", d->driving.gear.ratio_ct);
	printf("IVDCT->driving.gear.ratio: %ld\n", d->driving.gear.ratio);
	printf("IVDCT->driving.gear.current_ct: %lu\n", d->driving.gear.current_ct);
	printf("IVDCT->driving.gear.current: %ld\n", d->driving.gear.current);
	
	printf("IVDCT->driving.steering_wheel.angle_ct: %lu\n", d->driving.steering_wheel.angle_ct);
	printf("IVDCT->driving.steering_wheel.angle: %ld\n", d->driving.steering_wheel.angle);
	
	printf("IVDCT->driving.brake.pedal_pos_ct: %lu\n", d->driving.brake.pedal_pos_ct);
	printf("IVDCT->driving.brake.pedal_pos: %ld\n", d->driving.brake.pedal_pos);
	printf("IVDCT->driving.brake.temp_warning_ct: %lu\n", d->driving.brake.temp_warning_ct);
	printf("IVDCT->driving.brake.temp_warning: %lu\n", d->driving.brake.temp_warning);
	printf("IVDCT->driving.brake.tcs_ct: %lu\n", d->driving.brake.tcs_ct);
	printf("IVDCT->driving.brake.tcs: %lu\n", d->driving.brake.tcs);
	printf("IVDCT->driving.brake.abs_ct: %lu\n", d->driving.brake.abs_ct);
	printf("IVDCT->driving.brake.abs: %lu\n", d->driving.brake.abs);

	printf("IVDCT->diag.battery.voltage_ct: %lu\n", d->diag.battery.voltage_ct);
	printf("IVDCT->diag.battery.voltage: %ld\n", d->diag.battery.voltage);
	printf("IVDCT->diag.engine.coolant_temp_ct: %lu\n", d->diag.engine.coolant_temp_ct);
	printf("IVDCT->diag.engine.coolant_temp: %ld\n", d->diag.engine.coolant_temp);
	printf("IVDCT->diag.fuel.economy_ct: %lu\n", d->diag.fuel.economy_ct);
	printf("IVDCT->diag.fuel.economy: %ld\n", d->diag.fuel.economy);
	
	printf("IVDCT->lights.hazard_sig_ct: %lu\n", d->lights.hazard_sig_ct);
	printf("IVDCT->lights.hazard_sig: %lu\n", d->lights.hazard_sig);

	for (int i = 1; i < KVH_IVDCT_AXLE_WEIGHT_MAX_NUM; ++i) {
		if(d->weight.axle_weight == NULL)
			continue;
		printf("IVDCT->weight.axle_weight[%d].location_ct: %lu\n", i, d->weight.axle_weight[i].location_ct);
		printf("IVDCT->weight.axle_weight[%d].location: %ld\n", i, d->weight.axle_weight[i].location);
		printf("IVDCT->weight.axle_weight[%d].weight_ct: %lu\n", i, d->weight.axle_weight[i].weight_ct);
		printf("IVDCT->weight.axle_weight[%d].weight: %ld\n", i, d->weight.axle_weight[i].weight);
	}
}