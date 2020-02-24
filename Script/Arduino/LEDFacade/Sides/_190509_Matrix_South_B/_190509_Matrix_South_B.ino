//--------------------LIBRARIES--------------------
#include <WiFi101.h>
#include <MQTTClient.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

#define PIN1 6                                                          //define pin 15 for Neopixel LED
#define PIN2 7
#define PIN3 4
#define PIN4 5
#include <Adafruit_NeoPixel.h>                                          //download library "Adafruit Neopixel"
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <ArduinoJson.h>                                                //download library "ArduinoJson"

const char* ssid = "digital";
const char* pass = "reciprocity";



//--------------------LED COLOR VALUES--------------------
uint8_t colorMax[3] = {255, 0, 0};                                      //warmer color temperature
uint8_t colorMid[3] = {155, 0, 155};                                    //equal color temperature
uint8_t colorMin[3] = {0, 0, 255};                                     //cooler color temperature
uint8_t colorWindMax[3] = {255,255,255};
uint8_t colorT[3];                                                      //current LED color
uint8_t colorT01[3];                                                    //color for HUZZAH_01 Bottom Left
uint8_t colorT02[3];                                                    //color for HUZZAH_02 Top Left
uint8_t colorT03[3];                                                    //color for HUZZAH_03 Bottom Right
uint8_t colorT04[3];                                                    //color for HUZZAH_04 Top Right
uint8_t colorT01a[3];
uint8_t colorT02a[3];
uint8_t colorT03a[3];
uint8_t colorT04a[3];
uint8_t colorT01b[3];
uint8_t colorT02b[3];
uint8_t colorT03b[3];
uint8_t colorT04b[3];
uint8_t lastColorT01[3];
uint8_t lastColorT02[3];
uint8_t lastColorT03[3];
uint8_t lastColorT04[3];

//--------------------ADJUSTABLE VARIABLES--------------------
const bool invertX = true;
const bool invertY = false;
const int glowfade = 3;                                                 //determines how fast the color changes
int maxtD = 10;                                                         //temperature range
int maxwD = 10;                                                         //humidity range
double OWM_Temperature = 64;                                            //OWM temperature
double OWM_Humidity = 0;                                                //OWM humidity
double wind_scale = 100;                                                //windspeed scale from 100

float temperature_percentage;                                           //percentage of temperatureD of maxtD
float wind_percentage = 0;                                              //percentage of windD of maxwD
float humid_percentage = 0;

float RevP_Wind_01 = 0;                                                 //RevP sensor wind
float RevP_Temperature_01 = 0;                                          //RevP sensor temperature
float AM2320_Humidity_01 = 0;
float RevP_Wind_02 = 0;                                                 //RevP sensor wind
float RevP_Temperature_02 = 0;                                          //RevP sensor temperature
float AM2320_Humidity_02 = 0;
float RevP_Wind_03 = 0;                                                 //RevP sensor wind
float RevP_Temperature_03 = 0;                                          //RevP sensor temperature
float AM2320_Humidity_03 = 0;
float RevP_Wind_04 = 0;                                                 //RevP sensor wind
float RevP_Temperature_04 = 0;                                          //RevP sensor temperature
float AM2320_Humidity_04 = 0;

float temperatureD_01 = 0;                                                 //temperature difference
float windD_01 = 0;                                                        //wind speed difference
float humidD_01 = 0;
float temperatureD_02 = 0;                                                 //temperature difference
float windD_02 = 0;                                                        //wind speed difference
float humidD_02 = 0;
float temperatureD_03 = 0;                                                 //temperature difference
float windD_03 = 0;                                                        //wind speed difference
float humidD_03 = 0;
float temperatureD_04 = 0;                                                 //temperature difference
float windD_04 = 0;                                                        //wind speed difference
float humidD_04 = 0;

