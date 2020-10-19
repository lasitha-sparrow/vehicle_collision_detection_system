#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char MESSAGE[200];
char lat[12];
char lon[12];
char webID[10] = "0711331097";
char webID1[10] = "0711954433";
char webID2[10] = "0714431192";
char webID3[10] = "0728460636";
char webID4[10] = "0714387965";

int preasurePin = A0;
int force;

int flamePin = A1;
int flame;
char flam[7];

char date[12];
char tim[10];

char phone[16];
char pnum[16];
char datetime[24];

float la,lo,ws;

int y,m,d,h,mi,s;

#define PIN_TX 52
#define PIN_RX 53
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,

void sendSMS(int c, char Phone[16]);
void getGPS();
void readSMS();

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);

  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      Serial.print("Sim808 init error\r\n");
      delay(1000);
  }
  delay(3000);

  while(!sim808.attachGPS())
  {
    Serial.println("Open the GPS power failure");
    delay(1000);
  }
  delay(3000);

  Serial.println("Open the GPS power success");

  Serial.println("SIM Init success");
      
  Serial.println("Init Success, please send SMS message to me!");

  digitalWrite(13,LOW);
  
}

void loop()
{
  //*********** Detecting unread SMS ************************
   messageIndex = sim808.isSMSunread();
   
   force = analogRead(preasurePin);
   
   flame = analogRead(flamePin);

   if(flame <= 75)
   {
     sprintf(flam, "True");
   }
   else
   {
     sprintf(flam, "False");
   }
  
   //*********** At least, there is one UNREAD SMS ***********
   if (messageIndex > 0)
   {
      digitalWrite(13,HIGH); 

      readSMS();

      if(strcmp(message, webID) == 0)
      {
        getGPS();
        
        sendSMS(1, webID);
        sendSMS(1, webID1);
        sendSMS(1, webID2);
        sendSMS(1, webID3);
        sendSMS(1, webID4);
      }
      else if(message[0]=='0' && message[1]=='7')
      {
        getGPS();

        sendSMS(1, webID);        
        sendSMS(1, webID1);
        sendSMS(1, webID2);
        sendSMS(1, webID3);
        sendSMS(1, webID4);

        sendSMS(2, message);
      }
      else
      {
        getGPS();
        
        sendSMS(1, webID);
        sendSMS(1, webID1);
        sendSMS(1, webID2);
        sendSMS(1, webID3);
        sendSMS(1, webID4);
        
        sendSMS(2, phone);
        
      }
      
      //************* Turn off the GPS power ************
      sim808.detachGPS();

      Serial.println("Please send SMS message to me!");

      digitalWrite(13,LOW);
   }

   if(force >= 10 || flame <= 75)
   {
      digitalWrite(13,HIGH);
      
      Serial.println(force);
      Serial.println(flam);
      
      getGPS();
      
      //sendSMS("0711954433");
      //sendSMS("0711331097");
  
      sendSMS(3, webID);      
      sendSMS(3, webID1);
      sendSMS(3, webID2);
      sendSMS(3, webID3);
      sendSMS(3, webID4);

      //************* Turn off the GPS power ************
      sim808.detachGPS();
      Serial.println("Turn Off The GPS Power!");

      Serial.println("Please send SMS message to me!");

      digitalWrite(13,LOW);
   }

  force = 0;
  flame = 0;
  flam[7] = "";
}

void readSMS()
{
  Serial.print("messageIndex: ");
  Serial.println(messageIndex);
  
  sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
             
  //***********In order not to full SIM Memory, is better to delete it**********
  sim808.deleteSMS(messageIndex);
  Serial.print("From number: ");
  Serial.println(phone);  
  Serial.print("Datetime: ");
  Serial.println(datetime);        
  Serial.print("Recieved Message: ");
  Serial.println(message);
}

void getGPS()
{ 
  while(!sim808.attachGPS())
  {
    Serial.println("Open the GPS power failure");
    delay(1000);
  }
  delay(3000);

  Serial.println("Open the GPS power success");
    
  while(!sim808.getGPS())
  {
    
  }

  Serial.print(sim808.GPSdata.year);
  Serial.print("/");
  Serial.print(sim808.GPSdata.month);
  Serial.print("/");
  Serial.print(sim808.GPSdata.day);
  Serial.print(" ");
  Serial.print(sim808.GPSdata.hour);
  Serial.print(":");
  Serial.print(sim808.GPSdata.minute);
  Serial.print(":");
  Serial.print(sim808.GPSdata.second);
  Serial.print(":");
  Serial.println(sim808.GPSdata.centisecond);
  Serial.print("latitude :");
  Serial.println(sim808.GPSdata.lat);
  Serial.print("longitude :");
  Serial.println(sim808.GPSdata.lon);
  Serial.print("speed_kph :");
  Serial.println(sim808.GPSdata.speed_kph);
  Serial.print("heading :");
  Serial.println(sim808.GPSdata.heading);
  Serial.println();

  la = sim808.GPSdata.lat;
  lo = sim808.GPSdata.lon;
  ws = sim808.GPSdata.speed_kph;

  y = sim808.GPSdata.year;
  m = sim808.GPSdata.month;
  d = sim808.GPSdata.day;

  h = sim808.GPSdata.hour;
  mi = sim808.GPSdata.minute;
  s = sim808.GPSdata.second;

  dtostrf(la, 4, 6, lat); //put float value of la into char array of lat. 6 = number of digits before decimal sign. 2 = number of digits after the decimal sign.
  dtostrf(lo, 4, 6, lon); //put float value of lo into char array of lon
  
  sprintf(date, "%d/%d/%d", m,d,y);
  sprintf(tim, "%d:%d:%d", h,mi,s);
  
}

