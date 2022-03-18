#include "EEPROM.h"
#define EEPROM_SIZE 100

#if defined(ESP8266)
#include <ESP8266WiFi.h>         
#else
#include <WiFi.h>          
#endif
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h> 

#include "FirebaseESP32.h"
FirebaseData firebaseData;

#include "RTClib.h"
RTC_DS3231 RTC;
#include <Wire.h>
#include <SPI.h>

String personId;
int year_1,month_1,date_1,hour_1,min_1;
int year_2,month_2,date_2,hour_2,min_2;
int year_3,month_3,date_3,hour_3,min_3;
int year_4,month_4,date_4,hour_4,min_4;

int auth_verification;
int auth_verification_confirmed;
int address = 1;
int address1 = 3;
int address2 = 5;
int address3 = 7;
int address4 = 9;
int checking_wifi_state;
int wifi_check;
int esp32_year;
int esp32_month;
int esp32_day;
int esp32_hour;
int esp32_minutes;
int esp32_seconds;
int esp32_getdata;
int year_rtc;
int month_rtc;
int day_rtc;
int hour_rtc;
int minute_rtc;
int second_rtc;
int save_alarm1_year,save_alarm1_month,save_alarm1_date,save_alarm1_hour,save_alarm1_min;
int save_alarm2_year,save_alarm2_month,save_alarm2_date,save_alarm2_hour,save_alarm2_min;
int save_alarm3_year,save_alarm3_month,save_alarm3_date,save_alarm3_hour,save_alarm3_min;
int save_alarm4_year,save_alarm4_month,save_alarm4_date,save_alarm4_hour,save_alarm4_min;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
   pinMode(13, INPUT);//Wifi Switch
   pinMode(14, INPUT);//Authemticate Pin
   pinMode(15, INPUT);//Disable Sleep Mode
   pinMode(02, OUTPUT);//Bluetooth Led
   pinMode(04, OUTPUT);//Wifi Led (If there is Internet)
   pinMode(12, OUTPUT);//Wifi Led (To Check if there is no Internet)
   pinMode(16, OUTPUT);//Relay 1
   pinMode(17, OUTPUT);//Relay 2
   pinMode(18, OUTPUT);//Relay 3
   pinMode(19, OUTPUT);//Relay 4

   RTC.begin();
   
   if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); 
    delay(1000000);
  }

  auth_verification_confirmed=EEPROM16_Read(address);
  save_alarm1_year=EEPROM16_Read1(address1);
  save_alarm2_year=EEPROM16_Read2(address2);
  save_alarm3_year=EEPROM16_Read3(address3);
  save_alarm4_year=EEPROM16_Read4(address4);
  save_alarm1_month=EEPROM.read(11);
  save_alarm2_month=EEPROM.read(12);
  save_alarm3_month=EEPROM.read(13);
  save_alarm4_month=EEPROM.read(14);
  save_alarm1_date=EEPROM.read(15);
  save_alarm2_date=EEPROM.read(16);
  save_alarm3_date=EEPROM.read(17);
  save_alarm4_date=EEPROM.read(18);
  save_alarm1_hour=EEPROM.read(19);
  save_alarm1_hour=EEPROM.read(20);
  save_alarm1_hour=EEPROM.read(21);
  save_alarm1_hour=EEPROM.read(22);
  save_alarm1_min=EEPROM.read(23);
  save_alarm1_min=EEPROM.read(24);
  save_alarm1_min=EEPROM.read(25);
  save_alarm1_min=EEPROM.read(26);

    if(digitalRead(13)==HIGH)
   { 
       WiFiManager wifiManager;
       wifiManager.startConfigPortal("Self-Activating-Switch");
       Serial.println("YOUR DEVICE IS READY TO CONNECT");       
   }
  
  Firebase.begin("URL", "API-KEY");
  Firebase.reconnectWiFi(true);  

}

