#include <Wire.h>
#include "RTClib.h"

// *****************************************************************************
// Requisitos para as rotinas de manipulacao de arquivos.
#include "limits.h"
#include "SD.h"
bool      SDok             = false;      // Se SD foi inicializado corretamente
int       nivelDeDebug = 0;              // Nivel de debug (caregado do cartao)
File      arquivo, planilha, diario, forca;
// *****************************************************************************

const String VERSAO = "0.0.1";

RTC_DS1307 RTC;

DateTime now;
String        horaString, ultimoHorario, dataString, ultimaData;

unsigned long distancia = 0, ultimaDistanciaTotal = 0, ultimaDistancia = 0;
unsigned long distanciaHoraZero = 0;
float         distanciaReal = 0;
String        strDistanciaReal = "", strDistanciaDiariaReal = "";
float         distanciaDiariaReal = 0;
String        strDistancia = "";
String        strDistanciaDiaria = "";
unsigned long tempoDeCiclo = 0;
bool          mostraDoisPontos = false;
unsigned int  contadorDeDias = 0;

unsigned int  horaDoReset = 0;
boolean       reseteiDisplay = false;

unsigned int  instanteSemSD = 0;
unsigned int  tempoParaVerificarSD = 10000; // 10 mil milesegundos: 10 segundos

int           ultimaHora = 0;

boolean       bootup = true;

void inicializaSD(){
  Serial.print("Inicializando SD...");  
  pinMode(53, OUTPUT); // trocado de 11 para 53 para usar Arduino Mega
  if (!SD.begin(4)) {
    Serial.println(" NOK");    
    SDok = false;
  }
  else{
    SDok = true;
    Serial.println(" OK");
  }  
}

String formataData(void){
  String retorno = "";
  retorno  = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
  retorno += "/";
  retorno += (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
  retorno += "/";
  retorno += String(now.year(), DEC);
  return retorno;
}

String formataHoraReset(unsigned int hora){
  String retorno = "";  
  retorno  = (hora < 10 ? "0" : "") + String(hora, DEC);
  retorno += ":00:00";
  return retorno;  
}

String formataHora(void){
  String retorno = "";  
  retorno  = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
  retorno += ":";
  retorno += (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
  retorno += ":";
  retorno += (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
  return retorno;
}

// Interrupcao 4 esta associada ao pino 19 na Mega
void setup(){
  analogReference(INTERNAL1V1);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  
  pinMode(19, OUTPUT);
  bootup = true;
  Serial.begin(9600);
  while (!Serial);{
    ; // wait for serial port to connect. Needed for Leonardo only
  }    
  Serial.println("Dinamotro de forca de corte - " + VERSAO + " (PUCRS 2019)");
  Serial.print("Configurando RTC...");
  Wire.begin();
  RTC.begin();
  // retirar o "!" da linha " if (! rtc.isrunning()) {"
  // desta forma ele ficara sempre atualizando, visto que o código 
  // ficara da seguinte forma "se RTC rodando atualize"
  //if(!RTC.isrunning()) {
  //  RTC.adjust(DateTime(__DATE__,__TIME__));
  //}
    //Serial.println(__DATE__);  
    //Serial.println(__TIME__);  
    //RTC.adjust(DateTime("Jun  5 2016","23:59:50"));

  Serial.println("  [ OK ]");

  inicializaSD();
  if (SDok){ // Caso o SD tenha inicializado, abre o arquivo com as horas.
    Serial.print("Verificando forca.cvs... ");
    forca = SD.open("forca.csv");
    if (forca){
      forca.close();
      Serial.println("[ OK ]");
    }else{
      Serial.print("Nao encontrado, criando um novo...");
      forca = SD.open("forca.csv", FILE_WRITE);    
      forca.println("millis; Data; Hora; valor ad");
      forca.close();
    }
    
  }   
  now = RTC.now();
  dataString = formataData();
  horaString = formataHora();
  
  if ( (dataString != ultimaData) && (horaString == formataHoraReset(horaDoReset)) && !reseteiDisplay && horaDoReset ) {  
    distanciaHoraZero = distancia;
    reseteiDisplay = true;
  }
  if ( (horaString != formataHoraReset(horaDoReset)) && reseteiDisplay && horaDoReset )
    reseteiDisplay = false;
  
  ultimaHora = now.hour();
}
  
void loop(){
  now = RTC.now();
  dataString = formataData();
  horaString = formataHora();

  if (!SDok && (millis() - instanteSemSD > tempoParaVerificarSD) ){
    inicializaSD();
    instanteSemSD = millis();
  }
  
  if (horaString != ultimoHorario) {    
    if (!mostraDoisPontos) horaString.replace(":", " ");
    if (!mostraDoisPontos) horaString.replace(" ", ":");
    mostraDoisPontos = !mostraDoisPontos;
    Serial.print(dataString);       
    Serial.print(" ");
    Serial.print(horaString);
    Serial.print(" ");
    Serial.println(analogRead(A0));
  }
  //forca.println("millis; Data; Hora; valor ad");
  forca = SD.open("forca.csv", FILE_WRITE);
  forca.print(millis());
  forca.print(";"); forca.print(dataString);
  forca.print(";"); forca.print(horaString);
  forca.print(";"); forca.print(analogRead(A0));
  forca.println(";");
  forca.close();
}
