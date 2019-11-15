/* Pinout
D2    RX
D3    TX
D4    IN LEP
D5    IN DIS
D6    IN AVR
D6    LED Error
D11   Speaker
D12   LED GSM 
D13   key test
*/

#include <SoftwareSerial.h>
SoftwareSerial gsm(3,2); // RX, TX
int IN_LEP=4;
int IN_DIS=5;
int IN_AVR=6;
int LED_LEP=7;
int LED_DIS=8;
int LED_AVR=9;
int LED_ER=10;
int SPK=11;
int TEST_KEY=13;
boolean LEP_st=0;
boolean DIS_st=0;
boolean AVR_st=0;
boolean KEY_st=0;
int stat=0;
int stdis=0;
boolean  stdissms=0;
int stavron=0;
boolean  stavrsmson=0;
int stavroff=0;
boolean  stavrsmsoff=0;

#define dizsec 50             //50 секунд на запуск дизеля
#define avrsec 15               //15 секунд на сработку АВР


void setup() {
     Serial.begin(9600);               // Скорость обмена данными с компьютером
     Serial.println("GSM tester V.2.4");
     gsm.begin(9600);               // Скорость обмена данными с модемом
     gsm.println("AT"); 
     Serial.println("Turn on AOH:");
     gsm.println("AT+CLIP=1"); // включить АОН
     delay(1000);
     Serial.println("Text format sms:");
     gsm.println("AT+CMGF=1"); // текстовый формат SMS
     delay(1000);
     Serial.println("Mode GSM:");
     gsm.println("AT+CSCS=\"GSM\""); // кодировка текста - GSM
     delay(1000);
     pinMode(IN_LEP, INPUT);
     pinMode(IN_DIS, INPUT);
     pinMode(IN_AVR, INPUT);
     pinMode(TEST_KEY, INPUT);
     pinMode(LED_LEP, OUTPUT);
     pinMode(LED_DIS, OUTPUT);
     pinMode(LED_AVR, OUTPUT);
     pinMode(LED_ER, OUTPUT);
     pinMode(SPK, OUTPUT);
     starttest();
}

