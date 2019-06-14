//************************************************
//*******************INCLUDE**********************
//************************************************	
#include <HID-Project.h>
#include <HID-Settings.h>
//************************************************
//*******************DEFINE***********************
//************************************************
//#define debug             // Uncomment to enable debug messages via serial

#define 	Apin			2
#define 	Bpin			3
#define 	encSWpin		4

#define 	joySWpin		15
#define 	Xpin			20
#define 	Ypin			21

#define		SW1pin			5
#define		SW2pin			6
#define		SW3pin			7
#define		SW4pin			8
#define		SW5pin			9
#define		SW6pin			10
#define		SW7pin			16
#define		SW8pin			14
#define		SW9pin			15
#define		SW10pin			18
#define		SW11pin			19

#define 	stepPerTrig   	1   // Steps of encoder for change
#define 	changeDir         	// Comment to change direction of rotations
//************************************************
//********************VAR*************************
//************************************************
uint16_t	Xpos 		= 0;
uint16_t	Ypos 		= 0;

uint8_t 	encSW		= 0;
uint8_t 	aData       = 0;
uint8_t 	aLast       = 0;
uint32_t    counter     = 0;
//************************************************
//********************INIT************************
//************************************************
void setup() 
{
	#ifdef debug
		Serial.begin(9600);
	#endif
	// Encoder:
	pinMode(Apin,  INPUT);
	pinMode(Bpin,  INPUT);
	pinMode(encSWpin, INPUT);  
	digitalWrite(Apin,  HIGH);
	digitalWrite(Bpin,  HIGH);
	digitalWrite(encSWpin, HIGH);
	// Joystic:
	pinMode(joySWpin,  INPUT);
  
	digitalWrite(joySWpin,  HIGH);
}
//************************************************
//*******************MAIN*************************
//************************************************
void loop() 
{
	uint8_t encData = 0;
	
	// HMI reading:
	encData = readEncoder();
	encSW   = readSW(encSWpin,encSW); 
	readJoystic();

}

void writeHID(uint16_t key) 
{
	Consumer.write(key);
	return;
}

void readJoystic(void)
{
	Xpos = analogRead(Xpin);
	Ypos = analogRead(Ypin);
	
}

uint8_t readSW(uint8_t pin, uint8_t state)
{
	uint8_t out   = 0;
	uint8_t input = digitalRead(pin);
	if ((input == 0) && (!state))
	{
		#ifdef debug
			Serial.print("Button: Clicked\n");
		#endif
		out = 1;
	}
	if ((input != 0) && (state))
	{
		out = 1;
	}
	
	return out;
}

uint8_t readEncoder(void)
{
	uint8_t out = 0;	// 0 - no data, 1 - up data, 2 - down data
	
	aData = digitalRead(Apin);
	if (aLast != aData)
	{
		if (digitalRead(Bpin) != aData)
		{
			counter++;
		}
		else
		{
			counter--;
		}
	}
	aLast = aData;
	if (counter > stepPerTrig)
	{
		#ifdef changeDir
			out = 2;
		#else
			out = 1;
		#endif
		#ifdef debug
		Serial.print("Encoder: Up\n");
		#endif
		counter = 0;
	}
	if (counter < (0-stepPerTrig))
	{
		#ifdef changeDir
			out = 1;
		#else
			out = 2;
		#endif
		#ifdef debug
			Serial.print("Encoder: Down\n");
		#endif
		counter = 0;
	}
	return out;
}







