

#include <avr/io.h>
//#define F_CPU 16000000UL

#include "FreeRTOS/FreeRTOSConfig.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include <util/delay.h>


TaskHandle_t  Task1_Handler = NULL;
TaskHandle_t  Task2_Handler = NULL;

void uart_init(){
	UCSRB |= (1<<TXEN);
	UBRRL = 51;
}
void uart_send(char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void uart_send_str(char * str){
	for(int i =0 ; str[i] != '\0';i++){
		uart_send(str[i]);
	}
}

void uart_send_number(int number){
	char buff[10];
	itoa(number, buff, 10);
	uart_send_str(buff);
}

void task1(void * para){
	
	struct vars *ptr = para;
	int x = 0;
	while(1){
		uart_send_str(ptr->z);
		uart_send_str("\t\r");
		vTaskDelay(500);
		
	}
	//vTaskDelete(NULL);
}

void task2(void * para){
	while(1){
		uart_send_str((char*)para);
		uart_send_str("\t\r");
		vTaskDelay(500);
		if(uxTaskPriorityGet( Task1_Handler ) > 2){
			vTaskDelete(Task1_Handler);
		}
	}
	//vTaskDelete(NULL);
}


int main(void)
{
	/* Replace with your application code */
	
	uart_init();
	
	xTaskCreate(task1,"Task_1",100,NULL,1,&Task1_Handler);
	xTaskCreate(task2,"Task_2",100,NULL,1,&Task2_Handler);

	vTaskStartScheduler();
	
	
	
	while (1)
	{
		
	}
}



