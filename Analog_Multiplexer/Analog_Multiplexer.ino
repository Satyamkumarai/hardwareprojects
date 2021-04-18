

const int trigger =7;         //The trigger pin
const int N = 5;              // Number of pins to multiplex
const int out = A0;           //The analog input to the arduino

//pin 2 ,3 ,4 used for codes 000 to 111 => 0 to 7
//pin 7 is used as the trigger.

void setup() {
  Serial.begin(115200);
  pinMode(A0,INPUT);
//  pinMode(8,OUTPUT);      //This is useless
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);  
//  digitalWrite(8,1);
}

void loop() {
//  Serial.println(analogRead(A0));
  for (int i=0;i<5;i++){
    //Turn On 
    digitalWrite(2, i & 1);
    digitalWrite(3, (i>>1) & 0b010);
    digitalWrite(4, (i>>2 )& 0b100);
    digitalWrite(trigger,1);
    
    //Read the Data
    int val = analogRead(out);
    digitalWrite(trigger,0);
    digitalWrite(2, 0);
    digitalWrite(3, 0);
    digitalWrite(4, 0);
    
    //turn off 
//    Serial.print("Selected: ");
//    Serial.print(i&1);
//    Serial.print((i>>1)&0b010);
//    Serial.print((1>>2)&0b100);

    
    Serial.print(val);
    Serial.print(" ");
    }
    
 Serial.println("============================");
  delay(100);

}
