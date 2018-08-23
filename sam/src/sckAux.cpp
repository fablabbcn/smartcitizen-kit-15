#include "sckAux.h"

AlphaDelta		alphaDelta;
GrooveI2C_ADC		grooveI2C_ADC;
INA219			ina219;
Groove_OLED		groove_OLED;
WaterTemp_DS18B20 	waterTemp_DS18B20;
I2Cexp_TCA9548A 	i2Cexpander;
Atlas			atlasPH = Atlas(SENSOR_ATLAS_PH);
Atlas			atlasEC = Atlas(SENSOR_ATLAS_EC);
Atlas			atlasDO = Atlas(SENSOR_ATLAS_DO);
Atlas 			atlasTEMP = Atlas(SENSOR_ATLAS_TEMPERATURE);
Moisture 		moistureChirp;
SHT31 			groove_SHT31;

bool I2Cdetect(byte address) {

	Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

	if (error == 0) return true;
	else return false;
}

bool AuxBoards::begin(OneSensor* wichSensor) {

	if (i2Cexpander.detected) {
		if (wichSensor->muxPort >= 0) i2Cexpander.selectPort(wichSensor->muxPort);
		else i2Cexpander.disable();
	}

	switch (wichSensor->type) {

		case SENSOR_I2C_EXPANDER_TCA9548A:		i2Cexpander.begin(); return false; break;
		case SENSOR_ALPHADELTA_SLOT_1A:
		case SENSOR_ALPHADELTA_SLOT_1W:
		case SENSOR_ALPHADELTA_SLOT_2A:
		case SENSOR_ALPHADELTA_SLOT_2W:
		case SENSOR_ALPHADELTA_SLOT_3A:
		case SENSOR_ALPHADELTA_SLOT_3W:
		case SENSOR_ALPHADELTA_HUMIDITY:
		case SENSOR_ALPHADELTA_TEMPERATURE: 	return alphaDelta.begin(); break;
		case SENSOR_GROOVE_I2C_ADC: 			return grooveI2C_ADC.begin(); break;
		case SENSOR_INA219_BUSVOLT:
		case SENSOR_INA219_SHUNT:
		case SENSOR_INA219_CURRENT:
		case SENSOR_INA219_LOADVOLT: 			return ina219.begin(); break;
		case SENSOR_GROOVE_OLED: 				wichSensor->interval = 4; return groove_OLED.begin(); break;
		case SENSOR_WATER_TEMP_DS18B20:			return waterTemp_DS18B20.begin(); break;
		case SENSOR_ATLAS_PH:					return atlasPH.begin();
		case SENSOR_ATLAS_EC:
		case SENSOR_ATLAS_EC_SG: 				return atlasEC.begin(); break;
		case SENSOR_ATLAS_DO:
		case SENSOR_ATLAS_DO_SAT: 				return atlasDO.begin(); break;
		case SENSOR_ATLAS_TEMPERATURE:			return atlasTEMP.begin(); break;
		case SENSOR_CHIRP_TEMPERATURE:
		case SENSOR_CHIRP_LIGHT:
		case SENSOR_CHIRP_MOISTURE:				return moistureChirp.begin(); break;
		case SENSOR_GROOVE_TEMP_SHT31:
		case SENSOR_GROOVE_HUM_SHT31: 			return groove_SHT31.begin(); break;
		default: break;
	}

	return false;
}

