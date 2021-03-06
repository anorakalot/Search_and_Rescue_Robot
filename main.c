/*
 * rmcge002_final_project_1.c
 *
 * Created: 11/1/2018 9:21:02 AM
 * Author : Dylan
 */ 
 #define F_CPU 8000000UL
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
#include "util/delay.h"

unsigned long left_cnt = 0;
unsigned long right_cnt = 0;

//unsigned long tasksPeriod = 10;
void left_count(){
	left_cnt ++;
}

void right_count(){
	right_cnt ++;
}


enum TICK_STATES{TICK_INIT,TICK_START,TICK_CNT}tick_state;

void tick_init(){
	tick_state = TICK_INIT;
}


void tick_tick(){
	switch(tick_state){
		case TICK_INIT:
			tick_state = TICK_START;
			break;
		case TICK_START:
			tick_state = TICK_CNT;
			break;
		case TICK_CNT:
			tick_state = TICK_CNT;
			break;

			
	}
	switch(tick_state){
		case TICK_INIT:
			break;
		case TICK_START:
			break;
		case TICK_CNT:
			left_count();
			right_count();
			break;
	}
}

/*


void TimerISR(){
	left_count();
	right_count();
}


// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}


void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}


// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPB_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
*/

///*
void interrupt_init(){
//setup INT0 and INT1 to trigger on any pin change
EICRA = 0x05;
//enable INT0 and INT1
EIMSK = 0x03;

sei();
}
//*/

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


void PWM_init(){

	
	//set as correct pwm phase 
	//and so that it clears when up counting 
	//and set when down counting
	//may need to change A if set ocr1a values are counter intuitive
	TCCR1A = 0xA1;
	
	//makes it so it has correct pwm phase 
	//and so it goes off of 255 phases
	TCCR1B = 0x05;

	//set as correct pwm phase
	//and so that it clears when up counting
	//and set when down counting
	//may need to change A if set ocr1a values are counter intuitive
	TCCR2A = 0xA1;
	
	//makes it so it has correct pwm phase
	//and so it goes off of 255 phases
	TCCR2B = 0x05;
}




enum IR_STATES{IR_START,IR_INIT,IR_LEFT,IR_MIDDLE,IR_RIGHT}ir_state;


unsigned short middle_reading;
unsigned short left_reading;
unsigned short right_reading;

unsigned short curr_middle_reading;
unsigned short curr_left_reading;
unsigned short curr_right_reading;

unsigned short x;
unsigned char C;
unsigned char D;


void ir_init(){
	ir_state = IR_START;
}

void ir_tick(){
	switch(ir_state){
		case IR_START:
			ir_state = IR_INIT;
			break;
		case IR_INIT:
			ir_state = IR_LEFT;
			//set up admux and output port for  ir left
			PORTA = (PORTA & 0x0F) | 0x10;
			ADMUX = (ADMUX & 0xF0) | 0x00;
			break;
		case IR_LEFT:
			///*
			ir_state = IR_MIDDLE;
			
			//set up admux and output port for  ir middle
			PORTA = (PORTA & 0x0F) | 0x20;
			ADMUX = (ADMUX & 0xF0) | 0x01;
			//*/

			//ir_state = IR_LEFT;

			break;
		case IR_MIDDLE:
			///*
			ir_state = IR_RIGHT;
			//set up admux and output port for next ir right
			PORTA = (PORTA & 0x0F) | 0x40;
			ADMUX = (ADMUX & 0xF0) | 0x02;
			//*/

			//ir_state = IR_MIDDLE;
			break;
		case IR_RIGHT:
			///*
			ir_state = IR_LEFT;
			//set up admux and output port for next irleft
			PORTA = (PORTA & 0x0F) | 0x10;
			ADMUX = (ADMUX & 0xF0) | 0x00;
			//*/

			//ir_state = IR_RIGHT;
			break;
		default:
			ir_state = IR_START;
			break;
	}
	switch(ir_state){
		case IR_START:
			break;
		case IR_INIT:
			ADC_init();
			
			//ADMUX = (ADMUX & 0xF0)| 0x00; 
			break;
		case IR_LEFT:
			//PORTA = (PORTA & 0x0F) | 0x10;
			//ADMUX = (ADMUX & 0xF0) | 0x00;	
			
			x = ADC;
			left_reading = x;

			///*
			PORTC = (char)x;
			PORTD = (char)(x >> 8);
			//*/
			break;
		case IR_MIDDLE:
			//PORTA = (PORTA & 0x0F) | 0x20;
			//ADMUX = (ADMUX & 0xF0) | 0x01;
			
			x = ADC;
			middle_reading = x;
			///*
			PORTC = (char)x;
			PORTD = (char)(x >> 8);
			//*/

			//PORTC = 0xFF;
			//PORTD = 0xFF;

			break;

		case IR_RIGHT:
			//PORTA = (PORTA & 0x0F) | 0x40;
			//ADMUX = (ADMUX & 0xF0) | 0x02;
			
			x = ADC;
			right_reading = x;

			///*
			PORTC = (char)x;
			PORTD = (char)(x >> 8);
			//*/

			break;
	}
}

