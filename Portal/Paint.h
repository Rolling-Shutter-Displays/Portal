/*
 Paint.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/

struct Paint : Program {

  byte parameter[256];
  colour cursorC;
  int pos, oldPos;
  
  void setup() {}
  void reset() {}
  
  void draw() {
    //Copy background
    display.copyBackground();
 
    //Cursor
    display.line( oldPos , cursorC );

    // A border problem, OMG finally!
    
    int diff = pos - oldPos;
    
    Serial.print( oldPos ); Serial.print( "," ); Serial.print( pos ); Serial.println( diff );

    if( parameter[pos] >> 4 ) {
      if( abs( diff ) > WIDTH/4 ) {
        if( oldPos < pos ) {
          display.fillSafe( pos , oldPos , ( parameter[pos]&0x0F ) );
        } else {
          display.fillSafe( oldPos , pos , ( parameter[pos]&0x0F ) );
        }
      } else {
        if( oldPos < pos ) {
          display.fillSafe( oldPos , pos  , ( parameter[pos]&0x0F ) );
        } else {
          display.fillSafe( pos , oldPos , ( parameter[pos]&0x0F ) );
        }
      }
      
    }
    
    //display.fillSafe( 255 , 0 , WHITE );
    cursorC = display.get( pos );
    oldPos = pos;
    ( frameCount%30 > 15 ) ? display.line( pos , WHITE ) : display.line( pos , BLACK );

  }

  // This is a toy data structure, but i like it, is flexible
  unsigned int parameterSize() { 
    return 512; 
  }
  
  void parameterSet( unsigned int param , byte value ) { 
    // This strongly depends on the fact that we know that we are dealing with 2 pots, this is not abstraction. Useful, but it's a headache.
    // Always is different with paint, always.
    // Breake the law, lets bitwise a litle bit.
    pos = param/2;
    
    if( param&1 ) {
      parameter[pos] &= 0xF0; 
      parameter[pos] |= (byte)map( value , 0 , 256 , 0 , 8 ); // LSB : The 8 eight colours 
    } else {
      parameter[pos] &= 0x0F; 
      parameter[pos] |= ( (byte)map( value , 0 , 256 , 0 , 2 ) << 4 ); // MSB : idle or paint
    }
    
  }
  
  byte parameterGet( unsigned int param ) { 
    // This needs to be re-writing, tomorrow
    return parameter[param];
  }
  
  
} paint;
