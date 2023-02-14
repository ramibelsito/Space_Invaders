#ifndef PLAYER_H    //Evitamos definiciones múltiples
#define PLAYER_H

	#include "standard.h" //Generalización de datos constantes y definiciones que son públicas para todos los módulos
	
    //Devuelve la razón por la que se interrumpió el juego. Recibe la configuración del nivel y los objetos del nivel
    int init_game (configs_t configuration, int pausa, int next_level);

    //Devuelve la acción correspondiente a la tecla presionada. Por ejemplo: 'W' significa 'arriba'
    int input (void);

    void * updateDisplay(void *);   

    typedef struct
    {
        int * pausa;
        int * position;
        unsigned int * dificultad;
        highscore_t * highscore_p;
    }
    display_t;

#endif /*PLAYER_H*/
