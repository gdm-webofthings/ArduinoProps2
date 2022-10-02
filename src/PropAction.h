/*
  Name:		 PropAction.h
  Author:  Faure Systems <dev at faure dot systems>
           Tim De Paepe <tim.depaepe@arteveldehs.be>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Provide an easy way of triggering functions at a set interval.
*/

#ifndef PROPACTION_H
#define PROPACTION_H

class PropAction {

  public:
    PropAction();
    PropAction(unsigned long interval, void (*function)());

    void reset(unsigned long interval);
    void disable();
    void enable();
    void check();
    bool tick();

    unsigned long getInterval();

  private:
    bool _active;
    unsigned long _previous;
    unsigned long _interval;
    void (*_execute)();
};

#endif