void sendSMS(int c, char Phone[16])
{
  
  if(Phone[0]=='0' && Phone[1]=='7' && Phone[2]=='1' && Phone[3]=='1' && Phone[4]=='3' && Phone[5]=='3' && Phone[7]=='0' && Phone[8]=='9' && Phone[9]=='7')
  {
    pnum[0] = '0';
    pnum[1] = '7';
    pnum[2] = '1';
    pnum[3] = '1';
    pnum[4] = '3';
    pnum[5] = '3';
    pnum[6] = '1';
    pnum[7] = '0';
    pnum[8] = '9';
    pnum[9] = '7';    
  }
  else if(Phone[0]=='0' && Phone[1]=='7' && Phone[2]=='1' && Phone[3]=='1' && Phone[4]=='9' && Phone[5]=='5' && Phone[7]=='4' && Phone[8]=='3' && Phone[9]=='3')
  {
    pnum[0] = '0';
    pnum[1] = '7';
    pnum[2] = '1';
    pnum[3] = '1';
    pnum[4] = '9';
    pnum[5] = '5';
    pnum[6] = '4';
    pnum[7] = '4';
    pnum[8] = '3';
    pnum[9] = '3'; 
  }
  else if(Phone[0]=='0' && Phone[1]=='7' && Phone[2]=='1' && Phone[3]=='4' && Phone[4]=='4' && Phone[5]=='3' && Phone[7]=='1' && Phone[8]=='9' && Phone[9]=='2')
  {
    pnum[0] = '0';
    pnum[1] = '7';
    pnum[2] = '1';
    pnum[3] = '4';
    pnum[4] = '4';
    pnum[5] = '3';
    pnum[6] = '1';
    pnum[7] = '1';
    pnum[8] = '9';
    pnum[9] = '2'; 
  }
  else if(Phone[0]=='0' && Phone[1]=='7' && Phone[2]=='2' && Phone[3]=='8' && Phone[4]=='4' && Phone[5]=='6' && Phone[7]=='6' && Phone[8]=='3' && Phone[9]=='6')
  {
    pnum[0] = '0';
    pnum[1] = '7';
    pnum[2] = '2';
    pnum[3] = '8';
    pnum[4] = '4';
    pnum[5] = '6';
    pnum[6] = '0';
    pnum[7] = '6';
    pnum[8] = '3';
    pnum[9] = '6'; 
  }
  else if(Phone[0]=='+' && Phone[1]=='9' && Phone[2]=='4')
  {
    pnum[0] = '0';
    pnum[1] = Phone[3];
    pnum[2] = Phone[4];
    pnum[3] = Phone[5];
    pnum[4] = Phone[6];
    pnum[5] = Phone[7];
    pnum[6] = Phone[8];
    pnum[7] = Phone[9];
    pnum[8] = Phone[10];
    pnum[9] = Phone[11];
  }
  else
  {
    pnum[0] = Phone[0];
    pnum[1] = Phone[1];
    pnum[2] = Phone[2];
    pnum[3] = Phone[3];
    pnum[4] = Phone[4];
    pnum[5] = Phone[5];
    pnum[6] = Phone[6];
    pnum[7] = Phone[7];
    pnum[8] = Phone[8];
    pnum[9] = Phone[9];
  }

  if(c == 1)
  {
    sprintf(MESSAGE, "ATD9,Place,%s,%s,%d,%s,%s,%s", lat,lon,force,flam,date,tim);
  }

  if(c == 2)
  {
    sprintf(MESSAGE, "Place :- http://maps.google.com/maps?q=%s,%s\n",lat, lon);
  }

  if(c == 3)
  {
    sprintf(MESSAGE, "ATD9,Accident,%s,%s,%d,%s,%s,%s", lat,lon,force,flam,date,tim);
  }

  Serial.println("Start to send message ...");
  
  Serial.println(MESSAGE);
  Serial.println(pnum);
  
  sim808.sendSMS(pnum,MESSAGE);

  c = 0;
  Phone[16] = "";
  pnum[16] = "";
  MESSAGE[200] = "";
  
}

