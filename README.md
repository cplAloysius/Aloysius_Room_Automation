# Aloysius_Room_Automation
In this repo, I log my experience attempting to automate various appliances in my room with:
- [ESP8266 webserver to send commands remotely](#esp8266-webserver)
- 433MHz radio frequency transmitter to communicate with smart light switch
- IR transmitter to communicate with Mitsubishi air conditioner
- Arduino nano with 2 channel relay and 433Mhz RF receiver fitted into a KDK standing fan
- Arduino nano with 2 channel relay and 433Mhz RF receiver controlling a [linear actuator](https://www.aliexpress.com/item/32994017416.html?srcSns=sns_Copy&spreadType=socialShare&bizType=ProductDetail&social_params=6000055555836&aff_fcid=6220632c5d194a48b3acb87fb7b1a12e-1707451589702-03227-_oFqycTc&tt=MG&aff_fsk=_oFqycTc&aff_platform=default&sk=_oFqycTc&aff_trace_key=6220632c5d194a48b3acb87fb7b1a12e-1707451589702-03227-_oFqycTc&shareId=6000055555836&businessType=ProductDetail&platform=AE&terminal_id=c1178bbb82d24e60a75b2a2936b5c051&afSmartRedirect=y) to operate a sliding door
- iOS Application with widget to control everything from my phone

## ESP8266 Webserver

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/6f8f98e0-ea3c-4833-8a9b-41f400491a98.jpg" width="300">

The ESP8266 board connects to my home wifi network using the [ESP8266WiFi Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html) and hosts a web server in that network using the [ESP8266WebServer Library](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)

Once the web server is running, I can send HTTP requests to it by entering its IP address into my browser's address bar followed with some arguments ```192.168.1.x/webserver?lights=ON```

I then handle the request by looking out for those arguments and their values
```
void handleArg() {
  String message = "";
  String keyStr = String(key);

//Lights
  if (server.arg("lights") == "ON" && server.arg("paramKey") == keyStr) {
    Serial.println("lights on");
  }
  if (server.arg("lights") == "OFF" && server.arg("paramKey") == keyStr) {
    Serial.println("lights on");
  }
}
```

## Lights

I replaced the mechanical light switch in my room with a [smart light switch](https://www.aliexpress.com/i/1005004284626383.html) which is controlled via an RF remote. 

I first used an RF receiver to read the signals of the remote to learn the code for turning the light switch on and off.

Next, I soldered an RF transmitter onto the ESP8266 so that it can mimic those signals and control the light switch throgh the webserver


