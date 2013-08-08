//Botao Tarrafa

#include <EEPROM.h>

// Pinos
#define memory 0
#define bPin 2
#define led A0

int leds[] = {  10, 11, 9  };   // Usando os pinos VERMELHO - VERDE - AZUL

static int valor;
int desligando = 0;

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
}

void loop(){
  if(valor==1){
      digitalWrite(led,HIGH);
  }
  
  //Desliga tudo
  else{
        for(int i = 0; i < 3; i++){digitalWrite(leds[i], LOW);}
              digitalWrite(led,LOW);
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
  }
  last_interrupt_time = interrupt_time;
}


