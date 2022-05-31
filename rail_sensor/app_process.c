/***************************************************************************//**
 * @file
 * @brief app_process.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "sl_component_catalog.h"
#include "rail.h"

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "app_task_init.h"
#endif


#include "app_log.h"
#include "common.h"
#include "simple_rail_tx.h"
// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------
//static uint8_t tx_buf[16];
//static uint8_t sent = 0;
static const uint8_t payload[PAYLOAD_LENGTH] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
//uint8_t txBuffer[BUFFER_LENGTH];
static uint8_t ready_send = 0;

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------
/******************************************************************************
 * Application state machine, called infinitely
 *****************************************************************************/
void app_process_action(RAIL_Handle_t rail_handle)
{
  (void) rail_handle;

  // its aborted
//  uint32_t v = 0x4000000;
//  uint32_t tb = (v & RAIL_EVENT_TX_BLOCKED) != 0;
//  uint32_t ta = (v & RAIL_EVENT_TX_ABORTED) != 0;
////
//  app_log_warning("aborted = %x, blocked = %x\r\n", ta, tb);

  ///////////////////////////////////////////////////////////////////////////
  // Put your application code here!                                       //
  // This is called infinitely.                                            //
  // Do not call blocking functions from here!                             //
  ///////////////////////////////////////////////////////////////////////////
// if (!sent) {
//  sl_simple_rail_tx_transmit(tx_buf, 16);
//  sent = 1 ;
// }
  if (ready_send && rail_handle)
  {
      //sl_status_t status = sl_simple_rail_tx_transmit(payload, 1);
      //if (status != SL_STATUS_OK){
      //    app_log_error("failed to tx\r\n");
      //}
//      RAIL_Status_t status;
//      status = RAIL_WriteTxFifo(rail_handle, payload, PAYLOAD_LENGTH, false);
//      if (status != RAIL_STATUS_NO_ERROR) {
//          app_log_error("failed to write tx fifo\r\n");
//      }
//      status = RAIL_StartTx(rail_handle, 0, RAIL_TX_OPTIONS_DEFAULT, NULL);
//      if (status != RAIL_STATUS_NO_ERROR) {
//          app_log_error("failed to RAIL_StartTx\r\n");
//      }
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

  ///////////////////////////////////////////////////////////////////////////
  // Put your RAIL event handling here!                                    //
  // This is called from ISR context.                                      //
  // Do not call blocking functions from here!                             //
  ///////////////////////////////////////////////////////////////////////////

#if defined(SL_CATALOG_KERNEL_PRESENT)
  app_task_notify();
#endif
}


// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------
