/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 	
 *	Demo Link: https://drive.google.com/file/d/1gjYx5sDbsvtoCBjricZTHqOzRkNY81df/view?usp=sharing
 */
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "scheduler.h"
#include "timer.h"
#include "simAVRHeader.h"
#endif

	
//state machines
enum displayStates{o1, o2, o3, o4, o5, o6, o7, o8};
enum playerStates{p1, p2, p3};
enum aiStates {a1, a2, a3, a4};
enum ballStates {begin, m1, m2, m3 ,m4, m5, m6, m7};

//display arrays
unsigned char rows[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char columns[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

unsigned char flag = 0x00;
unsigned char diaL = 0x00;
unsigned char diaR = 0x00;

//count
int score = 0;


//buttons
unsigned char b1 = 0x00;
unsigned char b2 = 0x00;
unsigned char set = 0x00;

int ballTick(int state) {
        set = ~PINA & 0x04;

	switch(state) {
		case begin:
			if(set) {
				state = m1;
			}
			break;
		case m1:
			if(set) { state = begin;}
			
			state = m2;
			break;
		case m2:
			if(set) {state = begin;}
			if(flag) {state = m1;}
			
			else if(!flag) {state = m3;}
			break;
		case m3:
			if(set) {state = begin;}
                        if(flag) {state = m2;}

			else if(!flag) {state = m4;}

			break;
		case m4:
			if(set) {state = begin;}
                        if(flag) {state = m3;}

			else if(!flag) {state = m5;}

			break;
		case m5:
			if(set) {state = begin;}
                        if(flag) {state = m4;}

			else if(!flag) {state = m6;}

			break;
		case m6:
			if(set) {state = begin;}
			state = m5;
			break;
		default:
			break;
	}

	switch(state) {
		case begin:
			PORTB = 0x00;
			break;
		case m1:
			PORTB = 0x01;
			
			if(flag) {
				if(diaL && !diaR ) {
					rows[5] = 0xFF;
					rows[6] = 0x17;
				}
				else if(diaR && !diaL) {
					rows[5] = 0xFF;
					rows[6] = 0x1E;
				}
				else {
					rows[5] = 0xFF;
					rows[6] = 0x1B;
				}

			}
			else {
					rows[6] = 0x1B;
			}

			flag = 0x00;
			break;

		case m2:
			if(flag) {
				if(diaL && !diaR) {
					rows[4] = 0xFF;
					rows[5] = 0x17;
				}
				else if(diaR && !diaL) {
					rows[4] = 0xFF;
					rows[5] = 0x1E;
				
				}
				else {
					rows[4] = 0xFF;
					rows[5] = 0x1B;
				}

			}
			else {
				rows[6] = 0xFF;
                                rows[5] = 0x1B;
                        }

			break;
		case m3:
			if(flag) {
				if(diaL && !diaR) {
					rows[3] = 0xFF;
					rows[4] = 0x17;
					
				}
				else if(diaR && !diaL) {
					rows[3] = 0xFF;
					rows[4] = 0x1E;
				
				}
				else {
					rows[3] = 0xFF;
					rows[4] = 0x1B;
				}

			}
			else {
				rows[5] = 0xFF;
                                rows[4] = 0x1B;
                        }
			break;
		case m4:
			if(flag) {
				if(diaL && !diaR) {
					rows[2] = 0xFF;
					rows[3] = 0x17;
				}
				else if(diaR && !diaL) {
					rows[2] = 0xFF;
					rows[3] = 0x1E;
				
				}
				else {
					rows[2] = 0xFF;
					rows[3] = 0x1B;
				}
			}
			else {
				rows[4] = 0xFF;
                                rows[3] = 0x1B;
                        }
			break;
		case m5:
			if(flag) {
				if(diaL && !diaR) {
					rows[1] = 0xFF;
					rows[2] = 0x17;
				}
				else if(diaR && !diaL) {
					rows[1] = 0xFF;
					rows[2] = 0x1E;		
				}
				else {
					rows[2] = 0x1B;
					rows[1] = 0xFF;
				}
			}

			else {
				rows[3] = 0xFF;
                                rows[2] = 0x1B;
			}
			break;
		case m6:
				if(rows[0] == 0x11) {
					rows[1] = 0x1B;
					rows[2] = 0xFF;
				}
				else if(rows[0] == 0x03) {
					rows[1] = 0x1B;
					rows[2] = 0xFF;
					rows[1] = 0xFF;
					rows[1] = 0x17;
					diaL = 0x01;
				}
				else {
					rows[1] = 0x1B;
					rows[2] = 0xFF;
					rows[1] = 0xFF;
					rows[1] = 0x1E;
					diaR = 0x01;
				
				}

			

			flag = 0x01;
			break;
		default:
			break;
	
	
	}


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
		case o1:
			state = o2;
			break;
		case o2:
			state = o3;
			break;
		case o3:
			state = o4;
			break;
		case o4:
			state = o5;
			break;
		case o5:
			state = o6;
			break;
		case o6:
			state = o7;
			break;
		case o7:
			state = o8;
			break;
		case o8:
			state = o1;
			break;
		default:
			break;
	
	}

	switch(state) {
		case o1:
			PORTD = rows[0];
			PORTC = columns[0];
                        break;
                case o2:
			PORTD = rows[1];
                        PORTC = columns[1];

                        break;
                case o3:
			PORTD = rows[2];
                        PORTC = columns[2];

                        break;
                case o4:
			PORTD = rows[3];
                        PORTC = columns[3];

                        break;
                case o5:
			PORTD = rows[4];
                        PORTC = columns[4];

                        break;
                case o6:
			PORTD = rows[5];
                        PORTC = columns[5];

                        break;
                case o7:
			PORTD = rows[6];
                        PORTC = columns[6];

                        break;
                case o8:
			PORTD = rows[7];
                        PORTC = columns[7];

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
	task3.period = 500;
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
