#include <AlphaDelta.h>

bool AlphaDelta::begin() {

	if (!I2Cdetect(sht31Address)) return false;

	if (alreadyStarted) return true;
	alreadyStarted = true;

	sht31.begin();

	// Set all potentiometers to 0
	setPot(Slot1.electrode_A, 0);
	setPot(Slot1.electrode_W, 0);
	setPot(Slot2.electrode_A, 0);
	setPot(Slot2.electrode_W, 0);
	setPot(Slot3.electrode_A, 0);
	setPot(Slot3.electrode_W, 0);

	return true;
}

float AlphaDelta::getTemperature() {

	return sht31.readTemperature();
}

float AlphaDelta::getHumidity() {

	return sht31.readHumidity();
}

uint32_t AlphaDelta::getPot(Electrode wichElectrode) {

	return ((255 - readI2C(wichElectrode.resistor.address, wichElectrode.resistor.channel)) * ohmsPerStep);
}

void AlphaDelta::setPot(Electrode wichElectrode, uint32_t value) {

	int data=0x00;
	if (value>100000) value = 100000;
	data = 255 - (int)(value/ohmsPerStep);		// POT's are connected 'upside down' (255 - step)
	
	writeI2C(wichElectrode.resistor.address, 16, 192);        	// select WR (volatile) registers in POT
	writeI2C(wichElectrode.resistor.address, wichElectrode.resistor.channel, data);
}

uint8_t AlphaDelta::getPGAgain(MCP342X adc) {
	uint8_t gainPGA = adc.getConfigRegShdw() & 0x3;
	return pow(2, gainPGA);
}

float AlphaDelta::getElectrodeGain(Electrode wichElectrode) {

	return (((getPot(wichElectrode) + 85) / 10000.0f) + 1) * getPGAgain(wichElectrode.adc);
}

// Returns electrode value in mV
double AlphaDelta::getElectrode(Electrode wichElectrode) {

	static int32_t result;

	// Gain can be changed before calling this funtion with: wichElectrode.gain = newGain (0->gain of 1, 1->gain of 2, 2->gain of 3 or 3->gain 0f 8)
	wichElectrode.adc.configure( MCP342X_MODE_ONESHOT | MCP342X_SIZE_18BIT | wichElectrode.gain);	
	wichElectrode.adc.startConversion(wichElectrode.channel);
	wichElectrode.adc.getResult(&result);

	return (result * 0.015625) / getPGAgain(wichElectrode.adc);
}

#ifdef deltaTest
void AlphaDelta::runTester(uint8_t wichSlot) {

	Electrode wichElectrode_W;
	Electrode wichElectrode_A;

	switch(wichSlot) {
		case 1: { 
			wichElectrode_W = Slot1.electrode_W;
			wichElectrode_A = Slot1.electrode_A;
			break;
		} case 2: { 
			wichElectrode_W = Slot2.electrode_W;
			wichElectrode_A = Slot2.electrode_A;
			break;
		} case 3: { 
			wichElectrode_W = Slot3.electrode_W;
			wichElectrode_A = Slot3.electrode_A;
			break;
		}
		default: break;
	}

	// Print headers
	SerialUSB.println("testW,readW,testA,readA");

	// Output from -1440 to +1400 nA
	for (int16_t i=-1400; i<1400; i++) {
  		tester.setCurrent(tester.electrode_W, i);
  		SerialUSB.print(tester.getCurrent(tester.electrode_W));
  		SerialUSB.print(",");
  		SerialUSB.print(getElectrode(wichElectrode_W));
  		SerialUSB.print(",");
  		tester.setCurrent(tester.electrode_A, i);
  		SerialUSB.print(tester.getCurrent(tester.electrode_A));
  		SerialUSB.print(",");
  		SerialUSB.println(getElectrode(wichElectrode_A));
  		
  	}
}

void AlphaDelta::setTesterCurrent(int16_t wichCurrent, uint8_t wichSlot) {

	Electrode wichElectrode_W;
	Electrode wichElectrode_A;

	switch(wichSlot) {
		case 1: { 
			wichElectrode_W = Slot1.electrode_W;
			wichElectrode_A = Slot1.electrode_A;
			break;
		} case 2: { 
			wichElectrode_W = Slot2.electrode_W;
			wichElectrode_A = Slot2.electrode_A;
			break;
		} case 3: { 
			wichElectrode_W = Slot3.electrode_W;
			wichElectrode_A = Slot3.electrode_A;
			break;
		}
		default: break;
	}

	SerialUSB.print("Setting test current to: ");
	SerialUSB.println(wichCurrent);

	tester.setCurrent(tester.electrode_W, wichCurrent);
	tester.setCurrent(tester.electrode_A, wichCurrent);

	SerialUSB.print("Tester Electrode W: ");
	SerialUSB.println(tester.getCurrent(tester.electrode_W));
	SerialUSB.print("Alphadelta ");
	SerialUSB.print(wichSlot);
	SerialUSB.print("W: ");
	SerialUSB.println(getElectrode(wichElectrode_W));

	SerialUSB.print("Tester Electrode A: ");
	SerialUSB.println(tester.getCurrent(tester.electrode_A));
	SerialUSB.print("Alphadelta ");
	SerialUSB.print(wichSlot);
	SerialUSB.print("A: ");
	SerialUSB.println(getElectrode(wichElectrode_A));
	
}
#endif

