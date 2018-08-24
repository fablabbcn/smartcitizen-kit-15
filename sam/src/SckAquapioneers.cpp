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
		case SENSOR_AQP_WATER_LVL:		aqpUltraSonic.updateWaterLevel(); return round(aqpUltraSonic.lvlAverage); break;
		case SENSOR_AQP_RISING_TIME: 		aqpUltraSonic.updateRisingCounter(); return round(aqpUltraSonic.tRising * 2.0 / 60000.0) / 2.0; break; //convert milliseconds to minutes and round to nearest half
		case SENSOR_AQP_DECREASING_TIME: 	aqpUltraSonic.updateDecreasingCounter(); return round(aqpUltraSonic.tDecreasing * 2.0 / 60000.0) / 2.0; break;
		case SENSOR_AQP_STAGNATING_TIME: 	aqpUltraSonic.updateStagnatingCounter(); return round(aqpUltraSonic.tStagnating * 2.0 / 60000.0) / 2.0; break;
		case SENSOR_AQP_LIGHT: 	return aqpLightSensor.getReading(); break;

		default: break;
	}
	return -9999;
}

bool AqpUltraSonic::begin()
{
	// lvlMoreFiltered.reset(IDEAL_WATER_LEVEL);
	return true;
}

bool AqpUltraSonic::updateWaterLevel()
{
	uint8_t i = 1;
	uint8_t errorCounter = 0;
	lvlAveragePrevious = lvlAverage;
	while (i <= 100 && errorCounter < 100)
	{
		float lvlRaw = measureDistance();
		// If value is out of range, retry
		if ((lvlRaw > 10) && (lvlRaw < TANK_HEIGHT)) {
			if (i > 1) lvlAverage = ((i-1) * lvlAverage + lvlRaw)/(float)i;
			else lvlAverage = lvlRaw;
			i++;
		}
		else errorCounter++;
		delay(10);
	}

	updateState();
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
	//1/v = 29.154519 microsecond per centimeter
	float distCm = (float)durationTimeUs / 29.0 / 2.0;
	if (distCm == 0) return 0;
	else return (PING_HEIGHT - distCm);
}

void AqpUltraSonic::updateState()
{
	previousState = currentState;

	if (lvlAverage > (lvlAveragePrevious + STAGNATION_THRESHOLD)) currentState = 1;
	else if (lvlAverage < (lvlAveragePrevious - STAGNATION_THRESHOLD)) currentState = -1;
	else currentState = 0;
}


bool AqpUltraSonic::updateRisingCounter()
{
	uint32_t tDifRising = millis() - tPreviousRising;
	if(currentState == 1){
		if (previousState == 1) tRising += tDifRising;
		else tRising = tDifRising;
	}
	tPreviousRising = millis();
	return true;
}

bool AqpUltraSonic::updateDecreasingCounter()
{
	uint32_t tDifDecreasing = millis() - tPreviousDecreasing;
	if(currentState == -1){
		if (previousState == -1) tDecreasing += tDifDecreasing;
		else tDecreasing = tDifDecreasing;
	}
	tPreviousDecreasing = millis();
	return true;
}

bool AqpUltraSonic::updateStagnatingCounter()
{
	uint32_t tDifStagnating = millis() - tPreviousStagnating;
	if(currentState == 0){
		if (previousState == 0) tStagnating += tDifStagnating;
		else tStagnating = tDifStagnating;
	}
	tPreviousStagnating = millis();
	return true;
}

bool AqpLightSensor::begin()
{
	Wire.begin();
	TSL2561.init();
	return true;
}

float AqpLightSensor::getReading()
{
	if (!I2Cdetect(I2Caddress)) return -9999;
	return TSL2561.readVisibleLux();
}
