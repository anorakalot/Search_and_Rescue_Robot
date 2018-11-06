/*
 * rmcge002_final_project_1.c
 *
 * Created: 11/1/2018 9:21:02 AM
 * Author : Dylan
 */ 
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.

}



enum IR_EMITTER_STATES{IR_EMIT_START,IR_EMIT_INIT,IR_EMIT_PULSE,IR_EMIT_OFF}ir_emit_state;


void ir_emit_init(){
	ir_emit_state = IR_EMIT_START;
}

void ir_emit_tick(){
	switch(ir_emit_state){
		case IR_EMIT_START:
		ir_emit_state = IR_EMIT_INIT;
		break;
		case IR_EMIT_INIT:
		ir_emit_state = IR_EMIT_PULSE;
		break;
		case IR_EMIT_PULSE:
		ir_emit_state = IR_EMIT_OFF;
		break;
		case IR_EMIT_OFF:
		ir_emit_state = IR_EMIT_PULSE;
		default:
		break;
	}
	switch(ir_emit_state){
		case IR_EMIT_START:
		break;
		case IR_EMIT_INIT:
		break;
		case IR_EMIT_PULSE:
		PORTA = (PORTA & 0xEF) | 0x10;
		break;
		case IR_EMIT_OFF:
		PORTA = (PORTA & 0xEF) | 0x10;
		break;
	}
}

unsigned short front_reading;
unsigned short left_reading;
unsigned short right_reading;

unsigned short x;

enum IR_RECIEVER_STATES{IR_REC_START,IR_REC_INIT,RECIEVE}ir_rec_state;

void ir_rec_init(){
	ir_rec_state = IR_REC_START;
}

void ir_rec_tick(){
	switch(ir_rec_state){
		case IR_REC_START:
			ir_rec_state = IR_REC_INIT;
			break;
		case IR_REC_INIT:
			ir_rec_state = RECIEVE;
			break;
		case RECIEVE:
			ir_rec_state = RECIEVE;
			break;
	}
	switch(ir_rec_state){
		case IR_REC_START:
			break;
		case IR_REC_INIT:
			ADC_init();
			break;
		case RECIEVE:
			x = ADC;
			PORTC = (char)x;
			PORTD = (char)(x >> 8);
			break;
	}
}


void ir_emit_task(){
	ir_emit_init();
	for(;;){
		ir_emit_tick();
		vTaskDelay(100);
	}
}

void ir_rec_task(){
	ir_rec_init();
	for(;;){
		ir_rec_tick();
		vTaskDelay(100);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{	
	xTaskCreate(ir_emit_task,(signed portCHAR *)"ir_emit_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}

void StartSecPulse_2(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(ir_rec_task,(signed portCHAR *)"ir_rec_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}




int main(void)
{
	DDRA = 0xF0;

	DDRC = 0xFF;
	DDRD = 0xFF;


	/*
	while(1){
		PORTC = 0xFF;
		PORTD = 0xFF;
	}
	*/


	StartSecPulse(1);
	StartSecPulse_2(1);
	vTaskStartScheduler();
	


	return 0;
}