#include "v2x_handover_main.h"

struct v2xho_gnss_data_t *G_gnss_data;

static int f_i_V2XHO_Gnss_Put_Gnssata(struct gps_data_t *gps_data);
static void *th_v_V2XHO_Gnss_Task_Reading(void *pData);

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_UTIL_GNSS

static int f_i_V2XHO_Gnss_Put_Gnssata(struct gps_data_t *gps_data){
	pthread_mutex_lock(&(G_gnss_data->mtx));
 // TODO:: G_gnss_data->ts 도 채워야 5분 기준에 따른 인증서 변경동작이 제대로 동작한다.
    clock_gettime(CLOCK_REALTIME, &(G_gnss_data->ts));
	uint64_t msec = ((uint64_t)(G_gnss_data->ts.tv_sec) * 1000) + ((uint64_t)(G_gnss_data->ts.tv_nsec) / 1000000);
	G_gnss_data->min = (uint32_t)((msec / (60 * 1000)) % 60);
    G_gnss_data->sec = (uint32_t)(msec % (1000 * 60));

    G_gnss_data->lat_raw = gps_data->fix.latitude;
    G_gnss_data->lon_raw = gps_data->fix.longitude;
	memcpy(&G_gnss_data->fix, &gps_data->fix, sizeof(struct gps_fix_t));
    if (gps_data->fix.mode >= MODE_NO_FIX) 
    {
        G_gnss_data->status.unavailable = false;
        G_gnss_data->status.is_monitored = true;
        if (gps_data->fix.mode >= MODE_2D) 
        {
            G_gnss_data->status.is_healthy = true;
            G_gnss_data->lat = (int)(gps_data->fix.latitude * (1e7));
            G_gnss_data->lon = (int)(gps_data->fix.longitude * (1e7));
            G_gnss_data->speed = (uint32_t)(gps_data->fix.speed * (50));
            G_gnss_data->heading = (uint32_t)(gps_data->fix.track * (80));
			if (gps_data->fix.mode >= MODE_3D) 
			{
				G_gnss_data->elev = (int)(gps_data->fix.altHAE * (10));
			}
        }
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
  pthread_mutex_unlock(&(G_gnss_data->mtx));
  return 0;
}


static void *th_v_V2XHO_Gnss_Task_Reading(void *pData)
{
	(pData = pData);	
	int32_t ret;
	struct gps_data_t gps_data;
	ret = gps_open(GPSD_SHARED_MEMORY, 0, &gps_data);
	if(ret < 0)
	{
		return NULL;
	}
	printf("Start read Gnss_data\n");

	while(1)
	{
		ret = gps_read(&(gps_data), NULL, 0);
		if(ret >= 0)
		{
			ret = f_i_V2XHO_Gnss_Put_Gnssata(&(gps_data));
		}else{
			
		}
		/* wait periodic timer (timeout = 250ms) */
		usleep(250 * 1000);
	}
	gps_close(&(gps_data));
}

extern int F_i_V2XHO_Gnss_Init_Gnssata(pthread_t *gnss_thread_t)
{
	struct sched_param gnss_thread_param;
	pthread_attr_t gnss_thread_attrs;
  //Allocation_Postion - Free_Position line:None
	G_gnss_data =(struct v2xho_gnss_data_t*)calloc(sizeof(struct v2xho_gnss_data_t), 1);
	pthread_mutex_init(&(G_gnss_data->mtx), NULL);

	pthread_attr_init(&gnss_thread_attrs);
	pthread_attr_setschedpolicy(&gnss_thread_attrs, SCHED_RR);
	gnss_thread_param.sched_priority = 30;

	pthread_attr_setschedparam(&gnss_thread_attrs, &gnss_thread_param);
	if (pthread_create(gnss_thread_t, &gnss_thread_attrs, *th_v_V2XHO_Gnss_Task_Reading, (void *) NULL) < 0)
	{}
	pthread_detach(*gnss_thread_t);
	pthread_attr_destroy(&gnss_thread_attrs);

  return 0;
}

extern int th_v_V2XHO_Gnss_PPS_Read(void *d)
{
	d = (void*)d;

	return 0;
}