#include <Arduino.h>

//-----------------------------------------------


static const int A_SHR_CLK  = 2;
static const int A_SHR_DATA = 3;
static const int D_SHR_CLK	= 4;
static const int D_SHR_DATA = 5;
static const int SHR_RESET_PIN  = 6;
//HALF time period of SHR Microseconds 
static const int SHR_CLK_HALF_PERIOD  = 0;



//Analog and Digital mux <Select pins>
// static const int A_mux_sel[] = {2, 3, 4, 5};
// static const int D_mux_sel[] = {6, 7, 8, 9};

//Number of Analog and Digital mux select Pins
const int A_sel_N = 4;
const int D_sel_N = 4;

//Number of Rows And Columns
const int N_ROWS = 12;
const int N_COLS = 12;
boolean skip = false;
//-----------------------------------------------
 
//Analog  and Digital <signal pins>
static const int A_SIG = A0;
static const int D_SIG = 10;

//Analog  and Digital <Enable Pins>
static const int A_EN = 11;
static const int D_EN = 12;

//MUX Enable 
#define EN_ON 0
#define EN_OFF 1 

//MUX D_SIG
#define D_SIG_ON 0
#define D_SIG_OFF 1

//Number of bit the Shift register has..
#define  SHR_N_BITS 8
#define SHR_RESET 0


//----------HELPERS---------------------

void shift_reg_pulse_clk(uint8_t clk_pin){
	digitalWrite(clk_pin,HIGH);
	delayMicroseconds(SHR_CLK_HALF_PERIOD);
	digitalWrite(clk_pin,LOW);
	delayMicroseconds(SHR_CLK_HALF_PERIOD);
}
//Reset a particular Shift reg
void reset_shift_reg(uint8_t reset_pin){
	/*Resets the Shift Registers..*/
	digitalWrite(reset_pin,LOW);
	delayMicroseconds(10);
	digitalWrite(reset_pin,HIGH);
}

//Write a data to a particular shift reg..
void write_shift_reg(uint8_t data_pin,uint8_t clk_pin,int data,uint8_t total_bits=SHR_N_BITS){
	//First 4 bits
	for (int i=0;i<total_bits/2;i++){
		//set data
		// Serial.print("Writing : ");
		// Serial.print(data);
		// Serial.print("-> ");
		// Serial.println((data >>i)%2);

		digitalWrite(data_pin,(data >>i)%2);
		// PULSE CLOCK
		shift_reg_pulse_clk(clk_pin);
	}
	//Most significant NIBBLE
	for (int i=0;i<total_bits/2;i++){
		digitalWrite(data_pin,LOW);
		shift_reg_pulse_clk(clk_pin);
	}
}

#define THRESHOLD 950
#define MAXVAL 1023


//--------------------------------------
void setup(){
	Serial.begin(2400000);

	//Init Shift Register Pins
	pinMode(A_SHR_CLK,OUTPUT);
	pinMode(A_SHR_DATA,OUTPUT);
	pinMode(D_SHR_CLK,OUTPUT);
	pinMode(D_SHR_DATA,OUTPUT);
	pinMode(SHR_RESET_PIN,OUTPUT);

	//Mux Signal Pins
	// pinMode(A_SIG,INPUT_PULLUP);
	pinMode(D_SIG,OUTPUT);

	//Mux Enable Pins
	// pinMode(D_EN,OUTPUT);
	// pinMode(A_EN,OUTPUT);

	//Init Shift Registers
	reset_shift_reg(SHR_RESET_PIN);
	digitalWrite(A_SHR_CLK,LOW);
	digitalWrite(A_SHR_DATA,LOW);
	digitalWrite(D_SHR_CLK,LOW);
	digitalWrite(D_SHR_DATA,LOW);


}

void loop(){
	for (int r=0;r<N_ROWS;r++){
		// Serial.print("Row :");
		// Serial.println(r);
		// skip=0;
		//Set the row number 
		write_shift_reg(D_SHR_DATA,D_SHR_CLK,r,SHR_N_BITS);

		// set_mux_sel(D_mux_sel,D_sel_N,r);
		
		//Enable The Digital Mux
		// delay(1000);
		//Activate RoW	
		digitalWrite(D_SIG,D_SIG_ON);


		for (int c=0;c<N_COLS;c++){

			//Set the col number 
			// set_mux_sel(A_mux_sel,A_sel_N,c);
			// Serial.print("\tCol: ");
			// Serial.println(c);
			// Serial.print("\t");
			// skip=1;
			write_shift_reg(A_SHR_DATA,A_SHR_CLK,c,SHR_N_BITS);
			// delay(100);
			//READ CUR  ROW X COLUMN
			int val=0;
			// delay(10);
			val = analogRead(A_SIG);
			if (val > THRESHOLD )
				val = MAXVAL;
			Serial.print(val);
			Serial.print(" ");
			//output the value..
			// delay(10);
			
		
		} // Col Done 

		//Deactivate Row
		digitalWrite(D_SIG,D_SIG_OFF);
		

		// //Disable the Two Muxes
		// digitalWrite(D_EN,EN_OFF);
		// digitalWrite(D_EN,EN_OFF);



	}//Row Done
	Serial.println();
	// delay(10);?

}

