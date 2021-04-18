/*
filename: my_handlers.h
This file contains the handler Functions required by main.cpp
*/
#pragma once
#include "my_config.h"
#include "mapping_sensor.h"

#define MaxClients 1
extern bool NUM [MaxClients] ;
extern int  numClients ;
// extern void readValues(String&,int=0,int=0);
// When a WebSocket message is received
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
	switch (type)
	{
		Serial.print("webSocketEventType:");
		Serial.println(type);
	case WStype_DISCONNECTED: // if the websocket is disconnected
		Serial.printf("Client [%u] Disconnected!\n", num);
		NUM[num] = false;
		numClients -=1;
		break;
	case WStype_CONNECTED:
	
		// if a new websocket connection is established
		numClients+=1;
		if (num >= MaxClients)
		{
			webSocket.sendTXT(num, "Connection limit Exceeded!");
			webSocket.disconnect(num);
		}else
		{
			IPAddress ip = webSocket.remoteIP(num);
			Serial.printf("Client [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
			NUM[num] = false;
		}
	
	break;
	case WStype_TEXT: // if new text / bin data is received
	case WStype_BIN:  
		Serial.printf("Recieved  Message from Client [%u]: %s\n", num, payload);
		NUM[num] = !NUM[num];
		if (NUM[num])
		{
			Serial.printf("Data Streaming.. For Client [%u]\n", num);
		}
		else
		{
			Serial.printf("Streaming Stopped for Client [%u]\n", num);
		}
		break;
	case WStype_ERROR:
		Serial.println("AN error Occured!");
		break;
	case WStype_PING:
		Serial.println("WStype_PING");
		break;
	case WStype_PONG:
		Serial.println("WStype_PONG a PONG");
		break;
	case WStype_FRAGMENT_TEXT_START:
		Serial.println("WStype_FRAGMENT_TEXT_START");
		break;
	case WStype_FRAGMENT_BIN_START:
		Serial.println("WStype_FRAGMENT_BIN_START");
		break;
	case WStype_FRAGMENT_FIN:
		Serial.println("WStype_FRAGMENT_FIN");
		break;
	case WStype_FRAGMENT:
		Serial.println("WStype_FRAGMENT");
		break;
	}

	
}
void startSending()
{
	if (numClients>0)
	{
		String buffer;
		// accelSensor.getAcceleration(&accelerometer_x, &accelerometer_y, &accelerometer_z);
		// String payload = String(accelerometer_x) + " " + String(accelerometer_y) + " " + String(accelerometer_z) + "\n";
		readValues(buffer);
		
		//handle all clients..
		for (int i = 0; i < MaxClients; i++)
		{	
			yield();
			if (NUM[i])
			{
				webSocket.sendTXT(i,buffer );
			}
		}
	}
}


void startWebSocket()
{									   // Start a WebSocket server
	webSocket.begin();				   // start the websocket server
	webSocket.onEvent(webSocketEvent); // if there's an incomming websocket message, go to function 'webSocketEvent'
	delay(100);
	Serial.println();
	Serial.println("WebSocket server started.");
}


void toggleLed()
{
	digitalWrite(LEDpin, !digitalRead(LEDpin));
}



void getLedState()
{
	toggleLed();
	String led_state = digitalRead(LEDpin) ? "ON" : "OFF";
	server.send(200, "text/plain", led_state);
}

// void hand
void getAccelData()
{
	String data;
	//Getting the accel data
	for (int i = 0; i < 5; i++)
	{
		delay(10);
		data += String(accelerometer_x) + " " + String(accelerometer_y) + " " + String(accelerometer_z) + "\n";
	}
	server.send(200, "text/plain", data);
	Serial.println(data);
}

// 404 
void handle_NotFound()
{
	Serial.println("404 Error.");
	server.send(404, "text/plain", "Not found");
}