enum MOTOR_STATES{MOTOR_START,MOTOR_INIT,MOTOR_WAIT,GO_ONE_CELL,TURN_LEFT,TURN_RIGHT,TURN_REVERSE,MOTOR_TEST}motor_state;
void motor_init(){
	motor_state = MOTOR_START;	
}

double kp = 0.5;
double kd = 0.4;
double ki = 0.0001;

unsigned char p_control;
unsigned char i_control;
unsigned char d_control;

unsigned short error = 0;
unsigned short prev_error = 0;
unsigned long error_buildup = 0;
unsigned long error_reset_cnt = 0;



unsigned char base_speed = 100;

unsigned char motor_left;
unsigned char motor_right;



unsigned long curr = 0;

unsigned short limit = 0;


bool has_middle_wall(){
	if ( middle_reading> 300){
		return true;
	}
	else{
		return false;
	}
}

///*
bool has_left_wall(){
	if (left_reading > 300){
		return true;
	}
	else{
		return false;
	}

}

bool has_right_wall(){
	if (right_reading > 300){
		return true;
	}
	else{
		return false;
	}
}
//*/

bool has_left_wall_2(unsigned short reading){
	if (reading > 300){
		return true;
	}
	else{
		return false;
	}

}

bool has_right_wall_2(unsigned short reading){
	if (reading > 300){
		return true;
	}
	else{
		return false;
	}
}

//maybe not use 
void calibrate_readings(){

}


void regulate_sensor_left(){
	
	if (left_reading == 0){
		left_reading +=1;
	}
}

void regulate_sensor_right(){
	if (right_reading == 0){
		left_reading +=1;
	}
}

void pid_control(){
	curr_left_reading = left_reading;
	curr_right_reading = right_reading;



	if (has_left_wall_2(curr_left_reading)!= true || has_right_wall_2(curr_right_reading) != true){
		motor_left = base_speed;
		motor_right = base_speed;
		return;
	} 


	error_reset_cnt ++;




	if (error_reset_cnt > 1000){
		error_buildup = 0;
		error_reset_cnt = 0;
	}
	//might need to change to base_speed + (p_control+ i_control + d_control);
	if (curr_left_reading > curr_right_reading){
		prev_error = error;
		error = abs(curr_left_reading - curr_right_reading);
		error_buildup += error;

		p_control = error * kp;

		i_control = error_buildup * ki;

		d_control = (error-prev_error) * kd;


		motor_left = base_speed - (p_control+ i_control + d_control);

		motor_right = base_speed; 
		
	}
	//might need to change to base_speed + (p_control+ i_control + d_control);
	else if (curr_left_reading < curr_right_reading){
			prev_error = error;
			error = abs(curr_left_reading - curr_right_reading);
			error_buildup += error;

			p_control = error * kp;

			i_control = error_buildup * ki;

			d_control = (error-prev_error) * kd;


			motor_left = base_speed ;

			motor_right = base_speed - (p_control+ i_control + d_control);
			
	}
}


