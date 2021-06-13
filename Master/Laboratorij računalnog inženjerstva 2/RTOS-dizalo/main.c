#include "stm32f10x.h"
#include "cmsis_os.h"
#include "Board_LED.h"
#include <stdbool.h>
#define __EXTI_INTERRUPTS         0x00002001
#define __EXTI_IMR                0x00002001
#define __EXTI_EMR                0x00000000
#define __EXTI_RTSR               0x00000000
#define __EXTI_FTSR               0x00002001
#define __AFIO_EXTICR1            0x00000000

#define SECOND_TICKS 15000

#define LIFT_COUNT 2

/* metoda za inicijalizaciju vanjskih interupta */
void EXTI_init(void);

/* sve radne dretve */
void lift_thread(void const *data);
void lift_scheduler_thread(void const *data);
void lift_status_thread(void const *data);

/* metoda za rad sa LEDicama*/
void LED_update(int index, int value);

/* funkcije za ispis na serijsko sucelje */
void USART1_Init(void);
void USART_print(const char *text);
void USART_print_char(char c);

#define MOVING_UP 'G'
#define MOVING_DOWN 'D'
#define WAITING_CLOSED 'S'
#define WAITING_OPEN '-'
#define WAITING ' '

enum {
    LS_WAITING 							= 0x01,
    LS_GOING_UP							= 0x02,
    LS_GOING_DOWN						= 0x04,
    LS_STARTED_GOING_UP			= 0x08,
    LS_STARTED_GOING_DOWN		= 0x10,
    LS_STOPING							= 0x20
};


typedef struct {
	int floor;
	char direction;
} request_t;

typedef struct {
    int id;
    int state;
    int floor;
    char display_state;

    osMessageQId queue;
    request_t requests[10];
    int request_num;
} lift_t;

lift_t  lifts[LIFT_COUNT];

osThreadDef(lift_thread, osPriorityNormal, 2, 0);
osThreadDef(lift_scheduler_thread, osPriorityNormal, 1, 0);
osThreadDef(lift_status_thread, osPriorityNormal, 1, 0);
osMessageQDef(lift_A_queue, 10, int); 
osMessageQDef(lift_B_queue, 10, int); 
osMessageQDef(schedule_queue, 10, int); 
osMessageQId(schedule_queue);

const char *lift_status(const lift_t *lift)
{
    switch (lift->display_state) {
        case MOVING_UP:
            return "G ";
        case MOVING_DOWN:
            return "D ";
        case WAITING_OPEN:
            return "--";
        case WAITING_CLOSED:
            return "S ";
        default:
            return "  ";
    }
}

/* ispis trenutnog stanja na zaslon izvan dizala */
void lift_status_thread(void const *argument)
{
	  int i, j;
    while(true) {
		    for(i=0; i<LIFT_COUNT; i++) {
            USART_print("Dizalo ");
            USART_print_char(i + 'A');
            USART_print(": ");
            USART_print_char(lifts[i].floor + '0');
            USART_print(lift_status(lifts + i));
            USART_print_char(' ');
        }
				USART_print_char('\n');
				USART_print_char('\n');
				
        
        for(i=0; i<LIFT_COUNT; i++) {
					USART_print("Lista:");
          for(j=0; j<lifts[i].request_num; j++) {
                USART_print_char(lifts[i].requests[j].floor + '0');
								if(j != lifts[i].request_num-1) {
									USART_print_char(',');
								} else {
									USART_print_char(' ');
								}
            }
            if(lifts[i].request_num < 7) {
                for(j=lifts[i].request_num; j<7; j++) {
                    USART_print("  ");
                }
            }
        }

				USART_print_char('\n');
        osDelay(1 * SECOND_TICKS);// osvjezavanje svake sekunde
    }
}

bool is_contained(request_t *requests, int num, request_t request) {
	int i;
	for(i=0; i<num; i++) {
		if(requests[i].floor == request.floor) return true;
	}
	return false;
}


void update_queue(lift_t *lift, request_t request) {
    if(is_contained(lift->requests, lift->request_num, request)) {
        return; // we can ignore repeating requests
    }
    if(lift->request_num == 0) {
        lift->request_num = 1;
        lift->requests[0] = request;
        if(lift->floor > request.floor) {
            lift->state = LS_STARTED_GOING_DOWN;
        } else if (lift->floor < request.floor) {
            lift->state = LS_STARTED_GOING_UP;
        } else { // na istom smo katu pa samo treba otvoriti vrata
            lift->state = LS_WAITING;
        }
    } else {
        lift->requests[lift->request_num] = request;
			  lift->request_num++;
    }
}