//--------------------CONSTANT VARIABLES--------------------
const char* SHIFTR_OWM_Temperature = "/OWM_Temperature";                //incoming messages from shiftr.io
const char* SHIFTR_OWM_Humidity = "/OWM_Humidity";                      //incoming messages from shiftr.io
const char* SHIFTR_MKR_01_Temperature = "/01_MKR_Temperature";          //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_01_Wind = "/01_MKR_Wind";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_01_Humidity = "/01_MKR_Humidity";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_02_Temperature = "/06_MKR_Temperature";          //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_02_Wind = "/06_MKR_Wind";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_02_Humidity = "/06_MKR_Humidity";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_03_Temperature = "/04_MKR_Temperature";          //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_03_Wind = "/04_MKR_Wind";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_03_Humidity = "/04_MKR_Humidity";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_04_Temperature = "/06_MKR_Temperature";          //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_04_Wind = "/06_MKR_Wind";                        //Arduino MKR1000 incoming messages to shiftr.io
const char* SHIFTR_MKR_04_Humidity = "/06_MKR_Humidity";                        //Arduino MKR1000 incoming messages to shiftr.io
//const char* SHIFTR_MKR_DT = "/MKR_DT";    

const int pause = 1000;                                                  //delay amount in milliseconds
const int LEDSS = 4;                                                    //Number of LED strips
const int LEDSP = 87;                                                   //Number of LED Pixels on each strip

