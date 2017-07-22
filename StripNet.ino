#include <TimerOne.h>
#include "LPD6803.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
int dataPin = 9;       // 'yellow' wire
int clockPin = 7;      // 'green' wire
// Don't forget to connect 'blue' to ground and 'red' to +5V
// Timer 1 is also used by the strip to send pixel clocks

// Set the first variable to the NUMBER of addresses.
LPD6803 strip = LPD6803(50, dataPin, clockPin);

int lastFunction;

byte spatialR1, spatialR2, spatialG1, spatialG2, spatialB1, spatialB2;
int default1 = 0;
int default2 = 32;

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
  setGlobalColor(default1, true);
  setGlobalColor(default2, false);
}


void loop() {


 Serial.println( F("onLoop") );
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
          colorWipe(64, 50);
          break;
        case 1:
          mySwitch.resetAvailable();
          rainbow(50, true);
          break;
        case 2:
          mySwitch.resetAvailable();
          spatialMagic(default1, default2, 50, true);
          break;
        case 3:
          mySwitch.resetAvailable();
          panWipe(default1, default2, 50, true);
          break;
        case 4:
          wipeToBlack(45);
          break;
        case 5:
        case 6:
          mySwitch.resetAvailable();
          perejil(50);
          break;
        case 7:
          mySwitch.resetAvailable();
          locateSite(50, true);
          break;
        default:
          mySwitch.resetAvailable();
          wipeToBlack(45);
          break;  
      }
    }
  }

}

/*void runLastFunction()
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
}*/


void perejil(uint8_t wait)
{
  int i;
  unsigned long t;
  
  for (i=0; i < 10; i++)
  {
    strip.setPixelColor(i, Wheel(31));
  }
  for (i=10; i < 13; i++)
  {
    strip.setPixelColor(i, Wheel(44));
  }
  for (i = 13; i < 19; i++)
  {
    strip.setPixelColor(i, Wheel(31));
  }
  for (i = 19; i < 22; i++)
  {
    strip.setPixelColor(i, Wheel(44));
  }
  for (i = 22; i < 40; i++)
  {
    strip.setPixelColor(i, Wheel(31));
  }
  for (i = 40; i < 50; i++)
  {
    strip.setPixelColor(i, 0);
  }  

    strip.show();   // write all the pixels out
    doNotEditThisDangerousFunction();
    t = millis();
    while(true)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
  
}


void locateSite(uint8_t wait, bool infinite)
{
  int i, j;
  unsigned long t;
  for(j = 0; j < 3; j ++)
  {
    //LOOP 1, HIGH
    for (i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(33)); //green?
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait * 2)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
    //LOOP 2, LOW
    for (i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, 0); //black
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait * 2)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
  }
  
  for(j = 0; j < 3; j ++)
  {
    //LOOP 3, HIGH
    for (i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(66)); //green?
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait * 4)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
    //LOOP 4, LOW
    for (i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, 0); //black
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait * 2)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
  }

  for(j = 0; j < 3; j ++)
  {
    //LOOP 5, HIGH
    for (i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(33)); //green?
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait * 2)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
    //LOOP 6, LOW
    for (i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, 0); //black
    } 
    strip.show();   // write all the pixels out
    t = millis();
    while(millis() - t < wait * 2)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
  }


    t = millis();
    while(millis() - t < wait * 10)
    {
      if(mySwitch.available())
      {
        return;
      }
    //wait
    }
  
  if(infinite == true)
  {
    return locateSite(wait, infinite);
  }
  return;

  
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
      strip.setPixelColor(i, Wheel(c));
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

            if(receivedKey == 0) //Speed decrease key
            {
              mySwitch.resetAvailable();
              colorWipe(64, 50);
              return;
            }
            else if(receivedKey == 2) //Speed decrease key
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
//se le va la flapa a partir de 96 por WheelPos>>5
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

void setGlobalColor(byte WheelPos, bool pair)
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
  
  if(pair == true)
  {
    spatialR1 = r;
    spatialG1 = g;
    spatialB1 = b;
  }
  else
  {
    spatialR2 = r;
    spatialG2 = g;
    spatialB2 = b;
  }

  return;
}

