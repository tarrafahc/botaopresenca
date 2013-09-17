/*
  Botao Tarrafa - Presenca
  www.Tarrafa.net
*/

#include <EEPROM.h>

#define memory 0 // Memoria
#define bPin 2   // Pino do botao
#define led A0   // Led do botao

int leds[] = {  10, 11, 9  };   // Usando os pinos VERMELHO - VERDE - AZUL

static int valor;
int desligando = 0;
unsigned long time;

//Random Color Generator Vars
int redNow;
int blueNow;
int greenNow;
int redNew;
int blueNew;
int greenNew;

void setup() {
  //Leds
  pinMode(led,OUTPUT);
  for(int i = 0; i < 3; i++){pinMode(leds[i], OUTPUT);}
  
  //Begin Serial on 9600Bps
  Serial.begin(9600);
  
  //Read memory, turns led if it should
  valor = EEPROM.read(memory);
  digitalWrite(led,valor);
  
  //Interrupt for the button, 0 = Digital Pin 2
  attachInterrupt(0, butaoPress, FALLING);
    
 //Random Color Start Seed
 redNow = random(255);    redNew = redNow;
 blueNow = random(255);   blueNew = blueNow;
 greenNow = random(255);  greenNew = greenNow;
}

void loop(){
  //Ligado, vamos brincar com leds?
  if(valor==1){
    digitalWrite(led,HIGH);  
    
    //------------[Random Color Generator Tabajara]-----------------\\
  
      redNew = random(255);
      blueNew = random(255);
      greenNew = random(255);
      
      // fade to new colors
      while ((redNow != redNew) ||  (blueNow != blueNew) ||  (greenNow != greenNew)){
        if(redNow>redNew){redNow--;} else if(redNow<redNew){redNow++;}
        if(blueNow>blueNew){blueNow--;} else if(blueNow<blueNew){blueNow++;}
        if(greenNow>greenNew){greenNow--;} else if(greenNow<greenNew){greenNow++;}
        
        analogWrite(leds[2], blueNow);
        analogWrite(leds[0], redNow);
        analogWrite(leds[1], greenNow);
        delay(10); 
        if(valor==0){goto desligar;}
      }
    
    //-----------------------------------------------------------------\\
    
  }
  
  //Desliga tudo
  else{
    desligar:
    //Desliga LED Botao
    digitalWrite(led,LOW);
    //Se for mais do que 3mins, ele desliga a luz branca
    if(millis() > time + 60000){  for(int i = 0; i < 3; i++){ digitalWrite(leds[i], LOW); }}
    else{ for(int i = 0; i < 3; i++){ digitalWrite(leds[i], HIGH); }}

  }
}


//Funcao para mudar o valor e enviar pela serial!
void butaoPress(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // Decounce
  if (interrupt_time - last_interrupt_time > 200){
    valor = !valor;
    EEPROM.write(memory, valor);
    Serial.println(valor,DEC);
    time = millis();
  }
  last_interrupt_time = interrupt_time;
}
