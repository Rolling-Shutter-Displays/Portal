/*
 Utils.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/

void ShiftByOne( int begin , int end , Channel *ch ) {

  if ( begin > end ) { // Shift Right
    int i = end;
    do {
      ( ch->get( i + 1 ) ) ? ch->line( i ) : ch->clear( i ) ;
      i++;
    } while ( i < begin );
    ch->clear( begin );
  }

  if ( begin < end ) { // Shift Left
    int i = end;
    do {
      ( ch->get( i - 1 ) ) ? ch->line( i ) : ch->clear( i ) ;
      i--;
    } while ( i > begin );
    ch->clear( begin );
  }

}

bool ShiftByOne( int begin , int end , Screen *d ) {
  
  //Check boundaries
  if ( ( begin< 0 ) || ( end < 0 ) ) return false;
  if ( ( begin > WIDTH ) || ( end > WIDTH ) ) return false;

  if ( begin > end ) { // Shift Right
    int i = end;
    
    do {
      d->line( i , d->get( i + 1 ) );
      i++;
    } while ( i < begin );
    
    d->clear( begin );
  }

  if ( begin < end ) { // Shift Left
    int i = end;
    
    do {
      d->line( i , d->get( i - 1 ) );
      i--;
    } while ( i > begin );
    
    d->clear( begin );
  }

  return true;
}

void RollOver( int begin , int end , Channel *ch ) {

  if ( begin > end ) { //Shift Right
    int i = end;
    bool _end = ch->get(i);
    do {
      ( ch->get( i + 1 ) ) ? ch->line( i ) : ch->clear( i ) ;
      i++;
    } while ( i < begin );
    _end ? ch->line( begin ) : ch->clear( begin );
  }

  if ( begin < end ) { //Shift Left
    int i = end;
    bool _end = ch->get(i);
    do {
      ( ch->get( i - 1 ) ) ? ch->line( i ) : ch->clear( i ) ;
      i--;
    } while ( i > begin );
    _end ? ch->line( begin ) : ch->clear( begin );
  }
}

void copyBuffer( char *src , char *dst ) {
  for( uint8_t i = 0 ; i < BWIDTH ; i++ ) {
    *( dst + i ) = *( src + i );
  }
}

void copyBuffer( char *src , char *dst , int t ) {
  
  t = t % ( WIDTH + 1 );
  
  for( int _pos = 0 ; _pos < WIDTH ; _pos++ ) {
    
    int p = _pos + t;
    
    if ( *( src + _pos / 8 ) & ( 1 << _pos % 8 ) ) {
      
      if ( ( p ) > WIDTH ) { p = p - WIDTH - 1; }
      
      *( dst + p / 8 ) |= ( 1 << p % 8 );
    }
  }
  
}


// Not sure

bool fillSafe( int y0 , int y1 , int x0 , int x1 , Channel *ch ) { 

  //Check boundaries
  if ( ( y0 < 0 ) || ( y1 < 0 ) ) return false;
  if ( ( y0 > WIDTH ) || ( y1 > WIDTH ) ) return false;
  
  if ( ( x0 < y0 ) && ( x1 < y0 ) ) return false;
  if ( ( x0 > y1 ) && ( x1 > y1 ) ) return false;
        
  if ( x1 > x0 ) {       
    if ( x1 > y1 ) x1 = y1;
    if ( x0 < y0 ) x0 = y0;
    
    do {
      ch->line( x1 );
      x1--;
    } while( x1 > x0 );
            
    ch->line( x0 );
            
    } else if( x1 == x0 ) {    
    
      if ( ( x1 > y0 ) && ( x1 < y1 ) ) ch->line( x1 );
    
  } else {
    if ( x0 > y1 ) x0 = y1;
    if ( x1 < y0 ) x1 = y0;
    
    do {
      ch->line( x0 );
      x0--;
    } while( x0 > x1 );        
    
      ch->line( x1 );
    }
}

bool fillSafe( int y0 , int y1 , int x0 , int x1 , colour c , Screen *d ) { 

  //Check boundaries
  if ( ( y0 < 0 ) || ( y1 < 0 ) ) return false;
  if ( ( y0 > WIDTH ) || ( y1 > WIDTH ) ) return false;
  
  if ( ( x0 < y0 ) && ( x1 < y0 ) ) return false;
  if ( ( x0 > y1 ) && ( x1 > y1 ) ) return false;
        
  if ( x1 > x0 ) {       
    if ( x1 > y1 ) x1 = y1;
    if ( x0 < y0 ) x0 = y0;
    
    do {
      d->line( x1 , c );
      x1--;
    } while( x1 > x0 );
            
    d->line( x0 , c);
            
    } else if( x1 == x0 ) {    
    
      if ( ( x1 > y0 ) && ( x1 < y1 ) ) d->line( x1 , c );
    
  } else {
    if ( x0 > y1 ) x0 = y1;
    if ( x1 < y0 ) x1 = y0;
    
    do {
      d->line( x0 , c );
      x0--;
    } while( x0 > x1 );        
    
      d->line( x1 , c );
    }
}

void dither( int x0 , int x1 , Channel *ch ) {
  if(( x0 > WIDTH )||( x1 > WIDTH )||( x0 < 0 )||( x1 < 0 )) return;
  if( x1 > x0 ) {
    do {
      if( x1&1 ) ch->line( x1 );
      x1--;
    } while( x1 > x0 );

    if( x0&1 ) ch->line( x0 );
  }
}