request_t request_parse(int gpio) {
    request_t request;
    if(gpio & 0x0100){
        request.direction = MOVING_UP;
    } else if(gpio & 0x0200){
        request.direction = MOVING_DOWN;
    } else {
        request.direction = WAITING;
    }
    request.floor = (gpio & 0x00F0) ? ((gpio & 0x00F0) >> 4) : gpio & 0x000F;
    return request;
}

void clear_queue(lift_t *lift) {
	int i=0, j=0;
	for(i=0; i<lift->request_num; i++) {
		if(lift->floor == lift->requests[i].floor) {
			for(j=i+1; j<lift->request_num; j++) {
				lift->requests[j-1] = lift->requests[j];
			}
			lift->request_num--;
 		}
	}
	if(lift->request_num == 0) {
		lift->state = LS_WAITING;
	} else if(lift->floor > lift->requests[0].floor) {
     lift->state = LS_STARTED_GOING_DOWN;
  } else if (lift->floor < lift->requests[0].floor) {
     lift->state = LS_STARTED_GOING_UP;
  }
}
                

void lift_thread(void const *argument)
{
    lift_t *lift = (lift_t*)argument;
    osEvent evt;
	  request_t request;
	
    while(true) {
        evt = osMessageGet(lift->queue, 0);    
			  if(evt.status == osEventMessage) {
            request = request_parse(evt.value.v);
            update_queue(lift, request);
        }
        switch (lift->state) {
            case LS_WAITING:
                lift->display_state = WAITING_OPEN;
                evt = osMessageGet(lift->queue, osWaitForever);
                if(evt.status == osEventMessage) {
                    request_t request = request_parse(evt.value.v);
                    update_queue(lift, request);
                }
                break;
            case LS_GOING_UP:
                lift->display_state = MOVING_UP; // krecemo se kretati prema gore
                osDelay(4 * SECOND_TICKS);
                lift->floor++;
                LED_update(lift->id, lift->floor);
                if(lift->floor == lift->requests[0].floor) {
                    lift->state = LS_STOPING;
                }
                break;
            case LS_GOING_DOWN:
                lift->display_state = MOVING_DOWN; // krecemo se kretati prema gore
                osDelay(4 * SECOND_TICKS);
                lift->floor--;
                LED_update(lift->id, lift->floor);
                if(lift->floor == lift->requests[0].floor) {
                    lift->state = LS_STOPING;
                }
                break;
            case LS_STARTED_GOING_UP:
            case LS_STARTED_GOING_DOWN:
                lift->display_state = WAITING_OPEN; // cekanje sa otvorenim vratima
                osDelay(3 * SECOND_TICKS);
                lift->display_state = WAITING_CLOSED; // cekanje sa zatvorenim vratima
                osDelay(1.5 * SECOND_TICKS);
                lift->state = (lift->state == LS_STARTED_GOING_UP) ? LS_GOING_UP : LS_GOING_DOWN;
                break;
            case LS_STOPING:
                lift->display_state = WAITING_CLOSED; // cekanje sa zatvorenim vratima
                osDelay(1.5 * SECOND_TICKS);
                clear_queue(lift);
                break;
        }
    }
}

bool is_in_way(lift_t *lift, request_t request) {
    if(lift->state == LS_WAITING) {
        return true;
    } else {
        if(request.direction == MOVING_UP) {
            if((lift->state == LS_GOING_UP || lift->state == LS_STARTED_GOING_UP) && request.floor > lift->floor) {
                return true;
            }
        }
        if(request.direction == MOVING_DOWN) {
            if((lift->state == LS_GOING_DOWN || lift->state == LS_STARTED_GOING_DOWN) && request.floor < lift->floor) {
                return true;
            }
        }
    }
    return false;
}

void lift_scheduler_thread(void const *data)
{
    bool delegated;
	  int i;
	  osEvent evt;
	  request_t request;
	
    while(true) {
        delegated = false;
        evt = osMessageGet(schedule_queue, osWaitForever);
        request = request_parse(evt.value.v);
			  for(i=0; i<LIFT_COUNT; i++) {
            if(is_in_way(&lifts[i], request)) {
                osMessagePut(lifts[i].queue, evt.value.v, osWaitForever);
                delegated = true;
                break;
            }
        }
        if(!delegated) {
            if(lifts[0].request_num > lifts[1].request_num) {
                osMessagePut(lifts[1].queue, evt.value.v, osWaitForever);
            } else {
                osMessagePut(lifts[0].queue, evt.value.v, osWaitForever);
            }
        }
    }
}

void EXTI15_10_IRQHandler  (void)
{
	EXTI->PR |= (1<<0);	

	if(GPIOA->IDR & (1<<11)) {
		// ako je pozvano dizalo A iznutra 
		if ((int)GPIOA->IDR & 0x000F){
					osMessagePut(lifts[0].queue, GPIOA->IDR, 0);
		}
		// ako je pozvano dizalo B iznutra
		if ((int)GPIOA->IDR & 0x00F0){
				osMessagePut(lifts[1].queue, GPIOA->IDR, 0);
		}
	}
}

