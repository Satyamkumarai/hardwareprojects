//Do I need to control Enable of mux?
static const int N = 12;                            //Number of Rows or columns
static const uint8_t mux_output = A0;                   //This pin is used to select Column {MUX OUTPUT}
static const uint8_t mux_select[] = {2, 3, 4, 5};       //these pins are used to select rows (4 (bits) Select pins )
static const int mux_select_len = 4;
static const uint8_t sensor_trig =10;


void setup() {

  Serial.begin(115200);

  //init mux_select lines
  for (int i = 0 ; i < mux_select_len; i++) {
    pinMode(mux_select[i], OUTPUT);
  }
  pinMode(sensor_trig, OUTPUT);
}

void loop() {
  long value = 0;

  //For Each Row;
  for (int r = 0; r < N; r++) {

    //Activate the row..
    digitalWrite(sensor_trig, 0);

    //For Each Column..
    for (int c = 0; c < N; c++) {
      //for each bit position   ( Need to use register level coding to optimise .)
      for (int b = 0; b < mux_select_len; b++) {
        //triggering respective select lines belonging to the row.
        digitalWrite(mux_select[b], (r & (2 ^ b)) >> b);
      }
      
      value = analogRead(mux_output);
    
      Serial.print(value);
      Serial.print(" ");
    }//each column
      digitalWrite(sensor_trig, 1);
    //Deactivate the row..

  }// Each row
  Serial.println();
  delay(10);

}