float AuxBoards::getReading(OneSensor* wichSensor) {

	if (i2Cexpander.detected) {
		if (wichSensor->muxPort >= 0) {
			if (!i2Cexpander.selectPort(wichSensor->muxPort)) return -9998;
		}
		else i2Cexpander.disable();
	}

	switch (wichSensor->type) {
		case SENSOR_ALPHADELTA_SLOT_1A:	 	return alphaDelta.getElectrode(alphaDelta.Slot1.electrode_A); break;
		case SENSOR_ALPHADELTA_SLOT_1W: 	return alphaDelta.getElectrode(alphaDelta.Slot1.electrode_W); break;
		case SENSOR_ALPHADELTA_SLOT_2A: 	return alphaDelta.getElectrode(alphaDelta.Slot2.electrode_A); break;
		case SENSOR_ALPHADELTA_SLOT_2W: 	return alphaDelta.getElectrode(alphaDelta.Slot2.electrode_W); break;
		case SENSOR_ALPHADELTA_SLOT_3A: 	return alphaDelta.getElectrode(alphaDelta.Slot3.electrode_A); break;
		case SENSOR_ALPHADELTA_SLOT_3W: 	return alphaDelta.getElectrode(alphaDelta.Slot3.electrode_W); break;
		case SENSOR_ALPHADELTA_HUMIDITY: 	return alphaDelta.getHumidity(); break;
		case SENSOR_ALPHADELTA_TEMPERATURE: return alphaDelta.getTemperature(); break;
		case SENSOR_GROOVE_I2C_ADC: 		return grooveI2C_ADC.getReading(); break;
		case SENSOR_INA219_BUSVOLT: 		return ina219.getReading(ina219.BUS_VOLT); break;
		case SENSOR_INA219_SHUNT: 			return ina219.getReading(ina219.SHUNT_VOLT); break;
		case SENSOR_INA219_CURRENT: 		return ina219.getReading(ina219.CURRENT); break;
		case SENSOR_INA219_LOADVOLT: 		return ina219.getReading(ina219.LOAD_VOLT); break;
		case SENSOR_WATER_TEMP_DS18B20:		return waterTemp_DS18B20.getReading(); break;
		case SENSOR_ATLAS_PH:				return atlasPH.newReading; break;
		case SENSOR_ATLAS_EC:				return atlasEC.newReading; break;
		case SENSOR_ATLAS_EC_SG:			return atlasEC.newReadingB; break;
		case SENSOR_ATLAS_DO:				return atlasDO.newReading; break;
		case SENSOR_ATLAS_DO_SAT:			return atlasDO.newReadingB; break;
		case SENSOR_ATLAS_TEMPERATURE:		return atlasTEMP.newReading; break;
		case SENSOR_CHIRP_MOISTURE:			return moistureChirp.getReading(moistureChirp.CHIRP_MOISTURE); break;
		case SENSOR_CHIRP_TEMPERATURE:		return moistureChirp.getReading(moistureChirp.CHIRP_TEMPERATURE); break;
		case SENSOR_CHIRP_LIGHT:			return moistureChirp.getReading(moistureChirp.CHIRP_LIGHT); break;
		case SENSOR_GROOVE_TEMP_SHT31: 		if (groove_SHT31.update()) return groove_SHT31.temperature; break;
		case SENSOR_GROOVE_HUM_SHT31: 		if (groove_SHT31.update()) return groove_SHT31.humidity; break;
		default: break;
	}

	return -9999;
}

bool AuxBoards::getBusyState(OneSensor* wichSensor) {

	if (i2Cexpander.detected) {
		if (wichSensor->muxPort >= 0) i2Cexpander.selectPort(wichSensor->muxPort);
		else i2Cexpander.disable();
	}

	switch(wichSensor->type) {
		case SENSOR_GROOVE_OLED:		return true; break;
		case SENSOR_ATLAS_PH: 			return atlasPH.getBusyState(); break;
		case SENSOR_ATLAS_EC:
		case SENSOR_ATLAS_EC_SG: 		return atlasEC.getBusyState(); break;
		case SENSOR_ATLAS_DO:
		case SENSOR_ATLAS_DO_SAT: 		return atlasDO.getBusyState(); break;
		case SENSOR_ATLAS_TEMPERATURE:	return atlasTEMP.getBusyState(); break;
		case SENSOR_CHIRP_MOISTURE:		return moistureChirp.getBusyState(moistureChirp.CHIRP_MOISTURE); break;
		case SENSOR_CHIRP_TEMPERATURE:	return moistureChirp.getBusyState(moistureChirp.CHIRP_TEMPERATURE); break;
		case SENSOR_CHIRP_LIGHT: 		return moistureChirp.getBusyState(moistureChirp.CHIRP_LIGHT); break;
		default: return false; break;
	}
}

