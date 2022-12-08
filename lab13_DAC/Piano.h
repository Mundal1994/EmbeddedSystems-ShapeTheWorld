// Piano.h
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

#define NONE	0
#define PLAY_C	9555//80 Mz ___ 523,251Hz * len (wave_len) (ex 16)
#define PLAY_D	8512//80 Mz ___ 587,330Hz * len (wave_len)
#define PLAY_E	7583//80 Mz ___ 659,255Hz * len (wave_len)
#define PLAY_G	6576//80 Mz ___ 783,991Hz * len (wave_len)


// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void); 


// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void);
