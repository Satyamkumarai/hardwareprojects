#include "BluetoothSerial.h"
#define dataPin 35
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int pressure;


void setup() {
 
 Serial.begin(115200);
 SerialBT.begin("ESP32bro");
 Serial.println("Ready");
 pinMode(4,OUTPUT);
 pinMode(dataPin,INPUT);
 digitalWrite(4,1);
}

void loop(){
  int pressure=0;
  digitalWrite(4,1);
  
  pressure=analogRead(dataPin);
  
  digitalWrite(4,0);
  delay(50);
  Serial.println(pressure);
  SerialBT.println(pressure); 
}
