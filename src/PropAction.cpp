/*
  Name:    PropAction.cpp
  Author:  Faure Systems <dev at faure dot systems>
           Tim De Paepe <tim.depaepe@arteveldehs.be>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Provide an easy way of triggering functions at a set _interval.
*/

#include "PropAction.h"
#include "arduino.h"

PropAction::PropAction()
{
  _active = true;
  _previous = 0;
  _interval = 400;
  _execute = NULL;
}

PropAction::PropAction(unsigned long intervl, void (*function)())
{
  _active = true;
  _previous = 0;
  _interval = intervl;
  _execute = function;
}

void PropAction::reset(unsigned long intervl)
{
  _active = true;
  _previous = 0;
  _interval = intervl;
}

void PropAction::disable()
{
  _active = false;
}

void PropAction::enable()
{
  _active = true;
}

void PropAction::check() {
  if (_active && (millis() - _previous >= _interval))
  {
    _previous = millis();
    if (_execute) _execute();
  }
}

bool PropAction::tick() {
  if (_active && (millis() - _previous >= _interval))
  {
    _previous = millis();
    return true;
  }
  return false;
}

unsigned long PropAction::getInterval()
{
  return _interval;
}
