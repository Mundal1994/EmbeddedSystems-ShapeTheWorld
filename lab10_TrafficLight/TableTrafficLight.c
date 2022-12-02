// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

#define SENSOR  						(*((volatile unsigned long *)0x4002401C))
#define LIGHT_WALK   				(*((volatile unsigned long *)0x40025028))
#define LIGHT_CAR   				(*((volatile unsigned long *)0x400050FC))
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))

#define goW   0
#define waitW 1
#define goS   2
#define waitS 3
#define goWalk 4
#define on_1	5
#define off_1	6
#define on_2	7
#define off_2	8
#define on_3	9
#define off_3	10
#define	all		11

// Linked data structure
struct State {
	unsigned long Walk;
	unsigned long Car;
	unsigned long Time; // delay in 10ms units
  unsigned long Next[8];}; // next state for inputs 0,1,2,3
typedef const struct State STyp;
	
STyp FSM[12]={
	{0x02,0x0C,10,{goW,goW,waitW,waitW,waitW,waitW,waitW,waitW}},
	{0x02,0x14,10,{goS,goS,goS,goS,goWalk,goWalk,goS,goS}},
	{0x02,0x21,10,{goS,waitS,goS,waitS,waitS,waitS,waitS,waitS}},
	{0x02,0x22,10,{goW,goW,goW,goW,goWalk,goW,goWalk,goWalk}},
	{0x08,0x24,10,{goWalk,on_1,on_1,on_1,goWalk,on_1,on_1,on_1}},//change to 100 when testing
	{0x00,0x24,5,{off_1,off_1,off_1,off_1,off_1,off_1,off_1,off_1}},
	{0x02,0x24,5,{on_2,on_2,on_2,on_2,on_2,on_2,on_2,on_2}},
	{0x00,0x24,5,{off_2,off_2,off_2,off_2,off_2,off_2,off_2,off_2}},
	{0x02,0x24,5,{on_3,on_3,on_3,on_3,on_3,on_3,on_3,on_3}},
	{0x00,0x24,5,{all,all,all,all,all,all,all,all}},
	{0x02,0x24,5,{goWalk,goW,goS,goW,goWalk,goW,goS,goW}}};

unsigned long S;  // index to the current state
unsigned long Input;

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
// 800000*12.5ns equals 10ms
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

void Port_Init(void)
{
	unsigned long volatile delay;

	SYSCTL_RCGC2_R |= 0x32;
  delay = SYSCTL_RCGC2_R;

	// Port E
  GPIO_PORTE_AMSEL_R &= ~0x07; // disable analog function on PE2-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // enable regular GPIO
  GPIO_PORTE_DIR_R &= ~0x07;   // inputs on PE2-0
  GPIO_PORTE_AFSEL_R &= ~0x07; // regular function on PE2-0
  GPIO_PORTE_DEN_R |= 0x07;    // enable digital on PE2-0
  
	// Port B
	GPIO_PORTB_AMSEL_R &= ~0x3F; // disable analog function on PB5-0
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // enable regular GPIO
  GPIO_PORTB_DIR_R |= 0x3F;    // outputs on PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F; // regular function on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;    // enable digital on PB5-0
	
	// Port F
	GPIO_PORTF_CR_R = 0x0A;           // allow changes to PF1 & PF3
	GPIO_PORTF_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTF_AMSEL_R &= ~0x0A;      // disable analog on PF1 & PF3
  GPIO_PORTF_AFSEL_R &= ~0x0A;      // disable alt funct on PF1 & PF3
  GPIO_PORTF_DEN_R |= 0x0A;         // enable digital I/O on PF1 & PF3
	GPIO_PORTF_DIR_R |= 0x0A;         // PF1 & PF3 outputs
}
	
int main(void){ 
	volatile unsigned long delay;
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
	Port_Init();
	SysTick_Init();
  EnableInterrupts();
  while(1){
    LIGHT_CAR = FSM[S].Car;  // set lights
		LIGHT_WALK = FSM[S].Walk;
		SysTick_Wait10ms(FSM[S].Time);
    Input = SENSOR;     // read sensors
    S = FSM[S].Next[Input]; 
  }
}
