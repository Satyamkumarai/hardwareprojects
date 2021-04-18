#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include "my_config.h"
#include "my_handlers.h"
#include "mapping_sensor.h"
// #include "pressure_matrix.h"

bool NUM[MaxClients] = {false};
int numClients = 0;
const long BAUDRATE = 9600;


// -------------------------------------SETUP----------------------------------

void setup()
{
	//Serial DEBUG
	Serial.begin(BAUDRATE);
	pinMode(LEDpin, OUTPUT);
	// digitalWrite(LEDpin, HIGH);
	Serial.println("**##");
	digitalWrite(LEDpin, LOW);

	for (int i = 0; i < A_sel_N; i++)
		pinMode(A_mux_sel[i], OUTPUT);
	for (int i = 0; i < D_sel_N; i++)
		pinMode(D_mux_sel[i], OUTPUT);

	// //Mux Signal Pins
	// // pinMode(A_SIG,INPUT_PULLUP);
	// pinMode(D_SIG,OUTPUT);

	// //Mux Enable Pins
	// // pinMode(D_EN,OUTPUT);
	// // pinMode(A_EN,OUTPUT);
	// digitalWrite(D_SIG,D_SIG_OFF);
	// // digitalWrite(A_EN,EN_ON);
	// // digitalWrite(D_EN,EN_ON);





	//configure the Acesspoint with SSID and PASSWORD
	WiFi.softAP(ssid, password);

	//Configure the local_IP of the ESP (Important since we are using Static IP)
	WiFi.softAPConfig(local_ip, gateway, subnet);

	//Handle Root
	server.on("/", []() {server.send_P(200,"text/html",webpage);Serial.println("."); }); //send_P for the var defined in flash memory

	server.on("/accel", getAccelData);

	server.onNotFound(handle_NotFound);

	//Starting the HTTP server

	server.begin();
	Serial.println("HTTP server started");

	startWebSocket(); // Start a WebSocket server
	Serial.println("Websocket server started");

	delay(100);
}

//===============================LOOP===========================================
void loop()
{
	webSocket.loop();
	server.handleClient();
	delay(100);
	startSending();
}