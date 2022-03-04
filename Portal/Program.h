/*
 Program.h
 Copyright 2022 Facundo Daguerre (derfaq)
 
 This file is part of Portal.
 Portal is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*/

struct Program {

  virtual void setup() {}
  virtual void reset() {}
  // virtual void destroy() {} ? why not, something more dinamic
  // this mean free memory. Critical when uses buffers.
  
  // RSD
  virtual void draw() {}

  // Parameter exposure, for HUI, MIDI, Serial, etc
  // This is a toy data structure
  virtual unsigned int parameterSize() {}
  virtual void parameterSet( unsigned int param , byte value ) {}
  virtual byte parameterGet( unsigned int param ) {}
};
