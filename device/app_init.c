#include "sl_rail_util_init.h"
#include "sl_sleeptimer.h"

void timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);
sl_sleeptimer_timer_handle_t my_timer;
uint32_t timer_timeout = 65000;

RAIL_Handle_t app_init(void)
{
	// Get RAIL handle, used later by the application
	RAIL_Handle_t rail_handle = sl_rail_util_get_handle(
			SL_RAIL_UTIL_HANDLE_INST0);

	sl_sleeptimer_start_periodic_timer(&my_timer, timer_timeout, timer_callback,
			(void*) NULL, 0, 0);

	RAIL_Status_t status = RAIL_StartRx(rail_handle, 0, NULL);
	if (status != RAIL_STATUS_NO_ERROR)
	{
		while (1)
		{
		}
	}

	return rail_handle;
}
