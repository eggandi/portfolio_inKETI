#include "relay_gnss.h" 
#include "relay_config.h"

struct relay_inno_gnss_data_t *G_gnss_data; 
struct relay_inno_gnss_data_bsm_t *G_gnss_bsm_data; 

static int RELAY_INNO_Gnss_Put_Data(struct gps_data_t *gps_data); 
static void *RELAY_INNO_Gnss_Task_Reading(void *pData); 

static int RELAY_INNO_Gnss_Put_Data(struct gps_data_t *gps_data)
{
	while(G_gnss_data->isused == true)
	{
		usleep(1000);
	}
	G_gnss_data->isused = true;
 // TODO:: G_gnss_data->ts 도 채워야 5분 기준에 따른 인증서 변경동작이 제대로 동작한다.
    clock_gettime(CLOCK_REALTIME, &(G_gnss_data->ts));
	uint64_t msec = ((uint64_t)(G_gnss_data->ts.tv_sec) * 1000) + ((uint64_t)(G_gnss_data->ts.tv_nsec) / 1000000);
	G_gnss_data->min = (uint32_t)((msec / (60 * 1000)) % 60);
	G_gnss_data->sec = (uint32_t)(msec % (1000 * 60));

	G_gnss_data->lat_raw = gps_data->fix.latitude;
	G_gnss_data->lon_raw = gps_data->fix.longitude;
	memcpy(&G_gnss_data->fix, &gps_data->fix, sizeof(struct gps_fix_t));
	G_gnss_data->elev = 0;
	switch(gps_data->fix.mode)
	{
		default:
		case MODE_NOT_SEEN:
			G_gnss_data->status.is_monitored = false;
			G_gnss_data->status.unavailable = true;
			G_gnss_data->status.is_healthy = false;
			break;
		case MODE_NO_FIX:
			G_gnss_data->status.is_monitored = true;
			G_gnss_data->status.unavailable = false;
			G_gnss_data->status.is_healthy = false;
			break;
		case MODE_3D:
			G_gnss_data->elev = (int)(gps_data->fix.altHAE * (10));
		case MODE_2D:
			G_gnss_data->status.is_monitored = true;
			G_gnss_data->status.unavailable = false;
			G_gnss_data->status.is_healthy = true;
			G_gnss_data->lat = (int)(gps_data->fix.latitude * (1e7));
			G_gnss_data->lon = (int)(gps_data->fix.longitude * (1e7));
			G_gnss_data->speed = (uint32_t)(gps_data->fix.speed * (50));
			G_gnss_data->heading = (uint32_t)(gps_data->fix.track * (80));
			break;
	}
	G_gnss_data->num_sv = gps_data->satellites_visible;
	if (G_gnss_data->num_sv < 5) {
		G_gnss_data->status.num_sv_under_5 = true;
	}
	G_gnss_data->pdop = gps_data->dop.pdop;
	if (G_gnss_data->pdop < 5) {
		G_gnss_data->status.pdop_under_5 = true;
	}
	G_gnss_data->pps.real.tv_sec = gps_data->pps.real.tv_sec;
	G_gnss_data->pps.real.tv_nsec = gps_data->pps.real.tv_nsec;
	G_gnss_data->pps.clock.tv_sec = gps_data->pps.clock.tv_sec;
	G_gnss_data->pps.clock.tv_nsec = gps_data->pps.clock.tv_nsec;
	G_gnss_data->policy = gps_data->policy;
	G_gnss_data->pos_accuracy.semi_major = (uint32_t)(gps_data->gst.smajor_deviation * (0.02));
	G_gnss_data->pos_accuracy.semi_minor = (uint32_t)(gps_data->gst.sminor_deviation * (0.02));
	G_gnss_data->pos_accuracy.orientation = (uint32_t)(gps_data->gst.smajor_orientation * (1.820416));
	G_gnss_data->acceleration_set.lat = (int)(gps_data->attitude.acc_y);
	G_gnss_data->acceleration_set.lon = (int)(gps_data->attitude.acc_x);
	G_gnss_data->acceleration_set.vert = (int)(gps_data->attitude.acc_z * (0.050968));
	G_gnss_data->acceleration_set.yaw = (int)(gps_data->attitude.yaw *  (0.1));
	G_gnss_data->isused = false;
	if(G_gnss_bsm_data == NULL)
	{
		return 0;
	}
	if(G_gnss_bsm_data->isused == true)
	{
		G_gnss_bsm_data->isused = true;
		if (G_gnss_data->status.is_healthy == true) 
		{
			*G_gnss_bsm_data->lat = (G_gnss_data->lat);
			*G_gnss_bsm_data->lon = (G_gnss_data->lon);
			*G_gnss_bsm_data->speed = (G_gnss_data->speed);
			*G_gnss_bsm_data->heading = (G_gnss_data->heading);
			if(gps_data->fix.mode >= MODE_3D)
			{
				*G_gnss_bsm_data->elev = (G_gnss_data->elev);
			}else{
				*G_gnss_bsm_data->elev = -2047;
			}
		}else if(G_gnss_data->status.is_healthy == false && G_relay_inno_config.v2x.j2735.bsm.tx_forced == true)
		{
			*G_gnss_bsm_data->lat = G_gnss_data->lat_raw;
			*G_gnss_bsm_data->lon = G_gnss_data->lon_raw;
			*G_gnss_bsm_data->elev = -2047;
			*G_gnss_bsm_data->speed = G_gnss_data->speed;
			*G_gnss_bsm_data->heading = G_gnss_data->heading;
		}
		*G_gnss_bsm_data->pos_accuracy.semi_major = (G_gnss_data->pos_accuracy.semi_major);
		*G_gnss_bsm_data->pos_accuracy.semi_minor = (G_gnss_data->pos_accuracy.semi_minor);
		*G_gnss_bsm_data->pos_accuracy.orientation = (G_gnss_data->pos_accuracy.orientation);
		*G_gnss_bsm_data->acceleration_set.lat = (G_gnss_data->acceleration_set.lat);
		*G_gnss_bsm_data->acceleration_set.lon = (G_gnss_data->acceleration_set.lon);
		*G_gnss_bsm_data->acceleration_set.vert = (G_gnss_data->acceleration_set.vert);
		*G_gnss_bsm_data->acceleration_set.yaw = (G_gnss_data->acceleration_set.yaw);
	}
  return 0;
}

