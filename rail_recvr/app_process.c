#include "sl_component_catalog.h"
#include "rail.h"

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "app_task_init.h"
#endif

#include "app_log.h"
#include "sl_sleeptimer.h"
#include <stdint.h>
#include <stdbool.h>
#include "rail.h"
static volatile uint32_t count = 0;
static uint8_t read_buff[128];
static uint32_t *value = (uint32_t*) &read_buff[0];
static bool got_packet = false;

static uint64_t last_events;
void app_process_action(RAIL_Handle_t rail_handle)
{
	if (!got_packet)
		return;

	RAIL_RxPacketHandle_t packet_handle;
	RAIL_RxPacketInfo_t packet_info;
	packet_handle = RAIL_GetRxPacketInfo(rail_handle,
			RAIL_RX_PACKET_HANDLE_OLDEST_COMPLETE, &packet_info);
	if (packet_handle != RAIL_RX_PACKET_HANDLE_INVALID)
	{
		RAIL_RxPacketDetails_t packet_details;
		RAIL_CopyRxPacket(read_buff, &packet_info);
		RAIL_GetRxPacketDetails(rail_handle, packet_handle, &packet_details);
		RAIL_ReleaseRxPacket(rail_handle, packet_handle);

		printf("RX");
		for (int i = 0; i < packet_info.packetBytes; i++)
		{
			printf(" 0x%02X", read_buff[i]);
		}
		printf("; RSSI=%d dBm\n", packet_details.rssi);
		printf("\r\n");
	}
	//pkt->
//	uint16_t bytes_avail = RAIL_GetRxFifoBytesAvailable(rail_handle);
//	if (bytes_avail > 0 && bytes_avail < 17) {
//		RAIL_ReadRxFifo(rail_handle, read_buff, bytes_avail);
//	}
	got_packet = false;
	//app_log_info("latest packet state: received=%lu last value=%lu\r\n", count,
	//		value);
}

/******************************************************************************
 * RAIL callback, called if a RAIL event occurs
 *****************************************************************************/
void sl_rail_util_on_event(RAIL_Handle_t rail_handle, RAIL_Events_t events)
{
	last_events = events;
#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif
	//app_log_warning("rx Events: %lx\r\n", events);

	if (events & RAIL_EVENTS_RX_COMPLETION)
	{
		if (events & RAIL_EVENT_RX_PACKET_RECEIVED)
		{
			RAIL_HoldRxPacket(rail_handle);
			++count;
			got_packet = true;
		}
	}
}

void timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
	//app_log_warning("\rreceived packet cnt: %u\r\n", count);
}
