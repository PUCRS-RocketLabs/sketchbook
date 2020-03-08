//-----=PUCRS = ROCKET = LABS=--------
//    
//  Altьmetro MK1
//  Autores: L. Giacomazzi, R. Wuerdig.
//  Processador: ATMEGA 328P
//  Perifжricos: BMP 180 (BarЗmetro e TermЗmetro)
//               LED
//               Servo 9G(modificado para atuar como motor DC)

/** Change log
 * 
 * Reindentation to save horizontal space
 * Move getPressure() function to before setup, if using a prototype for
 * this function the function can go back to the end or to another file...
 */


#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 altimetro; // Objeto sensor

//Definiушes Globais
#define PQD_SERVO 9          //Endereуo do motor de acionamento da recuperaусo
#define LED_1 7              //Endereуo do LED 
#define intervalo 50         //Intervalo de coleta de dados do sensor BMP 180
#define abertura 3.0         //Distância de abertura do paraquedas apзs apogeu.
#define t_acionamento 3000   //Tempo em que o servo fica ativo
#define t_separa 1000        //Tempo entre amostras de informaусo
#define t_info 300           //Intervalo da piscada de informaусo

//Variaveis
boolean armado = false;       //Situação do paraquedas
boolean pisca = false;        //Comutador do pisca-pisca
double nivel;                 //Pressсo de referЖncia para altitude = 0
double pres;                  //Pressсo instantРnea
double alti;                  //Altitude instantРnea
double apogeu=0;              //Armazenamento do Apogeu
double mem_apg=0;             //Variável de trabalho do Apogeu
int disp_alt[4];              //Vetor de dados para os algarismos de altitude
int cont;                     //Contador (aux)


double getPressure(){
  char status;
  double T,P,p0,a;

  status = altimetro.startTemperature();      //Inicializa o termometro, retorna o tempo de espera em ms ou 0 para erro
  if (status != 0)
    {
      delay(status);                            //Espera atж a finalizar a leitura
      status = altimetro.getTemperature(T);     //LЖ a temperatura e armazena em T.
    
      if (status != 0)
	{
	  status = altimetro.startPressure(3);    //inicializa o barometro.
	  if (status != 0)
	    {
	      delay(status);
	      status = altimetro.getPressure(P,T);  //LЖ a pressсo e armazena em P, com T como temperatura de referЖncia

	      if (status != 0)
		{
		  return(P);            
		}
	    }
	}
    }
}

void setup(){
  pinMode(PQD_SERVO, OUTPUT);
  pinMode(LED_1, OUTPUT);
  Serial.begin(9600);
  
  if(altimetro.begin()) {
    nivel = getPressure();                    //inicializa o sensor
    armado  = true;                           //paraquedas armado
  }
  else {
    while(1)
      digitalWrite(LED_1,HIGH);             //Mantжm o LED ligado, sinalizando falha.
  }
}

void loop() {
  
  while(armado==true){
          
    pres = getPressure(); 
    alti = altimetro.altitude(pres,nivel);    //Coleta pressсo e determina altitude em relaусo ao nьvel capturado ao iniciar o programa
    Serial.println(alti);
          
    digitalWrite(LED_1,pisca);                //Pisca o LED a cada leitura, informando sistema sadio.
    pisca=!pisca;
  
    if (alti>apogeu)
      apogeu=alti;                          //Armazena o apogeu
    else {
      if (apogeu-alti>abertura){            //Aciona o pqd em uma distРncia determinada abaixo do apogeu.
	digitalWrite(PQD_SERVO,HIGH);
	delay(t_acionamento);           //Aciona o Servo do PQD
	digitalWrite(PQD_SERVO,LOW);
	armado=false;                   //Desarma o parquedas e quebra o loop.
      }
    }
    delay(intervalo);                         //Tempo entre leituras
  }
  Serial.print("ABERTURA - Apogeu:");
  Serial.println(apogeu);

  //algoritimo do LED para informar altitude de apogeu
  
  mem_apg=apogeu;                                    //Armazena o apogeu em uma variрvel de trabalho, conservando o valor original para referЖncia
  for(cont=0;cont<3;cont++) {                         //Tres piscadas rрpidas sinalizam inьcio de mensagem.
    digitalWrite(LED_1,HIGH);
    delay(75);
    digitalWrite(LED_1,LOW);
    delay(75);
  }
  delay(t_separa);
  
  // piscando a altitude do apogeu
  
  disp_alt[3]=mem_apg/1000.0;

  for(cont=0;cont<disp_alt[3];cont++) {
    digitalWrite(LED_1,HIGH);
    delay(t_info);
    digitalWrite(LED_1,LOW);
    delay(t_info);
  }
  mem_apg=mem_apg-(disp_alt[3]*1000.0);
  delay(t_separa);  //--------------------------------------
  
  disp_alt[2]=mem_apg/100.0;
  mem_apg=mem_apg-(disp_alt[2]*100.0);
  
  if(disp_alt[2]==0 && disp_alt[3]>0)
    disp_alt[2]=10;  
                                
  for(cont=0;cont<disp_alt[2];cont++) {
    digitalWrite(LED_1,HIGH);
    delay(t_info);
    digitalWrite(LED_1,LOW);
    delay(t_info);
  }
 
  delay(t_separa); //------------------------------------------
  
  disp_alt[1]=mem_apg/10.0;                              
  mem_apg=mem_apg-(disp_alt[1]*10.0);
  
  if(disp_alt[1]==0 && disp_alt[2]>0)
    disp_alt[1]=10;  
    
  for(cont=0;cont<disp_alt[1];cont++) {
    digitalWrite(LED_1,HIGH);
    delay(t_info);
    digitalWrite(LED_1,LOW);
    delay(t_info);
  }
  
  delay(t_separa);  //--------------------------------------
  
  disp_alt[0]=mem_apg;
  if(disp_alt[0]==0)
    disp_alt[0]=10;
    
  for(cont=0;cont<disp_alt[0];cont++) {
    digitalWrite(LED_1,HIGH);
    delay(t_info);
    digitalWrite(LED_1,LOW);
    delay(t_info);
  }  
  delay(t_separa);  //--------------------------------------
}

