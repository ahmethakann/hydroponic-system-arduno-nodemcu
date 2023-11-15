#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <SoftwareSerial.h>
SoftwareSerial NodeMCU_SoftSerial (D1,D2); // RX,TX
char c;
String dataIn;

int8_t indexOfD, indexOfE, indexOfF;
String dataTime, dataRelay1Toggle, dataRelay2Toggle;

String dataSuSeviye;
String dataRelay1Durum, dataRelay2Durum;

#include <ESP8266WiFi.h>


#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
const long utcOffsetInSeconds = 3*3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

#include <OneWire.h> // OneWire kütüphane dosyası çağrıldı
#include <DallasTemperature.h> // DallasTemperature kütüphane dosyası çağrıldı
OneWire oneWire(D5);
DallasTemperature DS18B20(&oneWire);
DeviceAddress DS18B20adres;
float santigrat;
float sicaklik;


#define TdsSensorPin A0
#define VREF 3.3     // analog reference voltage(Volt) of the ADC
#define SCOUNT  5          // sum of sample poin
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0; //temperature = 25;
float ecValue=0;

#define Relay1 D6
int value1;
#define Relay2 D4
int value2;

// Set these to run example.
#define FIREBASE_HOST "hidro-16dfa-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "15iryozvA509dTiYCWsIb1N4LhlY8UNKmhWCGorg"
#define WIFI_SSID "TTNET_AirTies_Air5650_IH7H"
#define WIFI_PASSWORD "19992020"

void setup() {
  Serial.begin(57600); //Serial Monitor on

  NodeMCU_SoftSerial.begin(9600); // Open SoftSerial Arduino-NodeMCU

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

timeClient.begin();

  DS18B20.begin();
 DS18B20.getAddress(DS18B20adres, 0);
 DS18B20.setResolution(DS18B20adres, 12);

  pinMode(TdsSensorPin,INPUT);

 pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  //digitalWrite(Relay1,HIGH);
  //digitalWrite(Relay2,HIGH);
  Firebase.setInt("POMPA ON:OFF",0);        
  Firebase.setInt("LED ON:OFF",0); 
}

int n = 0;

void loop() 
{

timeClient.update();
time_t t=timeClient.getEpochTime();
String datetime=String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" -- "
                  +String(hour(t))+":"+String(minute(t))+":"+String(second(t));
Serial.println(datetime);
Serial.println(t);
    
    dataTime = t;   NodeMCU_SoftSerial.print(dataTime);    NodeMCU_SoftSerial.print("A");
       NodeMCU_SoftSerial.print(dataRelay1Toggle);   NodeMCU_SoftSerial.print("B");
       NodeMCU_SoftSerial.print(dataRelay2Toggle);   NodeMCU_SoftSerial.print("C");
       NodeMCU_SoftSerial.print("\n");

    Serial.println("dataSuSeviye = " + dataSuSeviye);
    Serial.println("dataRelay1Durum = " + dataRelay1Durum);
    Serial.println("dataRelay2Durum = " + dataRelay2Durum);

    while(NodeMCU_SoftSerial.available()>0)
    {
      c = NodeMCU_SoftSerial.read();
      if(c == '\n') {break;}
      else          {dataIn += c;}
      }

    if(c=='\n')
    {
        Parse_the_Data();
        
        //reset variables
        c=0;
        dataIn="";
      }

   dataRelay1Toggle=Firebase.getString("POMPA ON:OFF");
   
 
   dataRelay2Toggle=Firebase.getString("LED ON:OFF");
   
  

float sicaklik = SicaklikFonksiyonu();
float ec = EcMetreFonksiyonu();
float tds = ec*500;

  Serial.print("Sicaklik: ");  
  Serial.println(sicaklik);
  String fireSicaklik = String(datetime)+ " || " + String(sicaklik)+ "  " + String("°C");                   //Humidity integer to string conversion
  
  Serial.print("EC Degeri: ");  
  Serial.println(ec);
  String fireEc = String(datetime)+ " || " + String(ec) + "  " + String("mS/cm");
  
  Serial.print("TDS Degeri: ");  
  Serial.println(tds);
  String fireTds = String(datetime)+ " || " + String(tds) + "  " + String("ppm");


  
  Firebase.pushString("EC Kaydedilen", fireEc);         //setup path to send Temperature readings         
  Firebase.setString("EC Guncel", fireEc);         //setup path to send Temperature readings
  
  Firebase.pushString("TDS Kaydedilen", fireTds);
  Firebase.setString("TDS Guncel", fireTds);           


int dataRelay1DurumInt = dataRelay1Durum.toInt();
int dataRelay2DurumInt = dataRelay2Durum.toInt();

String dataSuSeviyeString = String(datetime)+ " || " + dataSuSeviye + " cm";
Firebase.setString("SU SEVIYESI", dataSuSeviyeString);
// Firebase.pushString("Su Seviyesi Kaydedilen", dataSuSeviyeString);

  if(dataRelay1DurumInt == 1){
  String pompaDuruyor = String(datetime)+ " || " + "Pompa Duruyor"  ;
  Firebase.setString("POMPA DURUM", pompaDuruyor);
  }  
  else if (dataRelay1DurumInt == 0) {
  String pompaCalisiyor = String(datetime)+ " || " + "Pompa Calisiyor"  ;
  Firebase.setString("POMPA DURUM", pompaCalisiyor);
  }

  if(dataRelay2DurumInt == 1){
  String ledDuruyor = String(datetime)+ " || " + "LED Duruyor"  ;
  Firebase.setString("LED DURUM", ledDuruyor);
  }  
  else if (dataRelay2DurumInt == 0) {
  String ledCalisiyor = String(datetime)+ " || " + "LED Çalışıyor"  ;
  Firebase.setString("LED DURUM", ledCalisiyor);
  }
  
Firebase.pushString("Sicaklik Kaydedilen ", fireSicaklik);
Firebase.setString("Sicaklik Guncel", fireSicaklik);
    
delay(3000);
}

void Parse_the_Data()
{
  indexOfD =dataIn.indexOf("D");
  indexOfE =dataIn.indexOf("E");
  indexOfF =dataIn.indexOf("F");

  dataSuSeviye = dataIn.substring (0, indexOfD);
  dataRelay1Durum = dataIn.substring (indexOfD+1, indexOfE);
  dataRelay2Durum = dataIn.substring (indexOfE+1, indexOfF);
}

float SicaklikFonksiyonu() {
  DS18B20.requestTemperatures();
  santigrat = DS18B20.getTempC(DS18B20adres);
  return santigrat;
}

float EcMetreFonksiyonu() {
  static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT + 1;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(santigrat-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      Serial.print("voltage:");
      Serial.print(averageVoltage,2);
      Serial.print("V   ");
      ecValue=tdsValue/500;
        return ecValue;    
 }
}
 int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}
