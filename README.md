# ESP8266 Nodemcu Leak Sensor with SMS functionality

This project builds a leak sensor using an esp8266 Nodemcu. This sensor will alert the user of any water leaks and report them via PushBullet. The
sensor will also check in every 30 days to make sure it's still alive.


<a href="http://imgur.com/EKuIALh"><img src="http://i.imgur.com/EKuIALh.png" title="source: imgur.com" /></a>

## Getting Started

These instructions will provide you with the breadboard schematic, the sketch, and the changes within the sketch you will need to make.
It will NOT go into configuring grafana or influxdb as there are plenty instructions found with a simple google search. It also assumes you have a working IDE for your NodeMCU ESP8266. Instructions on how to configure your ESP can be located here:

http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/

### Prerequisites

You will need the following environement setup:

#### Software:
1.Grafana or other graphing utility

2.Influxdb

3.Arduino IDE (DHT,ESP8266WiFi, and WiFiClient libraries)

#### Hardware:

1.Nodemcu ESP8266 Dev board

2.DHT22 Temp and Humidity Sensor

#### Breadboard Schematic
<a href="http://imgur.com/mi70bTi"><img src="http://i.imgur.com/mi70bTi.png" title="source: imgur.com" /></a>

### Configure Sketch

Download HumditySensor-test-http.ino and open with Arduino. These changes need to be made to the sketch based on your own environment.

#### Configure network information

```
const char* ssid     = "SSID"; ------- Change to your SSID
const char* password = "SECRET"; ------ Your PW
const char* host = "x.x.x.x"; ------ IP address of your influxdb server
```
#### Configure POST HTTP request

```
  String url = "/write?db=Custom";
  Serial.print("requesting URL: "); Serial.println(url);
  String body= "Humidity,location=Humidor value=" + String(h);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Content-length: " + body.length() +"\r\n" 
               "Content-Type: application/x-www-form-urlencoded" +
               "Connection: close\r\n\r\n" +
               body
              );
```
```
  String url = "/write?db=Custom"; <----------- Change db= to the database you want to send the data to (db=mydb)
```
```
  String body= "Humidity,location=Humidor value=" + String(h); <------ You can change the name and the location of the dataset
```

Upload the sketch to your esp8266

When configured correctly you should see the following output in your console (ctrl-shift-M)

<a href="http://imgur.com/db3Rcvc"><img src="http://i.imgur.com/db3Rcvc.png" title="source: imgur.com" /></a>

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

