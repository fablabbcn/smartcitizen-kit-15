#include "AlphaDeltaTester.h"

bool testerAlphaDelta::begin() {
	setCurrent(electrode_W, 0);
	setCurrent(electrode_A, 0);
	return true;
}

void testerAlphaDelta::setPot(Resistor wichPot, uint8_t wichValue) {
	Wire.beginTransmission(wichPot.address);
	Wire.write(wichPot.SET);
	Wire.write(wichValue);
	Wire.endTransmission(wichPot.address);
}

uint8_t testerAlphaDelta::readPot(Resistor wichPot) {
	int data = 0;
	
	Wire.beginTransmission(wichPot.address);
	Wire.write(wichPot.READ);
	Wire.endTransmission(wichPot.address);
	Wire.requestFrom(wichPot.address, 2);

	unsigned long time = millis();
	while (!Wire.available()) if ((millis() - time)>500) return 0x00;
	while (Wire.available()) data = Wire.read();
	return data;

}

void testerAlphaDelta::setCurrent(Electrode wichElectrode, int16_t wichCurrent) {
	
	int16_t currentIndex = wichCurrent + DELTA_CURRENT;
	uint8_t currentValues[2];
	uint16_t pause = 1;

	if (wichElectrode.type == ELECTRODE_W) {
		memcpy(currentValues, CURRENT_LOOKUP_W[currentIndex], 2);
		pause = abs(electrode_W.nA - wichCurrent);
		electrode_W.nA = wichCurrent;
	} else {
		memcpy(currentValues, CURRENT_LOOKUP_A[currentIndex], 2);
		pause = abs(electrode_A.nA - wichCurrent);
		electrode_A.nA = wichCurrent;
	}

	setPot(wichElectrode.pots[0], currentValues[0]);
	setPot(wichElectrode.pots[1], currentValues[1]);

	// TODO: hay que introducir un delay aqui que este relacionado con
	// la corriente anterior, el integrador que ajusta la corriente tarda
	// un tiempo en llegar hasta el valor pedido, aproximadamente la diferencia
	// desde el valor anterior hasta el valor pedido (nA = mS)

	// uint16_t pause = abs(wichElectrode.nA - wichCurrent);
	delay(pause);
	// wichElectrode.nA = wichCurrent;
}

int16_t testerAlphaDelta::getCurrent(Electrode wichElectrode) {
	
	uint8_t find0 = readPot(wichElectrode.pots[0]);
	uint8_t find1 = readPot(wichElectrode.pots[1]);

	uint16_t i = 0;

	if (wichElectrode.type == ELECTRODE_W) {
		while( (i < totalLevels_W) && (CURRENT_LOOKUP_W [i] [0]!= find0) ) i++;
		while( (i < totalLevels_W) && (CURRENT_LOOKUP_W [i] [1]!= find1) ) i++;
	} else if (wichElectrode.type == ELECTRODE_A) {
		while( (i < totalLevels_A) && (CURRENT_LOOKUP_A [i] [0]!= find0) ) i++;
		while( (i < totalLevels_A) && (CURRENT_LOOKUP_A [i] [1]!= find1) ) i++;
	}

	return i - DELTA_CURRENT;

}