# pragma once
#include <Arduino.h>
// ++++++++ SENSOR CONFIG ++++++++++++++
static const int A_mux_sel[] = {5, 4, 14, 12};		
static const int D_mux_sel[] = {13, 15, 16, 9};

//Number of Analog and Digital mux select Pins
const int A_sel_N = 4;
const int D_sel_N = 4;

//Number of Rows And Columns
const int N_ROWS = 5;
const int N_COLS = 5;

//-----------------------------------------------
 
//Analog  and Digital <signal pins>
static const int A_SIG = 10	;
static const int D_SIG = 3;

//Analog  and Digital <Enable Pins>
// static const int A_EN = 11;
// static const int D_EN = 12;

//MUX Enable 
#define EN_ON 0
#define EN_OFF 1 

//MUX D_SIG
#define D_SIG_ON 0
#define D_SIG_OFF 1
//----------HELPERS---------------------

void set_mux_sel(const int mux[],int N_pins,int val){
	/*Sets the mux select pins using binary representation of val 
		Example 12 => 1100 in binary
	*/
    //Ensure val < 2**N_pins -1 
	for (int i=0;i<N_pins;i++){
		digitalWrite(mux[i],(val >> i)%2);
	}
}

void readValues(String &values,int n_rows=N_ROWS,int n_cols=N_COLS){
	for (int r=0;r<n_rows;r++){
		
		//Set the row number 
		set_mux_sel(D_mux_sel,D_sel_N,r);
		
		//Enable The Digital Mux
		
		//Activate RoW	
		digitalWrite(D_SIG,D_SIG_ON);

		for (int c=0;c<n_cols;c++){
			//Set the col number 
			set_mux_sel(A_mux_sel,A_sel_N,c);

			//READ CUR  ROW X COLUMN
			long value= analogRead(A_SIG);
            values+=String(value);
            values+=" ";

			// val = min(MN_THRESHOLD,val);
			// val*=SENSITIVITY;
			
		} // Col Done 
		//Deactivate Row
		digitalWrite(D_SIG,D_SIG_OFF);
        values+="\n";
		// //Disable the Two Muxes
		// digitalWrite(D_EN,EN_OFF);
		// digitalWrite(D_EN,EN_OFF);
	}//Row Done
}