String AuxBoards::control(OneSensor* wichSensor, String command) {

	if (i2Cexpander.detected) {
		if (wichSensor->muxPort >= 0) i2Cexpander.selectPort(wichSensor->muxPort);
		else i2Cexpander.disable();
	}

	switch(wichSensor->type) {
		case SENSOR_ALPHADELTA_SLOT_1A:
		case SENSOR_ALPHADELTA_SLOT_1W:
		case SENSOR_ALPHADELTA_SLOT_2A:
		case SENSOR_ALPHADELTA_SLOT_2W:
		case SENSOR_ALPHADELTA_SLOT_3A:
		case SENSOR_ALPHADELTA_SLOT_3W: {

			if (command.startsWith("set pot")) {

				Electrode wichElectrode;

				switch(wichSensor->type) {
					case SENSOR_ALPHADELTA_SLOT_1A: wichElectrode = alphaDelta.Slot1.electrode_A;
					case SENSOR_ALPHADELTA_SLOT_1W: wichElectrode = alphaDelta.Slot1.electrode_W;
					case SENSOR_ALPHADELTA_SLOT_2A: wichElectrode = alphaDelta.Slot2.electrode_A;
					case SENSOR_ALPHADELTA_SLOT_2W: wichElectrode = alphaDelta.Slot2.electrode_W;
					case SENSOR_ALPHADELTA_SLOT_3A: wichElectrode = alphaDelta.Slot3.electrode_A;
					case SENSOR_ALPHADELTA_SLOT_3W: wichElectrode = alphaDelta.Slot3.electrode_W;
					default: break;
				}

				command.replace("set pot", "");
				command.trim();
				int wichValue = command.toInt();
				alphaDelta.setPot(wichElectrode, wichValue);
				return String F("Setting pot to: ") + String(wichValue) + F(" Ohms\n\rActual value: ") + String(alphaDelta.getPot(wichElectrode)) + F(" Ohms");

			} else if (command.startsWith("get uid")) {

				return String F("Eeprom UID: ") + String(alphaDelta.getUID());

			} else if (command.startsWith("help")) {

				return F("Available commands for this sensor:\n\r* set pot \n\r* get UID");

			#ifdef deltaTest
			} else if (command.startsWith("test")) {

				command.replace("test", "");
				command.trim();

				// Get slot
				String slotSTR = String(command.charAt(0));
				uint8_t wichSlot = slotSTR.toInt();

				command.remove(0,1);
				command.trim();

				if (command.startsWith("set")) {

					command.replace("set", "");
					command.trim();

					// Get value
					int wichValue = command.toInt();
					alphaDelta.setTesterCurrent(wichValue, wichSlot);

				} else if (command.startsWith("full")) {

					alphaDelta.runTester(wichSlot);

				} else {
					return F("Unrecognized test command!!\r\nOptions:\r\ntest slot set value (slot: 1-3, value:-1400/+1400 nA)\r\ntest slot full (test the full cycle on slot (1-3))");
				}

				return F("\nTesting finished!");
			#endif

			} else {
				return F("Unrecognized command!! please try again...");
			}

			break;

		}
		case SENSOR_ATLAS_PH:
		case SENSOR_ATLAS_EC:
		case SENSOR_ATLAS_EC_SG:
		case SENSOR_ATLAS_DO:
		case SENSOR_ATLAS_DO_SAT: {

			Atlas *thisAtlas;

			if (wichSensor->type == SENSOR_ATLAS_PH) thisAtlas = &atlasPH;
			else if (wichSensor->type == SENSOR_ATLAS_EC || wichSensor->type == SENSOR_ATLAS_EC_SG) thisAtlas = &atlasEC;
			else if (wichSensor->type == SENSOR_ATLAS_DO || wichSensor->type == SENSOR_ATLAS_DO_SAT) thisAtlas = &atlasDO;

			// 	 Calibration command options:
			// 		Atlas PH: (https://www.atlas-scientific.com/_files/_datasheets/_circuit/pH_EZO_datasheet.pdf) page 50
			// 			* com cal,[mid,low,high] 7.00
			// 			* com cal,clear
			// 		Atlas EC: (https://www.atlas-scientific.com/_files/_datasheets/_circuit/EC_EZO_Datasheet.pdf) page 52
			// 			* com cal,[dry,clear,84]
			// 			* com cal,low,1413
			// 			* com cal,high,12880
			// 		Atlas DO: (https://www.atlas-scientific.com/_files/_datasheets/_circuit/DO_EZO_Datasheet.pdf) page 50
			// 			* com cal
			// 			* com cal,0
			// 			* com cal,clear
			if (command.startsWith("com")) {

				command.replace("com", "");
				command.trim();
				thisAtlas->sendCommand((char*)command.c_str());

				uint8_t responseCode = thisAtlas->getResponse();
				if (responseCode == 254) delay(1000); responseCode = thisAtlas->getResponse();
				if (responseCode == 1) return thisAtlas->atlasResponse;
				else return String(responseCode);

			}
			break;

		} case SENSOR_CHIRP_LIGHT:
		case SENSOR_CHIRP_TEMPERATURE:
		case SENSOR_CHIRP_MOISTURE: {

			if (command.startsWith("get ver")) {

				return String(moistureChirp.getVersion());

			} else if (command.startsWith("help")) return F("Available commands for this sensor:\n\r* get ver");
			else return F("Unrecognized command!! please try again...");
			break;

		} default: return "Unrecognized sensor!!!";
	}
	return "Unknown error on control command!!!";
}

