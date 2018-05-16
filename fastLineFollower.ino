#include <QTRSensors.h>

#define sagtabanhiz 120 //right motor base speed
#define soltabanhiz 120 //left motor base speed
#define sagmotoryon  12 
#define sagmotorpwmpin 11
#define solmotoryon 7
#define solmotorpwmpin 6
#define MZ80 5
int mzdeger = 0;

//QTR8A sensor
QTRSensorsAnalog qtra((unsigned char[]) {A0, A1, A2, A3, A4 , A5 , A6, A7} ,8, 4, QTR_NO_EMITTER_PIN); 
unsigned int sensorValues[8];
int sonhata = 0;
float Kp = 0.1;
float Kd = 0.8;

int sagmotorpwm = 0;
int solmotorpwm = 0;
int zemin=1;// 1=follows the white line, 0=follows the black line
void setup()
{

   
      delay(2000);
      pinMode(sagmotoryon, OUTPUT);
      pinMode(sagmotorpwmpin, OUTPUT);
      pinMode(solmotoryon, OUTPUT);
      pinMode(solmotorpwmpin, OUTPUT);
      pinMode (1, OUTPUT);
      pinMode ( 8, OUTPUT );
      pinMode ( 13, OUTPUT);
      pinMode ( MZ80,INPUT);
      digitalWrite(8,LOW);     
      digitalWrite(12,LOW);     
      digitalWrite(2,HIGH); 
      int i;
      digitalWrite(1,HIGH);    

       
     for (int i = 0; i < 50; i++)
     { 
       qtra.calibrate(); 
       delay(4);
      } 
      
    digitalWrite(1,LOW);
    delay(2000);

   while(mzdeger==0)
    {
      mzdeger = digitalRead(MZ80);
     
      delay(1);    
    }
} 

void loop()
{ 
  // Pozisyon hesabı QTRlibrary tarafından yaptırılıyor
  unsigned int sensorValues[8];   
  unsigned int position = qtra.readLine(sensorValues,1,zemin); 
  int hata = position-3500;

 
  //////////// motorlara verilecek hız düzeltme oran hesabı
  int duzeltmehizi = Kp * hata + Kd*(hata - sonhata);
  sonhata = hata;
    
   
  //////////// Motorlara uygulanacak kesin hız ayarları
   sagmotorpwm = sagtabanhiz + duzeltmehizi  ;
   solmotorpwm = soltabanhiz - duzeltmehizi  ;
  
   sagmotorpwm = constrain(sagmotorpwm, -50, 255); ///// Burada motorlara uygulanacak PWM değerlerine sınırlandırma getirilmiştir.
   solmotorpwm = constrain(solmotorpwm, -50, 255); 
   motorkontrol(sagmotorpwm,solmotorpwm);
  //Seri monitörde hata, sağ ve sol motor hız değerlerini gösternek için alttaki satırı aktif yapınız
 //Serial.print(hata);  Serial.print(" "); Serial.print(sagmotorpwm); Serial.print(" "); Serial.println(solmotorpwm); delay(100);  

}
// Motor kontrol alt programı
void motorkontrol(int sagmotorpwm, int solmotorpwm){

  if(sagmotorpwm<=0)
  {
      sagmotorpwm=abs(sagmotorpwm);
      digitalWrite(sagmotoryon, LOW);
      analogWrite(sagmotorpwmpin, sagmotorpwm);
    }
  else 
  {
      digitalWrite(sagmotoryon, HIGH);
      analogWrite(sagmotorpwmpin, sagmotorpwm);
  }
  if(solmotorpwm<=0) 
  {
     solmotorpwm=abs(solmotorpwm);
     digitalWrite(solmotoryon, LOW);
     analogWrite(solmotorpwmpin, solmotorpwm);
  }
   else 
   {
    digitalWrite(solmotoryon, HIGH);
    analogWrite(solmotorpwmpin, solmotorpwm);
   }
}

