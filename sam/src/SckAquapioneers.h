#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

#include <MovingAverage.h>

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

class AqpUltraSonic {

	public:

		bool begin();
		float getReading(OneSensor* wichSensor);
		bool getBusyState(OneSensor* wichSensor);
		String control(OneSensor* wichSensor, String command);

	private:
		float measureDistance ();
		void getWaterLevel();
		void updateCounters();
		void getState();


		const int ultrasonic = (SIG2);

		// Create a moving average with alpha
		MovingAverage lvlFiltered(0.005);
		MovingAverage lvlMoreFiltered(0.001);


		// distance between bottom of tank and PING sensor in cm
		float pingHeight = 45;
		// Height of tank in cm
		float tankHeight = 30;

		//acceptable maximum water level in tank in cm
		float maxLvlIdeal = tankHeight - 5;
		float minLvlIdeal = tankHeight - 10;

		float tRiseIdeal = 3 * 1000; //Ideal rise time in milliseconds
		float tDecreaseIdeal = 5 *1000; //Ideal decrease time in milliseconds;

		float tRising, tDecreasing, tStagnating, tStagPrevious, tRisePrevious, tDecPrevious, tCurrent, tDif, lvlFilteredPrevious, lvlFilteredValue, lvlMoreFilteredPrevious, lvlMoreFilteredValue;
		int currentState, previousState;

};
