#include "sl_component_catalog.h"
#include "rail.h"

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "app_task_init.h"
#endif

#include "app_log.h"
#include "sl_sleeptimer.h"

static volatile uint8_t count = 0;

void app_process_action(RAIL_Handle_t rail_handle)
{
  (void) rail_handle;

}

/******************************************************************************
 * RAIL callback, called if a RAIL event occurs
 *****************************************************************************/
void sl_rail_util_on_event(RAIL_Handle_t rail_handle, RAIL_Events_t events)
{
  (void) rail_handle;
  (void) events;

#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif

  if ( events & RAIL_EVENTS_RX_COMPLETION ) {
      if ( events & RAIL_EVENT_RX_PACKET_RECEIVED ) {
          ++count;
      }
  }
}


void timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  app_log_warning("received packet cnt: %u\r\n", count);
}