void forward(unsigned char motor_1, unsigned char motor_2){
	OCR1A = motor_1;
	OCR1B = 0;

	OCR2A = motor_2;
	OCR2B = 0;

}



void go_one_cell(){
	curr = left_cnt;
	while(left_cnt -curr < 800){
		pid_control();
		forward(motor_left,motor_right);
	} 
}


void halt (){
	OCR1A = 20;
	OCR1B = 20;

	OCR2A = 20;
	OCR2B = 20;
}

void left_turn(){
	OCR1A = motor_left;
	OCR1B = 0;

	OCR2A = 0;
	OCR2B = motor_right;

}

void right_turn(){
	OCR1A = 0;
	OCR1B = motor_left;

	OCR2A = motor_right;
	OCR2B = 0;

}

void reverse(){
	OCR1A = 0;
	OCR1B = motor_left;

	OCR2A = 0;
	OCR2B = motor_right;

}


void reverse_turn(){
	
	OCR1A = motor_left;
	OCR1B = 0;

	OCR2A = 0;
	OCR2B = motor_right;

}


void reverse_turn_until(){
	curr = right_cnt;
	while(right_cnt - curr  <300){
		reverse_turn();
	}
}

void right_turn_until(){
	curr = right_cnt;
	while(right_cnt - curr  <300){
		right_turn();
	}
}

void left_turn_until(){
	curr = left_cnt;
	while(left_cnt - curr  <300){
		left_turn();
	}
}
void forward_until(){
	curr = left_cnt;
	while(left_cnt - curr < 300){
		forward(motor_left,motor_right);
	}
}

void halt_until(){
	halt();
	_delay_ms(1000);
}


//ISR(INT0_vect){
//	left_count();
//}

//ISR(INT1_vect){
//	right_count();
//}

unsigned char choice = 0;

void motor_tick(){
	switch(motor_state){
		case MOTOR_START:
			motor_state = MOTOR_INIT;
			break;
		case MOTOR_INIT:
			motor_state = MOTOR_WAIT;
			break;
		case MOTOR_WAIT:
			///*
			if (has_middle_wall() && has_left_wall() && has_right_wall()){
				motor_state = TURN_REVERSE;
			}
			else if (has_middle_wall() && !has_left_wall() && has_right_wall()){
				motor_state = TURN_LEFT;
			}
			else if (has_middle_wall() && has_left_wall() && !has_right_wall()){
				motor_state = TURN_RIGHT;
			}
			
			else if (has_middle_wall() && !has_left_wall() && !has_right_wall()){
				choice = left_cnt %2;
				if ((choice) == 0){
					motor_state = TURN_LEFT;
					}	
				else{
					motor_state = TURN_RIGHT;
				}
			}

			else if (!has_middle_wall() && has_left_wall() && !has_right_wall()){
				choice = left_cnt %2;
				if ((choice) == 0){
					motor_state = GO_ONE_CELL;
				}
				else{
					motor_state = TURN_RIGHT;
				}
			}

			else if (!has_middle_wall() && !has_left_wall() && has_right_wall()){
				choice = left_cnt % 2;
				if ((choice) == 0){
					motor_state = GO_ONE_CELL;
				}
				else{
					motor_state = TURN_LEFT;
				}
			}

			else if (!has_middle_wall() && !has_left_wall() && !has_right_wall()){
				choice = left_cnt % 3;
				if (choice == 0){
					motor_state = GO_ONE_CELL;
				}
				else if (choice == 1){
					motor_state = TURN_LEFT;
				}
				else{
					motor_state = TURN_RIGHT;
				}
			}
			

			else{
				motor_state = GO_ONE_CELL;
			}
			
			//*/
			//motor_state = MOTOR_TEST;
			break;
		
		case GO_ONE_CELL:
			motor_state = MOTOR_WAIT;
			break;
		case TURN_LEFT:
			motor_state = MOTOR_WAIT;
			break;
		case TURN_RIGHT:
			motor_state = MOTOR_WAIT;
			break;
		case TURN_REVERSE:
			motor_state = MOTOR_WAIT;
			break;

		case MOTOR_TEST:
			motor_state = MOTOR_TEST;
			break;
		
		default:
			motor_state = MOTOR_START;
			break;
	}

	switch(motor_state){
		case MOTOR_START:
			break;
		case MOTOR_INIT:
			PWM_init();
			limit = 30;
			base_speed = 100;
			motor_left = 0;
			motor_right = 0;
			//motor_left = base_speed;
			//motor_right = base_speed;
			
			
			//OCR1A = base_speed;
			//OCR1B = base_speed;


			break;
		case MOTOR_WAIT:
			break;
				
		case GO_ONE_CELL:
			halt_until();
			go_one_cell();
			break;
		case TURN_LEFT:
			halt_until();
			forward_until();
			halt_until();
			left_turn_until();
			break;
		case TURN_RIGHT:
			halt_until();
			forward_until();
			halt_until();
			right_turn_until();
			break;
		case TURN_REVERSE:
			halt_until();
			forward_until();
			halt_until();
			reverse_turn_until();
			break;

		case MOTOR_TEST:

		//if (middle_reading < limit){
			
			//forward(base_speed,base_speed);
			
			//OCR1A = 0;
			//OCR1B = 0;

			//OCR2A = 0;
			//OCR2B = 0;
		//}
		/*
		else{
			
		}
		//*/

			break;
	}
	
}


