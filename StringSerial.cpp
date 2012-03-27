/*

**   STRING-SERIAL   **
** class for Arduino **

Copyright (C) 2012 TONG Haowen Joel

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	@Name		: TONG Haowen Joel
	@Organization	: Singapore Academy for Young Engineers and Scientists (SAYES), Science Centre Singapore
	@Email		: code {at} joeltong {dot} org
	@Language	: Arduino-C
	@Descr		: This code simplies Serial communications using the Arduino using the Xbee radio.  Strings are C++ - style strings, and 				  are terminated by a terminator character (default: '%').
			
			  Works well with either SoftwareSerial class (Arduino IDE ver 1.0 above) or/and hardware Serial UART. 
			  
			  To use, place StringSerial.cpp and StringSerial.h in the directory "<PATH-TO-ARDUINO-IDE-LIBRARY>/StringSerial/".

*/

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






