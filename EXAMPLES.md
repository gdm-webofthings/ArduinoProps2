# ArduinoProps library examples
An adaptation of the internal led Blink example (https://www.arduino.cc/en/tutorial/blink) as a simple MQTT prop.

1. [**BlinkOnWifiProp**](#3-blinkonwifiprop-the-blink-example-on-a-wifi-prop-with-arduinoprops-library): the Blink example on a Wifi prop with *ArduinoProps library*

MQTT messages are received asynchronously therefore to keep the sketch responsive to MQTT commands, calls to delay() should be avoided (except short ones, say < 100 milliseconds).

With *Prop* class, the prop code is more readable because all MQTT *PubSubClient* stuff are hidden in *Prop* calls, therefore the prop code is easier to write and maintain.

Asynchronous-like behavior is obtained using *PropAction*, *TimedAction* or *VariableTimedAction*.

***Copy and change any of these sketches to build your own Arduino connected prop, you will only be limited by your imagination.***

## 1. *BlinkOnWifiProp*: the Blink example on a Wifi prop with *ArduinoProps library*

Sketch with *WifiProp* differs slightly from code with *BridgeProp*.

#### The board WiFiNINA firmware must be recent (> 1.0):
Uupdating WiFiNINA firmware is easy: [WiFiNINA firmware update](help/WifiNinaFirmware.md).


#### WiFi connection is done in loop():
To properly handle WiFi disconnections, the connection state must be tested and managed in the loop:
```csharp
void loop()
{
  if (!wifiBegun) {
    WiFi.begin(ssid, passphrase);
    Serial.println(WiFi.firmwareVersion());
    delay(250); // acceptable freeze for this prop (otherwise use PropAction for async-like behavior)
    // do static IP configuration disabling the dhcp client, must be called after every WiFi.begin()
    String fv = WiFi.firmwareVersion();
    if (fv.startsWith("1.0")) {
      Serial.println("Please upgrade the firmware for static IP");
      // see https://github.com/fauresystems/ArduinoProps/blob/master/WifiNinaFirmware.md
    }
    else {
      //WiFi.config(IPAddress(192, 168, 1, 21), // local_ip
      //	IPAddress(192, 168, 1, 1),  // dns_server
      //	IPAddress(192, 168, 1, 1),  // gateway
      //	IPAddress(255, 255, 255, 0)); // subnet
    }
    if (WiFi.status() == WL_CONNECTED) {
      wifiBegun = true;
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.subnetMask());
      Serial.println(WiFi.gatewayIP());
    } else {
      WiFi.end();
    }
  } else if (wifiBegun && WiFi.status() != WL_CONNECTED) {
    WiFi.end();
    wifiBegun = false;
  }

  prop.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls
}
```

## Author

**Faure Systems**
* company: FAURE SYSTEMS SAS
* web: <a href="https://faure.systems/" target="_blank">Faure Systems</a>