//--------------------STATIC--------------------
int t = 0;
int lastT = 0;
int LEDR[LEDSS][LEDSP];
int LEDG[LEDSS][LEDSP];
int LEDB[LEDSS][LEDSP];
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(LEDSP, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(LEDSP, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(LEDSP, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(LEDSP, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripes[LEDSS];// = {};

WiFiClient net;
MQTTClient client;

//--------------------SETUP--------------------
void setup() {

  Serial.begin(115200);

  //--------------------WIFI--------------------
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  Serial.println("\nsetup complete!");
  Serial.println();

  //--------------------LED STRIP--------------------
  //stripes[0] = strip1;
  //stripes[1] = strip2;
  //stripes[2] = strip3;
  //stripes[3] = strip4;
  pinMode(LED_BUILTIN, OUTPUT);
  strip1.begin(); strip2.begin(); strip3.begin(); strip4.begin();
  strip1.show(); strip2.show(); strip3.show(); strip4.show();

  //ARRAY SETUP
  for (int j = 0; j < LEDSS; j++){
    for (int i = 0; i < LEDSP; i++){
      LEDR[j][i] = 0;
      LEDG[j][i] = 0;
      LEDB[j][i] = 0;
    }
  }

  //initialize last color
  lastColorT01[0] = 1; lastColorT01[1] = 1;lastColorT01[2] = 1;
  lastColorT02[0] = 1; lastColorT02[1] = 1;lastColorT02[2] = 1;
  lastColorT03[0] = 1; lastColorT03[1] = 1;lastColorT03[2] = 1;
  lastColorT04[0] = 1; lastColorT04[1] = 1;lastColorT04[2] = 1;
}

//--------------------LOOP--------------------
void loop() {
  client.loop();
  if (!client.connected()) 
  {
    connect(SHIFTR_OWM_Temperature, SHIFTR_OWM_Humidity, SHIFTR_MKR_01_Temperature, SHIFTR_MKR_01_Wind, SHIFTR_MKR_01_Humidity, SHIFTR_MKR_02_Temperature, SHIFTR_MKR_02_Wind, SHIFTR_MKR_02_Humidity, SHIFTR_MKR_03_Temperature, SHIFTR_MKR_03_Wind, SHIFTR_MKR_03_Humidity, SHIFTR_MKR_04_Temperature, SHIFTR_MKR_04_Wind, SHIFTR_MKR_04_Humidity);
  }
  t = millis();
  if (t - lastT > pause) //TIME
  {
    lastT = t;
    
    //--------------------TEMPERATURE--------------------
    temperatureD_01 = RevP_Temperature_01 - OWM_Temperature;
    temperaturecolor(temperatureD_01, colorT01a);
    //client.publish(String(temperatureD_01));
    Serial.print("colorT01a R: ");Serial.print(colorT01a[0]);Serial.print(", colorT01a G: ");Serial.print(colorT01a[1]);Serial.print(", colorT01a B: ");Serial.println(colorT01a[2]);
    Serial.println();
    temperatureD_02 = RevP_Temperature_02 - OWM_Temperature;
    temperaturecolor(temperatureD_02, colorT02a);
    temperatureD_03 = RevP_Temperature_03 - OWM_Temperature;
    temperaturecolor(temperatureD_03, colorT03a);
    temperatureD_04 = RevP_Temperature_04 - OWM_Temperature;
    temperaturecolor(temperatureD_04, colorT04a);

    windcolor(RevP_Wind_01, colorT01a, colorT01b);
    Serial.print("colorT01 R: "); Serial.print(colorT01[0]); Serial.print(", colorT01 G: ");Serial.print(colorT01[1]);Serial.print(", colorT01 B: ");Serial.println(colorT01[2]);
    Serial.println();
    windcolor(RevP_Wind_02, colorT02a, colorT02b);
    windcolor(RevP_Wind_03, colorT03a, colorT03b);
    windcolor(RevP_Wind_04, colorT04a, colorT04b);

    humidcolor(AM2320_Humidity_01, colorT01b, colorT01);
    humidcolor(AM2320_Humidity_02, colorT02b, colorT02);
    humidcolor(AM2320_Humidity_03, colorT03b, colorT03);
    humidcolor(AM2320_Humidity_04, colorT04b, colorT04);
    
    uint8_t fadecolor[3];

    //glowfade
    //lastColorT0# is a global color that keeps track of the last color set outside of the loop. this means it is staying constant between cycles
    //we are using the two colorfade to go between the last color and the newest temp color, the last color is updated each loop
    //so the last color is in a way updating itself, which creates a more gradual fade in
    twocolorfade(glowfade, lastColorT01, colorT01, lastColorT01);
    twocolorfade(glowfade, lastColorT02, colorT02, lastColorT02);
    twocolorfade(glowfade, lastColorT03, colorT03, lastColorT03);
    twocolorfade(glowfade, lastColorT04, colorT04, lastColorT04);
    
    for (int j = 0; j < LEDSS; j++) //what strip
    {

      double x;
      x = (j*1.000/(LEDSS-1))*100;

      for(int i=0; i<LEDSP; i++) //what pixel
      { 
        double y;
        if(invertY) //testing if invertY is enabled
        {
          y = 1.000-((i*1.000/(LEDSP-1))*100); //we flip the y value here so it acts as if it is counting down instead of up
        }
        else
        {
          y = (i*1.000/(LEDSP-1))*100;
        }

        //fourcolorfade(x,y,colorT01, colorT02, colorT03, colorT04, fadecolor); //non gable
        fourcolorfade(x,y,colorT01, colorT02, colorT03, colorT02, fadecolor); //gable
        LEDR[j][i] = constrain(fadecolor[0], 0, 255);
        LEDG[j][i] = constrain(fadecolor[1], 0, 255);
        LEDB[j][i] = constrain(fadecolor[2], 0, 255);

        if (j == 0)
        {
          if(invertX) //we are testing the bool value for invert X and just flipping which LED line gets set
          {
            strip4.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);
            strip4.show();
          }
          else
          {
            strip1.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);
            strip1.show();
          }
        }
       
        if (j == 1)
        {
          if(invertX)
          {
            strip3.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);
            strip3.show();
          }
          else
          {
            strip2.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);
            strip2.show();
          }
        }
        if (j == 2)
        {
          if(invertX)
          {
            strip2.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);      //[i+numpix to skip] for doorway. nix if running full strip.
            strip2.show();
          }
          else
          {
            strip3.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);      //[i+numpix to skip] for doorway. nix if running full strip.
            strip3.show();
          }
        }
        if (j == 3)
        {
          if(invertX)
          {
            strip1.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);       //[i+numpix to skip] for doorway. nix if running full strip.
            strip1.show();
          }
          else
          {
            strip4.setPixelColor(i, LEDR[j][i], LEDG[j][i], LEDB[j][i]);       //[i+numpix to skip] for doorway. nix if running full strip.
            strip4.show();
          }
        }
        
      }//FOR I
    }//FOR J
    
  }//TIME  
}//LOOP