void loop() {  
auth_verification_confirmed=EEPROM16_Read(address);
save_alarm1_year=EEPROM16_Read1(address1);
save_alarm2_year=EEPROM16_Read2(address2);
save_alarm3_year=EEPROM16_Read3(address3);
save_alarm4_year=EEPROM16_Read4(address4);
save_alarm1_month=EEPROM.read(11);
save_alarm2_month=EEPROM.read(12);
save_alarm3_month=EEPROM.read(13);
save_alarm4_month=EEPROM.read(14);
save_alarm1_date=EEPROM.read(15);
save_alarm2_date=EEPROM.read(16);
save_alarm3_date=EEPROM.read(17);
save_alarm4_date=EEPROM.read(18);
save_alarm1_hour=EEPROM.read(19);
save_alarm1_hour=EEPROM.read(20);
save_alarm1_hour=EEPROM.read(21);
save_alarm1_hour=EEPROM.read(22);
save_alarm1_min=EEPROM.read(23);
save_alarm1_min=EEPROM.read(24);
save_alarm1_min=EEPROM.read(25);
save_alarm1_min=EEPROM.read(26);  
  // put your main code here, to run repeatedly:
   /*Check if WiFi is not available*/       
if(WiFi.status() != WL_CONNECTED)
     {
            Serial.println("Wifioff");
            digitalWrite(17,HIGH);
            WiFiManager wifiManager;
            wifiManager.setConfigPortalTimeout(16);
       if(!wifiManager.autoConnect("")) 
          {
          Serial.println("failed to connect and hit timeout"); 
          } 
      }

if(WiFi.status() == WL_CONNECTED)
   {  
    Serial.println("Wifi On");
     digitalWrite(17,LOW);
    /*Get users personal account ID*/
    Firebase.getString(firebaseData, "All User/1234567890/personId");
    if(firebaseData.dataType() == "string") 
    {
    personId=firebaseData.stringData();
    Serial.print("personId"); 
    Serial.println(personId);
    }
    else 
         { 
          Serial.println(firebaseData.errorReason());
          delay(4000);
         }
    
   /*Always set the Internet*/
   Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Internet_State",23);
   }

 /*Pin for authentication*/
    if(digitalRead(14)==HIGH)
        {
         Firebase.getInt(firebaseData, "/All User/1234567890/auth_Id"); 
           if(firebaseData.dataType() == "int") 
             {
                auth_verification=firebaseData.intData();
                Serial.print("Auth_verification"); 
                Serial.println(auth_verification);
                Firebase.setInt(firebaseData,"/All User/1234567890/Authentication_confirmed",auth_verification);
                EEPROM16_Write(address,auth_verification);
             } 
        }

         /*Get the required data*/
       Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_get_val"); 
       if(firebaseData.dataType() == "int") 
         {
          esp32_getdata=firebaseData.intData();
          Serial.print("your esp data"); 
          Serial.println(esp32_getdata);
          }
              /*GET the following time of ESP-32*/
       if(esp32_getdata==45)
    {
       DateTime now = RTC.now();
       Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/year_esp32",now.year());
       Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/month_esp32",now.month());
       Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/day_esp32",now.day());
       Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/hour_esp32",now.hour());
       Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/minutes_esp32",now.minute());
       Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/seconds_esp32",now.second());
      }
              
             /*SET the following time of ESP-32*/  
   if(esp32_getdata==43)
    {
      DateTime now = RTC.now();
    /*Get the year from firebase*/ 
     Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/year"); 
       if(firebaseData.dataType() == "int") 
         {
          year_rtc=firebaseData.intData();
          Serial.print("year"); 
          Serial.println(year_rtc);
         } 
         /*Get the month from firebase*/ 
     Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/month"); 
       if(firebaseData.dataType() == "int") 
         {
          month_rtc=firebaseData.intData();
          Serial.print("month"); 
          Serial.println(month_rtc);
         }     
          /*Get the day from firebase*/ 
     Firebase.getInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/day"); 
       if(firebaseData.dataType() == "int") 
         {
          day_rtc=firebaseData.intData();
          Serial.print("day"); 
          Serial.println(day_rtc);
         }
               
  /*Get the hour from firebase*/ 
     Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/hour"); 
       if(firebaseData.dataType() == "int") 
         {
          hour_rtc=firebaseData.intData();
          Serial.print("hour"); 
          Serial.println(hour_rtc);
         }
         /*Get the minutes from firebase*/ 
     Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/minutes"); 
       if(firebaseData.dataType() == "int") 
         {
          minute_rtc=firebaseData.intData();
          Serial.print("minute"); 
          Serial.println(minute_rtc);
         }
           /*Get the seconds from firebase*/ 
     Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_Time/seconds"); 
       if(firebaseData.dataType() == "int") 
         {
          second_rtc=firebaseData.intData();
          second_rtc = now.second()+10;
          Serial.print("second"); 
          Serial.println(second_rtc);
         }                          
          
    RTC.adjust(DateTime(year_rtc,month_rtc,day_rtc,hour_rtc,minute_rtc,second_rtc));  
    Serial.println("Your data has been sucessfully set");
    Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);    
    }

