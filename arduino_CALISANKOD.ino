#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <avr/wdt.h>

SoftwareSerial Arduino_SoftSerial (10,11); // RX,TX
char c;
String dataIn;

String dataTime, dataRelay1Toggle, dataRelay2Toggle;

int8_t indexOfA, indexOfB, indexOfC;
float dataSuSeviye;
//int dataRelay1Durum, dataRelay2Durum;

#define pompaRelay 7
//boolean pompaToggle;
int x = 0, s=16, d=43, p=0, qdakika ;
#define ledRelay 6



float SuSeviyesi=0;
const int trigPin = 2;
const int echoPin = 3;
long sure;
float mesafe;
String SuSeviyeString;
float SuSeviyeFloat;

void setup(void)
{

 
 Serial.begin(57600); //Serial Monitor
 
 Arduino_SoftSerial.begin(9600); // Open serial comm (arduino-nodemcu)

 wdt_disable();
 delay(3000);
 wdt_enable(WDTO_4S);
 Serial.print("WDT Enabled");
 
 pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //timer1 ayarları
  pinMode(pompaRelay, OUTPUT);
  pinMode(ledRelay, OUTPUT);
  digitalWrite(ledRelay,HIGH);
digitalWrite(pompaRelay,HIGH);


   cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
  }

ISR(TIMER1_COMPA_vect){   //timer1 interrupt'ı 1 saniyede bir tetikleniyor.
x++;          
if (x == 60){
  d++;
  x=0;
  p++;
  }                   
if(p==15) p=0;                   
if (d == 60){
  s++;
  d=0;
if(s == 24){
  s = 0; 
  }
} 
wdt_reset();

unsigned long intDataTime = strtol(dataTime.c_str(), NULL, 10);
  time_t t = intDataTime;
  if ( String(year(t)) == "1970"){
     if (p<=5){ 
        digitalWrite(pompaRelay,LOW);
        //pompa ilk başta druyor, if bloğuna girdiğinde çalışıyor. 
        }
     else {
        digitalWrite(pompaRelay,HIGH);//çalışan pompa bu bloğa girdiğinde tekrar duruyor.
        }
     if (21>=s && s>=6)digitalWrite(ledRelay,LOW);
     else if (6>s || s>21)digitalWrite(ledRelay,HIGH);                     
     }
}


                



void loop(void)
{

unsigned long intDataTime = strtol(dataTime.c_str(), NULL, 10);
time_t t = intDataTime;

String date_time=String(day(t))+"/"+String(month(t))+"/"+String(year(t))+" -- "
                  +String(hour(t))+":"+String(minute(t))+":"+String(second(t));
Serial.println(date_time);

      Serial.println("dataTime = " + dataTime);
      Serial.println("dataRelay1Toggle = " + dataRelay1Toggle);
      Serial.println("dataRelay2Toggle = " + dataRelay2Toggle);


String dataSuSeviye = String(SuSeviyeFonksiyonu(),0);      Arduino_SoftSerial.print(dataSuSeviye);    Arduino_SoftSerial.print("D");
String dataRelay1Durum = String(digitalRead(pompaRelay));  Arduino_SoftSerial.print(dataRelay1Durum);   Arduino_SoftSerial.print("E");
String dataRelay2Durum = String(digitalRead(ledRelay));    Arduino_SoftSerial.print(dataRelay2Durum);    Arduino_SoftSerial.print("F");
                                                           Arduino_SoftSerial.print("\n");



      while(Arduino_SoftSerial.available()>0)
    {
      c = Arduino_SoftSerial.read();
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

int saat = String(hour(t)).toInt();
int dakika = String(minute(t)).toInt(); 
int saniye = String(second(t)).toInt();
int yil = String(year(t)).toInt();

int value1 = dataRelay1Toggle.toInt();
int value2 = dataRelay2Toggle.toInt();
Serial.println(value1);
Serial.println(value2);
if (dakika <= 4 || 15 <= dakika <= 19 || 24 <= dakika <= 29 || 35 <= dakika == 39 || 45 <= dakika <= 49 || 55 <= dakika){
  qdakika = 1;
}
else {
  qdakika = 0;
  }
// POMPA RÖLESİ ZAMAN PROGRAMI, 2 DE ZAMAN AYARLI. 0 DA KAPALI 1 DE AÇIK. SU SEVİYESİ 5 CM ALTINDA İSE POMPA DURUR
if (yil != 1970 && value1 == 2 ) {
    if ( SuSeviyeFonksiyonu() >= 5.0 ){
      
        if ( qdakika==1 ){ 
          digitalWrite(pompaRelay,LOW); 
        }
        else digitalWrite(pompaRelay,HIGH);
    }
    else if(yil != 1970 && SuSeviyeFonksiyonu() < 5.0) digitalWrite(pompaRelay,HIGH);
}
else if (yil != 1970 && value1 == 1) digitalWrite(pompaRelay,LOW);
else if (yil != 1970 && value1 == 0) digitalWrite(pompaRelay,HIGH);
// POMPA RÖLESİ ZAMAN PROGRAMI, 2 DE ZAMAN AYARLI. 0 DA KAPALI 1 DE AÇIK. SU SEVİYESİ 5 CM ALTINDA İSE POMPA DURUR

// LED RÖLESİ ZAMAN PROGRAMI, 2 DE ZAMAN AYARLI. 0 DA KAPALI 1 DE AÇIK
if(yil != 1970 && value2 == 2) {
    if (21 >= saat && saat >= 6 ) digitalWrite(ledRelay,LOW); 
    else if  (6 > saat || saat > 21 ) digitalWrite(ledRelay,HIGH);
}  
else if (yil != 1970 && value2 == 1) digitalWrite(ledRelay,LOW);
else if (yil != 1970 && value2 == 0) digitalWrite(ledRelay,HIGH);
//LED RÖLESİ ZAMAN PROGRAMI, 2 DE ZAMAN AYARLI. 0 DA KAPALI 1 DE AÇIK

 Serial.print("Su Seviyesi: ");
 Serial.print(SuSeviyeFonksiyonu(),0);
 Serial.print(" cm");
 Serial.print(" --- Timer Dakika: ");
 Serial.print(d);
 Serial.print(" --- Timer Saat: ");
 Serial.println(s);
 Serial.println(x);
 Serial.println(p);
// float SuSeviyesi= SuSeviyeFonksiyonu();
// if (SuSeviyesi < 5){
// Serial.println("***ALARM! Su seviyesi dusuk, pompa durduruldu***");
//}
Serial.println();
wdt_reset();
delay(2000);
}

void Parse_the_Data()
{
  indexOfA =dataIn.indexOf("A");
  indexOfB =dataIn.indexOf("B");
  indexOfC =dataIn.indexOf("C");

  dataTime = dataIn.substring (0, indexOfA);
  dataRelay1Toggle = dataIn.substring (indexOfA+1, indexOfB);
  dataRelay2Toggle = dataIn.substring (indexOfB+1, indexOfC);
}

float SuSeviyeFonksiyonu() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  // Use 343 metres per second as speed of sound
  mesafe = 25- sure*0.0343/2; 
  //su haznesi yüksekliği 25 cm, sensör 2 cm mesafeden sonra sağlıklı ölçüm yapıyor.
  mesafe = max(mesafe, 0);    // motor yüksekliği 15 cm
  mesafe = min(mesafe, 30);
  return mesafe;             
  }
