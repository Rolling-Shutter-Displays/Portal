/*
  Portal.ino

  Hardware features:
    - High intensity output light trough 4 RGB LEDs in parallel
    - One white LED (not implemented in this version)
    - Encoder with button for navegate trough baks of parameters and programs
    - Potentiometer for control of the frequency
    - Two potentiometer for control of parameters
    - Free acces to Arduino pin-out for add-ons
    - Disadvantages: outsize, fragile, heavy work to built it.
    - Status: DISCONTINUED

  Programs inclued in this version:
    - Palettes
        + Description: 8 bars of colours across the screen (or camera?)
        + Use: The colour of the 8 bars can be changed moving the encoder and selecting
               the colour of 2 consecutive bars with the potentiometers.
    - Triangles
        + Description: Triangles and colour addition
        + Use: 3 triangles ( red , green and blue ) can be selected using the enconder. The first potentiometer
               control the steps of the shape, the second shift the potition of it.
    - Manantial
        + Description: Source and sink of lines randomly generated  
        + Use: 3 sources can be selected with the encoder. The first potentiometer
               control the colour, the second the probability wich a new line is generated.
    - Paint 
        + Description: For painting with RSD 
        + Use: Use the enconder to move the cursor, the first potentiometer will control if
               paint or not, and the second with wich colour.
    - Cube
        + Description: Animation of a 3D Cube 
        + Use: Control the colour palette with the first potentiomenter,
               and the playback velocity with second.
    - Netflix
        + Description: The netflix presentation in 8 colours and 256 pixels 
        + Use: Control playback velocity with the first potentiometer.

  created 28 Jun 2018 ( Operational - RSD Library 0.1 )
  presented 30 Jun 2018 ( Ciclo Oscilar Vol. 1 -  La Casa del Árbol, Buenos Aires, Arg ) 
            27 Jul 2018 ( A electronic magic night - C Est La Vie, La Plata, Arg )
  develop until 2 Dic 2018
  
  resumed 1 Dic 2021 ( RSD Libray v1.0 - HUI Abstraction )
  resumed 28 Feb 2022 ( Bank of parameters - Program parameterization and abstraction )
  
  published 3 Mar 2022

  Next tasks:
    - documentation ( needed )
    - double-click and long-press implemetantion ( not critical )
    - new versions with new packs of programs    ( not critical )
    
  Facundo Daguerre (derfaq) 2022, CC by-nc-sa.
*/


// RSD Definitions

#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

#define R_PIN  11
#define G_PIN   6
#define B_PIN   5

#define BWIDTH 32              // Byte resolution
#define WIDTH ((BWIDTH*8)-1)   // Line resolution 32*8 = 256 lines, -1 for "safe" 0 included element in array access op

RSD rsd;

//Channel(        pin ,       led mode , byte resolution )
Channel red(    R_PIN , COMMON_CATHODE , BWIDTH );
Channel green(  G_PIN , COMMON_CATHODE , BWIDTH );
Channel blue(   B_PIN , COMMON_CATHODE , BWIDTH );

Channel *ch[3] = { &red , &green , &blue };

// The type of screen: currently only for 1 led RGB, 
// for RGBW and for extend the concept to 2D displays 
// helps is wanted https://github.com/Rolling-Shutter-Displays/RSD
Screen display( &red , &green , &blue );

// Hardware User Interface Definitions
const int pinPote[3] = { A0 , A1 , A2 };
const int pinButton = A3;

#include <OneButton.h> // Thanks
OneButton button( pinButton , true );

#include <Encoder.h> // Thanks
Encoder myEnc( 2 , 3 );

// Programs
#include "Program.h"
#include "Utils.h"
#include "Palettes.h"
#include "Manantial.h"
#include "Paint.h"
#include "Triangles.h"
#include "Player.h"

Program* programs[6] = { &palettes , &triangles , &manantial , &paint , &playerCube , &playerNetflix };
const unsigned int program_size = 6;

int program = 0;

void setup() {
  
  // Setup of the RSD 
  rsd.begin( 30 , BWIDTH );      //begin( frequency , byte resolution )
  
  rsd.attachChannel( &red );     //attachChannel( channel )
  rsd.attachChannel( &green );
  rsd.attachChannel( &blue );

  rsd.attachDraw( draw );        //attachDraw( callback function )

  // Setup of Hardware Interface
  button.attachClick( click );
  button.attachDoubleClick( doubleClick );

  // I couldn't initialize players in other part, i don't like here, but works
  playerCube.r = &cube3D_r[0][0];
  playerCube.g = 0;
  playerCube.b = &cube3D_b[0][0];
  playerCube.parameterColour = true;
  playerCube.size = 60;

  playerNetflix.r = &netflix_r[0][0];
  playerNetflix.g = &netflix_g[0][0];
  playerNetflix.b = &netflix_b[0][0];
  playerNetflix.parameterColour = false;
  playerNetflix.size = 119;

  program = program_size - 1; 
  nextProgram();

  Serial.begin( 9600 );
}

void click() {
  // Program navegator
  nextProgram();
}

void doubleClick() {
  // Frequency navegator
}

void longPress() {
  // Turn-off Turn-on
}

void updateEnc() {
  
  static long oldPosition  = -999;
  long newPosition = myEnc.read()/4;

  if ( newPosition > oldPosition ) {
    nextParameterBank();
  } else if ( newPosition < oldPosition ) {
    prevParameterBank();
  }

  oldPosition = newPosition;
  
}

// Program and parameter navegator

int programParamSize = 0;
int programCurrentParam = 0;

const unsigned  int bankSize = 2;
unsigned int numOfBanks;
unsigned int idxBank;

void nextProgram() {
  
  if ( program < ( program_size - 1 ) ) { program++; } else { program = 0; }
  
  programParamSize = programs[program]->parameterSize();
  idxBank = 0;
  
  numOfBanks = programParamSize/bankSize;
  if( numOfBanks*bankSize < programParamSize ) {
    numOfBanks++;
  }
  
}

void nextParameterBank() {
  if ( idxBank < numOfBanks - 1 ) { idxBank++; } else { idxBank = 0; }

}

void prevParameterBank() {
  if ( idxBank > 0 ) { idxBank--; } else { idxBank = numOfBanks - 1; }
}

void updateBank() {
  
  programCurrentParam = idxBank * bankSize;
  programs[program]->parameterSet( programCurrentParam , analogRead( pinPote[1] )>>2 );
  
  programCurrentParam++;
  if( programCurrentParam < programParamSize ) {
    programs[program]->parameterSet( programCurrentParam , analogRead( pinPote[2] )>>2 );
  }
  
}

void loop() {
  
  // Run the engine
  rsd.update();
  
  // Tuning: Analog way
  float freq = 30.0 - ( ( (float)analogRead( pinPote[0] ) / 1023.0 ) - 0.5 );
  rsd.setFrequency( freq );

  // HUI update
  button.tick();
  updateEnc();
  updateBank();
  
}

void draw(){
 
  // let's draw!
  programs[program]->draw();

  // Serial.println( frameLost );
}