enum RASPI_STATES{RASPI_INIT,RASPI_START,SENSE}raspi_state;

void raspi_init(){
	raspi_state = RASPI_INIT;
}

void raspi_tick(){
	switch(raspi_state){
		case RASPI_INIT:
			raspi_state = RASPI_START;
			break;
		case RASPI_START:
			raspi_state = SENSE;
			break;
		case SENSE:
			raspi_state = SENSE;
			break;	
	}	
	switch(raspi_state){
		case RASPI_INIT:
			break;
		case RASPI_START:
			break;
		case SENSE:
			if ((PINB & 0x10) == 0x10){
				PORTB = 0x08;
			}
			else{
				PORTB = 0x00;
			}
			break;
	}
}


void ir_task(){
	ir_init();
	for(;;){
		ir_tick();
		vTaskDelay(10);
	}
}

void motor_task(){
	motor_init();
	for(;;){
		motor_tick();
		vTaskDelay(10);	
	}
}


void raspi_task(){
	raspi_init();
	for(;;){
		raspi_tick();
		vTaskDelay(100);
	}
}


void tick_task(){
	tick_init();
	for(;;){
		tick_tick();
		vTaskDelay(10);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{	
	xTaskCreate(ir_task,(signed portCHAR *)"ir_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}


void StartSecPulse_2(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(motor_task,(signed portCHAR *)"motor_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}



void StartSecPulse_3(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(raspi_task,(signed portCHAR *)"raspi_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}

void StartSecPulse_4(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(tick_task,(signed portCHAR *)"tick_task",configMINIMAL_STACK_SIZE,NULL,Priority,NULL);
}


int main(void)
{
	DDRA = 0xF0;
	PORTA = 0x0F;

	DDRC = 0xFF;
	PORTC = 0x00;
	
	DDRD = 0x0F;
	PORTD = 0xF0;


	DDRB = 0x0F;

	//interrupt_init();
	//TimerSet(100);
	//TimerOn();
	

	//DDRB = 0x00;
	/*
	while(1){
	//	if (PINB == 0x01){
			PORTC = 0xFF;
			PORTD = 0xFF;
	//	}
	//	else{
	//		PORTC = 0x00;
	//		PORTD = 0x00;
	//	}
	
	}
	*/
	
	///*
	//PWM_init();
	//while(1){
	
	//PORTD = 0xFF;
	//OCR1A= 200;
	//OCR1B = 0;

	//forward(base_speed,base_speed);
	//OCR2A = 200;
	//OCR2B = 0;
	
//	}
	//*/
	


	StartSecPulse(2);

	StartSecPulse_2(2);
	//StartSecPulse_3(2);

	StartSecPulse_4(1);
	vTaskStartScheduler();
	


	return 0;
}