/*Set all te alarms*/

//Board One
 if(esp32_getdata==46)
    {
        Firebase.getInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Plug/S1/year"); 
       if(firebaseData.dataType() == "int"){
         year_1=firebaseData.intData();
         Serial.print("year1:"); 
         Serial.println(year_1);
        }
        
        Firebase.getInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Plug/S1/month"); 
       if(firebaseData.dataType() == "int"){
         month_1=firebaseData.intData();
         Serial.print("month1:"); 
         Serial.println(month_1);
        }     

        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S1/date"); 
       if(firebaseData.dataType() == "int"){
         date_1=firebaseData.intData();
         Serial.print("date1:"); 
         Serial.println(date_1);
        }
        
        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S1/hour"); 
       if(firebaseData.dataType() == "int"){
         hour_1=firebaseData.intData();
         Serial.print("hour1:"); 
         Serial.println(hour_1);
        }

        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S1/min"); 
       if(firebaseData.dataType() == "int"){
         min_1=firebaseData.intData();
         Serial.print("min1:"); 
         Serial.println(min_1);
        }
        
         EEPROM16_Write1(address1,year_1);
         EEPROM.write(11,month_1);        
         EEPROM.write(15,date_1);
         EEPROM.write(19,hour_1);
         EEPROM.write(23,min_1);
         Serial.println("Your data has been sucessfully set");
         Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);
             
       }
     Serial.println("year_int1");
     Serial.println(save_alarm1_year);
     Serial.println("month_int1");
     Serial.println(save_alarm1_month);
     Serial.println("day_int1");
     Serial.println(save_alarm1_date);
     Serial.println("hour_int1");
     Serial.println(save_alarm1_hour);
     Serial.println("min_int1");
     Serial.println(save_alarm1_min);

     
//Board Two
            if(esp32_getdata==47)
    {

        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S2/year"); 
       if(firebaseData.dataType() == "int"){
         year_2=firebaseData.intData();
         Serial.print("year2:"); 
         Serial.println(year_2);
        }

        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S2/month"); 
       if(firebaseData.dataType() == "int"){
         month_2=firebaseData.intData();
         Serial.print("month2:"); 
         Serial.println(month_2);
        }

          Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S2/date"); 
       if(firebaseData.dataType() == "int"){
         date_2=firebaseData.intData();
         Serial.print("date2:"); 
         Serial.println(date_2);
        }

          Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S2/hour"); 
       if(firebaseData.dataType() == "int"){
         hour_2=firebaseData.intData();
         Serial.print("hour2:"); 
         Serial.println(hour_2);
        }

          Firebase.getInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Plug/S2/min"); 
       if(firebaseData.dataType() == "int"){
         min_2=firebaseData.intData();
         Serial.print("min2:"); 
         Serial.println(min_2);
        }
        
        EEPROM16_Write2(address2,year_2);
        EEPROM.write(12,month_2);         
        EEPROM.write(16,date_1);
        EEPROM.write(20,hour_1);
        EEPROM.write(24,min_1);
        Serial.println("Your data has been sucessfully set");
        Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);        
      }

     Serial.println("year_int2");
     Serial.println(save_alarm2_year);
     Serial.println("month_int2");
     Serial.println(save_alarm2_month);      
     Serial.println("day_int2");
     Serial.println(save_alarm2_date);
     Serial.println("hour_int2");
     Serial.println(save_alarm2_hour);
     Serial.println("min_int2");
     Serial.println(save_alarm2_min);