void spatialMagic(int c1, int c2, int wait, bool infinite)
{
  int i, leftSize, rightSize;
  long t;
  int nPixels = strip.numPixels();
  int borderOffset = 6;
  int centerOffset = 2;
  leftSize = ((nPixels % 2) == 0) ? (nPixels / 2) : ((nPixels / 2) - 1);
  rightSize = nPixels - leftSize; //may look useless for now
  leftSize = leftSize - borderOffset;
  rightSize = rightSize - borderOffset;

  /* PERSONAL CENTERING AID */
  leftSize += centerOffset;
  rightSize -= centerOffset;

  
  float interval, factor, revFactor;
  byte pr, pg, pb, revert;

  for(int j = 0; j < (nPixels * 2); j++)
  {
      revert = 25;
      for(i=0; i < borderOffset; i++)
      {
        strip.setPixelColor( ((i + revert) % nPixels), Color(spatialR1, spatialG1, spatialB1));
      }
      
      interval = (100.0 / leftSize) / 100;
      for (i=0; i < leftSize; i++)
      {
        factor = (i * interval / 2);
        revFactor = 1.0 - factor;
        
        pr = (float(spatialR1) * revFactor) + (float(spatialR2) * factor);
        pg = (float(spatialG1) * revFactor) + (float(spatialG2) * factor);
        pb = (float(spatialB1) * revFactor) + (float(spatialB2) * factor);
        
        strip.setPixelColor( (((i + borderOffset) + revert) % nPixels), Color(pr, pg, pb) );
        //strip.setPixelColor( (((i + borderOffset) + revert) % nPixels), Color(0, 0, 0) );
      }
      
      interval = (100.0 / rightSize) / 100;
      
      for (i=0; i < rightSize; i++)
      {
        factor = ((rightSize - i) * interval / 2);
        revFactor = 1 - factor;
        
        pr = (float(spatialR1) * factor) + (float(spatialR2) * revFactor);
        pg = (float(spatialG1) * factor) + (float(spatialG2) * revFactor);
        pb = (float(spatialB1) * factor) + (float(spatialB2) * revFactor);
        
        strip.setPixelColor( (((i + borderOffset + leftSize) + revert) % nPixels), Color(pr, pg, pb) );
        //strip.setPixelColor( (((i + borderOffset + leftSize) + revert) % nPixels), Color(0, 0, 0) );
      }
  
      for(i=0; i < borderOffset; i++)
      {
        strip.setPixelColor( (((i + borderOffset + leftSize + rightSize) + revert) % nPixels), Color(spatialR2, spatialG2, spatialB2) );
      }

       
      
    strip.show();
    
    t = millis();
    while(millis() - t < wait)
    {
      if(mySwitch.available())
      {
        int receivedKey = getRemote(mySwitch.getReceivedValue());

        if(receivedKey == 0) //Speed decrease key
        {
          mySwitch.resetAvailable();
          colorWipe(64, 50);
          return;
        }
        else if(receivedKey == 2) //Speed decrease key
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
              if(c1 > 0)
              {
                c1 -= 1;
                setGlobalColor(c1, true);
              }
              Serial.println(c1);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 5) //Color1 decrease key
            {
              if(c1 < 96)
              {
                c1 += 1;
                setGlobalColor(c1, true);
              }
              Serial.println(c1);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 6) //Color2 increase key
            {
              if(c2 > 0)
              {
                c2 -= 1;
                setGlobalColor(c2, false);
              }
              Serial.println(c2);
              mySwitch.resetAvailable();
            }

            else if(receivedKey == 7) //Color2 decrease key
            {
              if(c2 < 96)
              {
                c2 += 1;
                setGlobalColor(c2, false);
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
    return spatialMagic(c1, c2, wait, infinite);
  }
  return;   
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




void doNotEditThisDangerousFunction()
{

 int a, b, c, d, e, f, g;
 int xs, s, m, l, xl;
 int xso, so, mo, lo, xlo;
 int mu;
 float mt;
int i;
 mu = 3; 
 mt = 2.5;
  
 a = mu * 440;
 b = mu * 493;
 c = mu * 523;
 d = mu * 587;
 e = mu * 659;
 f = mu * 698;
 g = mu * 783;
 
 xso = 150;
 so = 250;
 mo = 400;
 lo = 600;
 xlo = 850;

 xs = xso / mt;
 s = so / mt;
 m = mo / mt;
 l = lo / mt;
 xl = xlo / mt;
  for(i = 0; i < 3; i ++)
  {
       tone(6, c);
        delay(m);
        noTone(6);  

        tone(6, e);
        delay(l);
        noTone(6);  

        tone(6, c);
        delay(m);
        noTone(6);  

        tone(6, f);
        delay(l);
        noTone(6);  

        tone(6, c);
        delay(m);
        noTone(6);  

        tone(6, e);
        delay(m);
        noTone(6);  

        tone(6, d);
        delay(m);
        noTone(6);  

        tone(6, c);
        delay(m);
        noTone(6);  

        tone(6, f);
        delay(l);
        noTone(6);
        
        delay(l);
  }
}

    
