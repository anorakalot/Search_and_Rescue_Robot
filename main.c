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
		break;

		default:
		ir_emit_state = IR_EMIT_START;
		break;
	}
	switch(ir_emit_state){
		case IR_EMIT_START:
		break;
		
		case IR_EMIT_INIT:
		break;
		
		case IR_EMIT_PULSE:
		//PORTA = (PORTA & 0xEF) | 0x10;
		//PORTA = (PORTA & 0xDF) | 0x20;
		PORTA = (PORTA & 0xBF) | 0x40;
		
		//PORTA = (PORTA & 0x8F) | 0x70;
		break;
		
		case IR_EMIT_OFF:
		//PORTA = (PORTA & 0xEF) | 0x10;
		//PORTA = (PORTA & 0xDF) | 0x20;
		PORTA = (PORTA & 0xBF) | 0x40;
		
		//PORTA = (PORTA & 0x8F) | 0x70;
		break;
	}
}

unsigned short front_reading;
unsigned short left_reading;
unsigned short right_reading;

unsigned short x;

enum IR_RECIEVER_STATES{IR_REC_START,IR_REC_INIT,RECIEVE}ir_rec_state;

unsigned char C;
unsigned char D;


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
		default:
			ir_rec_state = IR_REC_START;
			break;
	}
	switch(ir_rec_state){
		case IR_REC_START:
			break;
		case IR_REC_INIT:
			ADC_init();
			ADMUX = (ADMUX & 0xF0)| 0x02; 
			break;
		case RECIEVE:
			x = ADC;
			front_reading = x;
			//C  = (char)x;
			//D = (char)(x>>8);
			//PORTC = C;
			//PORTD = D;
			PORTC = (char)x;
			PORTD = (char)(x >> 8);
			break;
	}
}

enum MOTOR_STATES{MOTOR_START,MOTOR_INIT,MOTOR}motor_state;
void motor_init(){
	motor_state = MOTOR_START;	
}


void motor_tick(){
	switch(motor_state){
		case MOTOR_START:
			break;
		case MOTOR_INIT:
			break;
		case MOTOR:
			break;
		default:
			motor_state = MOTOR_START;
			break;
	}

	switch(motor_state){
		case MOTOR_START:
			break;
		case MOTOR_INIT:
			break;
		case MOTOR:
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
		vTaskDelay(50);
	}
}
void motor_task(){
	motor_init();
	for(;;){
		motor_tick();
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


void StartSecPulse_3(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(motor_task,(signed portCHAR *)"motor_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}


int main(void)
{
	DDRA = 0xF0;
	//PORTA = 0x0F;

	DDRC = 0xFF;
	//PORTC = 0x00;
	DDRD = 0xFF;
	//PORTD = 0x00;


	/*
	while(1){
		PORTC = 0xFF;
		PORTD = 0xFF;
	}
	//*/


	StartSecPulse(2);
	StartSecPulse_2(1);

	//StartSecPulse_3(1);

	vTaskStartScheduler();
	


	return 0;
}