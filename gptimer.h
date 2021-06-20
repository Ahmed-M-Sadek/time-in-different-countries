#ifndef GP_TIMER
#define GP_TIMER

#include <stdint.h>

#define TIMER_DELAY_100_MS 0x001869FF

void TimerInit();
void TimerDelayUS(int64_t delay_in_ns);

#endif // GP_TIMER