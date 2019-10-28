//************************************************
//*******************INCLUDE**********************
//************************************************	
#include <HID-Project.h>
#include <HID-Settings.h>
//************************************************
//********************DEBUG***********************
//************************************************
//#define debugEncoder
//#define debugJoystic
//#define debugSw

#ifdef debugEncoder
  #define debug
#endif

#ifdef debugJoystic
  #define debug
#endif

#ifdef debugSw
  #define debug
#endif
//************************************************
//*******************PINOUT***********************
//************************************************
#define 	Apin			  2
#define 	Bpin			  3
#define 	encSWpin		4

#define 	joySWpin		7
#define 	Xpin			  9
#define 	Ypin			  8


#define		SW1pin			5
#define		SW2pin			6
#define		SW3pin			10
#define		SW4pin			11
#define		SW5pin			12
/*
#define		SW6pin			10
#define		SW7pin			16
#define		SW8pin			14
#define		SW9pin			15
#define		SW10pin			18
#define		SW11pin			19
*/
//************************************************
//********************SETUP***********************
//************************************************
//ENCODER:
#define 	stepPerTrig   	  1   // Steps of encoder for change
//JOYSTIC:
#define   joysticThreshold  2
// MOVEMENT REVERSE:
#define   joyXrev
//#define   joyYrev
#define   encRev                // Comment to change direction of rotations

// Compas 3D:
// KEY_LEFT_CTRL  + '+'               zoom+
// KEY_LEFT_CTRL  + '-'               zoom-
// KEY_LEFT_SHIFT + KEY_UP_ARROW      zoom+
// KEY_LEFT_SHIFT + KEY_DOWN_ARROW    zoom-
// KEY_LEFT_CTRL  + KEY_LEFT_ARROW    turn left
// KEY_LEFT_CTRL  + KEY_RIGHT_ARROW   turn right
// KEY_LEFT_CTRL  + KEY_UP_ARROW      turn up
// KEY_LEFT_CTRL  + KEY_DOWN_ARROW    turn down
// KEY_LEFT_SHIFT + KEY_LEFT_ARROW    rotate CW
// KEY_LEFT_SHIFT + KEY_RIGHT_ARROW   rotate CCW
//************************************************
//********************VAR*************************
//************************************************
signed short int	  Xpos 		= 0;
signed short int	  Ypos 		= 0;
uint16_t            Xzero   = 0; 
uint16_t            Yzero   = 0;
uint8_t             Xdir    = 0;
uint8_t             Ydir    = 0;

uint8_t 	          encSW		= 0;
uint8_t             encSWst = 0;
uint8_t             joySW   = 0;
uint8_t             joySWst = 0;

uint8_t 	          aData   = 0;
uint8_t 	          aLast   = 0;
signed short int    counter = 0;
//************************************************
//********************INIT************************
//************************************************
void setup() 
{
  #ifdef debug
    initDebug();    // Debug
  #endif
	initEncoder();    // Encoder	
	initJoystic();    // Joystic
  Consumer.begin();
	return;
}
//************************************************
//*******************MAIN*************************
//************************************************
void loop() 
{
	uint8_t encData = 0;
	
	// HMI reading:
	encData = readEncoder();
	encSW = readSW(encSWpin,encSWst);
  joySW = readSW(joySWpin,joySWst);
	readJoystic();
  // HID
  //hidZoom(encData);
  hidRotateX(Xdir);
  hidRotateY(Ydir);
  hidRotateZ(encData);
  return;
}

//************************************************
//********************HID*************************
//************************************************
void hidZoom(uint8_t dir)
{
  if(dir)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    if (dir == 1)
    {
      Keyboard.press(KEY_UP_ARROW);
    }
    else if (dir == 2)
    {
      Keyboard.press(KEY_DOWN_ARROW);
    }
    delay(50);
    Keyboard.releaseAll();
  }
  return;
}

void hidRotateX(uint8_t dir)
{
  if(dir)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    if (dir == 1)
    {
      Keyboard.press(KEY_LEFT_ARROW);
    }
    else if (dir == 2)
    {
      Keyboard.press(KEY_RIGHT_ARROW);
    }
    delay(50);
    Keyboard.releaseAll();
  }
  return;
}

void hidRotateY(uint8_t dir)
{
  if(dir)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    if (dir == 1)
    {
      Keyboard.press(KEY_UP_ARROW);
    }
    else if (dir == 2)
    {
      Keyboard.press(KEY_DOWN_ARROW);
    }
    delay(50);
    Keyboard.releaseAll();
  }
  return;
}

