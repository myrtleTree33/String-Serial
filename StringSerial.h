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

/* StringSerial.h */

#ifndef StringSerial_h
#define StringSerial_h

#include "Arduino.h"
#include "../SoftwareSerial/SoftwareSerial.h"


class StringSerial {
	public:
		StringSerial(unsigned int baud, unsigned long timeOut);
		StringSerial(unsigned int baud, unsigned long timeOut, SoftwareSerial& softSerial);
		~StringSerial();
		
		boolean init();										//true when initialized
		boolean setDelimiter(char val);								//true if set
		boolean	setEOL(char val);								//true if set
		
		boolean send(String val);								//true when sent
		String receive();									//NULL: nothing | TIMEOUT: packet timed out | <STRING>: success
		boolean parse(String val); 								//true if parsed successfully, false if TIMEOUT or NULL or malformed packet
		
		/* Getters and setters */
		String header();
		String payload();
		
	private:
		SoftwareSerial*		_softSerial;
		boolean			_useSoftSerial;
		
		unsigned int		_baud;
		unsigned long		_timeOut;
			
		String			_header;
		String			_payload;
		char			_delimiter;		//to separate operator from operand
		char			_eol;			//end of line terminating character
		
		
};

#endif
