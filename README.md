# React JSON MQTT Potentiometer
Using React.js to control the brightness of the ESP8266 onboard led,
in this case, with JSON over MQTT.

![Browser Slider](/screenshot.png?raw=true)

## Hardwere
- WIFI Development Board [ESP8266 ESP-12E](https://www.banggood.com/Geekcreit-Doit-NodeMcu-Lua-ESP8266-ESP-12E-WIFI-Development-Board-p-985891.html?rmmds=search&cur_warehouse=USA)

## Documentation

- React documentation [on the website](https://reactjs.org/docs).
- ArduinoJson documentation [on the website](https://arduinojson.org/doc).
- PubSubClient documentation [on the website](https://pubsubclient.knolleary.net/api.html).

##  Installation
npm packages
```
$ cd react_app
$ npm install
$ npm start
```
Open the arduino sketch ([Arduino IDE](https://www.arduino.cc/en/Main/Software))

## Arduino libraries
You will need to install the following Arduino libraries.
*  [ArduinoJson](https://arduinojson.org/doc/installation)
*  [PubSubClient](https://github.com/knolleary/pubsubclient)
