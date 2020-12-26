#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* Sets up system hardware */
struct data {
	void *key;
	int led;
} dd[3];
static void prvSetupHardware(void) {
	SystemCoreClockUpdate();
	Board_Init();
	/* Initial LED0 state is off */
	Board_LED_Set(0, true);
	Board_LED_Set(1, true);
	Board_LED_Set(2, true);
}
/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters) {
	struct data *d = pvParameters;
	int led_n = (*d).led;
//int led_n = 0;
	bool LedState = false;
	while (1) {
		if (xSemaphoreTake((d->key), 50)) {
			Board_LED_Set(led_n, false);
			LedState = (bool) !LedState;
			/* About a 3Hz on/off toggle rate */
			vTaskDelay(1000);
			Board_LED_Set(led_n, true);
//vTaskDelay(3500);
			xSemaphoreGive((d->key));
			vTaskDelay(1000);
			4
		}
	}
}
int main(void) {
	xSemaphoreHandle xSemaphore = NULL;
	xSemaphore = xSemaphoreCreateMutex();
	prvSetupHardware();
	/* LED1 toggle thread */
	if (xSemaphore != NULL) {
		dd[0].key = xSemaphore;
		dd[0].led = 0;
		dd[1].key = xSemaphore;
		dd[1].led = 1;
		dd[2].key = xSemaphore;
		dd[2].led = 2;
		xTaskCreate(vLEDTask1, (signed char* ) "vTaskLed1",
				configMINIMAL_STACK_SIZE, &dd[0], (tskIDLE_PRIORITY+1UL),
				(xTaskHandle *) NULL);
		xTaskCreate(vLEDTask1, (signed char* ) "vTaskLed2",
				configMINIMAL_STACK_SIZE, &dd[1], (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);
		xTaskCreate(vLEDTask1, (signed char* ) "vTaskLed3",
				configMINIMAL_STACK_SIZE, &dd[2], (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);
		/* Start the scheduler */
		vTaskStartScheduler();
	}
	/* Should never arrive here */
	return 1;
}
