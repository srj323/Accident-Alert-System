#include<LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(5,6);
LiquidCrystal lcd(13,12,11,10,9,8);

int LED_Pin = 7;
int vibr_Pin =3;
//-------------------------------------------------------------
int i=0,k=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";

// The serial connection to the GPS module
SoftwareSerial gps(4,2);
//-------------------------------------------------------------
void setup(){
  pinMode(LED_Pin, OUTPUT);
  pinMode(vibr_Pin, INPUT); //set vibr_Pin input for measurment
  Serial.begin(9600); //init serial 9600
 // Serial.println("----------------------Vibration demo------------------------");
 mySerial.begin(9600);
  Serial.begin(9600);
    Serial.println("99");
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("GPS Besed Vehicle ");
  lcd.setCursor(0,1);
  lcd.print("Tracking System");
  delay(100);
  // Start the software serial port at the GPS's default baud
//  Serial.begin(GPSBaud);
  Serial.println();
 // ----------------------------------------------
   gps.begin(9600);
 // get_gps();
  //show_coordinate();
  //------------------------------------------------
}
void SendMessage()
{
   get_gps();
  show_coordinate();
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917981823134\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Accident has happened at lat : ");// The SMS text you want to send
  mySerial.println(latitude);
 mySerial.println("long : ");// The SMS text you want to send
mySerial.println(logitude);
mySerial.print("https://www.google.com/maps/search/?api=1&query=");
mySerial.print(latitude);
mySerial.print(",");
mySerial.print(logitude);
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
   Serial.println("MESSAGE SENT !");
  delay(1000);
}
void loop(){
 
  long measurement =TP_init();
  delay(50);
  Serial.print("measurment = ");
  Serial.println(measurement);
  if (measurement > 30000){
    // accident occur 
      Serial.print("measurment = ");
  Serial.println(measurement);
    digitalWrite(LED_Pin, HIGH);
    Serial.begin(9600);
        lcd.print("Are u Safe????");
        delay(2000);
        lcd.clear();
        lcd.print("Press Reset");
        delay(10000);
        SendMessage();
        lcd.clear();
        lcd.print("Message SENT!!");
        delay(1000);
  }
  else{
    digitalWrite(LED_Pin, LOW); 
  }
}

long TP_init(){
  delay(10);
  long measurement=pulseIn (vibr_Pin, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}
//--------------------------------------------------------
void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //Serial incoming data from GPS
   {
    char inChar = (char)gps.read();
    gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
  //lcd.clear();
  //lcd.print("Getting GPS Data");
  //lcd.setCursor(0,1);
  Serial.println("Please Wait.....");
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}

void show_coordinate()
{
    //lcd.clear();
    //Serial.println("Lat:");
    //Serial.print(latitude);
    //lcd.setCursor(0,1);
    //Serial.println("Log:");
    //Serial.println(logitude);
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);
    Serial.print("Speed(in knots)=");
    Serial.println(Speed);
    delay(2000);
    //lcd.clear();
    //lcd.print("Speed(Knots):");
    //lcd.setCursor(0,1);
    //lcd.print(Speed);
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=19;i<=20;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=22;i<=28;i++)         
      lat_minut+=gpsString[i];

  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];

  String log_minut="";
    for(i=35;i<=41;i++)
      log_minut+=gpsString[i];
    
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}
//---------------------------------------------------------------------------

