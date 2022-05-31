#include "sl_component_catalog.h"
#include "rail.h"

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "app_task_init.h"
#endif


#include "app_log.h"
#include "common.h"


static uint8_t ready_send = 0;


void app_process_action(RAIL_Handle_t rail_handle)
{
  (void) rail_handle;

  if (ready_send && rail_handle)
  {
      RAIL_RadioState_t cur_state = RAIL_GetRadioState(rail_handle);
      if (cur_state == RAIL_RF_STATE_IDLE || cur_state == RAIL_RF_STATE_ACTIVE) {
        uint8_t test = 123;
        RAIL_Status_t status =  RAIL_SetTxFifo(rail_handle, &test, 0, 1);
        if (status != RAIL_STATUS_NO_ERROR) {
            app_log_error("failed to write tx fifo\r\n");
        }
        status = RAIL_StartTx(rail_handle, 0, RAIL_TX_OPTIONS_DEFAULT, NULL);
        if (status != RAIL_STATUS_NO_ERROR) {
            app_log_error("failed to RAIL_StartTx\r\n");
        }
      } else {
          app_log_warning("skipped tx, radio not idle: %02x\r\n", cur_state);
      }
      ready_send = 0;
  }
}


void timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  app_log_warning("timer fired \r\n");
  ready_send = 1;

}

/******************************************************************************
 * RAIL callback, called if a RAIL event occurs
 *****************************************************************************/
void sl_rail_util_on_event(RAIL_Handle_t rail_handle, RAIL_Events_t events)
{
  (void) rail_handle;
  (void) events;

    app_log_warning("sl_rail_util_on_event: completion event %02x\r\n", events);


#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif
}
