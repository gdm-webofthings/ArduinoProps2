# *Arduino Prop* sketch
*A look at the Arduino Prop code.*

#### Arduino prop classes
* *Prop* is a base class for the connected prop.

* Class reference
    - [Prop base class](Prop.md)
    - [PropAction class](PropAction.md)
    - [PropData class](PropData.md)

#### Arduino prop program
1. [Define the *Prop*](#1-defines-the-prop)
2. [Setup the *Prop*](#2-setup-the-prop)
    * Change timings
3. [Call in *loop()*](#3-call-in-loop)
4. [Implement `InboxMessage::run()` callback](#4-implement-inboxmessagerun-callback)
6. [Examples](#6-examples)
7. [Compatible](#7-compatible-hardware) hardware


## 1. Defines the *Prop*

### Defining a *WifiProp*
```csharp
#include "ArduinoProps.h"

// Setup your WiFi network
const char* ssid = "";
const char *passphrase = "";

// Builtin led is not available with the shield
#undef LED_BUILTIN
#define LED_BUILTIN 8

// If you're running our Escape Room control software (Room 2.0) you have to respect
// prpos inbox/outbox syntax Room/[escape room name]/Props/[prop name]/inbox|outbox
// https://live-escape.net/go/room

WifiProp prop(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                "192.168.1.42", // your MQTT server IP address
                1883); // your MQTT server port;

PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropDataLogical led(u8"led");

void clignote(); // forward
PropAction clignoteAction = PropAction(1000, clignote);
```
The *AduinoProps* library uses **<a href="https://github.com/arduino-libraries/WiFiNINA" target="_blank">WiFiNINA</a>** WiFi library for **Arduino Uno WiFi Rev 2**, **Arduino NANO 33 IoT**, **Arduino MKR WiFi 1010** and **Arduino MKR VIDOR 4000**.

Please update the WiFiNINA firmware: [WiFiNINA firmware update](WifiNinaFirmware.md).

Create *WifiProp* instance with MQTT parameters, then create *PropData* and *PropAction* instances.

> All I/O pins are available, the WiFi shield is connected to Arduino with SPI port.

### MQTT topics
The prop listens to the *inbox* topic for command messages and send data and messages into the *outbox* topic.

If you're running Live Escape Room 2.0 control software, you have to respect *inbox*/*outbox* syntax:

Room 2.0 *inbox* topic:
```csharp
format:  Room/[escape room name]/Props/[prop name]/inbox
example: u8"Room/Demoniak/Props/Arduino Contrôleur/inbox"
```
Room 2.0 *outbox* topic:
```csharp
format:  Room/[escape room name]/Props/[prop name]/outbox
example: u8"Room/Demoniak/Props/Arduino Contrôleur/outbox"
```
See [README.md](../README.md): **4. Application protocol for escape room 2.0 prop**.

### *PropData* instances
*PropData* provides a facility to monitor data sent into MQTT outbox.

See [PropData.md](PropData.md) for *PropData* reference.

### *PropAction* instances
To maintain a non-blocking asynchronous behavior of your prop, use *PropAction* objects to trigger action periodically.

Reading I/O and simple computing (arithmetics) can be done at every sketch loop but long computing have to run periodically.

For example, to blink a led, don't block the skecth with a `delay(1000)` call to switch the output: instead, ***check if 1 second is elapsed to switch the output***.

See [PropAction.md](PropData.md) for *PropAction* reference.


## 2. Setup the *Prop*
`InboxMessage::run` is the callback executed when a message is received in the MQTT inbox.
```csharp
void setup()
{
  // SAME CODE FOR ALL PROPS
  prop.addData(&clignoter);
  prop.addData(&led);

  prop.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

```

- add *PropData* references to the prop
- call `prop.begin(InboxMessage::run)`

```csharp
void setup()
{
  // can do more static IP configuration
  //WiFi.config(IPAddress(), IPAddress(), IPAddress());
  //WiFi.config(IPAddress(), IPAddress());
  //WiFi.config(IPAddress());

  WiFi.begin(ssid, passphrase);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  ...
}

```

### Change timings
Default timings are:
* check data changes every 400 milliseconds
* *Prop* remains silent for up to 30 seconds

Timings can be changed anywhere in the code with `resetIntervals()`:
```csharp
  prop.resetIntervals(250, 15); // check data changes every 250 milliseconds, silent up to 15 seconds
```


## 3. Call in *loop()*
```csharp
void loop()
{
  prop.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls
}

```
Couldn't be more simple!

Just call `prop.loop()` when entering the sketch `loop()`, it will maintain MQTT broker connection and run callback when receiving MQTT messages.


## 4. Implement `InboxMessage::run()` callback
`InboxMessage::run` is the callback executed when a message is received in the MQTT inbox, you have to implement it.

Example for Room 2.0 commands:
```csharp
void InboxMessage::run(String a) {

  if (a == u8"app:startup")
  {
    prop.sendAllData();
    prop.sendDone(a);
  }
  else if (a == "clignoter:1")
  {
    clignoter.setValue(true);

    prop.sendAllData(); // all data change, we don't have to be selctive then
    prop.sendDone(a); // acknowledge prop command action
  }
  else if (a == "clignoter:0")
  {
    clignoter.setValue(false);

    prop.sendAllData(); // all data change, we don't have to be selctive then
    prop.sendDone(a); // acknowledge prop command action
  }
  else
  {
    // acknowledge omition of the prop command
    prop.sendOmit(a);
  }
}
```

## 6. Examples
The library comes with a number of example sketches. See **File > Examples > ArduinoProps** within the Arduino IDE application.

See [EXAMPLES.md](../EXAMPLES.md) for an adaptation of the Blink example (https://www.arduino.cc/en/tutorial/blink) as a simple MQTT prop:

1. **BlinkOnWifiProps**: the Blink example on a Wifi prop with *ArduinoProps library*

## Author

**Faure Systems**
* company: FAURE SYSTEMS SAS
* mail: *dev at faure dot systems*
* web: <a href="https://faure.systems/" target="_blank">Faure Systems</a>