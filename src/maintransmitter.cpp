#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PA_LEVEL RF24_PA_MIN    // Potencia mínima (recomendado si es solo receptor)
//#define PA_LEVEL RF24_PA_LOW    // Potencia baja
//#define PA_LEVEL RF24_PA_HIGH   // Potencia alta
//#define PA_LEVEL RF24_PA_MAX    // Potencia máxima (requiere adaptador con regulador)

#define DATA_RATE RF24_1MBPS    // Por defecto en ambos módulos
//#define DATA_RATE RF24_250KBPS  // Transmisión lenta pero mayor rango
//#define DATA_RATE RF24_2MBPS    // Transmisión rapida pero menor rango

#define CHANNEL 76 // Cambiar canal si hay problemas de interferencia (Debe coincidir con receptor)

#define BUTTON 2   // PIN donde tienes conectado el push button

RF24 radio(9, 8); // Instancia de Radio NRF24L01 PIN 9 = CE, PIN 8 = CSN

bool STATE = false; // Variable boleana que envía el estado del push del emisor

const byte address[6] = "00001"; // Dirección de lectura (Debe coincidir con receptor)

void setup()
{
  pinMode(BUTTON, INPUT); // PIN de boton como entrada digital

  while (!Serial);      //  Inicializar puerto serial y esperar a que se estabilice
    Serial.begin(9600); //  a 9600 baudios
  
  radio.begin();        // Iniciar Radio NRF24L01
  radio.openWritingPipe(address);    // Abrir un canal de escritura en direccion establecida
  
  radio.setDataRate(DATA_RATE); //  Configurar radio para usar DATA_RATE definido por usuario
  radio.setPALevel(PA_LEVEL);   //  Configurar radio para usar el nivel de potencia PA_LEVEL definido por usuario
  radio.setChannel(CHANNEL);    //  Configurar radio para usar CHANNEL definido por usuario

  //    Obtener datos de configuración por puerto serial (para propósitos de debugging)
  Serial.println(radio.getDataRate());
  Serial.println(radio.getPALevel());
  Serial.println(radio.getChannel());

  radio.stopListening();   // Configurar radio como Transmisor
  
  delay(1000);
}

void loop()
{
  STATE = digitalRead(BUTTON);        //  Obtener estado del push button
  radio.write(&STATE, sizeof(STATE)); //  Enviar por radio el estado de variable STATE
  delay(5);                           //  Retardo de estabilización
}