void AuxBoards::print(String payload) {

	groove_OLED.print(payload);
}

void AuxBoards::displayReading(String title, String reading, String unit, String time) {

	groove_OLED.displayReading(title, reading, unit, time);
}

bool GrooveI2C_ADC::begin() {

	if (!I2Cdetect(deviceAddress)) return false;

	Wire.beginTransmission(deviceAddress);		// transmit to device
	Wire.write(REG_ADDR_CONFIG);				// Configuration Register
	Wire.write(0x20);
	Wire.endTransmission();
	return true;
}

float GrooveI2C_ADC::getReading() {

	uint32_t data = 0;

	Wire.beginTransmission(deviceAddress);		// transmit to device
	Wire.write(REG_ADDR_RESULT);				// get result
	Wire.endTransmission();

	Wire.requestFrom(deviceAddress, 2);			// request 2byte from device
	delay(1);

	if (Wire.available()<=2) {
		data = (Wire.read()&0x0f)<<8;
		data |= Wire.read();
	}

	return data * V_REF * 2.0 / 4096.0;
}

bool INA219::begin() {

	if (!I2Cdetect(deviceAddress)) return false;

	ada_ina219.begin();

	// By default the initialization will use the largest range (32V, 2A).  However
	// To use a slightly lower 32V, 1A range (higher precision on amps):
	//ada_ina219.setCalibration_32V_1A();

	// Or to use a lower 16V, 400mA range (higher precision on volts and amps):
	ada_ina219.setCalibration_16V_400mA();
	return true;
}

float INA219::getReading(typeOfReading wichReading) {

	switch(wichReading) {
		case BUS_VOLT: {

			return ada_ina219.getBusVoltage_V();
			break;

		} case SHUNT_VOLT: {

			return ada_ina219.getShuntVoltage_mV();
			break;

		} case CURRENT: {

			return ada_ina219.getCurrent_mA();
			break;

		} case LOAD_VOLT: {

			float busvoltage 	= ada_ina219.getBusVoltage_V();
			float shuntvoltage 	= ada_ina219.getShuntVoltage_mV();

			return busvoltage + (shuntvoltage / 1000);
			break;

		}
	}

	return 0;
}

