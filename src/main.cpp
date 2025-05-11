//********** Bibliotecas internas ***********
#include <main.h>

/* variaveis */
const uint16_t setpoint=3500;
float kp_base=0;
float kd_base=0;
bool run=0;
const int8_t led= 2;

/*  helice     */
const uint8_t helices = 22;
bool libera_helices=true;

const int8_t Qtd_dados = 40;
float vetor_erro[Qtd_dados];
float vetor_kp[Qtd_dados];
float vetor_kd [Qtd_dados];
unsigned long tempo_anterior = 0;
const int8_t intervalo = 200; // 200 ms
int8_t indice_erro =0;

/*    Funcoes   */
void funcao_registrarDados(float erro_atual, float kp_atual, float kd_atual);
void funcao_envia_dados();
void funcao_reseta_variaveis();
/* funcao_setup */
void setup(){
    Serial.begin(115200);
    pinMode(led,OUTPUT);
    /* helices  */
    pinMode(helices,OUTPUT);

    comunicacao_setup();
    /* configuracao dos motores */
    motor_d.configuracao(21,19,2,3);
    motor_e.configuracao(18,4,0,1);
    /* configuracao do modulo sensor de 8 canais */
    qtr_setup();

    camelinho.stop();
}
/* funcao_loop */
void loop(){
  funcao_bluetooth();/*funcao responsavel por receber o comando do app*/
   /*Condicao start*/
   if(run){
    camelinho.start();
    unsigned long tempo_atual = millis();
    if (tempo_atual - tempo_anterior >= intervalo) {
      tempo_anterior = tempo_atual;
      funcao_registrarDados(erro,kp,kd);
    }
  } 
}
/* FUNCAO PARA REGISTRAR OS DADOS  */
void funcao_registrarDados(float erro_atual, float kp_atual, float kd_atual) {
  if (indice_erro < Qtd_dados) {
    vetor_erro[indice_erro]= erro_atual;
    vetor_kp[indice_erro]=kp_atual;
    vetor_kd [indice_erro] = kd_atual;
    indice_erro++;
  }
}
/* FUNCAO PARA ENVIAR DADOS  */
void funcao_envia_dados(){
  /* ERRO */
  SerialBT.println(" VALORES DE ERRO");
  for (int i = 0; i < Qtd_dados; i++) {    
    SerialBT.println(vetor_erro[i]);
  }
  /* KP */
  SerialBT.println(" VALORES DE KP");
  for (int i = 0; i < Qtd_dados; i++) {
    SerialBT.println(vetor_kp[i]);
  }
  /* KD */
  SerialBT.println(" VALORES DE KD");
  for (int i = 0; i < Qtd_dados; i++) {
    SerialBT.println(vetor_kd[i]);
  }
  
}
/* FUNCAO PARA RESETAR AS VARIAVEIS */
void funcao_reseta_variaveis(){
  run=false;
  erro_anterior=0;
  kp=kp_base;
  kd=kd_base;
  libera_helices=true;
  indice_erro=0;

  for (int i = 0; i < Qtd_dados; i++) {
    vetor_erro[i] = 0;
    vetor_kp[i]=0;
    vetor_kd[i]=0;
  }
}


