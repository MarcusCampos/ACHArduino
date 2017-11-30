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
#define NUMPIXELS 20

//Inicializar a biblioteca
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Array do GENIUS
int genius_mem[100];
uint32_t cores[4];
int sons[4];
int lastPos;

// Pins
#define APDS9960_INT 2 //Pino onde está o sensor de gestos

// Variaveis do sensor de gestos
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

//Constantes do relógio
#define dez_hora_1 0
#define dez_hora_2 1

#define unid_hora_1 2
#define unid_hora_2 3
#define unid_hora_4 4
#define unid_hora_8 5

#define dez_min_1 6
#define dez_min_2 7
#define dez_min_4 8

#define unid_min_1 9
#define unid_min_2 10
#define unid_min_4 11
#define unid_min_8 12

#define dez_seg_1 13
#define dez_seg_2 14
#define dez_seg_4 15

#define unid_seg_1 16
#define unid_seg_2 17
#define unid_seg_4 18
#define unid_seg_8 19

#define botao_verde 8
#define botao_amarelo 9
#define botao_vermelho 10
#define botao_azul 11

#define saida_som 12

int hora;
int minuto;
int segundo;
unsigned long start;

uint32_t azul;
uint32_t amarelo;
uint32_t verde;
uint32_t vermelho; 

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
  segundo = 49;
  start = 0;

  //Iniciar botões
  pinMode(botao_verde, INPUT_PULLUP);
  pinMode(botao_amarelo, INPUT_PULLUP);
  pinMode(botao_vermelho, INPUT_PULLUP);
  pinMode(botao_azul, INPUT_PULLUP);

  azul = pixels.Color(0,0,255);
  verde = pixels.Color(0,255,0);
  vermelho = pixels.Color(255,0,0);
  amarelo = pixels.Color(255,120,0);

  //Alimentar array de cores
  cores[0] = azul;
  cores[1] = verde;
  cores[2] = vermelho;
  cores[3] = amarelo;

  //Alimentar o array de notas
  sons[0] = 264;
  sons[1] = 297;
  sons[2] = 330;
  sons[3] = 352;

  //Iniciar monitor serial
  Serial.begin(9600);

  randomSeed(analogRead(0));
}

// the loop routine runs over and over again forever:
void loop() {

int res_botao;

  if( isr_flag == 1 ) {
    Serial.println("Entrou");
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  else
  {
    res_botao = verifica_botao();

    if(res_botao!=-1)
    {
      atualiza_hora();
      acender_leds(cores[res_botao], 500);
    }
  }
  
}

void atualiza_hora()
{
  //Gerenciar marcadores de horario
  unsigned long time;
  time = millis();
  int somar;
  unsigned long resto;

  unsigned long add;
  add = (time - start)/1000;
  resto = (time - start)%1000;

  start = time - resto;

  //Alterar o horario do sistema
  segundo = segundo + add;
  add = segundo/60;
  segundo = segundo%60;

  minuto = minuto + add;
  add = minuto/60;
  minuto = minuto%60;
  
  hora = hora + add;
  hora = hora%24;

  Serial.print(hora,DEC);
  Serial.print(":");
  Serial.print(minuto,DEC);
  Serial.print(":");
  Serial.println(segundo,DEC);
  
}

void acender_leds(uint32_t cor, int timing)
{
  acende_leds(hora, 0, 1, 13, 2, 3, 4, 5, cor);
  acende_leds(minuto, 6, 7, 8, 9, 10, 11, 12, cor);
  acende_leds(segundo, 13, 14, 15, 16, 17, 18, 19, cor);

  pixels.show();
  delay(timing);
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
        Serial.println("Iniciar GENIUS");
        iniciar_genius();
        break;
      case DIR_NEAR:
      case DIR_FAR:
        Serial.println("Mostrar o horário");
        mostrar_horario();
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
  cor_s = random(4);

  //colocar a cor na última posição do array
  genius_mem[lastPos] = cor_s;

  //Incrementar o contador
  lastPos = lastPos + 1;

  //Mostrar sequencia
  showGeniusSeq();

  //Conferir a entrada do usuario
  confereEntrada();
  lastPos = 0;
  
}

void showGeniusSeq()
{

  for(int i=0; i<lastPos; i++)
  {
    atualiza_hora();
    tone(saida_som, sons[genius_mem[i]]);
    acender_leds(cores[genius_mem[i]], 500);
    noTone(saida_som);
    apaga_leds();
    delay(250);
  }
  
}


void confereEntrada()
{
  int i = 0;
  
  for(i=0; i<lastPos; i++)
  {
    if(genius_mem[i] == le_botao())
    {
      //Se for igual, piscar a cor e passar para o próximo
      atualiza_hora();
      tone(saida_som, sons[genius_mem[i]]);
      acender_leds(cores[genius_mem[i]], 500);
      noTone(saida_som);
      apaga_leds();
    }
    else
    {
      //Senao, limpar o array e sair dos metodos
      tone(saida_som, 392);
      delay(250);
      tone(saida_som, 262);
      delay(500);
      noTone(saida_som);
      break;
    }
  }

  if(i==lastPos)
  {
    //Terminou a sequência - sortear uma nova cor e reiniciar
    delay(1000);
    ativar_genius();
  }
  else
  {
    //Não terminou a sequência, sair
  }
  
}

//Ver qual botão está ligado
int le_botao() 
{

  int result = -1;
  
  while (result == -1) 
  {
    if (digitalRead(botao_azul) == LOW) 
    {
      result = 0;
    }
    else if (digitalRead(botao_verde) == LOW) 
    {
      result = 1;
    }
    else if (digitalRead(botao_vermelho) == LOW) 
    {
      result = 2;
    }
    else if (digitalRead(botao_amarelo) == LOW)
    {
      result = 3;
    }
  }
  
  return result;
}

//Método que faz a verificação do botão corrente
int verifica_botao()
{
  int result = -1;

  if (digitalRead(botao_azul) == LOW) 
  {
    result = 0;
  }
  else if (digitalRead(botao_verde) == LOW) 
  {
    result = 1;
  }
  else if (digitalRead(botao_vermelho) == LOW) 
  {
    result = 2;
  }
  else if (digitalRead(botao_amarelo) == LOW)
  {
    result = 3;
  }

 return result;
}

//Método para mostrar o horário durante 5 segundos
void mostrar_horario()
{
  int i;
  
  //1) Sortear a cor
  int cor1, cor2, cor3;
  cor1 = random(256);
  cor2 = random(256);
  cor3 = random(256);

  for(i = 0; i < 5; i++)
  {
      atualiza_hora();
      acender_leds(pixels.Color(cor1, cor2, cor3), 1000);
  }
}

