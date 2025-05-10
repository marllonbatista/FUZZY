//********** Bibliotecas internas ***********
#include <main.h>

// variaveis
const uint16_t setpoint=3500;
float kp_base=0;
float kd_base=0;
bool run=0;
const int8_t led= 2;
uint8_t contador =0;
float erro_banco=0;
//********** helice **********
const uint8_t helices = 22;
bool libera_helices=false;
long int tempo_helices=0;

float dados_erro[20];
float dados_kp[20];
unsigned long tempo_anterior = 0;
const unsigned long intervalo = 200; // 200 ms
int8_t indice_erro =0;

void registrarErro(float erro_atual);
//***** funcao_setup *****
void setup(){
    Serial.begin(115200);
    pinMode(led,OUTPUT);
    //********** helices  *****/
    pinMode(helices,OUTPUT);

    comunicacao_setup();
    //***** configuracao dos motores *****
    motor_d.configuracao(21,19,2,3);
    motor_e.configuracao(18,4,0,1);
    //***** configuracao do modulo sensor de 8 canais *****
    qtr_setup();
    camelinho.stop();
}
//***** funcao_loop *****
void loop(){
  funcao_bluetooth();//funcao responsavel por receber o comando do app
   //Condicao start
   if(run){
    camelinho.start();
    unsigned long tempo_atual = millis();

    if (tempo_atual - tempo_anterior >= intervalo) {
    tempo_anterior = tempo_atual;
      registrarErro(erro_banco);
    }
  } 
}
void registrarErro(float erro_atual) {
  if (indice_erro < 20) {
    dados_erro[indice_erro]= erro_atual;
    dados_kp[indice_erro]=kp;
    indice_erro++;
  }
}

