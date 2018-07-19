#include "SckAquapioneers.h"


OneWire oneWire(SIG3);
DallasTemperature aqp_WaterTemp(&oneWire);

AqpUltraSonic aqpUltraSonic;

bool SckAqp::begin()
{

	// Water Temperature
	aqp_WaterTemp.begin();
	aqp_WaterTemp.setResolution(10);
	aqp_WaterTemp.setWaitForConversion(true); 	// Wait for conversion to end

	// ultrasonic sensor
	aqpUltraSonic.begin();

	return true;
};

float SckAqp::getReading(OneSensor* wichSensor)
{

	switch(wichSensor->type) {

		case SENSOR_AQP_WATER_TEMP:		aqp_WaterTemp.requestTemperatures(); return aqp_WaterTemp.getTempCByIndex(0); break;
		case SENSOR_AQP_WATER_LVL:	return aqpUltraSonic.getWaterLevel(); break;
		case SENSOR_AQP_RISING_TIME: aqpUltraSonic.updateCounters(); return tRising; break;
		case SENSOR_AQP_DECREASING_TIME: aqpUltraSonic.updateCounters(); return tDecreasing; break;
		case SENSOR_AQP_STAGNATING_TIME: aqpUltraSonic.updateCounters(); return tStagnating; break;

		default: break;

	}

}

bool AqpUltraSonic::begin(){
	  lvlFiltered.reset(25);
	  lvlMoreFiltered.reset(25);
		return true;
}


void AqpUltraSonic::getWaterLevel() {
	float	dist = measureDistance();
	float lvlRaw = pingHeight - dist;
  lvlFilteredPrevious = lvlFilteredValue;
	lvlMoreFilteredPrevious = lvlMoreFilteredValue;

  //Remove weird values
  if ( (lvlRaw < 10) || (lvlRaw > tankHeight + 20)){
    lvl = lvlFilteredPrevious;
  } else{
		lvl = lvlRaw;
	}

	lvlFiltered.update( lvl );
	lvlMoreFiltered.update( lvl );

	lvlFilteredValue = lvlFiltered.get();
	lvlMoreFilteredValue = lvlMoreFiltered.get();
}


float AqpUltraSonic::measureDistance (){
  float duration, cm;
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(ultrasonic, OUTPUT);
  digitalWrite(ultrasonic, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonic, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultrasonic, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ultrasonic, INPUT);
  duration = pulseIn(ultrasonic, HIGH);

  // convert the time into a distance
  cm = duration / 29 / 2;
  return cm;
}


void AqpUltraSonic::getState(){
	//Tells us if water level has been rising or decreasing since last measure
	previousState = currentState;
	float absDif = abs(lvlMoreFilteredValue - lvlMoreFilteredPrevious);

	if (lvlMoreFilteredValue > lvlMoreFilteredPrevious){
		currentState = 1;
	}
	if (lvlMoreFilteredValue < lvlMoreFilteredPrevious){
		currentState = -1;
	}
	if (absDif < 1){
		//if difference between two last measures is small, consider the flow is stagnating
		currentState = 0;
	}
}

void AqpUltraSonic::updateCounters(){
	getState();

	if (currentState == 1 && previousState == 1){
		tRisePrevious = tCurrent;
		tCurrent = millis();
		tDif = tCurrent - tRisePrevious;
		tRising = tRising + tDif;
	}

	if (currentState == -1 && previousState == -1){
		tDecPrevious = tCurrent;
		tCurrent = millis();
		tDif = tCurrent - tDecPrevious;
		tDecreasing = tDecreasing + tDif;
	}

	if (currentState == 0 && previousState == 0){
		tStagPrevious = tCurrent;
		tCurrent = millis();
		tDif = tCurrent - tStagPrevious;
		tStagnating = tStagnating + tDif;
	}

	if (currentState == 1 && previousState != 1){
		tRisePrevious = tCurrent;
		tCurrent = millis();
		tDif = tCurrent - tRisePrevious;
		tRising = tDif;
	}

	if (currentState == -1 && previousState != -1){
		tDecPrevious = tCurrent;
		tCurrent = millis();
		tDif = tCurrent - tDecPrevious;
		tDecreasing = tDif;
	}

	if (currentState == 0 && previousState != 0){
		tStagPrevious = tCurrent;
		tCurrent = millis();
		tDif = tCurrent - tStagPrevious;
		tStagnating = tDif;
	}
}
