#include "player.h"
#include "entorno.h"
#include <pthread.h>

#include "disdrv.h"         //Display
#include "joydrv.h"         //inputs
#include "dibujosRaspi.h"

#define IN_ALPHABET(X) ( ((X) >= 'A') && ((X) <= 'Z') )

static int printMenu(int opcion, void* frontend, int* pausa, highscore_t * highscore_p, unsigned int *dificultad);

static void printPuntaje ( int puntaje );

//Devuelve la razón por la que se interrumpió el juego. Recibe la configuración del nivel y los objetos del nivel
int init_game (configs_t configuration, int pausa, int next_level)
{

	//Si pausé, el "entorno" no debe ser modificado, para que todo esté igual al retomar.
	if (!pausa) {setBackend(configuration, next_level);}
	
	coords_t pos, npos;
	pos.x = jugador.posicion[X];
	pos.y = jugador.posicion[Y];
	npos = pos;

    //Inicializamos los threads
    pthread_t mov_balas, mov_aliens, colisiones;
	int ch2,ch3,ch4;	//Check
	stop = 0;
	
    ch2=pthread_create(&mov_balas,	NULL, updateBullets,NULL);	//Desplaza a las balas
    ch3=pthread_create(&colisiones,	NULL, colitions,	NULL);	//Chequea y actúa en consecuencia de las colisiones
    ch4=pthread_create(&mov_aliens,	NULL, updateAliens,	NULL);	//Desplaza a los aliens y los hace disparar aleatoriamente.

	if (ch2||ch3||ch4)	//Los check prenderán su flag si el thread no pudo crearse.
	{
		//Si los threads no se crean correctamente, los cerramos e informamos que hubo un error.
		pthread_join (mov_aliens, NULL);
		pthread_join (colisiones, NULL);
		pthread_join(mov_balas, NULL);
		return ERROR;
	}


	int inputs;
    do 
    {
		inputs = input();
        if( inputs == DERECHA)//Si muevo a la derecha y no se choca con el limite.
		{
            if(jugador.posicion[X] < (MAX_DISP_X - HITBOX_JUGADOR_X ))
            {
			    npos.x += HITBOX_JUGADOR_X/2;
            }
		}

		if(inputs == IZQUIERDA)//Si muevo a la izq y no se choca con el limite.
		{
            if (jugador.posicion[X] > DISP_MIN)
            {
			    npos.x -= HITBOX_JUGADOR_X/2;
            }
		}

        if(inputs == ARRIBA)
		{
            if (balas[N].active == OFF)//Se activa una vala a la vez.
            {
                balas[N].active = ON;
				//Se crea la bala sale centrada en el extremo superior del jugador.
                balas[N].posicion[X] = pos.x + HITBOX_JUGADOR_X/2;	
                balas[N].posicion[Y] = pos.y - HITBOX_JUGADOR_Y + 1;
            }
		}
		pos = npos;	//actualiza a la posición actual	
        jugador.posicion[X] = pos.x;//Las guardo en el jugador;
        jugador.posicion[Y] = pos.y;
		
		#ifdef RASPI 
			usleep(80*ONE_MS); 
		#endif

	//Si se apreto enter, murio el jugador o murieron todos los aliens, termina el programa.
	} while( (inputs != ENTER) && (jugador.vida > 0) && ( aliensVivos > 0 ) );	

    stop = 1;   //Pedimos que los threads frenen

    //Vuelven los threads.
    pthread_join (mov_aliens, NULL);
    pthread_join (colisiones, NULL);
    pthread_join(mov_balas, NULL);

    //Analizamos cuál fue la causa de la interrupción
    if (jugador.vida <= MUERTO)
    {
        return MUERTO;
    }
	else if ( aliensVivos <= OFF )
    {
        return NEXT_LEVEL;  //Si no quedan más aliens, pasamos al siguiente nivel
    }
    else
    {
        return ENTER;   //ENTER es pausa
    }
}

