#include <Fuzzy.h>

float G_membership=0; /* ERRO GRANDE POSITIVO */ 
float P_membership =0; /* ERRO PEQUENO POSITIVO */
float Z_membership=0; /* ERRO ZERO POSITIVO */

float GN_membership=0; /* ERRO GRANDE NEGATIVO */
float PN_membership =0; /* ERRO PEQUENO POSITIVO */
float ZN_membership=0; /* ERRO ZERO NEGATIVO */
/*                FUNCOES           */
float triangular_decrescente(float x, int16_t minimo, int16_t maximo);
float triangular_crescente(float x, int16_t minimo, int16_t maximo);
void funcao_pertinencia_entrada_erro(float y);
void funcao_pertinencia_entrada_delta_erro(float z,float delterro);
float funcao_agregacao_kp();
float funcao_agregacao_kd();
void funcao_defuzzificacao(float delta_kp, float delta_kd);

/* FUNCAO FUZZY  */
void funcao_fuzzy(float erro,float erro_anterior){
    /* ZERA AS PERTINENCIAS */
    G_membership = P_membership = Z_membership = 0.0;
    GN_membership=0.0; PN_membership =0.0; ZN_membership =0.0;
    /* KP */
    float erro_abs= fabs(erro);
    funcao_pertinencia_entrada_erro(erro_abs);
    float delta_kp=funcao_agregacao_kp();
    /* ZERA AS PERTINENCIAS */
    G_membership = P_membership = Z_membership = 0.0;
    /* KD */
    float delt_erro = erro - erro_anterior;
    float delt_erro_abs=fabs(delt_erro);
    funcao_pertinencia_entrada_delta_erro(delt_erro_abs,delt_erro);
    float delta_kd = funcao_agregacao_kd();
    /* DEFUZZIFICACAO */
    funcao_defuzzificacao(delta_kp,delta_kd);
}
 /* defuzzificação (centroide p/singletons) DEFUZZIFICACAO (CENTROIDE P/SINGLETONS) */
 void funcao_defuzzificacao(float delta_kp, float delta_kd){
    kp = kp_base + delta_kp;
    kd = kd_base +  delta_kd;   
 }
 /* FUNCAO AGREGACAO PARA SAIDA KD */
 float funcao_agregacao_kd(){
   /* CENTRO DAS FUNCOES DE SAIDA */
    const float centro_D = -10;  /* DIMINUI KD */
    const float centro_M = 0.0;  /* MANTEM KD */
    const float centro_A = 10;   /*AUMENTA KD */
    /* AGREGACAO METODO DO CENTRO DE GRAVIDADE */ 
    float numerador = (G_membership * centro_D) + ( P_membership * centro_D) +
        (Z_membership* centro_M) + (ZN_membership* centro_M)+
        ( GN_membership * centro_A) + ( PN_membership * centro_A) ;

    float denominador = P_membership + PN_membership+ 
        Z_membership + ZN_membership + G_membership + GN_membership ;

     if (denominador != 0) {
         return( numerador / denominador);
     }else{
        return 0;
     }     
}
/* FUNCAO AGREGACAO PARA SAIDA KP */
float funcao_agregacao_kp(){
    /* CENTRO DAS FUNCOES DE SAIDA */
    const float centro_D = -4;  /* DIMINUI KD */
    const float centro_M = 0.0; /* MANTEM KD */
    const float centro_A = 4;  /*AUMENTA KD */

    /* AGREGACAO METODO DO CENTRO DE GRAVIDADE */ 
     float numerador = (P_membership * centro_D) +
        (Z_membership * centro_M) +
        (G_membership * centro_A);

     float denominador = P_membership + Z_membership + G_membership;

     if (denominador != 0) {
         return( numerador / denominador);
     }else{
        return 0;
     }    
}
/* FUNCAO DE PERTINENCIA PARA ENTRADA DERIVADA DO ERRO */
void funcao_pertinencia_entrada_delta_erro(float z, float delterro){
    /* ERRO GRANDE */ 
    if  (60 <= z && z <= 100){
        if (delterro >=0){
            G_membership = triangular_crescente(z,60,100);
        }
        else{
            GN_membership = triangular_crescente(z,60,100);
        }   
    }
    /* ERRO PEQUENO */ 
    if(20<= z and z<= 50){
        if (delterro >=0){
            P_membership = triangular_crescente(z,20,50);
        }else{
            PN_membership = triangular_crescente(z,20,50);
        }    
    }
   else  if (50< z and z<=80){
        if (delterro >=0){
            P_membership = triangular_decrescente(z,80,50);
        }else{
            PN_membership = triangular_decrescente(z,80,50);
        } 
    }
    /* ERRO ZERO  */ 
    if( 0 <= z and  z<= 40){
        if (delterro >= 0){
            Z_membership = triangular_decrescente(z,40,0);
        }else{
            ZN_membership = triangular_decrescente(z, 40, 0);
        }   
    }  
}
/* FUNCAO DE PERTINENCIA PARA ENTRADA  ERRO */
void funcao_pertinencia_entrada_erro(float y){
    /* ERRO GRANDE  */
    if  (60 <= y && y <= 100){
         G_membership = triangular_crescente(y,60,100);
    }
    /* ERRO PEQUENO  */
    if(20<= y and y<= 50){
        P_membership = triangular_crescente(y,20,50);
    }
    else  if (50<y and y<=80){
        P_membership = triangular_decrescente(y,80,50);
    }
    /* ERRO ZERO */
    if (0<=y and y<= 40){
        Z_membership = triangular_decrescente(y,40,0);
    }
}

float triangular_decrescente(float x, int16_t minimo, int16_t maximo){
    float saida;
    saida = (minimo - x)/ float (minimo - maximo);
    return constrain(saida, 0.0, 1.0);
}

float triangular_crescente(float x, int16_t minimo, int16_t maximo){
    float saida;
    saida = ( x - minimo)/ float(maximo - minimo);
    return constrain(saida, 0.0, 1.0);
}