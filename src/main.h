#ifndef MAIN_H // Verifica se MAIN ainda não foi definido
#define MAIN_H // Define MAIN_H

 #include <Arduino.h>

//********** Bibliotecas internas ***********
#include <Motor.h>
#include <Pd.h>
#include <Comunicacao.h>
#include <Estado.h>
#include <Qtr.h>
#include <Fuzzy.h>

// Variáveis globais
extern bool run;

extern const uint16_t setpoint;
extern float kp_base;
extern float kd_base;

extern const int8_t led;

extern const uint8_t helices;
extern bool libera_helices;

extern void funcao_registrarDados(float erro_atual, float kp_atual, float kd_atual);
extern void funcao_envia_dados();
extern void funcao_reseta_variaveis();

#endif // Fim da verificação
