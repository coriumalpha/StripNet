#include <TimerOne.h>
#include "LPD6803.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
int dataPin = 9;       // 'yellow' wire
int clockPin = 6;      // 'green' wire
// Don't forget to connect 'blue' to ground and 'red' to +5V
// Timer 1 is also used by the strip to send pixel clocks

// Set the first variable to the NUMBER of addresses.
LPD6803 strip = LPD6803(50, dataPin, clockPin);

int lastFunction;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0); //Interrupt 0, digital pin 2 on UNO, tested on MEGA.
    
  // The Arduino needs to clock out the data to the pixels
  // this happens in interrupt timer 1, we can change how often
  // to call the interrupt. setting CPUmax to 100 will take nearly all all the
  // time to do the pixel updates and a nicer/faster display, 
  // especially with strands of over 100 dots.
  // (Note that the max is 'pessimistic', its probably 10% or 20% less in reality)
  
  strip.setCPUmax(45);  // start with 50% CPU usage. up this if the strand flickers or is slow. Tested on MEGA, doesn't work quite well on UNO :D
  strip.begin(); // Start up the LED counter
  strip.show(); // Update the strip, to start they are all 'off'

  lastFunction = 0; //Set lastFunction to default value
}


void loop() {


 Serial.println("onLoop");
 if (mySwitch.available())
 {
    int value = mySwitch.getReceivedValue();
    
    if (value == 0)
    {
      Serial.print("Unknown encoding");
    }
    else
    {
      switch(getRemote(value))
      {
        case 0:
        mySwitch.resetAvailable();
          runLastFunction();
          break;
        case 1:
        mySwitch.resetAvailable();
          rainbow(50, true);
          break;
        case 2:
        mySwitch.resetAvailable();
          panWipe(25, 30, 50, true);
          break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        default:
        mySwitch.resetAvailable();
          wipeToBlack(45);
          break;  
      }
    }
  }

}

void runLastFunction()
{
  switch(lastFunction)
  {
    default:
    case 0:
      wipeToBlack(50);
      return colorWipe(Wheel(56), 50); //COLOR PULSE!! :D
    case 1:
      return panWipe(25, 30, 50, true);
  }
}

void rainbow(uint8_t wait, bool infinite) {
  int i, j;
  long t;

  for (j=0; j < 96 * 3; j++) {     // 3 cycles of all 96 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 96));
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
  }
  if(infinite == true)
  {
    return rainbow(wait, infinite);
  }
  return;
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 96 * 5; j++) {     // 5 cycles of all 96 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 96 / strip.numPixels()) + j) % 96) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint16_t c, uint8_t wait) {
  int i, t;
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      t = millis();
      while(millis() - t < wait)
      {
        if(mySwitch.available())
        {
          return;
        }
        //wait
      }
  }
  return; 
}


void panWipe(uint16_t c, uint16_t c2, int wait, bool infinite) {
  int i;
  long t;
  int nPixels = strip.numPixels();

  for(int j = 0; j < nPixels; j++)
  {
    for (i=0; i < (nPixels / 2); i++) {
        strip.setPixelColor(((i + j) % nPixels), Wheel(map(i, 0, (nPixels / 2), c, c2)));
    }
    for (i=(nPixels / 2); i < nPixels; i++) {
        strip.setPixelColor(((i + j) % nPixels), Wheel(map((nPixels - i), 0, (nPixels / 2), c, c2)));
    }
        strip.show();
        t = millis();
        while(millis() - t < wait)
        {
          if(mySwitch.available())
          {
            int receivedKey = getRemote(mySwitch.getReceivedValue());
            
            if(receivedKey == 2) //Speed decrease key
            {
              wait += 5;
              Serial.println(wait);
              mySwitch.resetAvailable();
            }
            
            else if(receivedKey == 3) //Speed increase key
            {
              if(wait > 45)
              {
                wait -= 5;
              }
              Serial.println(wait);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 4) //Color1 increase key
            {
              if(c > 0)
              {
                c -= 1;
              }
              Serial.println(c);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 5) //Color1 decrease key
            {
              if(c < 96)
              {
                c += 1;
              }
              Serial.println(c);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 6) //Color2 increase key
            {
              if(c2 > 0)
              {
                c2 -= 1;
              }
              Serial.println(c2);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 7) //Color2 decrease key
            {
              if(c2 < 96)
              {
                c2 += 1;
              }
              Serial.println(c2);
              mySwitch.resetAvailable();
            }
            
            else if(receivedKey == 1) //Pause key received
            {
                wipeToBlack(50);
                mySwitch.resetAvailable(); //Clean buffer after "Pause" key pressed
                
                while(!mySwitch.available()) //On pause, wait until new key
                {}
                
                while(mySwitch.available()) //Resume when any key (break)
                {
                    mySwitch.resetAvailable();
                    break; 
                }
            }
          }
          //wait
        }
  }

  if(infinite)
  {
  return panWipe(c, c2, wait, infinite);
  }
  return; 
}

void wipeToBlack(uint8_t wait) {
  uint8_t c = 0;
  int i;
  long t;
  for (i=0; i <= (strip.numPixels() / 2); i++) {
      strip.setPixelColor(i, c);
      strip.setPixelColor((strip.numPixels() - i), c);
      strip.show();
      t = millis();
      while(millis() - t < wait)
      {
        //wait
      }
  }
  return; 
}

/* Helper functions */

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}

//Input a value 0 to 127 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(byte WheelPos)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
    case 0:
      r=31- WheelPos % 32;   //Red down
      g=WheelPos % 32;      // Green up
      b=0;                  //blue off
      break; 
    case 1:
      g=31- WheelPos % 32;  //green down
      b=WheelPos % 32;      //blue up
      r=0;                  //red off
      break; 
    case 2:
      b=31- WheelPos % 32;  //blue down 
      r=WheelPos % 32;      //red up
      g=0;                  //green off
      break; 
  }
  return(Color(r,g,b));
}

int getRemote(int value)
{
  switch(value)
      {
        case 5592512:
          return 0;
          break;
        case 5592368:
          return 1;
          break;
        case 5592560:
          return 2;
          break;
        case 5592332:
          return 3;
          break;
        case 5592524:
          return 4;
          break;
        case 5592380:
          return 5;
          break;
        case 5592572:
          return 6;
          break;
        case 5592323:
          return 7;
          break;
        default:
          return 99;
          break;  
      }
}

    
    
