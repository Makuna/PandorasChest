
const uint32_t c_msBookReleaseTrigger = 2000; // milliseconds it takes to spool
const uint16_t c_servoCenter = DEFAULT_PULSE_WIDTH + 10;

enum BookReleaseState
{
  BookReleaseState_Set,
  BookReleaseState_Triggered,
  BookReleaseState_Resetting,
};



// book release uses a constant rotation servo
// 90 means stop, 0 is fast one direction, 180 is fast in another direction


class BookRelease
{
public:
  BookRelease(uint8_t pin) :
      _pin(pin),
      _state(BookReleaseState_Set)
  {
    
  }

  void begin()
  {
    
  }
  
  void trigger()
  {
    if (_state == BookReleaseState_Set)
    {
      _servo.attach(_pin);
      _servo.write(MAX_PULSE_WIDTH);
      _msActionStartTime = millis(); // capture current time
      _state = BookReleaseState_Triggered;
      Serial.println("triggering");
    }
  }

  void loop()
  {
    // handle book release part of loop
    // consider putting into a class
    if (_state != BookReleaseState_Set)
    {
      uint32_t time = millis();
      
       if (time - _msActionStartTime > c_msBookReleaseTrigger)
       {
         if (_state == BookReleaseState_Triggered)
         {
           Serial.println("resetting");
           _state = BookReleaseState_Resetting;
           _servo.write(MIN_PULSE_WIDTH); // reset
           _msActionStartTime = time; // start timing from here
         }
         else if (_state == BookReleaseState_Resetting)
         {
           Serial.println("set");
           _state = BookReleaseState_Set;
           _servo.write(c_servoCenter); // center for not moving
           _servo.detach(); // stop all motion holding
         }
       }
    }
  }
  
private:
  Servo _servo;
  uint32_t _msActionStartTime;
  BookReleaseState _state;
  const uint8_t _pin;
};

