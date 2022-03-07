/*
 Triangles.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/

struct Triangles : Program {

  char buffer[BWIDTH];
  
  byte parameter[6];
  
  void setup() {}
  void reset() {}
  
  void draw() {

    display.clear();
    
    for( int i = 0 ; i < 3 ; i++ ) {

      triangle(     0 , WIDTH/2 , map( parameter[ i*2 ] , 0 , 256 , 0 , 40 ) , ch[i] );
      triangle( WIDTH , WIDTH/2 , map( parameter[ i*2 ] , 0 , 256 , 0 , 40 ) , ch[i] );

      copyBuffer( ch[i]->get() , buffer );
      ch[i]->clear();
      copyBuffer( buffer , ch[i]->get() , parameter[ i*2 + 1 ] );       
   }
  }

  void triangle( int begin , int end , int steps , Channel *ch ) {

    int interval = ( end - begin ) / steps;
    interval = abs( interval ) + 1;
    int diff;
  
    diff = frameCount%interval;

    if ( begin < end ) {
      for ( int i = 0 ; i < steps ; i++ ) {
        fillSafe( begin , end , begin + interval * i + diff , begin + interval * i + map( i , 0 , steps, 0 , interval ) + diff , ch );
      }
    } else if( begin > end ) {
      for ( int i = 0 ; i < steps ; i++ ) {
        fillSafe( end , begin , begin - interval * i - diff , begin - interval * i - map( i , 0 , steps, 0 , interval ) - diff , ch );
      }
    } else {
      ch->lineSafe( begin );
    }
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

} triangles;