void loop() {
  LEP_st = digitalRead(IN_LEP);     
  DIS_st = digitalRead(IN_DIS);  
  AVR_st = digitalRead(IN_AVR);  
  digitalWrite(LED_LEP, !LEP_st); 
  digitalWrite(LED_DIS, !DIS_st); 
  digitalWrite(LED_AVR, !AVR_st); 
  testkey();
   
  if(LEP_st==0 && DIS_st==1 && AVR_st==1){                           //Сеть работает, дизель и авр не работают
    digitalWrite(LED_ER, LOW); 
    stat = 0;
    stdissms=0;
    stavrsmson=0;
    stavrsmsoff=0;
    
  }
 
  if(LEP_st==1 && DIS_st==1 && AVR_st==1 && stat==0){           //Погас свет, дизель и АВР не работают
     digitalWrite(LED_ER, HIGH); 
     gsm.println("ATD+375447200782;");
     delay(500);
     tone(SPK, 1000,1000); 
     stat = 1;
     stdis = 0;
     delay(1500);
  } 
 
  if(LEP_st==1 && DIS_st==1 && AVR_st==1 && stat==1 && stdis<=dizsec){           //Погас свет, дизель и АВР не работают +no_phone + timer
     digitalWrite(LED_ER, HIGH);
     delay(500);
     tone(SPK, 1000,100); 
     stat = 1;
     stdis = stdis+1;
     delay(1000);
  } 
  
  if(LEP_st==1 && DIS_st==1 && AVR_st==1 && stat==1 && stdis==dizsec && stdissms==0){           //Погас свет, дизель и АВР не работают +no_phone + timer + sms
     digitalWrite(LED_ER, HIGH);
     sms(String("Pitanie net, DISEL nezapustilsia"), String("+375447200782")); 
     delay(500);
     tone(SPK, 1000,1000); 
     stat = 1;
     stdissms=1;
     delay(1000);
  } 
 if(LEP_st==1 && DIS_st==0 && AVR_st==1 && stat==1){           //Погас свет, дизель запустился, АВР не сработал
     digitalWrite(LED_ER, HIGH); 
     delay(500);
     tone(SPK, 1000,1000); 
     stat = 5;
     stdis = 0;
     delay(1500);
  } 
 
  if(LEP_st==1 && DIS_st==0 && AVR_st==1 && stat==5 && stavron<=avrsec){           //Погас свет, дизель запустился,АВР не сработал +no_phone + timer
     digitalWrite(LED_ER, HIGH);
     delay(500);
     tone(SPK, 1000,100); 
     stat = 5;
     stavron = stavron+1;
     delay(1000);
  } 
  
  if(LEP_st==1 && DIS_st==0 && AVR_st==1 && stat==5 && stavron==avrsec && stavrsmson==0){           //Погас свет, дизель запустился,АВР не сработал +no_phone + timer + sms
     digitalWrite(LED_ER, HIGH);
     sms(String("Pitanie net, DISEL zapustilsia, AVR ne srabotal"), String("+375447200782")); 
     delay(500);
     tone(SPK, 1000,1000); 
     stat = 5;
     stavrsmson=1;
     delay(1000);
  } 

 if(LEP_st==1 && DIS_st==0 && AVR_st==0 && stat==1){           //Погас свет, дизель запустился, АВР сработал
     digitalWrite(LED_ER, HIGH); 
     sms(String("Pitanie net, DISEL zapustilsia, AVR srabotal"), String("+375447200782"));
     delay(500);
     tone(SPK, 1000,1000); 
     stat = 6;
     delay(1500);
  } 

  if(LEP_st==1 && DIS_st==0 && AVR_st==0 && stat==5){           //Погас свет, дизель запустился, АВР сработал
     digitalWrite(LED_ER, HIGH); 
     sms(String("Pitanie net, DISEL zapustilsia, AVR srabotal"), String("+375447200782"));
     delay(500);
     tone(SPK, 1000,1000); 
     stat = 6;
     delay(1500);
  }
 
   if(LEP_st==1 && DIS_st==0 && AVR_st==0 && stat==6){           //Погас свет, дизель запустился, АВР сработал + no_sms
     digitalWrite(LED_ER, HIGH); 
     delay(500);
     digitalWrite(LED_ER, LOW); 
     delay(500);
  } 
  
  if(LEP_st==0 && DIS_st==0 && AVR_st==1 && stat==0){           //Непонятки с дизелем
     digitalWrite(LED_ER, HIGH);
    sms(String("Pitanie ot seti prisustvuet, no DISEL rabotaet"), String("+375447200782")); 
     delay(500);
     tone(SPK, 1000,1000); 
     stat =2 ;
     delay(1500);
  } 
  if(LEP_st==0 && DIS_st==0 && AVR_st==1 && stat==2){           //Непонятки с дизелем +no_sms
     digitalWrite(LED_ER, HIGH);
     delay(500);
     tone(SPK, 1000,100); 
     stat =2 ;
     delay(1500);
  } 
  
  if(LEP_st==0 && DIS_st==1 && AVR_st==0 && stat==0){           //Непонятки с АВР
     digitalWrite(LED_ER, HIGH);
    sms(String("Pitanie ot seti prisustvuet, no AVR rabotaet"), String("+375447200782")); 
     delay(500);
     tone(SPK, 1000,1000); 
     stat =3 ;
     delay(1500);
  } 
  if(LEP_st==0 && DIS_st==1 && AVR_st==0 && stat==3){           //Непонятки с АВР +no_sms
     digitalWrite(LED_ER, HIGH);
     delay(500);
     tone(SPK, 1000,100); 
     stat =3 ;
     delay(1500);
  } 
  
  if(LEP_st==0 && DIS_st==0 && AVR_st==0 && stat==0){           //Непонятки с дизелем и  АВР
     digitalWrite(LED_ER, HIGH);
    sms(String("Зitanie ot seti prisustvuet, no DISEL i AVR rabotaet"), String("+375447200782")); 
     delay(500);
     tone(SPK, 1000,1000); 
     stat =4 ;
     delay(1500);
  } 
  if(LEP_st==0 && DIS_st==0 && AVR_st==0 && stat==4){           //Непонятки с дизелем и АВР +no_sms
     digitalWrite(LED_ER, HIGH);
     delay(500);
     tone(SPK, 1000,100); 
     stat =4 ;
     delay(1500);
  } 

  }
    
   
void starttest(){
    digitalWrite(LED_LEP, HIGH); 
    delay(500); 
    digitalWrite(LED_DIS, HIGH); 
    delay(500); 
    digitalWrite(LED_AVR, HIGH); 
    delay(500); 
    digitalWrite(LED_ER, HIGH);
    delay(500); 
    tone(SPK, 2000,1000); 
    delay(500);
    digitalWrite(LED_LEP, LOW); 
    digitalWrite(LED_DIS, LOW); 
    digitalWrite(LED_AVR, LOW); 
    digitalWrite(LED_ER, LOW);   
}

void sms(String text, String phone) // отправка СМС
{
Serial.println("Start SMS send");
gsm.println("AT+CMGS=\"" + phone + "\"");
delay(500);
gsm.print(text);
delay(500);
gsm.print((char)26);
delay(500);
Serial.println("SMS send OK");
delay(2000);
}

void testkey ()     // Кнопка тест
{
KEY_st = digitalRead(TEST_KEY); 
if (KEY_st==1){
  digitalWrite(LED_LEP, HIGH); 
  digitalWrite(LED_DIS, HIGH); 
  digitalWrite(LED_AVR, HIGH);     
sms(String("ZACHISLENIE 1256.80 EUR KARTA #6237 DATA note"), String("+375447200782")); //отправляем СМС если нажата кнопка тест
}
}
