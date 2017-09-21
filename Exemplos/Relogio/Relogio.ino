//Importar bibliotecas
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Sensor de gestos
#include <Wire.h>
#include <SparkFun_APDS9960.h>

//--------------------------------//
//Constantes

//Pino onde a cor esta configurada
#define PIN 6

//Numero de leds
#define NUMPIXELS 14

//Inicializar a biblioteca
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Array do GENIUS
uint32_t genius_mem[100];
int lastPos;

// Pins
#define APDS9960_INT 2 //Pino onde está o sensor de gestos

// Variaveis do sensor de gestos
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

//Constantes do relógio
int dez_hora_1 = 0;
int dez_hora_2 = 1;

int unid_hora_1 = 2;
int unid_hora_2 = 3;
int unid_hora_4 = 4;
int unid_hora_8 = 5;

int dez_min_1 = 6;
int dez_min_2 = 7;
int dez_min_4 = 8;

int unid_min_1 = 9;
int unid_min_2 = 10;
int unid_min_4 = 11;
int unid_min_8 = 12;

int hora;
int minuto;
int segundo;

// the setup routine runs once when you press reset:
void setup() {                

  pixels.begin(); // This initializes the NeoPixel library.  

  // Setar o interruptor como entrada
  pinMode(APDS9960_INT, INPUT);

  // Inicializar APDS
  attachInterrupt(0, interruptRoutine, FALLING);
  apds.init();

  //Ativar o sensor de gestos
  apds.enableGestureSensor(true);

  //Iniciar as horas
  hora = 16;
  minuto = 47;
  segundo = 40;

  //Iniciar monitor serial
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {

  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  
  //atualiza_hora();
  //acender_leds();
}

void atualiza_hora()
{
  delay(1000);
  
  segundo = segundo + 1;
  
  if(segundo == 60)
  {
    minuto = minuto + 1;
    segundo = 0;
  }
  
  if(minuto == 60)
  {
    hora = hora + 1;
    minuto = 0;
  }
  
  if(hora == 24)
  {
    hora = 0;
  }
  
}

void acender_leds(uint32_t cor, int timing)
{
  acende_leds(hora, 0, 1, 13, 2, 3, 4, 5, cor);
  acende_leds(minuto, 6, 7, 8, 9, 10, 11, 12, cor);

  pixels.show();
  delay(timing*1000);
  apaga_leds();
  
}

void apaga_leds()
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }

  pixels.show();
}

void acende_leds(int valor, int led_dez_1, int led_dez_2, int led_dez_4, int led_unid_1, int led_unid_2, int led_unid_4, int led_unid_8, uint32_t cor)
{

  int quociente, resto;

  quociente = valor/10;
  resto = valor%10;

  //Acender dezenas
  if(quociente/4 == 1)
  {
    pixels.setPixelColor(led_dez_4, cor);
    quociente = quociente - 4;
  }
  else
  {
    pixels.setPixelColor(led_dez_4, pixels.Color(0,0,0));
  }

  if(quociente/2 == 1)
  {
    pixels.setPixelColor(led_dez_2, cor);
    quociente = quociente - 2;
  }
  else
  {
    pixels.setPixelColor(led_dez_2, pixels.Color(0,0,0));
  }

  if(quociente/1 == 1)
  {
    pixels.setPixelColor(led_dez_1, cor);
    quociente = quociente - 1;
  }
  else
  {
    pixels.setPixelColor(led_dez_1, pixels.Color(0,0,0));
  }

  //Acender unidades
  if(resto/8 == 1)
  {
    pixels.setPixelColor(led_unid_8, cor);
    resto = resto - 8;
  }
  else
  {
    pixels.setPixelColor(led_unid_8, pixels.Color(0,0,0));
  }
    
  if(resto/4 == 1)
  {
    pixels.setPixelColor(led_unid_4, cor);
    resto = resto - 4;
  }
  else
  {
    pixels.setPixelColor(led_unid_4, pixels.Color(0,0,0));
  }
  
  if(resto/2 == 1)
  {
    pixels.setPixelColor(led_unid_2, cor);
    resto = resto - 2;
  }
  else
  {
    pixels.setPixelColor(led_unid_2, pixels.Color(0,0,0));
  }
  
  if(resto==1)
  {
    pixels.setPixelColor(led_unid_1, cor);
  }
  else
  {
    pixels.setPixelColor(led_unid_1, pixels.Color(0,0,0));
  }
  
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
      case DIR_DOWN:
      case DIR_LEFT:
      case DIR_RIGHT:
        iniciar_genius();
        break;
      case DIR_NEAR:
      case DIR_FAR:
        acender_leds(pixels.Color(0, 255, 0), 5);
        break;
    }
  }
}

void iniciar_genius()
{ 
  ativar_genius();
}

void ativar_genius()
{
  int cor_s;

  //sortear uma cor
  cor_s = rand() % 4;

  //colocar a cor na última posição do array
  switch(cor_s)
  {
    case 0:
      genius_mem[lastPos] = pixels.Color(0,0,255);
      Serial.println("Azul");
      break;
    case 1:
      genius_mem[lastPos] = pixels.Color(0,255,0);
      Serial.println("Verde");
      break;
    case 2:
      genius_mem[lastPos] = pixels.Color(255,0,0);
      Serial.println("Vermelho");
      break;
    case 3:
      genius_mem[lastPos] = pixels.Color(125,130,0);
      Serial.println("Amarelo");
      break;
  }

  //Incrementar o contador
  lastPos = lastPos + 1;

  //Mostrar sequencia
  showGeniusSeq();
  
}

void showGeniusSeq()
{

  for(int i=0; i<lastPos; i++)
  {
    acender_leds(genius_mem[i], 1);
  }
  
}

