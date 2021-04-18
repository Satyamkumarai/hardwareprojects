//Comment The  Line below  If you're using the Arduino IDE..
#include <Arduino.h>
#include "pressure_matrix.h"
/*
Connections :
	4  pins from select lines of A_Mux <---> 4 Digital pins defined by 'A_mux_sel' array 	(Default [2,3,4,5])
	4  pins from select lines of D_Mux <---> 4 Digital pins defined by 'D_mux_sel' array 	(Default [6,7,8,9])
	The Analog Mux Sig  Pin			   <---> Pin defined by 'A_SIG' Variable 			 	(Default A0)
	The Digital Mux Sig Pin 		   <---> Pin Defined by 'D_SIG' Variable 			 	(Default D10)
*/
#define BAUDRATE 2000000

#define ROWS 12
#define COLS 12
uint8_t  A_sel[] = {6,7,8,9};
uint8_t  D_sel[] = {2,3,4,5};
#define L_THRESHOLD 900
#define MAX_VAL 1023
pressure_matrix matrix(ROWS,COLS);
void setup(){
	Serial.begin(BAUDRATE);
	matrix.set_a_mux_pins(A_sel,4);
	matrix.set_d_mux_pins(D_sel,4);
	matrix.D_SIG = 10;
	matrix.init();
}

void loop(){
	for (uint32_t r=0;r< matrix.N_ROWS;r++){
		for (uint32_t c=0;c<matrix.N_COLS;c++)
		{	
			long val = matrix.read(r,c);
			if (val > L_THRESHOLD )
				val = MAX_VAL;
			
			Serial.print(val);
			Serial.print(" ");
		}
	}
	Serial.println();
	delay(10);
}