//SHIFTR.IO UPDATES
void messageReceived(String topic, String payload, char * bytes, unsigned int length)
{
  //PARSE CLIENT
  if (topic == "/OWM_Temperature")
  {
    OWM_Temperature = payload.toFloat();
    Serial.print("/OWM_Temperature: ");
    Serial.println(OWM_Temperature);
  }
  if (topic == "/OWM_Humidity")
  {
    OWM_Humidity = payload.toFloat();
    Serial.print("/OWM_Humidity: ");
    Serial.println(OWM_Humidity);
    //humidity_scale = 100 - OWM_Humidity;
  }
  if (topic == "/01_MKR_Temperature")
  {
    RevP_Temperature_01 = payload.toFloat();
    Serial.print("01_MKR_Temperature: ");
    Serial.println(RevP_Temperature_01);
  }
  if (topic == "/01_MKR_Wind")
  {
    RevP_Wind_01 = payload.toFloat();
    Serial.print("01_MKR_Wind: ");
    Serial.println(RevP_Wind_01);
  }
   if (topic == "/01_MKR_Humidity")
  {
    AM2320_Humidity_01 = payload.toFloat();
    Serial.print("01_MKR_Humidity: ");
    Serial.println(AM2320_Humidity_01);
  }
  if (topic == "/06_MKR_Temperature")
  {
    RevP_Temperature_02 = payload.toFloat();
    Serial.print("06_MKR_Temperature: ");
    Serial.println(RevP_Temperature_02);
  }
  if (topic == "/06_MKR_Wind")
  {
    RevP_Wind_02 = payload.toFloat();
    Serial.print("06_MKR_Wind: ");
    Serial.println(RevP_Wind_02);
  }
    if (topic == "/06_MKR_Humidity")
  {
    AM2320_Humidity_02 = payload.toFloat();
    Serial.print("06_MKR_Humidity: ");
    Serial.println(AM2320_Humidity_02);
  }
  if (topic == "/04_MKR_Temperature")
  {
    RevP_Temperature_03 = payload.toFloat();
    Serial.print("04_MKR_Temperature: ");
    Serial.println(RevP_Temperature_03);
  }
  if (topic == "/04_MKR_Wind")
  {
    RevP_Wind_03 = payload.toFloat();
    Serial.print("04_MKR_Wind: ");
    Serial.println(RevP_Wind_03);
  }
    if (topic == "/04_MKR_Humidity")
  {
    AM2320_Humidity_03 = payload.toFloat();
    Serial.print("04_MKR_Humidity: ");
    Serial.println(AM2320_Humidity_03);
  }
  if (topic == "/06_MKR_Temperature")
  {
    RevP_Temperature_04 = payload.toFloat();
    Serial.print("06_MKR_Temperature: ");
    Serial.println(RevP_Temperature_04);
  }
  if (topic == "/06_MKR_Wind")
  {
    RevP_Wind_04 = payload.toFloat();
    Serial.print("06_MKR_Wind: ");
    Serial.println(RevP_Wind_04);
  }
    if (topic == "/06_MKR_Humidity")
  {
    AM2320_Humidity_04 = payload.toFloat();
    Serial.print("06_MKR_Humidity: ");
    Serial.println(AM2320_Humidity_04);
  }

 //OWM_Temperature = 64;     // <-- COMMENT when running "owm.js" JavaScript
 //OWM_Humidity = 0;         // <-- COMMENT when running "owm.js" JavaScript
  /*RevP_Temperature_02 = 102;  //dummy reading
  RevP_Temperature_03 = 72;  //dummy reading
  RevP_Temperature_04 = 94;  //dummy reading
  RevP_Wind_02 = 0;         //dummy reading
  RevP_Wind_03 = 0;         //dummy reading
  RevP_Wind_04 = 0;         //dummy reading*/
  
}
