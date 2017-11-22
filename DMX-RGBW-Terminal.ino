/**
 * DMX RGBW Terminal
 * 
 * bitfasching, 2017
 * License: MIT
 */


// include DMX library
#include <LeoDMX.class.h>


// host communication speed
const unsigned long hostBaud = 115200;

// host data timeout [ms] (gap to identify end of data block)
const unsigned short hostDataGap = 3;

// number of RGBW spots
const unsigned short numSpots = 32;


// get DMX instance
LeoDMX DMX;

// channel data
const unsigned short numBytes = numSpots * 4;
unsigned char channels[numBytes] = {0};


// prepare host communication
void setup()
{
  // wait for host to be ready
  while ( !Serial );
  
  // set up communication
  Serial.begin( hostBaud );
  Serial.setTimeout( hostDataGap );
  
  // say hello
  Serial.println( "Example: R10 G20 B30 W40" );
  
  // reset DMX universe
  DMX.null();
}


// send out data
void loop()
{
  // RGBW colour cache
  static unsigned char red = 0;
  static unsigned char green = 0;
  static unsigned char blue = 0;
  static unsigned char white = 0;

  // send update?
  bool update = true;

  // try to read first byte in serial buffer
  char firstByte = Serial.read();

  switch ( firstByte )
  {
    // set colour component
    case 'R':
    case 'r':
      red = (unsigned char) Serial.parseInt();
      break;
    case 'G':
    case 'g':
      green = (unsigned char) Serial.parseInt();
      break;
    case 'B':
    case 'b':
      blue = (unsigned char) Serial.parseInt();
      break;
    case 'W':
    case 'w':
      white = (unsigned char) Serial.parseInt();
      break;
    // invalid character or nothing to read -> don't update
    default:
      update = false;
  }

  if ( update )
  {
    // loop over spots
    for ( short spot = 0; spot < numSpots; spot++ )
    {
      // repeat RGBW colour
      channels[ spot*4 + 0 ] = red;
      channels[ spot*4 + 1 ] = green;
      channels[ spot*4 + 2 ] = blue;
      channels[ spot*4 + 3 ] = white;
    }
    
    // update universe
    DMX.send( channels, numBytes );

    // log current colour setting
    Serial.print( "R" ); Serial.print( red );
    Serial.print( " G" ); Serial.print( green );
    Serial.print( " B" ); Serial.print( blue );
    Serial.print( " W" ); Serial.println( white );
  }
}
