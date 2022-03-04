/*
 Manantial.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/
 
struct Manantial : Program {
  
  byte parameter[6] = { 127 , 127 , 0 , 127 , 0 , 127 };

  void setup() {}
  void reset() {} 
  
  void draw() {

    display.copyBackground();

    display.clear( 0 , 1 );
    display.clear( WIDTH - 1 , WIDTH );
    display.clear( WIDTH/2 - 1 , WIDTH/2 + 2 );

    ShiftByOne( WIDTH/2 + 3 , WIDTH - 2 , &display );
    ShiftByOne( WIDTH/2 - 2 ,         2 , &display );

    colour c = (char)map( parameter[0] , 0 , 255 , 0 , 8 );
    
    if( !(int)random( parameter[1]>>5 ) ) {
      display.line( WIDTH/2 - 2 , c );
      display.line( WIDTH/2 + 3 , c );
    }
    display.line( 1 , c );
    

    c = (char)map( parameter[2] , 0 , 255 , 0 , 8 );
    
    if( !(int)random( parameter[3]>>5 ) ) {
      display.line( WIDTH/2 - 2 , c );
      display.line( WIDTH/2 + 3 , c );
    }
    display.line( WIDTH , c );
    display.line( 0 , c );
    
    c = (char)map( parameter[4] , 0 , 255 , 0 , 8 );
    
    if( !(int)random( parameter[5]>>5 ) ) {
      display.line( WIDTH/2 - 2 , c );
      display.line( WIDTH/2 + 3 , c );
    }
    display.fill( WIDTH/2 , WIDTH/2 + 1 , c );
    
    
  }
  
  // This is a toy data structure
  unsigned int parameterSize() { 
    return 6; 
  }
  
  void parameterSet( unsigned int param , byte value ) { 
    parameter[param] = value;
  }
  
  byte parameterGet( unsigned int param ) { 
    return parameter[param];
  }
  
} manantial;
