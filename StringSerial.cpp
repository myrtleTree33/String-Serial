/* StringSerial.cpp */

#include "Arduino.h"
#include "StringSerial.h"
#include "../SoftwareSerial/SoftwareSerial.h"


StringSerial::StringSerial(unsigned int baud, unsigned long timeOut) {
	_useSoftSerial	= false;
	_delimiter	= ' ';
	_eol		= '%';
	_baud		= baud;
	_timeOut	= timeOut;
}

StringSerial::StringSerial(unsigned int baud, unsigned long timeOut, SoftwareSerial& softSerial) {
	_useSoftSerial	= true;
	_delimiter	= ' ';
	_eol		= '%';
	_baud		= baud;
	_timeOut	= timeOut;
	_softSerial 	= &softSerial;
}

StringSerial::~StringSerial() {}

boolean StringSerial::init() {
	if (_useSoftSerial) {
		_softSerial->begin(_baud);
		return true;
	} else {
		Serial.begin(_baud);
		return true;
	}
	return false;
}


boolean StringSerial::setDelimiter(char val) {
	_delimiter = val;
	return true;
}

boolean StringSerial::setEOL(char val) {
	_eol = val;
	return true;
}


boolean StringSerial::send(String val) {
	if (_useSoftSerial) {
		for (int i = 0; i < val.length(); i++) {
			_softSerial->print(val[i]);
		}
		_softSerial->print(_eol);
		_softSerial->flush();
		
	} else {
		for (int i = 0; i < val.length(); i++) {
			Serial.print(val[i]);
		}
		Serial.print(_eol);
		Serial.flush();
	}
}

String StringSerial::receive() {
	String val = "";
	boolean isTransmitting = false;
	unsigned long startTime = 0;
	unsigned long diff = 0;
	
	if (_useSoftSerial) {						//using software UART
		if (_softSerial->available()) {
			isTransmitting = true;
			startTime = millis();
		}
	  
		while(isTransmitting) {
			diff = millis() - startTime;
		    
			if (diff > _timeOut)				//timeout
				return "TIMEOUT";
		      
			while(_softSerial->available()) {
				char getData = _softSerial->read(); 
				val += getData;
				if (val.endsWith((String) _eol))
					return val;
			}
		}
		return "NULL";
	
	} else {							//hardware UART
		if (Serial.available()) {
			isTransmitting = true;
			startTime = millis();
		}
	  
		while(isTransmitting) {
			diff = millis() - startTime;
		    
			if (diff > _timeOut)				//timeout
				return "TIMEOUT";
		      
			while(Serial.available()) {
				char getData = Serial.read(); 
				val += getData;
				if (val.endsWith((String) _eol))
					return val;
			}
		}
		return "NULL";
	}
}

boolean StringSerial::parse(String val) {
	int counter = 0;
	_header = "";
	_payload = "";
  
	if (val == "NULL" || val == "TIMEOUT") {
		_header = "";
		_payload = "";
		return false;
	}
  
	for (int i = 0; i < val.length(); i++) {
	if (counter == 1) {
		if (val[i] == '%') {
			_payload.trim();
			return true;
		} else
			_payload += val[i];

		} else if (val[i] == ' ') {
			counter++;
		} else {
			_header += val[i];
		}
	}
	return false;
}

String StringSerial::header() {
	return _header;
}

String StringSerial::payload() {
	return _payload;
}






