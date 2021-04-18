#include "pressure_matrix.h"
#include <Arduino.h>
pressure_matrix::pressure_matrix(uint32_t R, uint32_t C)
{
    N_COLS = C;
    N_ROWS = R;
    if (A_mux_sel == NULL)
    {
        A_mux_sel = (uint8_t *)malloc(sizeof(uint8_t) * A_sel_N);
        A_mux_sel[0] = 2;
        A_mux_sel[1] = 3;
        A_mux_sel[2] = 4;
        A_mux_sel[3] = 5;
    }
    if (D_mux_sel == NULL)
    {
        D_mux_sel = (uint8_t *)malloc(sizeof(uint8_t) * D_sel_N);
        D_mux_sel[0] = 6;
        D_mux_sel[1] = 7;
        D_mux_sel[2] = 8;
        D_mux_sel[3] = 9;
    }
}

uint32_t **pressure_matrix::read_all()
{
    if (buffer == NULL)
    {
        allocate_buffer();
    }
    for (uint32_t row = 0; row < N_ROWS; row++)
    {
        set_mux_sel(D_mux_sel, D_sel_N, row);
        digitalWrite(D_SIG, D_SIG_ON);
        for (uint32_t col = 0; col < N_COLS; col++)
        {
            set_mux_sel(A_mux_sel, A_sel_N, col);
            buffer[row][col] = analogRead(A_SIG);
        }
        digitalWrite(D_SIG, D_SIG_OFF);
    }
    return buffer;
}

uint32_t pressure_matrix::read(uint32_t row, uint32_t col)
{
    uint32_t val ;
    set_mux_sel(D_mux_sel, D_sel_N, row);
    digitalWrite(D_SIG, D_SIG_ON);
    set_mux_sel(A_mux_sel, A_sel_N, col);
    val = analogRead(A_SIG);
    digitalWrite(D_SIG, D_SIG_OFF);
    return val;
}


void pressure_matrix::set_mux_sel(uint8_t mux[], int N_pins, int val)
{
    for (int i = 0; i < N_pins; i++)
    {
        digitalWrite(mux[i], (val >> i) % 2);
    }
}
void pressure_matrix::init()
{
    for (uint8_t i = 0; i < A_sel_N; i++)
        pinMode(A_mux_sel[i], OUTPUT);
    for (uint8_t i = 0; i < D_sel_N; i++)
        pinMode(D_mux_sel[i], OUTPUT);
    pinMode(D_SIG, OUTPUT);
    digitalWrite(D_SIG, D_SIG_OFF);
    pinMode(A_SIG,INPUT_PULLUP);
}
void pressure_matrix::allocate_buffer()
{
    if (buffer == NULL)
    {
        buffer = new uint32_t *[N_COLS];
        for (uint32_t i = 0; i < N_COLS; i++)
        {
            buffer[i] = new uint32_t[N_ROWS];
        }
        return;
    }
}

void pressure_matrix::set_a_mux_pins( uint8_t  *  pins,int n_pins){
    free(A_mux_sel);
    A_mux_sel = pins;
    A_sel_N  = n_pins;
    init();
}

void pressure_matrix::set_d_mux_pins( uint8_t  *  pins,int n_pins){
    free(D_mux_sel);
    D_mux_sel = pins;
    D_sel_N  = n_pins;
    init();
}