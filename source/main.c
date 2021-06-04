/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "scheduler.h"
#include "timer.h"
#include "simAVRHeader.h"
#endif

	
//state machines
enum displayStates{output};
enum playerStates{p1, p2, p3};
enum aiStates {a1, a2, a3, a4};
enum ballStates {move};
//display arrays
unsigned char rows[8] = {0x11, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11};
unsigned char columns[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

//count
int i = 0;
int score = 0;

//buttons
unsigned char b1 = 0x00;
unsigned char b2 = 0x00;

int ballTick(int state) {



	return state;


}



int aiTick(int state) {
	switch(state) {
		case a1:
			state = a2;
			break;
	        case a2:
			state = a3;
			break;
		case a3:
			state = a4;
			break;
                case a4:
			state = a1;
		default:
			break;		
	
	}

	switch(state) {
		case a1:
			rows[0] = 0x11;
			break;
		case a2:
			rows[0] = 0x03;
			break;
		case a3:
			rows[0] = 0x11;
			break;
		case a4:
			rows[0] = 0x18;
			break;
		default:
			break;
	
	}

	return state;


}
int playerTick(int state) {
	b1 = ~PINA & 0x01;
	b2 = ~PINA & 0x02;

	switch(state) {
		case p1:
			if(b1) {state = p2;}
			else if(b2) {state = p3;}
			break;
		case p2:
			if(b2) {state = p1;}
			break;
		case p3:
			if(b1) {state = p1;}
			break;
		default:
			break;
	}

	switch(state) {
		case p1:
			rows[7] = 0x11;
			break;
		case p2:
			rows[7] = 0x03;
			break;
		case p3:
			rows[7] = 0x18;
			break;
		default:
			break;
	
	}

	return state;



}
int displayTick(int state) {
	switch(state) {
		case output:
			break;
		default:
			break;
	
	}

	switch(state) {
		case output:
			for(int i = 0; i < 8; i++) {
				PORTD = rows[i];
				PORTC = columns[i];
			}
			break;	
		default:
			break;
		
	}
	
	return state;


}


int main(void) {
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

    static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks= sizeof(tasks)/sizeof(task*);
	
	const char start = 0;

	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &playerTick;

	task2.state = start;
	task2.period = 5000;
	task2.elapsedTime = task2.period;
	task2.TickFct = &aiTick;

	task3.state = start;
	task3.period = 100;
	task3.elapsedTime = task3.period;
	task3.TickFct = &ballTick;

	task4.state = start;
	task4.period = 1;
	task4.elapsedTime = task4.period;
	task4.TickFct = &displayTick;
	TimerSet(1);
	TimerOn();

	unsigned short i;


     while (1) {
	for(i = 0; i < numTasks; i++) {
		if(tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
			
		}
		tasks[i]->elapsedTime += 1;
	}

	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
