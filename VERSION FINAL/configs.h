#ifndef CONFIGS_H    //Evitamos definiciones múltiples
#define CONFIGS_H

	#include "standard.h"   //Generalización de datos constantes y definiciones que son públicas para todos los módulos
    #include <string.h>

	//Recibe la dificultad que se busca y devuelve la configuración correspondiente.
    configs_t set_difficulty(unsigned int dificultad);

    void find_highscore(highscore_t * storage);  // extrae highscores del txt y los guarda en el puntero que se le manda
    int process_score( int new_score, highscore_t * storage); // se fija si hay nuevo record, recordar que devuelve -1 si no lo hay!
    // si pones el mismo puntaje que alguien que esta ultimo no funciona ( segmentation fault ) 
    // es porque HAY que CHEQUEAR que "process_score()" NO devuelva -1
    void shift_scores(int new_score, int indice, highscore_t * storage); // mueve los puntajes en la lista de scores del highscore_t que se le manda
    void add_string_to_array(char array[3][10], char* new_string, int index); // agrega un nombre en el index y mueve los nombres en la lista de highscore_t names
    void store_highscore(highscore_t * storage); // guarda los datos actualizados en el txt
    
    
#endif /*CONFIGS_H*/