static void *RELAY_INNO_Gnss_Task_Reading(void *pData)
{
	(pData = pData);	
	int32_t ret;
	struct gps_data_t gps_data;

	ret = gps_open(GPSD_SHARED_MEMORY, 0, &gps_data);
	if(ret < 0)
	{
		_DEBUG_PRINT("Fail to open GPSD - gps_open() failed: %d\n", ret);
		return NULL;
	}
	printf("Start read Gnss_data\n");

	while(1)
	{
		ret = gps_read(&(gps_data), NULL, 0);
		G_gnss_data->isused = false;
		if(ret >= 0)
		{
			ret = RELAY_INNO_Gnss_Put_Data(&(gps_data));
		}else{
			
		}
		usleep(G_relay_inno_config.relay.gnss_interval * 1000);
	}

	gps_close(&(gps_data));
	if(G_gnss_data !=	NULL)
	{
		free(G_gnss_data);
		G_gnss_data = NULL;
	}
}

extern int RELAY_INNO_Gnss_Init_Gnssata(pthread_t *gnss_thread_t)
{
  //Allocation_Postion - Free_Position line:None
	G_gnss_data = (struct relay_inno_gnss_data_t*)malloc(sizeof(struct relay_inno_gnss_data_t));

	if (pthread_create(gnss_thread_t, NULL, *RELAY_INNO_Gnss_Task_Reading, (void *) NULL) < 0)
	{
		_DEBUG_PRINT("Failed to create GNSS thread\n");
		return -1;
	}
	pthread_detach(*gnss_thread_t);

  return 0;
}
