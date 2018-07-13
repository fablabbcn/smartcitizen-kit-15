#include "SckAquapioneers.h"

SHT31 		aqp_SHT31;

OneWire oneWire(SIG3);
DallasTemperature aqp_WaterTemp(&oneWire);

bool SckAqp::begin()
{

	// Water Temperature
	aqp_WaterTemp.begin();
	aqp_WaterTemp.setResolution(10);
	aqp_WaterTemp.setWaitForConversion(true); 	// Wait for conversion to end
	


	return true;
};

float SckAqp::getReading(OneSensor* wichSensor)
{
	
	switch(wichSensor->type) {
	
		case SENSOR_AQP_WATER_TEMP:		aqp_WaterTemp.requestTemperatures(); return aqp_WaterTemp.getTempCByIndex(0); break;


		default: break;

	}	

}
