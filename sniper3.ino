//бибилинотеки :)
#include <Servo.h> 
#include <TM1637.h>
//контактные группы мишений и массив с попаданиями
int kontakti[4]={2,3,4,5};
int misheni[4]={0,0,0,0};
int prov[4]={0,0,0,0};
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
#define knopka1 10
#define knopka2 11

//----------------------------------------
//        ОПРОС КОНТАКТОВ
void opros(){
for (int a=0;a<=3;a++)
{
misheni[a]=digitalRead(kontakti[a]);  
Disp[a]=misheni[a];
}
tm1637.display(Disp);
}
//----------------------------------------

//----------------------------------------
//          МОРГАЛКА 
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
opros();
tm1637.set(2);
tm1637.display(Disp);
}
//----------------------------------------

//============ ПОДЪЕМ МИШЕНИЙ 1 ИЛИ 2 ИГРОКА ИЛИ ВСЕХ ====================
void podiemserv(int serva1, int serva2)
{
  if (serva1==1 && serva2==0){
   myservo1.write(180);
   delay (300); 
   myservo2.write(180);
   delay (300);
   myservo1.write(0);
   delay (300); 
   myservo2.write(0);
   delay (300);
  }
  else if (serva1==0 && serva2==1)
   {
   myservo3.write(180);
   delay (300);
   myservo4.write(180);
   delay (300);
   myservo3.write(0);
   delay (300);
   myservo4.write(0);
   delay (300);
   }
  else podiemvsexserv();
}
//=========================================================================

//========== ПОДЪЕМ ВСЕХ МИШЕНИЙ ==========================================
void podiemvsexserv(){
 myservo1.write(180);
 delay (300); 
 myservo2.write(180);
 delay (300);
 myservo3.write(180);
 delay (300);
 myservo4.write(180);
 delay (300);
  myservo1.write(0);
 delay (300); 
 myservo2.write(0);
 delay (300);
 myservo3.write(0);
 delay (300);
 myservo4.write(0);
 delay (300);
}
//============================================================================
// =============     ИГРА НА ОДНОГО ИГРОКА - ПОПАДИ 4 РАЗА ! =================
void igra1(){
podiemvsexserv();
opros();
int d=0;
do { 
 for (int a=0;a<=3;a++){ 
    //если сменилось состояние 
   if (digitalRead(kontakti[a])== 1)
	  { //если был 0, будет 1
	    if (misheni[a] == 0) misheni[a] = 1; 
            Disp[a]=misheni[a];
	    delay(200);
	  }
	  //если сменилось состояние 
	  if (digitalRead(kontakti[a]) == 0)
	  { //если был 1, будет 1
	    if (misheni[a] == 1) misheni[a] = 0; 
            Disp[a]=misheni[a];
	    delay(200);
	  }
  }
          //проверка на выйгрыш
  if (misheni[0]==1 && misheni[1]==1 && misheni[2]==1 && misheni[3]==1) 
  {
    podiemvsexserv();
    d=4 ;
  }
  else tm1637.display(Disp);
}

while (d!=4);

morgalka();
//повторно грузим игру 1?
}
//==========================================================================

//================  ИГРА НА ДВУХ ИГРОКОВ - ПО ДВЕ МИШЕНИ ===================
void igra2(){
podiemvsexserv();
opros();
int c=0;
do {
 for (int a=0;a<=3;a++){ 
    //если сменилось состояние 
   if (digitalRead(kontakti[a])== 1)
	  { //если был 0, будет 1
	    if (misheni[a] == 0) misheni[a] = 1; 
            Disp[a] = misheni[a];
	    delay(200);
	  }
	  //если сменилось состояние 
	  if (digitalRead(kontakti[a]) == 0)
	  { //если был 1, будет 1
	    if (misheni[a] == 1) misheni[a] = 0; 
            Disp[a]=misheni[a];
	    delay(200);
	  }
 }
 tm1637.display(Disp);
 //проверка выйгрыша 
 if ((misheni[0]==1 && misheni[1]==1) || (misheni[2]==1 && misheni[3]==1))  
 {
  podiemvsexserv(); 
  c=4; 
 }
 else tm1637.display(Disp);
 } 
while (c!=4);

if (misheni[0]==1 && misheni[1]==1)  
 {
 misheni[2]==0;
 misheni[3]==0;
 morgalka();
 }
if (misheni[2]==1 && misheni[3]==1) 
 {
 misheni[0]==0;
 misheni[1]==0;  
 morgalka();
 }
else  tm1637.display(Disp);
}
//==========================================================================

void setup() 
{ 
 tm1637.init();
 tm1637.set(2);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
 myservo1.attach(6);
 myservo2.attach(7);
 myservo3.attach(8);
 myservo4.attach(9);
}

void loop() 
{
//================ НАЖИТИЕ НА ПЕРВУЮ КЛАВИШУ - ИГРА НА ОДНОГО =============   
if (digitalRead(knopka1)==1) igra1(); 
//================= НАЖАТИЕ ВТОРОЙ КНОПКИ - ИГРА НА ДВОИХ ===================   
if (digitalRead(knopka2) ==1) igra2();  
}

