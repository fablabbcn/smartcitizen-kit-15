#pragma once

#include <EasyTransfer.h>
#include <Sensors.h>
#define ONE_DAY_IN_SECONDS 86400

enum EspCommand {
	ESP_NULL,					// 0
	//							------ Events
	ESP_DEBUG_EVENT,			// 1
	// 							------ Configuration
	ESP_WIFI_CONNECT_COM,		// 2
	ESP_GET_WIFI_COM,			// 3 @params uint8_t (the index of the network to retrieve)
	ESP_SET_WIFI_COM,			// 4 @params String ssid, String password
	ESP_CLEAR_WIFI_COM,			// 5 @params uint8_t index (the index of the network to be cleared)
	ESP_GET_NET_INFO_COM,		// 6
	ESP_SET_TOKEN_COM,			// 7 @params String Token
	ESP_CLEAR_TOKEN_COM,		// 8
	ESP_GET_TOKEN_COM,			// 9
								// @return bool: true if configuration saved, false otherwise
	ESP_GET_CONF_COM,			// 10 @return struct Configuration
	//					 		------ Control
	ESP_START_AP_COM,			// 11
	ESP_STOP_AP_COM,			// 12
	ESP_START_WEB_COM,			// 13
	ESP_STOP_WEB_COM,			// 14
	ESP_GET_VERSION_COM,		// 15
	ESP_CONSOLE_COM,			// 16
	ESP_MQTT_CONSOLE_COM,		// 17
	ESP_CONSOLE_PUBLISH,		// 18
	//					 		------ Get data
	ESP_GET_APLIST_COM,			// 19 @return String apList (json formatted)
	ESP_GET_TIME_COM,			// 20 @return String: epoch time
	ESP_SYNC_HTTP_TIME_COM,		// 21
	ESP_MQTT_HELLOW_COM,		// 22
	ESP_MQTT_PUBLISH_COM,		// 23
	ESP_MQTT_SUBSCRIBE_COM,		// 24
	ESP_MQTT_UNSUBSCRIBE_COM,	// 25
	ESP_MQTT_CLEAR_STATUS,		// 26
	ESP_GET_FREE_HEAP_COM,		// 27

	//					 		------ State
	ESP_GET_STATUS_COM,			// 28 @return struct espStatus:
								//		-- wifi: ESP_WIFI_CONNECTED_EVENT, ESP_WIFI_ERROR_EVENT, ESP_WIFI_ERROR_PASS_EVENT, ESP_WIFI_ERROR_AP_EVENT, ESP_WIFI_NOT_CONFIGURED
								//		-- mqtt: ESP_MQTT_HELLO_OK, ESP_MQTT_PUBLISH_OK, ESP_MQTT_ERROR_EVENT
								//		-- time: ESP_TIME_FAIL, ESP_TIME_UPDATED
								//		-- ap: ESP_AP_ON, ESP_AP_OFF
								//		-- web: ESP_WEB_ON, ESP_WEB_OFF
								//		-- token: ESP_TOKEN_OK, ESP_TOKEN_ERROR
								//		-- conf: ESP_CONF_NOT_CHANGED, ESP_CONF_CHANGED, ESP_CONF_WIFI_UPDATED, ESP_CONF_TOKEN_UPDATED

	ESP_SERIAL_DEBUG_TOGGLE,	// 29 Toggle serial debug output
	ESP_LED_OFF,				// 30 Turn off both leds
	ESP_LED_ON,					// 31 Turn on both leds

	// new simple communications
	ESP_WIFI_CONNECTED,
	MQTT_PUBLISH_SUCCESS,
	MQTT_PUBLISH_FAIL
}; 


// @return struct espStatus:
enum espStatusEvents {

	ESP_NULL_EVENT,

	//	-- wifi: ESP_WIFI_CONNECTED, ESP_WIFI_ERROR, ESP_WIFI_ERROR_PASS, ESP_WIFI_ERROR_AP	
	ESP_WIFI_CONNECTED_EVENT,
	ESP_WIFI_ERROR_EVENT,
	ESP_WIFI_ERROR_PASS_EVENT,
	ESP_WIFI_ERROR_AP_EVENT,
	ESP_WIFI_NOT_CONFIGURED,

	// -- mqtt: ESP_MQTT_HELLO_OK, ESP_MQTT_PUBLISH_OK, ESP_MQTT_ERROR_EVENT, ESP_MQTT_CONFIG_SUB_EVENT, ESP_MQTT_CONFIG_UNSUB_EVENT,
	ESP_MQTT_HELLO_OK_EVENT,
	ESP_MQTT_PUBLISH_OK_EVENT,
	ESP_MQTT_ERROR_EVENT,
	ESP_MQTT_CONFIG_SUB_EVENT,
	ESP_MQTT_CONFIG_UNSUB_EVENT,

	//	-- time: ESP_TIME_FAIL, ESP_TIME_UPDATED
	ESP_TIME_FAIL_EVENT,
	ESP_TIME_UPDATED_EVENT,

