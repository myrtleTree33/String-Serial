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
