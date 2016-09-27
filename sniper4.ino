//бибилинотеки :)
#include <Servo.h> 
#include <TM1637.h>
#include <IRremote.h>
//контактные группы мишений и массив с попаданиями
int kontakti[4]={3,4,5,6};
int misheni[4]={0,0,0,0};
int igrok1=0;
int igrok2=0;
//подключаем сервоприводы
Servo myservo1; 
Servo myservo2;
Servo myservo3;
Servo myservo4;
//подключаем дисплей
#define CLK 12
#define DIO 13
TM1637 tm1637(CLK,DIO);
int8_t Disp[4];
//клавиши управления
int RECV_PIN = 11; //вход ИК приемника
IRrecv irrecv(RECV_PIN);
decode_results results;

//---------------------------------
//            ЗАЧИСТКА КОНТАКТОВ
void clean()
{ for (int a=0;a<=3;a++){ 
  misheni[a] = 0; 
 Disp[a]=misheni[a];
 }
}
//---------------------------------
//        ОПРОС КОНТАКТОВ 1 игра
void opros1(){
for (int a=0;a<=3;a++){ 
    //если сменилось состояние 
   if (digitalRead(kontakti[a])== 1)
	  { //если был 0, будет 1
	    if (misheni[a] == 0) misheni[a] = 1; 
            Disp[a]=misheni[a];
	  }
	  //если сменилось состояние 
   if (digitalRead(kontakti[a]) == 0)
	  { 
            if (misheni[a] == 1) misheni[a] = 1; 
            Disp[a]=misheni[a];
	  }
  }
}
//--------------------------
//---------------------------------
//        ОПРОС КОНТАКТОВ 2 игра
void opros2(){
for (int a=0;a<=3;a++){ 
    //если сменилось состояние 
   if (digitalRead(kontakti[a])== 1)
	  { //если был 0, будет 1
	    if (misheni[a] == 0) misheni[a] = 1; 
	  }
	  //если сменилось состояние 
   if (digitalRead(kontakti[a]) == 0)
	  { 
            if (misheni[a] == 1) misheni[a] = 1; 
	  }
  }
}
//--------------------------
//          МОРГАЛКА 1
void morgalka()
{
 for (int a=0;a<=5;a++){
 tm1637.set(0);
 tm1637.display(Disp);
 delay(500);
 tm1637.set(7);
 tm1637.display(Disp);
 delay(500);
 }
 tm1637.set(2);
tm1637.display(Disp);
clean();
}
//----------------------------
//===== ПОДЪЕМ МИШЕНИЙ 1 ИЛИ 2 ИГРОКА ИЛИ ВСЕХ ======
void podiemserv(int serva1, int serva2)
{
  if (serva1==1 && serva2==0){
   myservo3.write(100);
   delay (300); 
   myservo4.write(100);
   delay (300);
   myservo3.write(0);
   delay (300); 
   myservo4.write(0);
   delay (300);
   misheni[0]=0;
   misheni[1]=0;
  }
  else if (serva1==0 && serva2==1)
   {
   myservo1.write(100);
   delay (300);
   myservo2.write(100);
   delay (300);
   myservo1.write(0);
   delay (300);
   myservo2.write(0);
   delay (300);
   misheni[2]=0;
   misheni[3]=0;
   }
  else podiemvsexserv();
}
//======= ПОДЪЕМ ВСЕХ МИШЕНИЙ =============
void podiemvsexserv(){
 myservo1.write(100);
 delay (300); 
 myservo2.write(100);
 delay (300);
 myservo3.write(100);
 delay (300);
 myservo4.write(100);
 delay (300);
 myservo1.write(0);
 delay (300); 
 myservo2.write(0);
 delay (300);
 myservo3.write(0);
 delay (300);
 myservo4.write(0);
}
//---------------------------------------------------------
// ======    ИГРА НА ОДНОГО ИГРОКА - ПОПАДИ 4 РАЗА ! ====
void igra1(){
podiemvsexserv();
clean();
int d=0;
do {
 opros1();
 tm1637.display(Disp);
           //проверка на выйгрыш
  if (misheni[0]==1 && misheni[1]==1 && misheni[2]==1 && misheni[3]==1) 
  {
    delay(300);
    podiemvsexserv();
    d=4 ;
  }
}
while (d!=4);
morgalka();
}
//---------------------------------------------------------
//====  ИГРА НА ДВУХ ИГРОКОВ - ПО ДВЕ МИШЕНИ СЧЕТ ДО 5 ===
void igra2(){
igrok1=0;
igrok2=0;
podiemvsexserv();
clean();
int c=0;
tm1637.display(0,igrok1);
tm1637.display(3,igrok2);
do {
 opros2();
 //проверка выйгрыша 
   if (misheni[0]==1 && misheni[1]==1)  
  {
    igrok1+=1;
   if (igrok1<=4) 
    {
     tm1637.display(0,igrok1);
     podiemserv(1,0);
    }
   else c=4;
 }
if (misheni[2]==1 && misheni[3]==1)  
  {
    igrok2+=1;
   if (igrok2<=4) 
    {
     tm1637.display(3,igrok2);
     podiemserv(0,1);
    }
   else c=4;
 }
}
while (c!=4);
morgalka();
}
//=====================

void setup() 
{ 
 clean();
 tm1637.init();
 tm1637.set(2);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
 myservo1.attach(7);
 myservo2.attach(8);
 myservo3.attach(9);
 myservo4.attach(10);
 irrecv.enableIRIn(); // включить приемник
 //tm1637.clearDisplay();
}
void loop() 
{ 
 if (irrecv.decode(&results)) {
 delay(300);
  if (results.value == 0xE0E020DF) 
   {
    igra1(); 
   }
  if (results.value == 0xE0E0A05F)
   {
    igra2();
       }
  irrecv.resume();
 }
}