void hidRotateZ(uint8_t dir)
{
  if(dir)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    if (dir == 1)
    {
      Keyboard.press(KEY_LEFT_ARROW);
    }
    else if (dir == 2)
    {
      Keyboard.press(KEY_RIGHT_ARROW);
    }
    delay(50);
    Keyboard.releaseAll();
  }
  return;
}
//************************************************
//******************JOYSTIC***********************
//************************************************
void readJoystic(void)
{
  uint16_t x,y;
	x = analogRead(Xpin);
	y = analogRead(Ypin);
  Xpos = x-Xzero;
  Ypos = y-Yzero;
  //**********************X**********************
  if ((Xpos < joysticThreshold) && (Xpos > -joysticThreshold))
  {
    Xpos = 0;
    Xdir = 0;
  }
  else if (Xpos > 0)
  {
    #ifdef joyXrev
      Xdir = 2;
    #else
      Xdir = 1;
    #endif
  }
  else if (Xpos < 0)
  {
    #ifdef joyXrev
      Xdir = 1;
    #else
      Xdir = 2;
    #endif
  }
  //**********************Y**********************
  if ((Ypos < joysticThreshold) && (Ypos > -joysticThreshold))
  {
    Ypos = 0;
    Ydir = 0; 
  }
  else if (Ypos > 0)
  {
    #ifdef joyYrev
      Ydir = 2;
    #else
      Ydir = 1;
    #endif
  }
  else if (Ypos < 0)
  {
    #ifdef joyYrev
      Ydir = 1;
    #else
      Ydir = 2;
    #endif
  }
 
	#ifdef debugJoystic
    Serial.print("Xpos = ");
    Serial.print(Xpos);
    Serial.print("; Ypos = ");
    Serial.print(Ypos);
    Serial.print("\n");
  #endif
  return;
}

void initJoystic(void)
{
  pinMode(joySWpin,  INPUT);
  digitalWrite(joySWpin,  HIGH);
  Xzero = analogRead(Xpin);
  Yzero = analogRead(Ypin);
}
//************************************************
//***********************SW***********************
//************************************************
uint8_t readSW(uint8_t pin, uint8_t &state)
{
	uint8_t out   = 0;
	uint8_t input = digitalRead(pin);
	if ((input == 0) && (state == 0))
	{
		#ifdef debugSw
			Serial.print("Button: Clicked\n");
		#endif
    state = 1;
		out   = 1;
	}
	else if ((input == 1) && (state == 1))
	{
    state = 0;
	}
  return out;
}
//************************************************
//******************ENCODER***********************
//************************************************
void initEncoder(void)
{
  pinMode(Apin,  INPUT);
  pinMode(Bpin,  INPUT);
  pinMode(encSWpin, INPUT);  
  digitalWrite(Apin,  HIGH);
  digitalWrite(Bpin,  HIGH);
  digitalWrite(encSWpin, HIGH);
  return;
}

uint8_t readEncoder(void)
{
	uint8_t out = 0;	// 0 - no data, 1 - up data, 2 - down data
	signed short int minstp = 0;
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

  minstp = 0-stepPerTrig;
  
	if (counter > stepPerTrig)
	{
		#ifdef encRev
			out = 2;
		#else
			out = 1;
		#endif
		counter = 0;
	}
	else if (counter < minstp)
	{
		#ifdef encRev
			out = 1;
		#else
			out = 2;
		#endif
		counter = 0;
	}
  else
  {
    out = 0;
  }
  #ifdef debugEncoder
    if (out == 1)
    {
      Serial.print("Encoder: UP\n");
    }
    if (out == 2)
    {
      Serial.print("Encoder: DOWN\n");
    }
    #endif
	return out;
}
//************************************************
//************************************************
//************************************************
void initDebug(void)
{
  Serial.begin(9600);
  delay(1000);
  Serial.print("_____________________________________________\n");
  Serial.print("        _____                            \n");
  Serial.print("       (, /   )            ,  /)    /)   \n");
  Serial.print("        _/__ / _  __  _/_    // ___// _  \n");
  Serial.print("        /     (_(_/ (_(___(_/(_(_)(/_(_(_\n");
  Serial.print("     ) /                   /)            \n");
  Serial.print("    (_/                   (/             \n");
  Serial.print("_____________________________________________\n");
  return;
}
