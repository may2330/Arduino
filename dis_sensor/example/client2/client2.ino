/*
 * The MIT License (MIT)

Copyright (c) 2015-2018 Jetsonhacks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// displayExample.cpp
/*
  �� �ڸ� 7���׸�Ʈ �����ϱ�

  �� ����ġ�� kocoaFAB���� ��������ϴ�.
  �� ����ġ�� ����� ����� �� �ֽ��ϴ�.
*/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "bitr43"
#define STAPSK  "bitbitr43"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.30";
const uint16_t port = 5004;

ESP8266WiFiMulti WiFiMulti;

int position_pin[] = { 1,2,3,4 };               //4�ڸ� ���� ��
int segment_pin[] = { 5,6,7,8,9,10,11,12 };     //���׸�Ʈ ���� ��
const int delayTime = 5;                      //�Ͻ����� �ð�

//0 ~ 9�� ǥ���ϴ� ���׸�Ʈ ��
byte data[] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6 };

void setup() {

	Serial.begin(115200);

	// We start by connecting to a WiFi network
	WiFi.mode(WIFI_STA);
	WiFiMulti.addAP(ssid, password);

	Serial.println();
	Serial.println();
	Serial.print("Wait for WiFi... ");

	while (WiFiMulti.run() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	delay(500);

	//4�ڸ� ���� �� ��¿����� ����
	for (int i = 0; i < 4; i++) {
		pinMode(position_pin[i], OUTPUT);
	}

	//���׸�Ʈ ���� �� ��¿����� ����
	for (int i = 0; i < 8; i++) {
		pinMode(segment_pin[i], OUTPUT);
	}
}

void loop() {
	Serial.print("connecting to ");
	Serial.print(host);
	Serial.print(':');
	Serial.println(port);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;

	if (!client.connect(host, port)) {
		Serial.println("connection failed");
		Serial.println("wait 5 sec...");
		delay(5000);
		return;
	}


	// This will send the request to the server
	client.println("hello from ESP8266");

	//read back one line from server
	while (client.connected()) {
		String line = client.readStringUntil('\r');
		Serial.println(line);
		show(1, line[0]);                //ù ��° �ڸ�, 1���
		show(2, line[1]);                //�� ��° �ڸ�, 2���
		show(3, line[2]);                //�� ��° �ڸ�, 3���
		show(4, line[3]);                //�� ��° �ڸ�, 4���
		delay(1000);
	}
	//count();                //count�Լ� ȣ��, count �Լ� ����� �ּ� ����(//�� ����� �˴ϴ�.)
}

void show(int position, int number) {
	//4�ڸ� �� ���ϴ� �ڸ� ����
	for (int i = 0; i < 4; i++) {
		if (i + 1 == position) {
			digitalWrite(position_pin[i], LOW);
		}
		else {
			digitalWrite(position_pin[i], HIGH);
		}
	}

	//8�� ���׸�Ʈ�� �����ؼ� ���ϴ� ���� ���
	for (int i = 0; i < 8; i++) {
		byte segment = (data[number] & (0x01 << i)) >> i;
		if (segment == 1) {
			digitalWrite(segment_pin[7 - i], HIGH);
		}
		else {
			digitalWrite(segment_pin[7 - i], LOW);
		}
	}
}

/*
 0001���� �ϳ��� ī��Ʈ�ϴ� �Լ�
*/
void count() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 10; k++) {
				for (int l = 0; l < 10; l++) {
					show(1, i);
					delay(delayTime);
					show(2, j);
					delay(delayTime);
					show(3, k);
					delay(delayTime);
					show(4, l);
					delay(delayTime);
				}
			}
		}
	}
	delay(1000);
}