bool Groove_OLED::begin() {

	if (!I2Cdetect(deviceAddress)) return false;

	U8g2_oled.begin();
	U8g2_oled.clearDisplay();

	U8g2_oled.firstPage();
	do { U8g2_oled.drawXBM(16, 16, 96, 96, scLogo); } while (U8g2_oled.nextPage());

	return true;;
}

void Groove_OLED::print(String payload) {

	// uint8_t length = payload.length();
	char charPayload[payload.length()];
	payload.toCharArray(charPayload, payload.length()+1);

	U8g2_oled.firstPage();

	do {
		U8g2_oled.setFont(u8g2_font_ncenB14_tr);
		U8g2_oled.drawStr(0,24, charPayload);
	} while (U8g2_oled.nextPage());
}

void Groove_OLED::displayReading(String title, String reading, String unit, String time) {

	uint8_t screenW = 128;

	String date;
	String hour;

	if (time.toInt() != 0) {
		date = time.substring(8,10) + "/" + time.substring(5,7) + "/" + time.substring(2,4);
		hour = time.substring(11,16);
	}

	U8g2_oled.firstPage();
	do {

		// Title
		U8g2_oled.setFont(u8g2_font_helvB10_tf);
		if (U8g2_oled.getStrWidth(title.c_str()) > screenW && title.indexOf(" ") > -1) {

			String first = title.substring(0, title.indexOf(" "));
			String second = title.substring(title.indexOf(" ")+1);

			U8g2_oled.drawStr((screenW-U8g2_oled.getStrWidth(first.c_str()))/2,11, first.c_str());
			U8g2_oled.drawStr((screenW-U8g2_oled.getStrWidth(second.c_str()))/2,23, second.c_str());

		} else U8g2_oled.drawStr((screenW-U8g2_oled.getStrWidth(title.c_str()))/2,11, title.c_str());

		// Reading
		U8g2_oled.setFont(u8g2_font_helvB24_tf);
		if (U8g2_oled.getStrWidth(reading.c_str()) > screenW) U8g2_oled.setFont(u8g2_font_helvB18_tf);
		U8g2_oled.drawStr((screenW-U8g2_oled.getStrWidth(reading.c_str()))/2, 70,  reading.c_str());

		// Unit
		U8g2_oled.setFont(u8g2_font_helvB14_tf);
		U8g2_oled.drawStr((screenW-U8g2_oled.getStrWidth(unit.c_str()))/2,90, unit.c_str());

		if (time.toInt() != 0) {

			// Date
			U8g2_oled.setFont(u8g2_font_helvB10_tf);
			U8g2_oled.drawStr(0,screenW,date.c_str());

			// Time
			U8g2_oled.drawStr(screenW-U8g2_oled.getStrWidth(hour.c_str()),screenW,hour.c_str());
			U8g2_oled.drawStr(screenW-U8g2_oled.getStrWidth(hour.c_str()),screenW,hour.c_str());
		}

	} while (U8g2_oled.nextPage());
}

bool WaterTemp_DS18B20::begin() {

	if (!I2Cdetect(deviceAddress)) return false;

	Wire.begin();

	DS_bridge.reset();
	DS_bridge.wireReset();
	DS_bridge.wireSkip();
	DS_bridge.wireWriteByte(0x44);

	detected = true;

	return true;
}

