 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

int dez_hora_1 = 2;
int dez_hora_2 = 3;

int unid_hora_1 = 5;
int unid_hora_2 = 6;
int unid_hora_4 = 7;
int unid_hora_8 = 8;


int dez_minuto_1 = 0; //TODO - FIX
int dez_minuto_2 = 0; //TODO - FIX
int dez_minuto_4 = 0; //TODO - FIX

int unid_minuto_1 = 0;//TODO - FIX
int unid_minuto_2 = 0;//TODO - FIX
int unid_minuto_4 = 0;//TODO - FIX
int unid_minuto_8 = 0;//TODO - FIX

int dez_segundo_1 = 0; //TODO - FIX
int dez_segundo_2 = 0; //TODO - FIX
int dez_segundo_4 = 0; //TODO - FIX

int unid_segundo_1 = 0;//TODO - FIX
int unid_segundo_2 = 0;//TODO - FIX
int unid_segundo_4 = 0;//TODO - FIX
int unid_segundo_8 = 0;//TODO - FIX

unsigned long t;

int hora;
int minuto;
int segundo;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(dez_hora_1, OUTPUT);
  pinMode(dez_hora_2, OUTPUT);

  pinMode(unid_hora_1, OUTPUT);
  pinMode(unid_hora_2, OUTPUT);
  pinMode(unid_hora_4, OUTPUT);
  pinMode(unid_hora_8, OUTPUT);

  pinMode(dez_minuto_1, OUTPUT);
  pinMode(dez_minuto_2, OUTPUT);
  pinMode(dez_minuto_4, OUTPUT);

  pinMode(unid_minuto_1, OUTPUT);
  pinMode(unid_minuto_2, OUTPUT);
  pinMode(unid_minuto_4, OUTPUT);
  pinMode(unid_minuto_8, OUTPUT);

  pinMode(dez_segundo_1, OUTPUT);
  pinMode(dez_segundo_2, OUTPUT);
  pinMode(dez_segundo_4, OUTPUT);

  pinMode(unid_segundo_1, OUTPUT);
  pinMode(unid_segundo_2, OUTPUT);
  pinMode(unid_segundo_4, OUTPUT);
  pinMode(unid_segundo_8, OUTPUT);

  t = millis();
  Serial.begin(9600);
  
  hora = 17;
  minuto = 59;
  segundo = 40;
}

// the loop routine runs over and over again forever:
void loop() {
  
  Serial.print(hora,DEC);
  Serial.print(":");
  Serial.print(minuto,DEC);
  Serial.print(":");
  Serial.println(segundo,DEC);
  
  atualiza_hora();
  acender_leds_hora();
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

void acender_leds_hora()
{
  int resto;
  
  switch(hora/10)
  {
    case 0:
      digitalWrite(dez_hora_1, LOW);
      digitalWrite(dez_hora_2, LOW);
      break;
    case 1:
      digitalWrite(dez_hora_1, HIGH);
      digitalWrite(dez_hora_2, LOW);
      break;
    case 2:
      digitalWrite(dez_hora_1, LOW);
      digitalWrite(dez_hora_2, HIGH);
      break;
  }
  
  resto = hora%10;
  
  if(resto/8 == 1)
  {
    digitalWrite(unid_hora_8, HIGH);
    resto = resto - 8;
  }
  else
  {
    digitalWrite(unid_hora_8, LOW);
  }
    
  if(resto/4 == 1)
  {
    digitalWrite(unid_hora_4, HIGH);
    resto = resto - 4;
  }
  else
  {
    digitalWrite(unid_hora_4, LOW);
  }
  
  if(resto/2 == 1)
  {
    digitalWrite(unid_hora_2, HIGH);
    resto = resto - 2;
  }
  else
  {
    digitalWrite(unid_hora_2, LOW);
  }
  
  if(resto==1)
  {
    digitalWrite(unid_hora_1, HIGH);
  }
  else
  {
    digitalWrite(unid_hora_1, LOW);
  }
}

void acender_minuto(){
  //dezena
  int dezena = minuto/10;

  if(dezena >= 4){
    digitalWrite(dez_minuto_4, HIGH);
    dezena = dezena - 4;
  }else {
    digitalWrite(dez_minuto_4, LOW);
  }

  if(dezena >= 2){
    digitalWrite(dez_minuto_2, HIGH);
    dezena = dezena - 2;
  }else {
    digitalWrite(dez_minuto_2, LOW);
  }

  if(dezena >= 1){
    digitalWrite(dez_minuto_1, HIGH);
    dezena = dezena - 1;
  }else {
    digitalWrite(dez_minuto_1, LOW);
  }

  //unidade
  int unidade = minuto%10;
  if(unidade >= 8){
    digitalWrite(unid_minuto_8, HIGH);
    unidade = unidade - 8;
  }else {
    digitalWrite(unid_minuto_8, LOW);
  }

  if(unidade >= 4){
    digitalWrite(unid_minuto_4, HIGH);
    unidade = unidade - 4;
  }else {
    digitalWrite(unid_minuto_4, LOW);
  }

  if(unidade >= 2){
    digitalWrite(unid_minuto_2, HIGH);
    unidade = unidade - 2;
  }else {
    digitalWrite(unid_minuto_2, LOW);
  }

  if(unidade >= 1){
    digitalWrite(unid_minuto_1, HIGH);
    unidade = unidade - 1;
  }else {
    digitalWrite(unid_minuto_1, LOW);
  }
}


void acender_segundo(){
  //dezena
  int dezena = segundo/10;

  if(dezena >= 4){
    digitalWrite(dez_segundo_4, HIGH);
    dezena = dezena - 4;
  }else {
    digitalWrite(dez_segundo_4, LOW);
  }

  if(dezena >= 2){
    digitalWrite(dez_segundo_2, HIGH);
    dezena = dezena - 2;
  }else {
    digitalWrite(dez_segundo_2, LOW);
  }

  if(dezena >= 1){
    digitalWrite(dez_segundo_1, HIGH);
    dezena = dezena - 1;
  }else {
    digitalWrite(dez_segundo_1, LOW);
  }

  //unidade
  int unidade = segundo%10;
  if(unidade >= 8){
    digitalWrite(unid_segundo_8, HIGH);
    unidade = unidade - 8;
  }else {
    digitalWrite(unid_segundo_8, LOW);
  }

  if(unidade >= 4){
    digitalWrite(unid_segundo_4, HIGH);
    unidade = unidade - 4;
  }else {
    digitalWrite(unid_segundo_4, LOW);
  }

  if(unidade >= 2){
    digitalWrite(unid_segundo_2, HIGH);
    unidade = unidade - 2;
  }else {
    digitalWrite(unid_segundo_2, LOW);
  }

  if(unidade >= 1){
    digitalWrite(unid_segundo_1, HIGH);
    unidade = unidade - 1;
  }else {
    digitalWrite(unid_segundo_1, LOW);
  }
}