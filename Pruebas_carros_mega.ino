#include <Servo.h>
#include <LiquidCrystal.h>
Servo servoMotor;

bool encendido = false, automatico = false, turbo = false;
////////////////////Velocidad/////////////////
int send0 = 2;
int sena0 = 3;
int rpm;
volatile byte pulsos;
unsigned long timeold;
float velocidad;
unsigned int muescas = 20;// Numero de muescas de la rueda
int Diametro_rueda = 64; // Diametro de la rueda
//////////////////////////////////////////////
int Ledrojo = 38;
int Ledverde = 36;
int pito = 34;

int comando, distanciaFrente, distanciaIzq, distanciaDer, velocidadmotor = 80, loopGiro = 0, distanciaObstaculo = 50;
int est = 1;
const int EchoPin = 47;
const int TriggerPin = 45;

int RuedaDerAd = 12;
int RuedaDerAt = 11;
int RuedaDerVel = 8;   

int RuedaIzqAd = 10;
int RuedaIzqAt = 9;
int RuedaIzqVel = 13;

LiquidCrystal lcd(22, 24, 26, 28, 30, 32);

void setup () {
  Serial.begin(9600);
  
  servoMotor.attach(6);
  servoMotor.write(90);
    
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
    
  pinMode (Ledrojo, OUTPUT); 
  pinMode (Ledverde, OUTPUT);
  
  pinMode (RuedaDerAd, OUTPUT); 
  pinMode (RuedaDerAt, OUTPUT);
  pinMode (RuedaDerVel, OUTPUT);
  pinMode (RuedaIzqAd, OUTPUT); 
  pinMode (RuedaIzqAt, OUTPUT);
  pinMode (RuedaIzqVel, OUTPUT);

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("INICIANDO..");
  lcd.setCursor(0,1);
  lcd.print("TORETO");
  delay(5000);
  lcd.clear();
  
  ///////////Configuracion Encoder Velocidad///////////////
  pinMode(send0, INPUT);
  pinMode(sena0,  INPUT);
  
  attachInterrupt(1, contador, FALLING);// Acciona el contador cada vez que hay una interrupcion en el pin digital
  pulsos = 0;
  rpm = 0;
  timeold = 0;
  ////////////////////////////////////////////////
}

void loop () {
 
  
  LeerBlue ();
  //Encendido ();
  //Automatico ();
  //Turbo ();
  DistanciaFrente ();
  //ConduccionManual ();
  //ConduccionAutomatica ();
 // if(distanciaFrente <= 30 && est == 1){
   // comando = 't';
   // Parar();
   // est = 0;
 // }
  
// est = 1;
  
  Acelerar ();
  Reversar ();
  GirarIzq ();
  GirarDer ();
  Parar();
  Pito();
    
    
  velocidadencoder();
  
 
}


//funciones

void LeerBlue(){
  if(Serial.available()>0){
    comando = Serial.read(); 
  }
} 

void Pito(){
  if(comando == 'r'){
    digitalWrite (pito, HIGH);
    delay(500);
    digitalWrite (pito, LOW);
  }
  comando = 0;
}

void LedEncendido(){
  digitalWrite (Ledverde, HIGH);
  digitalWrite (Ledrojo, LOW);
}

void Parar(){
  if(comando == 't'){ 
    digitalWrite (Ledverde, LOW);
    digitalWrite (Ledrojo, HIGH);
    digitalWrite (RuedaDerAd, LOW);
    digitalWrite (RuedaIzqAd, LOW);
    digitalWrite (RuedaDerAt, LOW);
    digitalWrite (RuedaIzqAt, LOW);
    analogWrite(RuedaDerVel, velocidadmotor);
    analogWrite(RuedaIzqVel, velocidadmotor);
    //lcd.setCursor(5,1);
    //lcd.print(velocidad);
   // velocidadencoder();
    delay(10);
    comando= 1;
   }
  //analogWrite(RuedaDerVel, 0);
  //analogWrite(RuedaIzqVel, 0);
}

void Acelerar (){
  //acelera hacia adelant
   if(comando == 'w'){ 
    LedEncendido();
    digitalWrite (RuedaDerAd, HIGH);
    digitalWrite (RuedaIzqAd, HIGH);
    digitalWrite (RuedaDerAt, LOW);
    digitalWrite (RuedaIzqAt, LOW);
    analogWrite(RuedaDerVel, velocidadmotor);
    analogWrite(RuedaIzqVel, velocidadmotor);
    //lcd.setCursor(5,1);
    //lcd.print(velocidad);
   // velocidadencoder();
    delay(10);
    comando= 1;
   }
  //analogWrite(RuedaDerVel, 0);
  //analogWrite(RuedaIzqVel, 0);
}

void Reversar (){
  //acelera hacia atras
  if(comando == 's'){
    LedEncendido();
    digitalWrite (Ledrojo, HIGH);
    digitalWrite (RuedaDerAd, LOW);
    digitalWrite (RuedaIzqAd, LOW);
    digitalWrite (RuedaDerAt, HIGH);
    digitalWrite (RuedaIzqAt, HIGH);
    analogWrite(RuedaDerVel, velocidadmotor);
    analogWrite(RuedaIzqVel, velocidadmotor);
    delay(10);
    comando =1;
  }
  //analogWrite(RuedaDerVel, 0);
  //analogWrite(RuedaIzqVel, 0);
}

