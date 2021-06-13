#include "stm32f10x.h"
#include "cmsis_os.h"
#include "Board_LED.h"
#define __EXTI_INTERRUPTS         0x00002001 
#define __EXTI_IMR                0x00002001 
#define __EXTI_EMR                0x00000000
#define __EXTI_RTSR               0x00000000
#define __EXTI_FTSR               0x00002001
#define __AFIO_EXTICR1            0x00000000

int posao[5] = {0};
int periodicke[3] = {0, 0, 2};

void led_Thread_periodic (void const *argument);
void led_Thread_nonperiodic (void const *argument);
void init_EXTI (void);	

osThreadDef(led_Thread_periodic, osPriorityAboveNormal, 3, 0);	
osThreadDef(led_Thread_nonperiodic, osPriorityNormal, 5, 0);

osThreadId main_Thread, thread_d1, thread_d2, thread_d3, thread_d4, 
	thread_d5, thread_d6, thread_d7, thread_d8;

osSemaphoreId sem4, sem5, sem6, sem7, sem8;									
osSemaphoreDef(sem4); osSemaphoreDef(sem5); osSemaphoreDef(sem6); osSemaphoreDef(sem7); osSemaphoreDef(sem8);

// radno cekanje
void delay (unsigned int count) {
	unsigned int index;
	for(index =0;index<count;index++) {;}
}

/*---------------------------*/
/*     PERIODICKE DRETVE     */
/*---------------------------*/

// callback funkcija timera, salje signal potrebnoj dretvi
void callback(void const *param){
	switch((uint32_t)param)
		{
			case 1UL:
				periodicke[0]=1;
				osSignalSet	(thread_d1,0x01);
			break;

			case 2UL:
				periodicke[1]=1;
				osSignalSet	(thread_d2,0x01);
			break;

			case 3UL:
				periodicke[2]=1;
				osSignalSet	(thread_d3,0x01);
			break;
		}
}

osTimerDef(timer1, callback);
osTimerDef(timer2, callback);
osTimerDef(timer3, callback);

// periodicke dretve 
void led_Thread_periodic (void const *argument) 
{
	for (;;) 
	{
		LED_On((uint32_t)argument);  
		
		if (periodicke[2]) delay(21154690);
		else if (periodicke[1]) delay(31707410);
		else if (periodicke[0]) delay(63486930);
		periodicke[0] = 0; periodicke[1] = 0; periodicke[2] = 0;
		
		LED_Off((uint32_t)argument);
		osSignalWait(0x01,osWaitForever);
	}
}

/*-----------------------------*/
/*     NEPERIODICKE DRETVE     */
/*-----------------------------*/

// prekidne rutine za pinove
// GPIOA - DIP, GPIOC - PUSH
void EXTI0_IRQHandler  (void)
{ 	
	EXTI->PR |= (1<<0);	
	if ((int)GPIOA->IDR & 0x0001) {
		osSemaphoreRelease(sem4);
	}
	if ((int)GPIOC->IDR & 0x0001) {
		++posao[0];
	}	
}

void EXTI1_IRQHandler  (void)
{
	EXTI->PR |= (1<<0);	
	if ((int)GPIOA->IDR & 0x0002) {
		osSemaphoreRelease(sem5);
	}
	if ((int)GPIOC->IDR & 0x0002) {
		++posao[1];
	}
}

void EXTI2_IRQHandler  (void)
{
	EXTI->PR |= (1<<0);		
	if ((int)GPIOA->IDR & 0x0004) {
		osSemaphoreRelease(sem6);
	}
	if ((int)GPIOC->IDR & 0x0004) {
		++posao[2];
	}	
}

void EXTI3_IRQHandler  (void)
{
	EXTI->PR |= (1<<0);	
	if ((int)GPIOA->IDR & 0x0008) {
		osSemaphoreRelease(sem7);
	}
	if ((int)GPIOC->IDR & 0x0008) {
		++posao[3];
	}		
}