//Board Three
   if(esp32_getdata==48)
    {
          Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S3/year"); 
       if(firebaseData.dataType() == "int"){
         year_3=firebaseData.intData();
         Serial.print("year3:"); 
         Serial.println(year_3); 
        } 
        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S3/month"); 
       if(firebaseData.dataType() == "int"){
         month_3=firebaseData.intData();
         Serial.print("month3:"); 
         Serial.println(month_3); 
        } 
         Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S3/date"); 
       if(firebaseData.dataType() == "int"){
         date_3=firebaseData.intData();
         Serial.print("date3:"); 
         Serial.println(date_3); 
        } 
           Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S3/hour"); 
       if(firebaseData.dataType() == "int"){
         hour_3=firebaseData.intData();
         Serial.print("hour3:"); 
         Serial.println(hour_3); 
        } 
           Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S3/min"); 
       if(firebaseData.dataType() == "int"){
         min_3=firebaseData.intData();
         Serial.print("min3:"); 
         Serial.println(min_3); 
        }
        
        EEPROM16_Write3(address3,year_3);
        EEPROM.write(13,month_3);         
        EEPROM.write(17,date_1);
        EEPROM.write(21,hour_1);
        EEPROM.write(25,min_1);
        Serial.println("Your data has been sucessfully set");
        Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);             
      }
      
     Serial.println("year_int3");
     Serial.println(save_alarm3_year);
     Serial.println("month_int3");
     Serial.println(save_alarm3_month); 
     Serial.println("day_int3");
     Serial.println(save_alarm3_date);
     Serial.println("hour_int3");
     Serial.println(save_alarm3_hour);
     Serial.println("min_int3");
     Serial.println(save_alarm3_min);

     