void GirarDer (){
  //acelera hacia la izquierda
  if(comando == 'd'){
    LedEncendido();
    digitalWrite (RuedaDerAd, LOW);
    digitalWrite (RuedaIzqAd, HIGH);
    digitalWrite (RuedaDerAt, HIGH);
    digitalWrite (RuedaIzqAt, LOW);
    analogWrite(RuedaDerVel, velocidadmotor);
    analogWrite(RuedaIzqVel, velocidadmotor);
    delay(10);
    comando =1;
  }
  //analogWrite(RuedaDerVel, 0);
  //analogWrite(RuedaIzqVel, 0);
}

void GirarIzq (){
  //acelera hacia la derecha
  if(comando == 'a'){
    LedEncendido();
    digitalWrite (RuedaDerAd, HIGH);
    digitalWrite (RuedaIzqAd, LOW);
    digitalWrite (RuedaDerAt, LOW);
    digitalWrite (RuedaIzqAt, HIGH);
    analogWrite(RuedaDerVel, velocidadmotor);
    analogWrite(RuedaIzqVel, velocidadmotor);
    delay(10);
    comando =1;
  }
  //analogWrite(RuedaDerVel, 0);
  //analogWrite(RuedaIzqVel, 0);
}

//funciones AI

void ConduccionAutomatica (){
  
    while(distanciaFrente >= distanciaObstaculo){
    //acelerar
    lcd.setCursor(0,0);
    lcd.print("Modo Automatico");
    comando = 'w';
    DistanciaFrente();
    //velocidadencoder();
    Acelerar ();
    }
  

 // if(distanciaFrente <= distanciaObstaculo){
 //   GirarAutomatico ();
  //}
}

void DistanciaFrente(){
  //medir distancia frente
  servoMotor.write(90);
  distanciaFrente = MedirDistancia(TriggerPin, EchoPin);
  
  delay(1);
  Serial.print("Distancia: ");
  Serial.println(distanciaFrente);
}

void DistanciaIzq(){
  servoMotor.write(180);
  distanciaIzq = MedirDistancia(TriggerPin, EchoPin);
  
  delay(2000);
  Serial.print("Distancia Izq: ");
  Serial.println(distanciaIzq);
}

void DistanciaDer(){
  servoMotor.write(0);
  distanciaDer = MedirDistancia(TriggerPin, EchoPin);
  
  delay(2000);
  Serial.print("Distancia Der: ");
  Serial.println(distanciaDer);
}

/*
void GirarAutomatico(){
  DistanciaIzq ();
  DistanciaDer ();
  DistanciaFrente ();
  
  if(distanciaIzq <= distanciaObstaculo && distanciaDer <= distanciaObstaculo){
    while(distanciaFrente <= (distanciaObstaculo*2)){
      comando = 's';
      DistanciaFrente();
      Reversar();
      
    }
  GirarAutomatico();
  }else if(distanciaIzq >= distanciaObstaculo && distanciaIzq > distanciaDer){
    //meter en un loop hasta que gire 90 grados
    while(loopGiro <= 20){
      comando = 'a';
      GirarIzq();
      loopGiro = loopGiro + 1;
    }
    loopGiro = 0;
    distanciaFrente = distanciaIzq;
    
  }else if(distanciaDer >= distanciaObstaculo && distanciaDer > distanciaIzq){
    //meter en un loop hasta que gire 90 grados
    while(loopGiro <= 20){
      comando = 'd';
      GirarDer();
      loopGiro = loopGiro + 1;
    }
    loopGiro = 0;
    distanciaFrente = distanciaDer;
    
  } 
}
*/
int MedirDistancia(int TriggerPin, int EchoPin) {
  long duration, distanceCm;
   
  digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
   
  duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
  distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
  return distanceCm;
}
void contador()// interrupcion, cada corte que de haga lo cuenta
{
  //Incrementa contador
  pulsos++;
}
void velocidadencoder(){
  if (millis() - timeold >= 1000)// Actualiza el contador cada 1 segundo
  {
    detachInterrupt(1);// Deshabilita la interrupcion para realizar los calculos
    rpm = (60 * 1000 / muescas ) / (millis() - timeold) * pulsos;// Calculo de las revoluciones por minuto
    velocidad = rpm * 3.1416 * Diametro_rueda * 60 / 1000000; // Cálculo de la velocidad en [Km/h] 
    timeold = millis();
    pulsos = 0;

    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    Serial.print("VELOCIDAD = ");
    lcd.setCursor(0,1);
    lcd.print("Vel=");
    lcd.setCursor(5,1);
    lcd.print(velocidad);
    Serial.println(velocidad);
    lcd.setCursor(9,1);
    lcd.print(" Km/h");
    Serial.println(" Km/h");
    
    attachInterrupt(1, contador, FALLING);// Nuevamente se activa la interrupcion
  }
}