void EXTI4_IRQHandler  (void)
{
	EXTI->PR |= (1<<0);	
	if ((int)GPIOA->IDR & 0x0010) {
		osSemaphoreRelease(sem8);
	}
	if ((int)GPIOC->IDR & 0x0010) {
		++posao[4];
	}		
}

// neperiodicke dretve
void led_Thread_nonperiodic (void const *argument) 
{
	uint32_t a = (uint32_t)argument;
	for (;;) 
	{
		switch(a)
		{
			case 3:
				osSemaphoreWait(sem4, osWaitForever);
			break;

			case 4:
				osSemaphoreWait(sem5, osWaitForever);
			break;

			case 5:
				osSemaphoreWait(sem6, osWaitForever);	
			break;
			
			case 6:
				osSemaphoreWait(sem7, osWaitForever);
			break;
			
			case 7:
				osSemaphoreWait(sem8, osWaitForever);
			break;
		}
		
		while (posao[(uint32_t)argument - 3] == 0)
			osDelay(50);
		
		while(posao[(uint32_t)argument - 3] > 0){
			LED_On((uint32_t)argument);                          
			delay(21154690);
			LED_Off((uint32_t)argument);
			--posao[(uint32_t)argument - 3];
		}	
	}
}

int main (void) 
{
	osTimerId timer1_handle = osTimerCreate(osTimer(timer1), osTimerPeriodic, (void *)1UL);	
	osTimerId timer2_handle = osTimerCreate(osTimer(timer2), osTimerPeriodic, (void *)2UL);	
	osTimerId timer3_handle = osTimerCreate(osTimer(timer3), osTimerPeriodic, (void *)3UL);
	
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	osTimerStart(timer1_handle, 108000);	//2sek
	osTimerStart(timer2_handle, 216000);	//4sek
	osTimerStart(timer3_handle, 432000);	//8sek

	LED_Initialize ();
	init_EXTI();
		
	sem4 = osSemaphoreCreate(osSemaphore(sem4), 0);
	sem5 = osSemaphoreCreate(osSemaphore(sem5), 0);
	sem6 = osSemaphoreCreate(osSemaphore(sem6), 0);
	sem7 = osSemaphoreCreate(osSemaphore(sem7), 0);
	sem8 = osSemaphoreCreate(osSemaphore(sem8), 0);
	
	thread_d1 = osThreadCreate(osThread(led_Thread_periodic), (void *)0UL);
	thread_d2 = osThreadCreate(osThread(led_Thread_periodic), (void *)1UL);
	thread_d3 = osThreadCreate(osThread(led_Thread_periodic), (void *)2UL);
	
	thread_d4 = osThreadCreate(osThread(led_Thread_nonperiodic), (void *)3UL);
	thread_d5 = osThreadCreate(osThread(led_Thread_nonperiodic), (void *)4UL);
	thread_d6 = osThreadCreate(osThread(led_Thread_nonperiodic), (void *)5UL);
	thread_d7 = osThreadCreate(osThread(led_Thread_nonperiodic), (void *)6UL);
	thread_d8 = osThreadCreate(osThread(led_Thread_nonperiodic), (void *)7UL);
	
	osKernelStart ();                         // start thread execution 
}

// omogucavanje vanjskog prekida
void init_EXTI(void){
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function	
	AFIO->EXTICR[0] &= 0xFFF0;                              // clear used pin
	AFIO->EXTICR[0] |= (0x000F & __AFIO_EXTICR1);           // set pin to use
	
	EXTI->IMR       |= 0x001F;       // unmask interrupt
	EXTI->EMR       |= 0x0000;       // unmask event
	EXTI->RTSR      |= 0x001F;       // set rising edge
	EXTI->FTSR      |= 0x0000;       // set falling edge

	if (__EXTI_INTERRUPTS & (1 << 0)) {                        // interrupt used
		NVIC->ISER[0] |= ~0x0;    // enable interrupts EXTI 
	}
	GPIOC->CRL &= ~0x00000000;	
}