void * updateDisplay(void * argumentosVarios)
{
    //Interpretamos el puntero como un puntero a display_t
    display_t * thData_p = (display_t*)argumentosVarios;
    
    int * position_p = thData_p->position;//Informa sobre donde esta parado el jugador
    int * pausa = thData_p->pausa;
    unsigned int * dificultad_p = thData_p ->dificultad;

    //Inicializamos el display.
    disp_init();
    disp_clear();
    dcoord_t coords;
    int i, j, previous_position = SPACE_INVADERS;

    //Thread se inicializa al comenzar el programa, entonces muestra el "Space Invaders".
    printMenu(SPACE_INVADERS, NULL, pausa, NULL, NULL);

    while (*position_p != EXIT_PROGRAM)//Mientras se ejecuta el programa.
    {
        if (*position_p == JUEGO)//Si estoy jugando.
        {
            while (!stop)
            {
                // Borramos las posiciones previas en el buffer.
                for (i = 0; i< MAX_DISP_Y; i++)
                {
                    for (j = 0; j < MAX_DISP_X; j++)
                    {
                        coords.x = j;
                        coords.y = i;
                        disp_write(coords, D_OFF);
                    }
                }

                // Prendemos las posiciones actuales en el buffer
                for (i = 0; i <= N; i++)
                {
                    if (alien[i].vida > 0)
                    {
                        printObject(alien[i].posicion, D_ON, ALIEN, alien[i].tipo);
                    } 
                    if (balas[i].active == ON)
                    {
                        coords.x = balas[i].posicion[X];
                        coords.y = balas[i].posicion[Y];
                        disp_write(coords, D_ON);
                    }
                }
                for (i = 0; i < M; i++)
                {
                    if (barreras[i].vida > 0)
                    {
                        printObject(barreras[i].posicion, D_ON, BARRERA, barreras[i].vida);
                    }
                }
                //Dibujo la cantidad de vidas actuales del jugador.
                for (i = 1; i <= jugador.vida; i++)
                {
                    coords.y = MAX_DISP_Y -1;
                    coords.x = MAX_DISP_X - i;
                    disp_write(coords, D_ON);
                }
                printObject(jugador.posicion, D_ON, JUGADOR, 0);
                disp_update();//Lleva todo el buffer a los LEDs.
            }
            disp_clear();
        }
        else
        {
            //Se muestran las opciones del menu mientras no se este jugando y no se termine el programa.
            while(stop && ((*position_p) != EXIT_PROGRAM))
            {
                switch(*position_p)//Dependiendo de donde estoy parado en el menu, muestra la "foto" correspondiente
                {
                    case PLAY:
                    {	
                        //Para evitar que "titile" el display, se actualiza solo una vez hasta que se cambie de opcion.
                        if (previous_position != PLAY)
                        {
                            printMenu(PLAY, NULL, pausa, NULL, NULL);
                            previous_position = PLAY;
                        }
                        break;
                    }
                    case HIGH_SCORES:
                    {
                        if (previous_position != HIGH_SCORES)
                        {
                            printMenu(HIGH_SCORES, NULL, pausa, NULL, NULL);
                            previous_position = HIGH_SCORES;
                        }
                        break;
                    }
                    case DIFICULTAD:
                    {
                        if (previous_position != DIFICULTAD)
                        {
                            printMenu(DIFICULTAD, NULL, pausa, NULL, NULL);
                            previous_position = DIFICULTAD;
                        }
                        break;
                    }
                    case QUIT:
                    {
                        if (previous_position != QUIT)
                        {
                            printMenu(QUIT, NULL, pausa, NULL, NULL);
                            previous_position = QUIT;
                        }
                        break;
                    }
                    case RESTART:
                    {
                        if (previous_position != RESTART)
                        {
                            printMenu(RESTART, NULL, pausa, NULL, NULL);
                            previous_position = RESTART;
                        }
                        break;
                    }
                    case EASY:
                    {
                        if (previous_position != EASY)
                        {
                            printMenu(EASY, NULL, pausa, NULL, NULL);
                            previous_position = EASY;
                        }
                        break;
                    }
                    case MEDIUM:
                    {
                        if (previous_position != MEDIUM)
                        {
                            printMenu(MEDIUM, NULL, pausa, NULL, NULL);
                            previous_position = MEDIUM;
                        }
                        break;
                    }
                    case HARD:
                    {
                        if (previous_position != HARD)
                        {
                            printMenu(HARD, NULL, pausa, NULL, NULL);
                            previous_position = HARD;
                        }
                        break;
                    }
                    case NEXT_LEVEL_ANIMATION:
                    {
                        if (previous_position != NEXT_LEVEL_ANIMATION)
                        {
                            printMenu(NEXT_LEVEL_ANIMATION, NULL, pausa, NULL, dificultad_p);
                            previous_position = NEXT_LEVEL_ANIMATION;
                        }
                        break;
                    }
                    case PUNTAJE:
                    {
                        if (previous_position != PUNTAJE)
                        {
                            printPuntaje(jugador.puntaje);
                            previous_position = PUNTAJE;
                        }
                    }
                    case SHOW_HIGHSCORE:
                    {
                        if (previous_position != SHOW_HIGHSCORE)
                        {
                            printMenu(SHOW_HIGHSCORE, NULL, pausa, NULL, NULL);
                            previous_position = SHOW_HIGHSCORE;
                        }
                        break;
                    }
                    
                    default:
                        break;
                }
            }
            disp_clear();
            previous_position = JUEGO;
        }
    }
    pthread_exit(NULL);
}

