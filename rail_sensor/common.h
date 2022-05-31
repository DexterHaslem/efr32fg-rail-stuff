#define PAYLOAD_LENGTH 16
#define BUFFER_LENGTH 256

extern uint8_t txBuffer[BUFFER_LENGTH];
void timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);
