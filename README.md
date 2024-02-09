# Aloysius_Room_Automation
In this repo, I log my experience attempting to automate various appliances in my room with:
- [ESP8266 webserver to send commands remotely](#esp8266-webserver)
- [433MHz radio frequency transmitter to communicate with smart light switch](#lights)
- [Arduino nano with 2 channel relay and 433Mhz RF receiver fitted into a KDK standing fan](#fan)
- [Arduino nano with 2 channel relay and 433Mhz RF receiver controlling a linear actuator to operate a sliding door](#door)
- [IR transmitter to communicate with Mitsubishi air conditioner](#air-conditioner)
- [iOS Application with widget to control everything from my phone](#ios-application)

Note that the code for this project is in my private repo as it contains sensitive data

## ESP8266 Webserver

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/6f8f98e0-ea3c-4833-8a9b-41f400491a98.jpg" width="400">

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

Using the [UniversalTelegramBot library](https://www.arduino.cc/reference/en/libraries/universaltelegrambot/), I created a Telegram bot that sends a message to my Telegram account every time the server restarts, so I know that it is active.

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/IMG_3998.jpg" width="400">

## Lights

I replaced the mechanical light switch in my room with a [smart light switch](https://www.aliexpress.com/i/1005004284626383.html) which is controlled via an RF remote. 

I first used an RF receiver to read the signals of the remote to learn the code for turning the light switch on and off.

Next, I soldered an RF transmitter onto the ESP8266 so that it can mimic those signals and control the light switch through the webserver

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/f147612b-6699-4d05-a9d3-0bbe03ce8bdc.jpg" width="300">

```
void handleArg() {
  String message = "";
  String keyStr = String(key);

//Lights
  if (server.arg("lights") == "ON" && server.arg("paramKey") == keyStr) {
    mySwitch.send(5393, 24);
  }
  if (server.arg("lights") == "OFF" && server.arg("paramKey") == keyStr) {
    mySwitch.send(5396, 24);
  }
}
```

## Fan

To communicate with the [standing fan](https://www.kdk.sg/shop/general-fan/stand-fan/p40us.html) I had in my room, I soldered a 2 channel normally open relay and an RF receiver to an Arduino Nano.

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/9c04e48e-e5a8-43cb-91d0-c162031af678.JPG" width="300">

The fan had buttons to select different speeds, so I just desoldered the wires to those buttons and wired them accordingly into the 2 channel relay for 2 different speeds.

When the web server receives a request to turn the fan on to speed 1, it sends an RF signal to the arduino. The arduino turns the appropriate relay into the closed position which turns the fan on.

This was a hard package to fit into the fan's body as there was little space to work with and the Arduino Nano needed a power plug for power. I had to drill 2 holes into the side of the fan body to route the power cable around which looks pretty cool in my opinion. 
<br/>
<br/>
<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/df656e68-9eec-4089-88eb-7868ec05b63f.JPG" width="200" height="325">
<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/4158023f-ea57-4891-ae4f-40bb81f84635.JPG" width="200" height="325">
<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/9b568935-f0f1-4e7a-9ef9-e12634cc407d.JPG" width="240" height="325">
<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/8f764b5a-6402-41ea-b329-5cd7d07ee661.JPG" width="240" height="325">

## Door

To control the sliding door for my room, I bought a 1 meter [linear actuator](https://www.aliexpress.com/item/32994017416.html?srcSns=sns_Copy&spreadType=socialShare&bizTypProductDetail&social_params=6000055555836&aff_fcid=6220632c5d194a48b3acb87fb7b1a12e-1707451589702-03227-_oFqycTc&tt=MG&aff_fsk=_oFqycTc&aff_platform=default&sk=_oFqycTc&aff_trace_key=6220632c5d194a48b3acb87fb7b1a12e-1707451589702-03227-_oFqycTc&shareId=6000055555836&businessType=ProductDetail&platform=AE&terminal_id=c1178bbb82d24e60a75b2a2936b5c051&afSmartRedirect=y) and drilled it onto the wall next to my door.

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/0c26dab2-8f68-4a99-9689-2ad08b214a59.JPG" width="500">

The linear actuator extends when power is supplied to it one way and retracts when the polarity is reveresed, so I wired it to a 2 channel relay accordingly.

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/f87fd2af-f846-4a6d-b077-3978e474cc6b.JPG" width="400">

Controlling the linear actuator is similar to that of the fan where an Arduino Nano with an RF receiver is connected to the relays.

## Air Conditioner

The Misubishi Electric Air Conditioner is controlled by an infrared remote. The signal from the remote can be imitated by an IR transmitter. An NPN MOSFET is used as a switch to drive the IR led. 

The images below shows the final form of the ESP8266 board ft. Darth Maul and Astro Boy.

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/c598e522-511a-47e0-aa75-0c8e1bdc8447%202.jpg" width="400"> <img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/d6937705-9906-43ba-b104-b0a4afd02403%202.jpg" width="400">

I initially tried to decode the signals from the aircon remote using an infrared receiver, but then discovered the [IRremoteESP8266 library](https://github.com/crankyoldgit/IRremoteESP8266/tree/master) which contains the [ir_Mitsubishi library](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/ir_Mitsubishi.h) and decided to use that. 

## iOS Application

Now that the hardware was set up, I needed an app to interact with my web server to control all the appliances from my phone. Since I use an iPhone, I decided to create the app on Xcode with Swift.

I kept the design of the app basic, similar to the iOS settings app. I used switches for the lights, fan and air conditioner and sliders for the fan and door to handle their different states.

The app works by sending a HTTP request to the webserver which corresponds to the button I press, removing the need for me to type ```192.168.1.x/webserver?lights=ON``` into my address bar every time I want to control something.

Since my web server can take multiple arguments at once, I created some basic routines that I do often such as one which turns the air con on, turns the fan off, and closes the door. Pressing this button sends a request similar to <br/>
```192.168.1.x/webserver?AC=ON&fan=OFF&door=CLOSE```

After awhile, it became annoying to have to open an app every time I want to control something. So I decided to create a widget that I can access easily from my phone's lock screen without having to open the app. Haptic feedback is felt on the phone when tapping a button and different opacity levels are used to show the different states of each appliance. Tapping and holding on the fan icon changes the speed of the fan.

<img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/RPReplay_Final1707461829-ezgif.com-video-to-gif-converter.gif" width="400"> <img src="https://github.com/cplAloysius/Aloysius_Room_Automation/blob/main/images/RPReplay_Final17074621522-ezgif.com-video-to-gif-converter.gif" width="400">

Since I do not have an Apple Developer account and would not like to pay $100/year for one, I was only able to load this app onto my iPhone for testing for 1 week at a time. After 1 week, the app will expire and I'd have to load it in again. 

After doing this for awhile, I discovered [Altstore](https://altstore.io) which allowed me to sideload apps into my iPhone without an Apple Developer account.