void EXTI9_5_IRQHandler  (void)
{
	EXTI->PR |= (1<<0);	
	
	if(GPIOA->IDR & (1<<10)) {
		// ako je pozvano dizalo izvana 
		if ((int)GPIOA->IDR & 0x00FF){
			osMessagePut(schedule_queue, GPIOA->IDR, 0);
		}
	}
}


int main (void) {
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  LED_Initialize();
	EXTI_init();
	USART1_Init();
	
  lifts[0].state = LS_WAITING;
  lifts[0].floor = 1;
  lifts[0].display_state = '-';
  lifts[0].id = 0;
  lifts[0].queue = osMessageCreate(osMessageQ(lift_A_queue),NULL);
  osThreadCreate(osThread(lift_thread), (void *)&lifts[0]);
	LED_update(0, 1);
	
  lifts[1].state = LS_WAITING;
  lifts[1].floor = 1;
  lifts[1].display_state = '-';
  lifts[1].id = 1;
  lifts[1].queue = osMessageCreate(osMessageQ(lift_B_queue),NULL);
  osThreadCreate(osThread(lift_thread), (void *)&lifts[1]);
	LED_update(1, 1);
	
	schedule_queue = osMessageCreate(osMessageQ(schedule_queue), NULL);
  
  osThreadCreate(osThread(lift_scheduler_thread), NULL);
  osThreadCreate(osThread(lift_status_thread), NULL);
	  
	osKernelStart ();                         // start thread execution 
}


/*----------------------------------------------------------------------------
  Inicijalizacija vanjskog prekida
 *----------------------------------------------------------------------------*/
void EXTI_init(void){
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function	
	AFIO->EXTICR[0] &= 0xFFF0;                              // clear used pin
	AFIO->EXTICR[0] |= (0x000F & __AFIO_EXTICR1);           // set pin to use
	
	EXTI->IMR       |= 0x0C00; // ((1 << 0) & __EXTI_IMR);     // unmask interrupt
	EXTI->EMR       |= 0x0000; // ((1 << 0) & __EXTI_EMR);     // unmask event
	EXTI->RTSR      |= 0x0C00; // ((1 << 0) & __EXTI_FTSR);    // set rising edge
	EXTI->FTSR      |= 0x0000; // ((1 << 0) & 0x0);            // set falling edge

	if (__EXTI_INTERRUPTS & (1 << 0)) {                        // interrupt used
		NVIC->ISER[0] |= ~0x0;    // enable interrupts EXTI 
		NVIC->ISER[1] |= ~0x0;
	}
	GPIOC->CRL &= ~0x00000000;	
}


/*----------------------------------------------------------------------------
  Inicijalizacija UART pinova
 *----------------------------------------------------------------------------*/
void USART1_Init (void) {
  int i;

  RCC->APB2ENR |=  (   1UL <<  0);        /* enable clock Alternate Function  */
  AFIO->MAPR   &= ~(   1UL <<  2);        /* clear USART1 remap               */

  RCC->APB2ENR |=  (   1UL <<  2);        /* enable GPIOA clock               */
  GPIOA->CRH   &= ~(0xFFUL <<  4);        /* clear PA9, PA10                  */
  GPIOA->CRH   |=  (0x0BUL <<  4);        /* USART1 Tx (PA9) output push-pull */
  GPIOA->CRH   |=  (0x04UL <<  8);        /* USART1 Rx (PA10) input floating  */

  RCC->APB2ENR |=  (   1UL << 14);        /* enable USART#1 clock             */

  USART1->BRR   = 0x0271;                 /* 115200 baud @ PCLK2 72MHz        */
  USART1->CR1   = ((   1UL <<  2) |       /* enable RX                        */
                   (   1UL <<  3) |       /* enable TX                        */
                   (   0UL << 12) );      /* 1 start bit, 8 data bits         */
  USART1->CR2   = 0x0000;                 /* 1 stop bit                       */
  USART1->CR3   = 0x0000;                 /* no flow control                  */
  for (i = 0; i < 0x1000; i++) __NOP();   /* avoid unwanted output            */

  USART1->CR1  |= ((   1UL << 13) );      /* enable USART                     */
}


/*----------------------------------------------------------------------------
  Metode za ispis na USART
*----------------------------------------------------------------------------*/
void USART_print(const char *message) {
    while(*message != '\0') {
        USART_print_char(*message);
        message++;
    }
}

void USART_print_char(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = (c & 0x1FF);
}

void LED_update(int index, int value) {
    static int current = 0;
    if(index == 0) {
        current &= 0x0F;
        current |= (value << 4);
    } else if(index == 1) {
        current &= 0xF0;
        current |= value;
    }
    LED_SetOut((uint32_t)current);
}
