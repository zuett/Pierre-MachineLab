#include <Servo.h> 


class Flasher
{
	// Class Member Variables
	// These are initialized at startup
	int ledPin;      // the number of the LED pin
	long OnTime;     // milliseconds of on-time
	long OffTime;    // milliseconds of off-time

	// These maintain the current state
	int ledState;             		// ledState used to set the LED
	unsigned long previousMillis;  	// will store last time LED was updated

  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
	ledPin = pin;
	pinMode(ledPin, OUTPUT);     
	  
	OnTime = on;
	OffTime = off;
	
	ledState = LOW; 
	previousMillis = 0;
  }

  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
    	ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }
};

class Sweeper
{
  Servo servo;
  int pos;
  int increment;
  int updateInterval;
  unsigned long lastUpdate;
  int servoPin;
  bool attached;  // keeps track if servo is currently attached

public:
  Sweeper(int interval)
  {
    updateInterval = interval;
    increment = 1;
    pos = 0;
    lastUpdate = 0;
    attached = false;   
  }

  void Attach(int pin)
  {
    servoPin = pin;
    if(!attached)       // only attach if not already attached
    {
      servo.attach(pin);
      attached = true;
    }
  }

  void Detach()
  {
    if(attached)         // only detach if currently attached
    {
      servo.detach();
      attached = false;
    }
  }

  void Update()
  {
    // Read potentiometer for speed
    int potVal = analogRead(A0);
    updateInterval = map(potVal, 0, 1023, 40, 5); // lower interval = faster
    servo.attach(9);

    if((millis() - lastUpdate) > updateInterval)
    {
      lastUpdate = millis();
      pos += increment;
      pos = constrain(pos, 0, 180);   // keep in valid range
      servo.write(pos);

      if ((pos >= 180) || (pos <= 0))
      {
        increment = -increment;
      }
    }
  }
};
 

Flasher led1(13, 200, 222);

Sweeper sweeper1(15);
 
 
void setup()
{
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  sweeper1.Attach(9);
}

void loop()
{
  if(digitalRead(4) == LOW)
  {
    sweeper1.Update();
    led1.Update();
  }
  else
  {
    sweeper1.Detach();
  }
}