float WaterTemp_DS18B20::getReading() {

 	while ( !DS_bridge.wireSearch(addr)) {

		DS_bridge.wireResetSearch();
		DS_bridge.wireReset();
		DS_bridge.selectChannel(0); 			// After reset need to set channel 0 because we are using the version with single channel (DS2482_100)
		DS_bridge.configure(conf);
 		DS_bridge.wireSkip();
 		DS_bridge.configure(conf); 				// Set bus on strong pull-up after next write, not only LSB nibble is required
 		DS_bridge.wireWriteByte(0x44); 			// Convert temperature on all devices
 		DS_bridge.configure(0x01);

	}

	//	Test if device is in reality the DS18B20 Water Temperature
	if (addr[0]==0x28) {

		// Read temperature data.
		DS_bridge.wireReset(); 				//DS_bridge.reset();
		DS_bridge.selectChannel(0); 		//necessary on -800
		DS_bridge.wireSelect(addr);
		DS_bridge.wireWriteByte(0xbe);      // Read Scratchpad command

		// We need to read 9 bytes
		for ( int i = 0; i < 9; i++) data[i] = DS_bridge.wireReadByte();

		// Convert to decimal temperature
		int LowByte = data[0];
		int HighByte = data[1];
		int TReading = (HighByte << 8) + LowByte;
		int SignBit = TReading & 0x8000;

		// If Temperature is negative
		if (SignBit) TReading = (TReading ^ 0xffff) + 1;

		int Tc_100 = (double)TReading * 0.0625 * 10;

		// If the reading is negative make it efective
		if (SignBit) Tc_100 = 0 - Tc_100;

		return ((float)(Tc_100) / 10) + 1;
	}

	return 0;
}

bool Atlas::begin() {

	if (!I2Cdetect(deviceAddress)) return false;

	if (beginDone) return true;
	beginDone = true;

	// Protocol lock
	if (!sendCommand((char*)"Plock,1")) return false;
	delay(shortWait);

	// This actions are only for conductivity (EC) sensor
	if (EC) {

		// Set probe
		if (!sendCommand((char*)"K,1.0")) return false;
		delay(shortWait);

		// ----  Set parameters
		if (sendCommand((char*)"O,?")) {
			delay(shortWait);
			if (!atlasResponse.equals("?O,EC,SG")) {
				const char *parameters[4] = PROGMEM {"O,EC,1", "O,TDS,0", "O,S,0", "O,SG,1"};
				for (int i = 0; i<4; ++i) {
					if (!sendCommand((char*)parameters[i])) return false;
					delay(longWait);
				}
			}
		} else return false;

	} else if (DO) {

		// ---- Set parameters
		if (sendCommand((char*)"O,?")) {
			delay(shortWait);
			if (!atlasResponse.equals((char*)"?O,%,mg")) {
				if (!sendCommand((char*)"O,%,1")) return false;
				delay(shortWait);
				if (!sendCommand((char*)"O,mg,1")) return false;
				delay(shortWait);
			}
		} else return false;
	}

	goToSleep();

	return true;
}

float Atlas::getReading() {

	return newReading;
}

bool Atlas::getBusyState() {



	switch (state) {

		case REST: {

			if (TEMP) {
				state = TEMP_COMP_SENT;
				break;
			}

			if (tempCompensation()) state = TEMP_COMP_SENT;
			break;

		} case TEMP_COMP_SENT: {
			if (millis() - lastCommandSent >= shortWait) {
				if (sendCommand((char*)"r")) state = ASKED_READING;
			}
			break;

		} case ASKED_READING: {

			uint16_t customWait = longWait;

			if (TEMP) customWait = mediumWait;

			if (millis() - lastCommandSent >= customWait) {

				uint8_t code = getResponse();

				if (code == 254) {
					// Still working (wait a little more)
					lastCommandSent = lastCommandSent + 200;
					break;

				} else if (code == 1) {

					// Reading OK
					state = REST;

					if (PH || TEMP)	newReading = atlasResponse.toFloat();
					if (EC || DO) {
						String first = atlasResponse.substring(0, atlasResponse.indexOf(","));
						String second = atlasResponse.substring(atlasResponse.indexOf(",")+1);
						newReading = first.toFloat();
						newReadingB = second.toFloat();
					}
					goToSleep();
					return false;
					break;

				} else {

					// Error
					state = REST;
					newReading = 0;
					goToSleep();
					return false;
					break;
				}
			}
			break;
		}
	}

	return true;
}