static int printMenu (int representacion, void * frontend, int* pausa, highscore_t * highscores_p, int * dificultad_p) 
{
	disp_clear();
	int i, j;
	const int * matriz;
	dcoord_t myPoint; 
	if ( dificultad_p != NULL)
	{
		matriz = detectMatrix ((char)(*dificultad_p + BUSQUEDA_MATRIZ_DIFICULTADES) );
	}
	else
	{
		matriz = detectMatrix ((char) representacion);
	}
	for (j = 0, myPoint.x = DISP_MIN; j < MAX_DISP_X; j++, myPoint.x++)
	{
		for (i = 0, myPoint.y = DISP_MIN; i < MAX_DISP_Y; i++, myPoint.y++)
		{
			if( (*(matriz + i*MAX_DISP_X + j) == '1') || (*(matriz + i*MAX_DISP_X + j) == 1) )
			{
				disp_write (myPoint, D_ON); //Prendo en el buffer.
			}
		}
	}
	disp_update(); //Lleva todo el buffer a los LEDs. Se hace aca porque en el menu utilizamos imagenes.
	return 0;
}


static void printPuntaje ( int puntaje )
{
	int i, j, numero, k;
	int multiplicador;
	const int * matriz;
	dcoord_t coords;
	coords.x = 0;
	coords.y = 5;
	for (i = 3; i >= 0; i ++)
	{
		if (i == 3)
		{
			multiplicador = 1000;
		}
		else if (i ==2)
		{
			multiplicador = 100;
		}
		else if (i == 1)
		{
			multiplicador = 10;
		}
		else if (i == 0)
		{
			multiplicador = 1;
		}
		numero = (puntaje / multiplicador) + '0';
		matriz = detectMatrix((char)numero);
		for (j = 0; j< MAX_DISP_Y ; j++)
		{
			for (k = 0; k < MAX_DISP_X; k++)
			{
				if (*(matriz + j*MAX_DISP_X +k) == 1)
				{
					coords.x = 1+k;
					disp_write(coords, D_ON);
				}
			}
		}
		coords.x = 0;
		coords.x += (NUMBER_X + 1);
	}
	disp_update();

}

int input (void)
{

	
	jcoord_t coord = {0,0};			
    jswitch_t analogico;
	joy_update();
	coord = joy_get_coord();
	analogico = joy_get_switch();
	
    if(coord.x > THRESHOLD)
    {
        return DERECHA;
    }
    if(coord.x < -THRESHOLD)
    {
        return IZQUIERDA;
    }
    if(coord.y > THRESHOLD )
    {
        return ARRIBA;
    }
    if(coord.y < -THRESHOLD)
    {
        return ABAJO;
    }
    if (analogico == J_PRESS)
    {
        return ENTER;
    }
    else
    {
        return -1;
    }
}