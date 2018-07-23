#pragma once

/* 	-----------------
 	|	 Sensors 	|
 	-----------------
*/
enum SensorLocation {
	BOARD_BASE,
	BOARD_URBAN,
	BOARD_AUX
};

enum SensorType {

	// Base Sensors
	SENSOR_BATTERY,
	SENSOR_VOLTIN,

	// Urban Sensors
	SENSOR_NOISE,
	SENSOR_HUMIDITY,
	SENSOR_TEMPERATURE,
	SENSOR_LIGHT,
	SENSOR_CO,
	SENSOR_CO_HEAT_TIME,
	SENSOR_CO_HEAT_CURRENT,
	SENSOR_CO_HEAT_SUPPLY_VOLTAGE,
	SENSOR_CO_HEAT_DROP_VOLTAGE,
	SENSOR_CO_LOAD_RESISTANCE,
	SENSOR_NO2,
	SENSOR_NO2_HEAT_TIME,
	SENSOR_NO2_HEAT_CURRENT,
	SENSOR_NO2_HEAT_SUPPLY_VOLTAGE,
	SENSOR_NO2_HEAT_DROP_VOLTAGE,
	SENSOR_NO2_LOAD_RESISTANCE,

	// I2C Auxiliary Sensors
	SENSOR_I2C_EXPANDER_TCA9548A,

	SENSOR_GASESBOARD_SLOT_1A,
	SENSOR_GASESBOARD_SLOT_1W,
	/* SENSOR_GASESBOARD_SLOT_1_CAL, */
	SENSOR_GASESBOARD_SLOT_2A,
	SENSOR_GASESBOARD_SLOT_2W,
	/* SENSOR_GASESBOARD_SLOT_2_CAL, */
	SENSOR_GASESBOARD_SLOT_3A,
	SENSOR_GASESBOARD_SLOT_3W,
	/* SENSOR_GASESBOARD_SLOT_3_CAL, */
	SENSOR_GASESBOARD_TEMPERATURE,
	SENSOR_GASESBOARD_HUMIDITY,

	SENSOR_GROOVE_I2C_ADC,

	SENSOR_INA219_BUSVOLT,
	SENSOR_INA219_SHUNT,
	SENSOR_INA219_CURRENT,
	SENSOR_INA219_LOADVOLT,

	SENSOR_WATER_TEMP_DS18B20,
	SENSOR_ATLAS_TEMPERATURE,
	SENSOR_ATLAS_PH,
	SENSOR_ATLAS_EC,
	SENSOR_ATLAS_EC_SG,
	SENSOR_ATLAS_DO,
	SENSOR_ATLAS_DO_SAT,

	SENSOR_CHIRP_MOISTURE,
	SENSOR_CHIRP_TEMPERATURE,
	SENSOR_CHIRP_LIGHT,

	SENSOR_PM_1,
	SENSOR_PM_25,
	SENSOR_PM_10,

	/* SENSOR_SHT31_TEMP, */
	/* SENSOR_SHT31_HUM, */

	// Actuators (This is temp)
	SENSOR_GROOVE_OLED,

	// Put one here for each muxed sensor
	// SENSOR_MUX01A,
	// SENSOR_MUX01B,
	// SENSOR_MUX02,

	SENSOR_COUNT	
};

const uint32_t minimal_sensor_reading_interval = 60;
const uint32_t default_sensor_reading_interval = 60;
const uint32_t max_sensor_reading_interval = 86400;		// One day

// Structs for RAM storing
struct SensorGroupByTime {		// 5 bytes
	uint32_t time;
	uint8_t numberOfReadings;
	uint32_t readingStartIndex;
};

struct SingleSensorReading {	// 5 bytes
	SensorType sensor;
	float reading;	
};

struct SensorGroupBuffer{
	uint32_t time;
	uint8_t numberOfReadings;
	SingleSensorReading data[SENSOR_COUNT];
};

