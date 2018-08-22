#include "SckAquapioneers.h"


OneWire oneWire(SIG3);
DallasTemperature aqp_WaterTemp(&oneWire);

AqpUltraSonic aqpUltraSonic;
AqpLightSensor aqpLightSensor;

bool SckAqp::begin()
{

	// Water Temperature
	aqp_WaterTemp.begin();
	aqp_WaterTemp.setResolution(10);
	aqp_WaterTemp.setWaitForConversion(true); 	// Wait for conversion to end

	// ultrasonic sensor
	aqpUltraSonic.begin();

	// light sensor
	aqpLightSensor.begin();

	return true;
};

float SckAqp::getReading(OneSensor* wichSensor)
{

	switch(wichSensor->type) {

		case SENSOR_AQP_WATER_TEMP:		aqp_WaterTemp.requestTemperatures(); return aqp_WaterTemp.getTempCByIndex(0); break;
		case SENSOR_AQP_WATER_LVL:		aqpUltraSonic.updateWaterLevel(); aqpUltraSonic.updateCounters(); return round(aqpUltraSonic.lvlAverage); break;
		case SENSOR_AQP_RISING_TIME: 		return aqpUltraSonic.tRising/60000.0; break;
		case SENSOR_AQP_DECREASING_TIME: 	return aqpUltraSonic.tDecreasing/60000.0; break;
		case SENSOR_AQP_STAGNATING_TIME: 	return aqpUltraSonic.tStagnating/60000.0; break;
		case SENSOR_AQP_LIGHT: 	return TSL2561.readVisibleLux(); break;

		default: break;
	}
	return -9999;
}

bool AqpUltraSonic::begin()
{
	lvlMoreFiltered.reset(IDEAL_WATER_LEVEL);
	return true;
}

bool AqpUltraSonic::updateWaterLevel()
{
	uint8_t i = 1;
	while (i <= 100)
	{
		float lvlRaw = measureDistance();
		// If value is out of range, retry
		if ((lvlRaw > 10) && (lvlRaw < TANK_HEIGHT)) {
			if (i > 1) lvlAverage = ((i-1) * lvlAverage + lvlRaw)/(float)i;
			else lvlAverage = lvlRaw;
			SerialUSB.print(lvlRaw);
			SerialUSB.print("\t");
			SerialUSB.println(lvlAverage);
			i++;
		}
		else SerialUSB.println("error");
		delay(10);
	}
	lvlMoreFilteredPrevious = lvlMoreFiltered.get();
	lvlMoreFiltered.update( lvlAverage );
	return true;
}

float AqpUltraSonic::measureDistance()
{
	uint32_t durationTimeUs;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(pinULTRASONIC, OUTPUT);
	digitalWrite(pinULTRASONIC, LOW);
	delayMicroseconds(2);
	digitalWrite(pinULTRASONIC, HIGH);
	delayMicroseconds(5);
	digitalWrite(pinULTRASONIC, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH
	// pulse whose duration is the time (in microseconds) from the sending
	// of the ping to the reception of its echo off of an object.
	pinMode(pinULTRASONIC, INPUT);
	durationTimeUs = pulseIn(pinULTRASONIC, HIGH);

	// convert the time into a distance in cm
	//v = 29.154519 microsecond per centimeter
	float distCm = (float)durationTimeUs / 29.0 / 2.0;
	if (distCm == 0) return 0;
	else return (PING_HEIGHT - distCm);
}

int AqpUltraSonic::getState()
{
	updateWaterLevel();
	if (lvlMoreFiltered.get() > (lvlMoreFilteredPrevious + STAGNATION_THRESHOLD)) return 1;
	else if (lvlMoreFiltered.get() < (lvlMoreFilteredPrevious - STAGNATION_THRESHOLD)) return -1;

	return 0;
}

bool AqpUltraSonic::updateCounters()
{
	if (millis() - tPrevious < 1000) return true;

	int currentState = getState();

	uint32_t tCurrent = millis();
	uint32_t tDif = tCurrent - tPrevious;

	switch(currentState) {

		case 1:
			if (previousState == 1) tRising += tDif;
			else tRising = tDif;
			break;

		case -1:
			if (previousState == -1) tDecreasing += tDif;
			else tDecreasing = tDif;
			break;

		case 0:

			if (previousState == 0) tStagnating += tDif;
			else tStagnating = tDif;
			break;
	}

	previousState = currentState;
	tPrevious = tCurrent;

	return true;
}

bool AqpLightSensor::begin()
{
	Wire.begin();
	TSL2561.init();
	return true;
}
