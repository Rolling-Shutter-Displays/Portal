/*
 Palettes.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/

struct Palette : Program {
  
  byte parameter[8] = { 0 , 255/8 , 255*2/8 , 255*3/8 , 255*4/8 , 255*5/8 , 255*6/8 , 255*7/8 };
  
  char palette[8] = { BLACK , BLUE , RED , MAGENTA , GREEN , CYAN , YELLOW , WHITE };
  
  void setup() {}
  void reset() {}
  
  void draw() {
    for( int i = 0 ; i < 8 ; i++ ) {
      
      palette[i] = (char)map( parameter[i] , 0 , 255 , 0 , 8 );
      
      display.fill( i*WIDTH/8 , ( i + 1 )*WIDTH/8 , palette[i] );
    }
  }

  // This is a toy data structure
  unsigned int parameterSize() { 
    return 8; 
  }
  
  void parameterSet( unsigned int param , byte value ) { 
    parameter[param] = value;
  }
  
  byte parameterGet( unsigned int param ) { 
    return parameter[param];
  }

} palettes;
