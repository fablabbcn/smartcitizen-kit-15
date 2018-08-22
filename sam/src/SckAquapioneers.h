#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Digital_Light_TSL2561.h>

#include <MovingAverage.h>

#include <Arduino.h>
#include <Sensors.h>
#include "sckAux.h"

const uint8_t SIG1 = 9;
const uint8_t SIG2 = 3;
const uint8_t SIG3 = 4;

bool I2Cdetect(byte address);


class SckAqp
{

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

class AqpUltraSonic
{

	public:
		const uint8_t TANK_HEIGHT = 30; 			// distance between bottom of tank and PING sensor in cm */
		const uint8_t PING_HEIGHT = TANK_HEIGHT - 1; 		// Height of tank in cm */
		const uint8_t IDEAL_WATER_LEVEL = TANK_HEIGHT - 7;
		const uint8_t MAX_WATER_LEVEL = TANK_HEIGHT - 2;
		const uint8_t STAGNATION_THRESHOLD = 1;

		// TODO alarms
		const uint8_t tRiseIdeal = 3 * 1000 * 60; 		//Ideal rise time in milliseconds
		const uint8_t tDecreaseIdeal = 5 *1000 * 60; 	//Ideal decrease time in milliseconds;

		float lvlAverage;
		// Create a moving average with alpha = 0.8
		MovingAverage lvlMoreFiltered = MovingAverage(0.8);

		bool begin();
		float getReading(OneSensor* wichSensor);
		bool updateWaterLevel();
		bool updateCounters();
		bool getBusyState(OneSensor* wichSensor);
		String control(OneSensor* wichSensor, String command);

		uint32_t tRising, tDecreasing, tStagnating;

	private:
		float measureDistance ();
		int getState();

		const int pinULTRASONIC = (SIG2);

		float lvlMoreFilteredPrevious;
		uint8_t tPrevious;
		int previousState = 0;




};

class AqpLightSensor
{
	public:
		bool begin();
};