void Atlas::goToSleep() {

	Wire.beginTransmission(deviceAddress);
	Wire.write("Sleep");
	Wire.endTransmission();
}

bool Atlas::sendCommand(char* command) {

	uint8_t retrys = 5;

	for (uint8_t i=0; i<retrys; ++i) {

		Wire.beginTransmission(deviceAddress);
		Wire.write(command);

		Wire.requestFrom(deviceAddress, 1, true);
		uint8_t confirmed = Wire.read();
		Wire.endTransmission();

		if (confirmed == 1) {
			lastCommandSent = millis();
			return true;
		}

		delay(300);
	}
	return false;
}

bool Atlas::tempCompensation() {

	String stringData;
	char data[10];
	float temperature = 0;

	if (waterTemp_DS18B20.detected) temperature = waterTemp_DS18B20.getReading();
	else if (atlasTEMP.detected) temperature = atlasTEMP.getReading();
	else {

		// No available sensor for temp compensation
		// Still we want the readings
		return true;
	}

	// Error on reading temperature
	if (temperature == 0) return false;

	sprintf(data,"T,%.2f",temperature);
	if (sendCommand(data)) return true;

	return false;
}

uint8_t Atlas::getResponse() {

	uint8_t code;

	Wire.requestFrom(deviceAddress, 20, 1);
	code = Wire.read();

	atlasResponse = "";

	switch (code) {
		case 0: 		// Undefined
		case 2:			// Error
		case 255:		// No data sent
		case 254: {		// Still procesing, not ready

			return code;
			break;

		} default : {

			while (Wire.available()) {
				char buff = Wire.read();
				atlasResponse += buff;
			}
			Wire.endTransmission();

			goToSleep();

			if (atlasResponse.length() > 0) {
				return 1;
			}

			return 2;
		}
    }
}

bool Moisture::begin() {

	if (!I2Cdetect(deviceAddress)) return false;
	if (alreadyStarted) return true;

	chirp.begin();

	alreadyStarted = true;
	return true;
}

float Moisture::getReading(typeOfReading wichReading) {

	switch(wichReading) {
		case CHIRP_MOISTURE: {

			return chirp.getCapacitance();
			break;

		} case CHIRP_TEMPERATURE: {

			return chirp.getTemperature() / 10.0;
			break;

		} case CHIRP_LIGHT: {

			measuringLight = false;
			return chirp.getLight(false);
			break;

		} default: break;
	}

	return 0;
}

bool Moisture::getBusyState(typeOfReading wichReading) {

	if (chirp.isBusy()) return true;

	if (wichReading == CHIRP_LIGHT) {

		if (measuringLight) {

			if (millis() - lightStarted < 3000) return true;

		} else {

			chirp.startMeasureLight();
			lightStarted = millis();
			measuringLight = true;
			return true;

		}
	}

	return false;
}

uint8_t Moisture::getVersion() {

	return chirp.getVersion();
}

void Moisture::sleep() {

	chirp.sleep();
}

