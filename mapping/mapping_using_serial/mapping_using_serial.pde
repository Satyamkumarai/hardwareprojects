
import processing.serial.*;
final int N = 25;
final int sz = 50;
//final int[] mx = {120,65,75,80,60,120,80,80,100,35,90,75,35,55,30,50,45,55,60,55,90,80,60,30,45};
final int mx = 80;
Serial myPort;  // The serial port

void setup() {
  // List all the available serial ports
  size(400, 400);
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[2], 115200);
  //myPort.write('a'); //Random data to start buffer
  for (int i=0; i<N; i++) {
    inBuffer[i] = new String("0");
  }
}

String[] inBuffer = new String[N];
int[] values = new int[N];
void draw() {
  while (myPort.available() > 0) {
    String data = myPort.readStringUntil('\n');    
    if (data != null) {
      inBuffer = split(data, ' ');
      for (int i=0; i<N; i++) {
        values[i] = int(inBuffer[i]);
        noStroke();
        int x = ((i % 5))*sz;
        int y = (int(i / 5))*sz;
        fill(map(values[i],0,mx,0,255));
        rect(x, y, sz, sz);
      }
    }
  }
}
