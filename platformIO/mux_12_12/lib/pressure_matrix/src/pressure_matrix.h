#pragma once
#include <Arduino.h>
//MUX Enable
#define MUX_EN_ON 0
#define MUX_EN_OFF 1

//MUX SIG ON
#define D_SIG_ON 0
//MUX SIG OFF 
#define D_SIG_OFF 1



class pressure_matrix
{   public:
        //Analog mux select pins (Default = 2,3,4,5)
        uint8_t *A_mux_sel;
        //Digital mux select pins  (Default = 6,7,8,9)
        uint8_t *D_mux_sel;
        //Number of Analog select Pins
        uint32_t A_sel_N = 4;
        //Number of  Digital mux select pins
        uint32_t D_sel_N = 4;
        //Number of Rows of sensor Matrix
        uint32_t N_ROWS = 5;
        //Number of Columns of sensor Matrix
        uint32_t N_COLS = 5;
        //Analog mux Signal pin
        uint8_t A_SIG = A0;
        // Digital mux Signal pin
        uint8_t D_SIG = 10;
        //The buffer to hold the array
        uint32_t **buffer;
        //Constructor
        pressure_matrix(uint32_t R, uint32_t C);
        /*Fills the Buffer with the data from the matrix \n***Please Make sure the buffer has enough space allocated...****/
        uint32_t ** read_all();
        /*Returns the value at row and col of the matrix..*/
        uint32_t read(uint32_t row, uint32_t col);
        /* INIT all the pins */
        void init();
        //Sets the A mux pins 
        void set_a_mux_pins(uint8_t  *  pins,int n_pins);
        //Sets the D mux pins
        void set_d_mux_pins(uint8_t  *  pins,int n_pins);

    // private:
    /*Sets the mux select pins using binary representation of val \nExample 12 => 1100 in binary*/
        void set_mux_sel(uint8_t * mux, int N_pins, int val);
        //Allocated the space for buffer;
        void allocate_buffer();
        
};
