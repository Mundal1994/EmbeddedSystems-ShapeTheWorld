// ***** 0. Documentation Section *****
// main.c for Lab 9
// Runs on LM4F120/TM4C123
// In this lab we are learning functional debugging by dumping
//   recorded I/O data into a buffer
// January 15, 2016

// Lab 9
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}
unsigned long Led;
unsigned long SW1;
unsigned long SW2;

void Delay(void){unsigned long volatile time;
  time = 80000; // 0.5sec
  while(time){
   time--;
  }
}
// first data point is wrong, the other 49 will be correct
unsigned long Time[50];
// you must leave the Data array defined exactly as it is
unsigned long Data[50];
int main(void){  unsigned long i,last,now,ended;
	TExaS_Init(SW_PIN_PF40, LED_PIN_PF1);  // activate grader and set system clock to 16 MHz
  PortF_Init();   // initialize PF1 to output
  SysTick_Init(); // initialize SysTick, runs at 16 MHz
  i = 0;          // array index
	ended = 0;
  last = NVIC_ST_CURRENT_R;
  EnableInterrupts();           // enable interrupts for the grader
	GPIO_PORTF_DATA_R = 0;
  while(1){
		SW1 = GPIO_PORTF_DATA_R&0x10;
		SW2 = GPIO_PORTF_DATA_R&0x01;
		if (!SW1 || !SW2)
		{
			Led = GPIO_PORTF_DATA_R;   // read previous
			Led = Led^0x02;            // toggle red LED
			GPIO_PORTF_DATA_R = Led;   // output 
			if(i<50){
				now = NVIC_ST_CURRENT_R;
				Time[i] = (last-now)&0x00FFFFFF;  // 24-bit time difference
				if (!SW1)
					Data[i] = GPIO_PORTF_DATA_R&0x03; // record PF1 and PF0
				else
					Data[i] = GPIO_PORTF_DATA_R&0x12; // record PF1 and PF4
				last = now;
				i++;
			}
			ended = 1;
    }
		else if(i<50 && ended){
			GPIO_PORTF_DATA_R = 0;
			now = NVIC_ST_CURRENT_R;
			Time[i] = (last-now)&0x00FFFFFF;  // 24-bit time difference
			Data[i] = GPIO_PORTF_DATA_R&0x11; // record no button press
			last = now;
			i++;
			ended = 0;
		}
		else
			GPIO_PORTF_DATA_R = 0;
    Delay();
  }
}
