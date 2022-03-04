/*
 Player.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/

#include "Animations.h"

const PROGMEM colour paletteTwoColors[28][2] = {
  { BLUE , BLACK } , { BLUE , RED } , { BLUE , MAGENTA } , { BLUE , GREEN } , { BLUE , CYAN } , { BLUE , YELLOW } , { BLUE , WHITE } ,
  { RED , BLACK } , { RED , MAGENTA } , { RED , GREEN } , { RED , CYAN } , { RED , YELLOW } , { RED , WHITE } ,
  { MAGENTA , BLACK } , { MAGENTA , GREEN } , { MAGENTA , CYAN } , { MAGENTA , YELLOW } , { MAGENTA , WHITE } ,
  { GREEN , BLACK } , { GREEN , CYAN } , { GREEN , YELLOW } , { GREEN , WHITE } ,
  { CYAN , BLACK } , { CYAN , YELLOW } , { CYAN , WHITE } ,
  { YELLOW , BLACK } , { YELLOW , WHITE } ,
  { WHITE , BLACK }
};

const int palette_size = 28;

struct Player : Program {

  char *r, *g, *b;
  int size;
  bool parameterColour;

  byte parameter[2];

  void setup() {}
  void reset() {}

  void draw() {

    static int frame;
    
    display.clear(); 

    for ( int i = 0 ; i < BWIDTH ; i++ ) {
      if( r ) *( ch[0]->get() + i ) |= pgm_read_byte( ( r + frame * sizeof(char[32]) / sizeof(char) ) + sizeof(char) * i );
      if( g ) *( ch[1]->get() + i ) |= pgm_read_byte( ( g + frame * sizeof(char[32]) / sizeof(char) ) + sizeof(char) * i );
      if( b ) *( ch[2]->get() + i ) |= pgm_read_byte( ( b + frame * sizeof(char[32]) / sizeof(char) ) + sizeof(char) * i );
    }

    // Playback velocity

    frame += (int)( (float)( parameter[1] * 6 ) / 255.0 - 3.0 );

    if ( frame > size - 1 ) frame = 0;
    if ( frame < 0 ) frame = size - 1;

    // Replace color

    if( parameterColour ) {
      int paletteN = 0;
      colour c0, c1;
    
      paletteN = map( parameter[0] , 0 , 256 , 0 , 28 );
      c0 = pgm_read_byte( &paletteTwoColors[ paletteN ][0] );
      c1 = pgm_read_byte( &paletteTwoColors[ paletteN ][1] );
      
      for( int i = 0 ; i <= WIDTH ; i++ ) {
        colour c = display.get( i );
        if( c == RED  ) { display.line( i , c0 ); }
        if( c == BLUE ) { display.line( i , c1 ); }
        if( c == MAGENTA ) { display.line( i , c1 ); } // Overlap
      }

    }

  }

  // This is a toy data structure
  unsigned int parameterSize() {
    return 2;
  }

  void parameterSet( unsigned int param , byte value ) {
    parameter[param] = value;
  }

  byte parameterGet( unsigned int param ) {
    return parameter[param];
  }

} playerCube, playerNetflix;
