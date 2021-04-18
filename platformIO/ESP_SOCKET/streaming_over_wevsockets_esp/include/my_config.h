/*
filename: my_config.h
This file contains the Configuration settings that is used in main.cpp
*/
#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
// +++++ PIN SETTINGS +++++

uint8_t LEDpin = LED_BUILTIN;

bool LED1status = LOW;



// +++++ WIFI Settings +++++
/* Put your access point SSID & Password */
const char *ssid = "NodeMCU";	   // Enter SSID here
const char *password = "12345678"; //Enter Password here


/* Put IP Address details */
IPAddress local_ip(192, 168, 10, 1); //The ip you'' use to connect..

IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);



WebSocketsServer webSocket(81); // create a websocket server on port 81

ESP8266WebServer server(80);


//++++ SENSOR SETTINGS +++++
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x=0, accelerometer_y=0, accelerometer_z=0; // variables for accelerometer raw data

int16_t gyro_x, gyro_y, gyro_z;							   // variables for gyro raw data

int16_t temperature;


bool streamData = false;
//The Home Page.. This is a multiline String Literal

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
	<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'>
</head>
<body>
	<p>LED state : <span id='ledState'>__</span></p>
	<button onclick='myFunction()'>CheckConnection</button>
	<button onclick='toggleStream()'>ToggleStream</button>
	<button onclick='stopSending()'>Stop</button>
</body>
<script>
	var socket= new WebSocket('ws://192.168.10.1:81');
	socket.onopen = function (event){socket.onmessage = function (event){console.log(event.data);}}
	function myFunction(){
		console.log('Button Clicked!');
			var xhr = new XMLHttpRequest();  
			var url = 'http://192.168.10.1/accel';
			xhr.onreadystatechange = function(){
				if (xhr.readyState == 4 && xhr.status == 200){
					console.log(xhr.responseText);
					document.getElementById('ledState').innerHTML = xhr.responseText;
				}
			};
			xhr.open('GET',url,true);
			xhr.send();
	}	
	function toggleStream(){
		if (socket.readyState == socket.CLOSED){
			socket = new WebSocket('ws://192.168.10.1:81');
			socket.onopen = function (event){socket.send('start');socket.onmessage = function (event){console.log(event.data);}}
		}
		if (socket.readyState==socket.OPEN)
			socket.send('stop');
	}
	function stopSending(){
		if (socket.readyState == socket.CLOSED){
			console.log('Already closed..!')
		}else if (socket.readyState == socket.OPEN){
			socket.close();
		}
	}
	document.addEventListener('DOMContentLoaded',myFunction,false);
</script>
</html>
)=====";



