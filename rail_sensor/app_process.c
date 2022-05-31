#include "sl_component_catalog.h"
#include "rail.h"

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "app_task_init.h"
#endif


#include "app_log.h"
#include "common.h"


static uint8_t ready_send = 0;

static uint8_t fifo_setup = 0;
#define BUFFER_LENGTH 256
#define PAYLOAD_LENGTH 16
static uint8_t tx_buffer[BUFFER_LENGTH];
static const uint8_t payload[PAYLOAD_LENGTH] =
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0F};


void app_process_action(RAIL_Handle_t rail_handle)
{
  if (!fifo_setup) {
      uint16_t fifo_ret = RAIL_SetTxFifo(rail_handle, tx_buffer, 0, BUFFER_LENGTH);
      if (fifo_ret == 0) {
          app_log_error("failed to write tx fifo\r\n");
      }
      fifo_setup = 1;
      return;
  }

  (void) rail_handle;

  if (ready_send && rail_handle)
  {
      RAIL_RadioState_t cur_state = RAIL_GetRadioState(rail_handle);
      if (cur_state == RAIL_RF_STATE_IDLE || cur_state == RAIL_RF_STATE_ACTIVE) {

        RAIL_WriteTxFifo(rail_handle, payload, PAYLOAD_LENGTH, false);
        //status = RAIL_StartTxStream(rail_handle, 0, RAIL_STREAM_CARRIER_WAVE);
        RAIL_Status_t status = RAIL_StartTx(rail_handle, 0, RAIL_TX_OPTIONS_DEFAULT, NULL);
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
  app_log_warning("tx Events: %lx\r\n", events);
  if ((events & RAIL_EVENT_TX_STARTED) != 0) {
      app_log_warning("got tx started\r\n");
  }
  if ((events & RAIL_EVENT_TX_ABORTED) != 0) {
      app_log_warning("got tx abort\r\n");
  }
  if ((events & RAIL_EVENT_TX_PACKET_SENT) != 0) {
      app_log_warning("packet sent\r\n");
  }

  if ((events & RAIL_EVENT_TX_BLOCKED) != 0) {
        app_log_warning("got tx blocked\r\n");
    }
  if ((events & RAIL_EVENT_TX_UNDERFLOW) != 0) {
        app_log_warning("got tx underflow\r\n");
    }
#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif
}
