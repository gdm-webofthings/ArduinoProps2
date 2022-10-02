#!/bin/bash
7z a -tzip ArduinoProps2.zip '-xr!*/*.zip/*' '-xr!*.git*' '-xr!DS_Store' '-xr!*/src/ArduinoProps2.ino' '-xr!createzip.sh' '../ArduinoProps2'
