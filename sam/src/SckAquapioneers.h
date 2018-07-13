#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>
#include <Sensors.h>
#include "sckAux.h"

const uint8_t SIG1 = 9;
const uint8_t SIG2 = 3;
const uint8_t SIG3 = 4;

bool I2Cdetect(byte address);


class SckAqp {
	
	public:

		byte devAddress[SENSOR_COUNT - 18] {
			0x44,		// SENSOR_AQP_TEMP,
			0x44,		// SENSOR_AQP_HUM,
			0x3c		// SENSOR_GROOVE_OLED,
		};

		bool begin();
		float getReading(OneSensor* wichSensor);
		bool getBusyState(OneSensor* wichSensor);
		String control(OneSensor* wichSensor, String command);
		void print(String payload);
		void displayReading(String title, String reading, String unit, String time);

	private:
};