class OneSensor {
public:
	SensorLocation location;
	SensorType type;
	const char *shortTitle;
	const char *title;
	const char *unit;
	float reading;
	uint32_t lastReadingTime;
	bool valid;
	bool controllable;
	uint8_t id;
	uint32_t interval;
	bool enabled;
	bool busy;
	int8_t muxPort;			// Tens + 0x70 is expander board address and units is the port (ej. 05 is address 0x70 and port 5) (ej. 47 is address 0x74 and port 7 )

	OneSensor(SensorLocation nLocation, SensorType nType, const char *nShortTitle, const char *nTitle, uint8_t nId=0, bool nEnabled=false, bool nControllable=false, const char *nUnit="", int8_t nMuxPort=-1) {
		location = nLocation;
		type = nType;
		shortTitle = nShortTitle;
		title = nTitle;
		unit = nUnit;
		reading = 0;
		lastReadingTime = 0;
		valid = false;
		controllable = nControllable;
		id = nId;
		interval = default_sensor_reading_interval;
		enabled = nEnabled;
		busy = false;
		muxPort = nMuxPort;
	}
};

class AllSensors {
public:
	
	OneSensor list[SENSOR_COUNT+1] {
	
		//SensorLocation 			SensorType 				shortTitle		title 						id		enabled		controllable	unit 		
						
		// Base Sensors						
		OneSensor { BOARD_BASE, 		SENSOR_BATTERY, 			"BATT",			"Battery", 					10,		true,		false,			"%"},
		OneSensor { BOARD_BASE, 		SENSOR_VOLTIN,				"BATT_VOLT",		"Input voltage",				0,		false,		false,			"V"},
		
	

		// Urban Sensors
		OneSensor { BOARD_URBAN, 		SENSOR_NOISE, 				"NOISE_A",		"Noise", 					29,		true,		false,			"dBc"},
		OneSensor { BOARD_URBAN, 		SENSOR_HUMIDITY,			"HUM",			"Humidity", 					13,		true,		false,			"%"},
		OneSensor { BOARD_URBAN, 		SENSOR_TEMPERATURE, 			"TEMP",			"Temperature", 					12,		true,		false,			"C"},
		OneSensor { BOARD_URBAN, 		SENSOR_LIGHT, 				"LIGHT",		"Light", 					14,		true,		false,			"Lux"},
		OneSensor { BOARD_URBAN, 		SENSOR_CO, 				"CO_MICS",		"Carbon monoxide", 				16,		false,		true,			"kOhm/ppm"},
		OneSensor { BOARD_URBAN, 		SENSOR_CO_HEAT_TIME, 			"CO_MICS_THEAT",	"Carbon monoxide heat time",			0,		false,		false,			"sec"},
		OneSensor { BOARD_URBAN, 		SENSOR_CO_HEAT_CURRENT, 		"CO_MICS_CURR",		"Carbon monoxide heat current",			0,		false,		false,			"mA"},
		OneSensor { BOARD_URBAN, 		SENSOR_CO_HEAT_SUPPLY_VOLTAGE, 		"CO_MICS_SV",		"Carbon monoxide heat supply voltage",		0,		false,		false,			"mV"},
		OneSensor { BOARD_URBAN, 		SENSOR_CO_HEAT_DROP_VOLTAGE, 		"CO_MICS_DV",		"Carbon monoxide heat drop voltage",		0,		false,		false,			"mV"},
		OneSensor { BOARD_URBAN, 		SENSOR_CO_LOAD_RESISTANCE, 		"CO_MICS_RL",		"Carbon monoxide load resistance",		0,		false,		false,			"Ohms"},
		OneSensor { BOARD_URBAN, 		SENSOR_NO2, 				"NO2_MICS",		"Nitrogen dioxide",				15,		false,		true,			"kOhm/ppm"},
		OneSensor { BOARD_URBAN, 		SENSOR_NO2_HEAT_TIME, 			"NO2_MICS_THEAT",	"Nitrogen dioxide heat time",			0,		false,		false,			"sec"},
		OneSensor { BOARD_URBAN, 		SENSOR_NO2_HEAT_CURRENT, 		"NO2_MICS_CURR",	"Nitrogen dioxide heat current",		0,		false,		false,			"mA"},
		OneSensor { BOARD_URBAN, 		SENSOR_NO2_HEAT_SUPPLY_VOLTAGE, 	"NO2_MICS_SV",		"Nitrogen dioxide heat supply voltage",		0,		false,		false,			"mV"},
		OneSensor { BOARD_URBAN, 		SENSOR_NO2_HEAT_DROP_VOLTAGE, 		"NO2_MICS_DV",		"Nitrogen dioxide heat drop voltage",		0,		false,		false,			"mV"},
		OneSensor { BOARD_URBAN, 		SENSOR_NO2_LOAD_RESISTANCE, 		"NO2_MICS_RL",		"Nitrogen dioxide load resistance",		0,		false,		false,			"Ohms"},

		// I2C Auxiliary Sensors
		
		// I2C Expander TCA9548A
		OneSensor { BOARD_AUX, 			SENSOR_I2C_EXPANDER_TCA9548A,		"I2C_EXP", 		"I2C Expander", 				0,		false,		false,			},

		// SCK Gases Board for Alphasense (3 Gas sensor Slots, + SHT31 Temp-Humidity)
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_1A,		"GB_1A",		"Gases Board 1A",				68,		false,		true,			"mV"},
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_1W,		"GB_1W",		"Gases Board 1W",				67,		false,		true,			"mV"},
		/* OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_1_CAL,		"GB_CO",		"Gases Board CO",				84,		false,		true,			"ppm"}, */
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_2A,		"GB_2A",		"Gases Board 2A",				62,		false,		true,			"mV"},
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_2W, 		"GB_2W",		"Gases Board 2W",				61,		false,		true,			"mV"},
		/* OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_2_CAL,		"GB_NO2",		"Gases Board NO2",				83,		false,		true,			"ppb"}, */
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_3A, 		"GB_3A",		"Gases Board 3A",				65,		false,		true,			"mV"},
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_3W, 		"GB_3W",		"Gases Board 3W",				64,		false,		true,			"mV"},
		/* OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_SLOT_3_CAL,		"GB_O3",		"Gases Board O3",				85,		false,		true,			"ppb"}, */
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_TEMPERATURE, 		"GB_TEMP",		"Gases Board Temperature", 			79,		false,		false,			"C"},
		OneSensor { BOARD_AUX, 			SENSOR_GASESBOARD_HUMIDITY, 		"GB_HUM",		"Gases Board Humidity",				80,		false,		false,			"%"},
	
		// Groove I2C ADC
		OneSensor { BOARD_AUX,			SENSOR_GROOVE_I2C_ADC,			"GRO_ADC",		"Groove ADC",					25,		false,		false,			"V"},

		// Adafruit INA291 High Side DC Current Sensor
		OneSensor { BOARD_AUX,			SENSOR_INA219_BUSVOLT,			"INA_VBUS", 		"INA219 Bus voltage",				0,		false,		false,			"V"},
		OneSensor { BOARD_AUX,			SENSOR_INA219_SHUNT,			"INA_VSHUNT", 		"INA219 Shunt voltage",				0,		false,		false,			"mV"},
		OneSensor { BOARD_AUX,			SENSOR_INA219_CURRENT,			"INA_CURR",		"INA219 Current",				0,		false,		false,			"mA"},
		OneSensor { BOARD_AUX,			SENSOR_INA219_LOADVOLT,			"INA_VLOAD", 		"INA219 Load voltage",				0,		false,		false,			"V"},

		OneSensor { BOARD_AUX,			SENSOR_WATER_TEMP_DS18B20,		"DS_WAT_TEMP", 		"DS18B20 Water temperature",			42,		false,		false,			"C"},
		OneSensor { BOARD_AUX,			SENSOR_ATLAS_TEMPERATURE,		"AS_TEMP",		"Atlas Temperature",				51,		false,		false,			"C"},
		OneSensor { BOARD_AUX,			SENSOR_ATLAS_PH,			"AS_PH",		"Atlas PH",					43,		false,		true,			"pH"},
		OneSensor { BOARD_AUX,			SENSOR_ATLAS_EC,			"AS_COND", 		"Atlas Conductivity",				45,		false,		true,			"uS/cm"},
		OneSensor { BOARD_AUX,			SENSOR_ATLAS_EC_SG,			"AS_SG",		"Atlas Specific gravity",			46,		false,		true,			},
		OneSensor { BOARD_AUX,			SENSOR_ATLAS_DO,			"AS_DO",		"Atlas Dissolved Oxygen",			48,		false,		true,			"mg/L"},
		OneSensor { BOARD_AUX,			SENSOR_ATLAS_DO_SAT,			"AS_DO_SAT", 		"Atlas DO Saturation",				49,		false,		true,			"%"},

		// I2C Moisture Sensor (chirp)
		// https://github.com/Miceuz/i2c-moisture-sensor
		OneSensor { BOARD_AUX, 			SENSOR_CHIRP_MOISTURE,			"CHRP_MOIS", 		"Soil Moisture",				50,		false,		true,			},
		OneSensor { BOARD_AUX, 			SENSOR_CHIRP_TEMPERATURE,		"CHRP_TEMP",		"Soil Temperature",				0,		false,		true,			"C"},
		OneSensor { BOARD_AUX, 			SENSOR_CHIRP_LIGHT,			"CHRP_LIGHT",		"Soil Light",					0,		false,		true,			},

		OneSensor { BOARD_AUX,			SENSOR_PM_1,				"PM_1",			"PM 1.0",					89,		false,		false,			"ug/m3"},
		OneSensor { BOARD_AUX,			SENSOR_PM_25,				"PM_25",		"PM 2.5",					87,		false,		false,			"ug/m3"},
		OneSensor { BOARD_AUX,			SENSOR_PM_10,				"PM_10",		"PM 10.0",					88,		false,		false,			"ug/m3"},

		// Temperature and Humidity Groove - SHT31
		/* OneSensor { BOARD_AUX,			SENSOR_SHT31_TEMP,			"EXT_TEMP",		"External Temperature",				0,		false,		false,			"C"}, */
		/* OneSensor { BOARD_AUX,			SENSOR_SHT31_HUM,			"EXT_HUM",		"External Humidity",				0,		false,		false,			"%"}, */

		// Later this will be moved to a Actuators.h file
		// Groove I2C Oled Display 96x96
		OneSensor { BOARD_AUX,			SENSOR_GROOVE_OLED,			"Groove OLED",								0,		false,		false,			},
		OneSensor { BOARD_BASE, 		SENSOR_COUNT,				"NOT_FOUND",		"Not found",					0,		false,		false,			}


		//-----------------------
		// Add New Sensor Here!!!

		// Muxed Sensors
		// Example of two Groove SHT31 (temperature and humidity) connected to a multiplexer on ports 0 and 1
		// OneSensor {	BOARD_AUX, 			SENSOR_MUX01A, 						"MUX01 Temperature SHT31", 				0,		true,		false,			"C",	01},
		// OneSensor {	BOARD_AUX, 			SENSOR_MUX01B,						"MUX01 Humidity SHT31", 				0,		true,		false,			"%",	01},
		// OneSensor {	BOARD_AUX, 			SENSOR_MUX02, 						"MUX02 Other sensor", 					0,		true,		false,			"C",	02},

	};

	OneSensor & operator[](SensorType type) {
    	return list[type];
	}

private:
	
};