//Board Four
         if(esp32_getdata==49)
    {
      Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S4/year"); 
       if(firebaseData.dataType() == "int"){
         year_4=firebaseData.intData();
         Serial.print("year4:"); 
         Serial.println(year_4);
        }  
             Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S4/month"); 
       if(firebaseData.dataType() == "int"){
         month_4=firebaseData.intData();
         Serial.print("month4:"); 
         Serial.println(month_4);
        }      
        Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S4/date"); 
       if(firebaseData.dataType() == "int"){
         date_4=firebaseData.intData();
         Serial.print("date4:"); 
         Serial.println(date_4);
        }  
         Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S4/hour"); 
       if(firebaseData.dataType() == "int"){
         hour_4=firebaseData.intData();
         Serial.print("hour4:"); 
         Serial.println(hour_4);
        }  
         Firebase.getInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Plug/S4/min"); 
       if(firebaseData.dataType() == "int"){
         min_4=firebaseData.intData();
         Serial.print("min4:"); 
         Serial.println(min_4);
        } 

        EEPROM16_Write4(address4,year_4);
        EEPROM.write(14,month_4);     
        EEPROM.write(18,date_1);
        EEPROM.write(22,hour_1);
        EEPROM.write(26,min_1);
        Serial.println("Your data has been sucessfully set");
        Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);  
      }
     
     Serial.println("year_int4");
     Serial.println(save_alarm4_year);
     Serial.println("month_int4");
     Serial.println(save_alarm4_month);   
     Serial.println("day_int4");
     Serial.println(save_alarm4_date);
     Serial.println("hour_int4");
     Serial.println(save_alarm4_hour);
     Serial.println("min_int4");
     Serial.println(save_alarm4_min);


     //Board One Dismiss
         if(esp32_getdata==50)
    {
       EEPROM16_Write1(address1,0000);
         EEPROM.write(11,00);        
         EEPROM.write(15,00);
         EEPROM.write(19,00);
         EEPROM.write(23,00);
         Serial.println("Your data has been sucessfully set");
         Firebase.setInt(firebaseData, "/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);
    }

       //Board Two Dismiss
         if(esp32_getdata==51)
    {
        EEPROM16_Write2(address2,0000);
        EEPROM.write(12,00);         
        EEPROM.write(16,00);
        EEPROM.write(20,00);
        EEPROM.write(24,00);
        Serial.println("Your data has been sucessfully set");
        Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_set_val",46);  
    }

       //Board Three Dismiss
         if(esp32_getdata==52)
    {
         EEPROM16_Write3(address3,0000);
        EEPROM.write(13,00);         
        EEPROM.write(17,00);
        EEPROM.write(21,00);
        EEPROM.write(25,00);
        Serial.println("Your data has been sucessfully set");
        Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_dismiss_val",46); 
    }

       //Board Four Dismiss
         if(esp32_getdata==53)
    {
       EEPROM16_Write4(address4,0000);
        EEPROM.write(14,00);     
        EEPROM.write(18,00);
        EEPROM.write(22,00);
        EEPROM.write(26,00);
        Serial.println("Your data has been sucessfully set");
        Firebase.setInt(firebaseData,"/All User/1234567890/"+auth_verification_confirmed+"/Esp32_dismiss_val",46); 
    }


    /*Print the following date*/
 DateTime now = RTC.now();
   
          Serial.println("Time");
          Serial.print("year"); 
          Serial.println(now.year());
          Serial.print("month"); 
          Serial.println(now.month());
          Serial.print("day"); 
          Serial.println(now.day());
          Serial.print("hour"); 
          Serial.println(now.hour());
          Serial.print("minute"); 
          Serial.println(now.minute());
          Serial.print("second"); 
          Serial.println(now.second());
              
   

}

void EEPROM16_Write(uint8_t a,uint16_t b)
{
  EEPROM.write(a,lowByte(b));
  EEPROM.commit();
  EEPROM.write(a+1,highByte(b));
  EEPROM.commit();
}

uint16_t EEPROM16_Read(uint8_t a)
{
  return word(EEPROM.read(a+1),EEPROM.read(a));
}

void EEPROM16_Write1(uint8_t c,uint16_t d)
{
  EEPROM.write(c,lowByte(d));
  EEPROM.commit();
  EEPROM.write(c+1,highByte(d));
  EEPROM.commit();
}

uint16_t EEPROM16_Read1(uint8_t c)
{
  return word(EEPROM.read(c+1),EEPROM.read(c));
}

void EEPROM16_Write2(uint8_t e,uint16_t f)
{
  EEPROM.write(e,lowByte(f));
  EEPROM.commit();
  EEPROM.write(e+1,highByte(f));
  EEPROM.commit();
}

uint16_t EEPROM16_Read2(uint8_t e)
{
  return word(EEPROM.read(e+1),EEPROM.read(e));
}

void EEPROM16_Write3(uint8_t g,uint16_t h)
{
  EEPROM.write(g,lowByte(h));
  EEPROM.commit();
  EEPROM.write(g+1,highByte(h));
  EEPROM.commit();
}

uint16_t EEPROM16_Read3(uint8_t g)
{
  return word(EEPROM.read(g+1),EEPROM.read(g));
}

void EEPROM16_Write4(uint8_t i,uint16_t j)
{
  EEPROM.write(i,lowByte(j));
  EEPROM.commit();
  EEPROM.write(i+1,highByte(j));
  EEPROM.commit();
}

uint16_t EEPROM16_Read4(uint8_t i)
{
  return word(EEPROM.read(i+1),EEPROM.read(i));
}
