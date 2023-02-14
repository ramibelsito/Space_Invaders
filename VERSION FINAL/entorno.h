#ifndef ENVIROMENT_H	//Evitamos definiciones múltiples
#define ENVIROMENT_H

#include "standard.h"   //Generalización de datos constantes y definiciones que son públicas para todos los módulos

//Threads del backend
void * updateAliens	(); //Actualización de aliens (sus disparos y su desplazamiento)
void * updateBullets(); //Actualización de balas (desplazamiento)
void * colitions	();	//Actualización de balas (colisiones y sus consecuencias)

/**
 * @brief inicializa el backend segun la configuracion asignada. 
 * @param configuration Struct que contiene la configuracion de un nivel. 
 * @param nextLevelFlag Flag que indica si se paso de nivel
*/
void setBackend (configs_t configuration, int nextLevelFlag);
//(Inicializa el entorno en función de la config recibida)

/*Cada vez que incluya a "entorno", tengo el prototipo de
sus variables globales, evitando definiciones dobles*/
extern aliens_t alien [N + 1]; 
extern bullet_t balas [N + 1];
extern barrera_t barreras [M];
extern jugador_t jugador;
extern int stop;
extern int aliensVivos;
extern int frecuencia_disparo;
extern int frecuencia_nave_nodriza;
extern float multiplicador_movimiento;


#endif /*ENVIROMENT_H*/

/*Cuenta la leyenda que acá hubieron structs muy copados*/