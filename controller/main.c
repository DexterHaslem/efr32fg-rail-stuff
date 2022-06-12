#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  #include "sl_power_manager.h"
#endif
#include "app_init.h"
#include "app_process.h"
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

#if !defined(SL_CATALOG_KERNEL_PRESENT)
/// A static handle of a RAIL instance
static RAIL_Handle_t rail_handle;
#endif

int main(void)
{
	// Initialize Silicon Labs device, system, service(s) and protocol stack(s).
	// Note that if the kernel is present, processing task(s) will be created by
	// this call.
	sl_system_init();

	sl_system_kernel_start();
}