	//	-- ap: ESP_AP_ON, ESP_AP_OFF
	ESP_AP_ON_EVENT,
	ESP_AP_OFF_EVENT,

	//	-- web: ESP_WEB_ON, ESP_WEB_OFF
	ESP_WEB_ON_EVENT,
	ESP_WEB_OFF_EVENT,

	//	-- token: ESP_TOKEN_OK, ESP_TOKEN_ERROR
	ESP_TOKEN_OK,
	ESP_TOKEN_ERROR,

	//	-- conf: ESP_CONF_NOT_CHANGED, ESP_CONF_CHANGED, ESP_CONF_WIFI_UPDATED, ESP_CONF_TOKEN_UPDATED
	ESP_CONF_NOT_CHANGED_EVENT,
	ESP_CONF_CHANGED_EVENT,
	ESP_CONF_WIFI_UPDATED,
	ESP_CONF_TOKEN_UPDATED,

	ESP_STATUS_EVENT_COUNT
};

enum espStatusTypes {
	
	ESP_STATUS_WIFI,
	ESP_STATUS_MQTT,
	ESP_STATUS_TIME,
	ESP_STATUS_AP,
	ESP_STATUS_WEB,
	ESP_STATUS_TOKEN,
	ESP_STATUS_CONF,

	ESP_STATUS_TYPES_COUNT
};

struct ESPstatus {
	
	// Main array for events (one value for each status type)
	espStatusEvents value[ESP_STATUS_TYPES_COUNT];

	// Alias for access each status value
	espStatusEvents& wifi 	= value[ESP_STATUS_WIFI];
	espStatusEvents& mqtt 	= value[ESP_STATUS_MQTT];
	espStatusEvents& time 	= value[ESP_STATUS_TIME];
	espStatusEvents& ap 	= value[ESP_STATUS_AP];
	espStatusEvents& web 	= value[ESP_STATUS_WEB];
	espStatusEvents& token 	= value[ESP_STATUS_TOKEN];
	espStatusEvents& conf 	= value[ESP_STATUS_CONF];


	const char *name[ESP_STATUS_TYPES_COUNT] = {
		"wifi",
		"mqtt",
		"time",
		"ap",
		"web",
		"token",
		"conf"
	};

	const char *eventTitle[ESP_STATUS_EVENT_COUNT] = {

		// null event
		"null",
		
		//	-- wifi: ESP_WIFI_CONNECTED, ESP_WIFI_ERROR, ESP_WIFI_ERROR_PASS, ESP_WIFI_ERROR_AP, ESP_WIFI_NOT_CONFIGURED
		"connected",
		"error",
		"password error",
		"ap not found",
		"not configured",

		// -- mqtt: ESP_MQTT_HELLO_OK, ESP_MQTT_PUBLISH_OK, ESP_MQTT_ERROR_EVENT, ESP_MQTT_CONFIG_SUB_EVENT, ESP_MQTT_CONFIG_UNSUB_EVENT,
		"hello ok",
		"publish ok",
		"error",
		"config subscribed",
		"config unsubscribed",

		//	-- time: ESP_TIME_FAIL, ESP_TIME_UPDATED
		"error",
		"updated",

		//	-- ap: ESP_AP_ON, ESP_AP_OFF
		"started",
		"stopped",

		//	-- web: ESP_WEB_ON, ESP_WEB_OFF
		"started",
		"stopped",

		//	-- token: ESP_TOKEN_OK, ESP_TOKEN_ERROR
		"ok",
		"error",

		//	-- conf: ESP_CONF_NOT_CHANGED, ESP_CONF_CHANGED, ESP_CONF_WIFI_UPDATED, ESP_CONF_TOKEN_UPDATED
		"not changed",
		"changed",
		"wifi updated",
		"token updated"
	};
};

struct BUS_Serial {
	uint8_t com;
	char param[240];
	bool waitAnswer = false;
};

enum SCKmodes {
	MODE_SETUP,
	MODE_NET,
	MODE_SD,
	MODE_FLASH,
	MODE_OFF,
	MODE_COUNT
};
const uint32_t minimal_publish_interval = 60;
const uint32_t default_publish_interval = 60;
const uint32_t max_publish_interval = 86400;				// One day

struct SensorConfig {
	bool enabled;
	uint32_t interval;
};

struct Configuration {
	bool valid = false;
	SCKmodes mode;
	SCKmodes persistentMode;								// This mode only changes on user configuration, it can only be MODE_SD or MODE_NET
	uint32_t publishInterval = default_publish_interval;	// in seconds
	SensorConfig sensor[SENSOR_COUNT];
	char ssid[64];
	char pass[64];
	char token[8] = "null";
};

static const uint16_t MQTTbufferSize = 480;

const String hardwareVer 	= "1.5";
const String SAMversion	= "0.2.0";
const String SAMbuildDate = String(__DATE__) + '-' + String(__TIME__);
const String ESPversion = "0.2.0";
const String ESPbuildDate = String(__DATE__) + '-' + String(__TIME__);