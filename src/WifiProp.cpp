/*
  Name:    WifiProp.cpp
  Author:  Faure Systems <dev at faure dot systems>
           Tim De Paepe <tim.depaepe@arteveldehs.be>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino with WiFiNINA.
*/

#include "WifiProp.h"

WifiProp::WifiProp(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
  : Prop(client_id, in_box, out_box, broker, port)
{
    _client.setClient(_wifiClient);
}

void WifiProp::begin(void(*on_message)(String))
{
    if (on_message) onInboxMessageReceived = on_message;
}