bool SHT31::begin()
{
        Wire.begin();
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        if (error != 0) return false;

        delay(1);               // In case the device was off
        sendComm(SOFT_RESET);   // Send reset command
        delay(50);              // Give time to finish reset
        update(true);

        return true;
}
bool SHT31::stop()
{

        // It will go to idle state by itself after 1ms
        return true;
}
bool SHT31::update(bool wait)
{
	Wire.setClock(100000);

        uint32_t elapsed = millis() - lastTime;
        if (elapsed < timeout) delay(timeout - elapsed);

        uint8_t readbuffer[6];
        sendComm(SINGLE_SHOT_HIGH_REP);

        Wire.requestFrom(address, (uint8_t)6);
        // Wait for answer (datasheet says 15ms is the max)
        uint32_t started = millis();
        while(Wire.available() != 6) {
                if (millis() - started > timeout) return 0;
        }

        // Read response
        for (uint8_t i=0; i<6; i++) readbuffer[i] = Wire.read();

        uint16_t ST, SRH;
        ST = readbuffer[0];
        ST <<= 8;
        ST |= readbuffer[1];

        // Check Temperature crc
        if (readbuffer[2] != crc8(readbuffer, 2)) return false;
        SRH = readbuffer[3];
        SRH <<= 8;
        SRH |= readbuffer[4];

        // check Humidity crc
        if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;
        double temp = ST;
        temp *= 175;
        temp /= 0xffff;
        temp = -45 + temp;
        temperature = (float)temp;

        double shum = SRH;
        shum *= 100;
        shum /= 0xFFFF;
        humidity = (float)shum;

        lastTime = millis();

        return true;
}
void SHT31::sendComm(uint16_t comm)
{
        Wire.beginTransmission(address);
        Wire.write(comm >> 8);
        Wire.write(comm & 0xFF);
        Wire.endTransmission();
}
uint8_t SHT31::crc8(const uint8_t *data, int len)
{

        /* CRC-8 formula from page 14 of SHT spec pdf */

        /* Test data 0xBE, 0xEF should yield 0x92 */

        /* Initialization data 0xFF */
        /* Polynomial 0x31 (x8 + x5 +x4 +1) */
        /* Final XOR 0x00 */

        const uint8_t POLYNOMIAL(0x31);
        uint8_t crc(0xFF);

        for ( int j = len; j; --j ) {
                crc ^= *data++;
                for ( int i = 8; i; --i ) {
                        crc = ( crc & 0x80 )
                                ? (crc << 1) ^ POLYNOMIAL
                                : (crc << 1);
                }
        }
        return crc;
}

uint8_t Groove_SHT31::crc8(const uint8_t *data, int len) {

 /* CRC-8 formula from page 14 of SHT spec pdf
 *
 * Test data 0xBE, 0xEF should yield 0x92
 *
 * Initialization data 0xFF
 * Polynomial 0x31 (x8 + x5 +x4 +1)
 * Final XOR 0x00
 */
	const uint8_t POLYNOMIAL(0x31);
	uint8_t crc(0xFF);

	for ( int j = len; j; --j ) {
		crc ^= *data++;
		for ( int i = 8; i; --i ) {
			crc = ( crc & 0x80 )
			? (crc << 1) ^ POLYNOMIAL
			: (crc << 1);
		}
	}
	return crc;
}

bool I2Cexp_TCA9548A::begin() {

	for (uint8_t i=0; i<7; i++) {
		if (I2Cdetect(devices[i].address)) {
			devices[i].present = 1;
			detected = true;
		}
	}
	return detected;
}

bool I2Cexp_TCA9548A::selectPort(uint8_t wichMuxPort) {

	uint8_t wichDevice = wichMuxPort / 10;
	uint8_t wichPort = wichMuxPort % 10;

	if (!devices[wichDevice].present) return false;
	if (wichPort > 7) return false;

	Wire.beginTransmission(devices[wichDevice].address);
	Wire.write(1 << wichPort);
	uint8_t response = Wire.endTransmission();
	if (response != 0) return false;

	return true;
}

bool I2Cexp_TCA9548A::disable() {

	for (uint8_t i=0; i<7; i++) {
		if (devices[i].present) {
			Wire.beginTransmission(devices[i].address);
			Wire.write(0);
			Wire.endTransmission();
		}
	}
	return true;
}


void writeI2C(byte deviceaddress, byte instruction, byte data ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write(instruction);
  Wire.write(data);
  Wire.endTransmission();
}

byte readI2C(byte deviceaddress, byte instruction) {
  byte  data = 0x0000;
  Wire.beginTransmission(deviceaddress);
  Wire.write(instruction);
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  unsigned long time = millis();
  while (!Wire.available()) if ((millis() - time)>500) return 0x00;
  data = Wire.read();
  return data;
}
