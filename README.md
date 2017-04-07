# ESP8266 Nodemcu Leak Sensor with SMS functionality

This project builds a leak sensor using an esp8266 Nodemcu. This sensor will alert the user of any water leaks and report them via PushBullet. The sensor will also check in every 30 days to make sure it's still alive.

## Getting Started

These instructions will provide you with the protoboard schematic, PushBullet configuration, the sketch, and the changes within the sketch you will need to make.

http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/

### Prerequisites

You will need the following environement setup:

#### Software:
1.PushBullet Account

2.Arduino IDE (DHT,ESP8266WiFi, and WiFiClient libraries)

#### Hardware:

1.Nodemcu ESP8266 Dev board

2.Protoboard or breadboard

2.Rain Sensor Rainwater Sensor - any water sensor will do

#### Breadboard Schematic
<a href="http://imgur.com/mi70bTi"><img src="http://i.imgur.com/mi70bTi.png" title="source: imgur.com" /></a>

#### Protoboard Design

### Configure Sketch

Download LeakSensor-GT.ino and open with Arduino. These changes need to be made to the sketch based on your own environment.

#### Configure digital input

```
define LEAK_PIN  5  ------- 5 maps to d01 on the nodemcu
```

#### Configure network information

```
const char* ssid     = "SSID"; ------- Change to your SSID
const char* password = "SECRET"; ------ Your PW
```

#### Configure your check values

```
int leakCheckInterval = 15000; // check for leaks every 15 seconds
int healthCheckInterval = 86400000; // runs a health check every 24 hours so we know we haven't died
```

#### Add your PushBullet API

1.Create a PushBullet account

2.Go to My Account

3.Under `Access Token` click `Create Access Token`

4.Modify sketch to add your api

```
const char* accessToken = "SECRET"; // PushBullet API for account
```

#### Configure POST HTTP body request

You can change the body to be whatever you want but it needs to follow the format below. I have used it to identify the location that 
the water leak has been detected in.

```
String body= "{\"body\":\"Water has been detected by location (Basement Furnace)\",\"title\":\"Water Detected\",\"type\":\"note\"}";

```

Upload the sketch to your esp8266

When configured correctly you should see the following output in your console (ctrl-shift-M)

<a href="http://imgur.com/SAMRSkO"><img src="http://i.imgur.com/SAMRSkO.png" title="source: imgur.com" /></a>

When water is detected you will see the following:

<a href="http://imgur.com/RLiho6g"><img src="http://i.imgur.com/RLiho6g.png" title="source: imgur.com" /></a>

### Configure Grafana Metrics

Your graph should look like this:

<a href="http://imgur.com/Y0EdoW7"><img src="http://i.imgur.com/Y0EdoW7.png" title="source: imgur.com" /></a>

"Humidity" and "Location" can be change to whatever you want when you configure the HTTP Post section of the sketch.


## Built With

* [Arduino](https://www.arduino.cc//) - Arduino IDE
* [Grafana](https://grafana.com/) - Graphing and analytics 
* [Influxdb](https://www.influxdata.com/open-source/#influxdb/) - Time series database

## Authors

* **Pete Sagat** - [psagat](https://github.com/psagat)

## License

This project is licensed under the GNU General Public License

