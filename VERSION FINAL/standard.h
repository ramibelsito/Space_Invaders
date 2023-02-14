/*Al compilar debemos escribir -DALLEGRO
o -DRASPI para definir ALLEGRO o RASPI*/

/*standard.h se incluye en los headers y no en los source files porque los headers usan las
definiciones de standard.h, entonces no podrían cargar los prototipos, etc.*/

//#define ALLEGRO //Esto está para que no salten errores de sintaxis mientras codeamos.

#ifndef STANDAR_H    //Evitamos definiciones múltiples
#define STANDAR_H


#include <stdio.h>	//Librerías estándar que usan todos los módulos.
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


    #ifdef RASPI

    	#include "disdrv.h"         //Display
        #include "joydrv.h"         //inputs

        #define N_ALIENS 3          // cantidad de aliens por fila
        #define J_FILAS 2           // cantidad de filas de aliens
        #define M 3                 // cantidad de barreras

		//Es para que el jugador no abuse de que ya sea fácil matar a la nave nodriza cuando hay menos aliens
		#define MIN_CANT_ALIENS_P_NODRIZA 1		//Si no hay más de X aliens, ya no aparece la nave nodriza
        
		#define MAX_DISP_X 16		//Dimensiones del display
        #define MAX_DISP_Y 16

        #define HITBOX_NODRIZA_X 2	//Hitboxes (son todas rectangulares)
        #define HITBOX_NODRIZA_Y 1
        #define HITBOX_ALIENS_X 3 
        #define HITBOX_ALIENS_Y 2
        #define HITBOX_BALAS_X 0
		#define HITBOX_BALAS_Y 0
        #define HITBOX_JUGADOR_X 3
        #define HITBOX_JUGADOR_Y 2
		#define HITBOX_BARRERA_X 3
		#define HITBOX_BARRERA_Y 2
        #define ALLEGRO_CORRECTION_X 0

        #define THRESHOLD 100	//Sensibilidad del analógico.

        #define SIZE_HEART 1	//Tamaño de algunas imágenes
        #define SIZE_FONT_SCORE 0

        #define FRECUENCIA_ALIENS 500	//No son literalmente la frecuencia de disparo, 
        #define DISPARO_RASPI 0.85 		//sino constantes que se usan para determinarla
        #define VELOCIDAD_BALAS 500*ONE_MS/3   

        #define BARRERA_POS_Y 9	//Coordenada Y de la barrera (en raspi están hardcodeadas)
    #endif

    #ifdef ALLEGRO 
        
        #define N_ALIENS 12 			// cantidad de aliens a imprimir + 1 por fila
        #define J_FILAS 3 				// cantidad de filas a imprimir + 1
        #define M 5 					// cantidad de barreras
		
		//Es para que el jugador no abuse de que ya sea fácil matar a la nave nodriza cuando hay menos aliens
		#define MIN_CANT_ALIENS_P_NODRIZA 9		//Si no hay más de X aliens, ya no aparece la nave nodriza

        #define HITBOX_NODRIZA_X 100	//Hitboxes (son rectangulares)
        #define HITBOX_NODRIZA_Y 50
        #define HITBOX_ALIENS_X 50
        #define HITBOX_ALIENS_Y 50
        #define HITBOX_BALAS_X 5
        #define HITBOX_BALAS_Y 40
        #define HITBOX_JUGADOR_X 75
        #define HITBOX_JUGADOR_Y 25
		#define HITBOX_BARRERA_X 100
		#define HITBOX_BARRERA_Y 30

        #define MAX_DISP_X 1280		//Dimensiones del display
        #define MAX_DISP_Y 720

        #define FPS 60.0			//Fotogramas por segundo

        #define SIZE_HEART 15		//Tamaño de algunas imágenes
        #define SIZE_FONT_SCORE 20
        #define ALLEGRO_CORRECTION_X 30

        #define VELOCIDAD_BALAS 500*ONE_MS/15	//No son los datos como tales, sino 
        #define FRECUENCIA_ALIENS 500			//ctes que nos permiten determinarlos
        #define DISPARO_RASPI 1	

        #define BARRERA_POS_Y 200 	//No es la posición tal cual, sino un espacio extra que es constante
    #endif

    typedef struct 
	{
		int x;
		int y;
	}
	coords_t;

    typedef struct 
    {
        int posicion[2];    //Coordenadas en el display, X e Y, respectivamente
        int previous_position[2]; //Se utiliza para permitir que se desplace.
        float velocidad;    //Rapidez de la bala
        int active;			//Informa si está activa o no 
        int direccion;      //La bala se dispara hacia arriba (jugador) o hacia abajo (aliens). Debería decir "sentido"
        int colisiones;     //Indica contra qué chocó
    } 
	bullet_t;

    typedef struct
    {
        int posicion[2];	//Coordenadas en el display, X e Y, respectivamente
        int previous_position[2]; //Se utiliza para permitir que se desplace.
        unsigned int vida;	//Cantidad de golpes que resiste
        int disparar;		//Este campo se llena aleatoriamente. Si coincide con con determinado número, dispara.
        int colisiones;		//Indica contra que fue su última colisión (ejemplo: bala i le pegó)
        int tipo;           //Tipo de alien.
    } 
	aliens_t;

    typedef struct 
    {
        int posicion[2];	//Coordenadas en el display, X e Y, respectivamente
        unsigned int vida;	//Cantidad de golpes que resiste
        int colisiones;		//Indica contra que fue la última colisión (ejemplo: bala i le pegó)
    } 
	barrera_t;

    typedef struct
    {
        int posicion[2];	//Coordenadas en el display, X e Y, respectivamente
        unsigned int vida;	//Cantidad de golpes que resiste
        int colisiones;		//Indica contra cuál fue la última colisión (ejemplo: bala i le pegó)
		int puntaje;		//puntaje duh
	}
	jugador_t;

       typedef struct
    {
        char names[3][10];	//Tres nombres de hasta 10 caracteres c/u
        int scores[3];		//El puntaje correspondiente a cada nombre
    } 
	highscore_t;


    typedef struct
	{
		unsigned int dificultad;		//Informa cuál es el nivel que se debe inicializar
		int frecuencia_disparo; 		
		int frecuencia_nave_nodriza;	//No son literalmente las frecuencias o velocidades
		float velocidad_aliens; 		//sino datos que nos permiten determinarlas.
        int velocidad_balas;
		unsigned int vida_barreras;		//vida de las barreras
		unsigned int vida_aliens; 		//vida de los aliens
		unsigned int vida_player; 		//vida del jugador
        
	}
	configs_t;


	//Para que de lo mismo usar mayusculas o minusculas.
    #define MAY_MIN(c)  (((c) >= 'A' && (c) <= 'Z') ? (c) - 'A' + 'a' : (c))
    
    #define ERROR -11   //El -1 ya es otra cosa

    #define ONE_MS 1000
    #define UP 1
    #define DOWN -1
    #define ON 1
    #define OFF 0
    #define X 0
    #define Y 1
    #define OK 1

    #define JUEGO 8
    #define MENU 9
    #define SPACE_INVADERS 10
    #define PLAY 11
    #define HIGH_SCORES 12
    #define QUIT 13
    #define DIFICULTAD 14
	#define SELECT_DIFFICULTY 15
	#define NEXT_LEVEL 16
	#define CONTINUE 17
    #define RESTART 18
    #define SHOW_HIGHSCORE 19
    #define NEXT_LEVEL_ANIMATION 20
    #define PUNTAJE 21
    #define EXIT_PROGRAM 99

    #define DERECHA 1
    #define IZQUIERDA 2
    #define ARRIBA 3
    #define ABAJO 4
    #define ENTER 5
    #define ESCAPE 6

    #define MUERTO 0

    #define EASY 1
    #define MEDIUM 2
    #define HARD 3
	#define HARDCORE 4
	#define HELL 5

    #define DISP_MIN 0

    #define ALIEN 		'A'
    #define BARRERA 	'B'
    #define MAPA 		'M'
    #define JUGADOR 	'J'
    #define PROYECTIL	'P'
    #define BALA 		'P'

    #define N   J_FILAS*N_ALIENS	//Para conocer la cantidad total de aliens.
    
    #define NODRIZA 99

    #define ACELERATION  0.85
    #define ACELERATION_TIME 30
    #define ACELERATION_LIMIT 3

    #define NUMBER_X 3
    #define NUMBER_Y 5
    #define BUSQUEDA_MATRIZ_DIFICULTADES 30
    

#endif /* STANDAR_H */
