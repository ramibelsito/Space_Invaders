#include "player.h"     //Control del jugador una vez iniciada la partida + display de raspi y allegro
#include "configs.h"    //Manejo de datos y estadísticas de los niveles y los récords
#include "standard.h"   //Generalización de datos constantes y definiciones que son públicas para todos los módulos

//Recibe la dificultad seleccionada. Devuelve qué ítem del menú principal eligió el jugador.
static int startMenu (configs_t * configuracion, highscore_t * highscore_p, unsigned int defaultDifficulty);

//Devuelve qué ítem del menú de dificultad seleccionó el jugador
static int difficultyMenu(void);

#ifdef ALLEGRO
    static void receive_letter(char string[], int i);
#endif


//Vars globales definidas fuera de este módulo y que igualmente se usan acá.
extern jugador_t jugador;

//Globales a nivel archivo. 
static int position = MENU;
static int pausa = 0;
static int next_level = 0;


int main(void)
{
    int QuitFlag = 0;   //Avisa si el jugador quiere cerrar el juego

    //Por defecto, el juego arranca en el nivel 1 (easy).
    static configs_t configuracion;
    configuracion.dificultad = EASY;
	unsigned int defaultDifficulty = EASY;

    //Cargamos información sobre los récords
    highscore_t highscore;
    highscore_t * highscore_p = &highscore;
    find_highscore(highscore_p);    
    //Definimos e inicializamos el thread que inicializa el frontend y maneja el display:
    pthread_t display;  
	display_t thData;
	thData.pausa = &pausa;          //Para que sepa cuándo salir del loop
    thData.position = &position;    //Para que sepa qué parte del menú quiere ver el jugador
	thData.dificultad = &configuracion.dificultad;
    thData.highscore_p = highscore_p;
    pthread_create(&display, NULL, updateDisplay, &thData); 

    /*Estoy con errores que creo que se deben a que mi pc es lenta
    y la inicialización de allegro (en el thread) no llega a hacerse
	antes de que el mainstream trate de usar las cosas de allegro*/
    usleep(500*ONE_MS);    
    
    //Mostramos el menú principal hasta que el jugador cierre el juego.
    while (QuitFlag != QUIT)
    {
        QuitFlag = startMenu(&configuracion, highscore_p, defaultDifficulty); //inicia la experiencia del jugador
    }
    //Le avisamos al thread del display que cEsE sU LaBoR y cerramos el programa 
    position = EXIT_PROGRAM;
    pthread_join (display, NULL);
    return 0;
}

static int startMenu (configs_t * configuracion, highscore_t * highscore_p, unsigned int defaultDifficulty) 
{
    
    //Arranco con la animación de SPACE_INVADERS
    int juego, inputs;
   
	//Si resulta que pasé de nivel, sigo jugando automáticamente. Seteo el nuevo nivel sin resetear el puntaje ni la vida.
    if (next_level)
    {
        
        position = PLAY;	//Va directo a "init_game".
    }
    else if (position == RESTART)//Arranco a jugar pero en este caso SÍ reseteo el puntaje y la vida del jugador.
    {
        position = PLAY;	//Va directo a "init_game".
        pausa = 0;			//Para set backend.
    }
    else	//Sino, entro al menú normalmente.
    {
        if (!pausa)
        {
            position = PLAY;	//Si no puse pausa, cargo un nuevo nivel
        }
        do
        {   
            inputs = input();	//Recibo los inputs del jugador para navegar a través del backend del menu
            
			if(inputs == ABAJO)	
            {
                //Agrando el ítem sobre el que estoy posicionado (feedback)
                switch (position)
                {
                    case SPACE_INVADERS:
                        position = PLAY;
                        break;
                    case PLAY:
                        position = HIGH_SCORES;
                        
                        break;
                    case HIGH_SCORES:
                       if(pausa)	//Si no estoy en pausa, me salteo la opcion de restart
                        {
                            position = RESTART;
                        }
                        else
                        {
                            position = DIFICULTAD;
                        }
                        break;
                    case DIFICULTAD:
                    case RESTART:
                        position = QUIT;
                        break;
                    

                    default: 
                        break;
                }
            }

            if(inputs == ARRIBA)
            {
                switch (position)
                {
                    case RESTART:
                    case DIFICULTAD:
                        position = HIGH_SCORES;
                        break;
                    case HIGH_SCORES:
                        position = PLAY;
                        break;
                    case QUIT:
                        if(pausa)//Si no estoy en pausa me salteo la opcion de restart
                        {
                            position = RESTART;
                        }
                        else
                        {
                            position = DIFICULTAD;
                        }
                        break;
                    default:
                        break;
                }
            }
            usleep(100*ONE_MS);//Para poder seleccionar bien las opciones.
        } while( inputs != ENTER );	
    }

	//Cuando apretemos enter, el lugar en el que estemos parados determina lo que hay que hacer
    switch(position)
    {
        case PLAY:
        {	            	
            *configuracion = set_difficulty(configuracion->dificultad);//Guardo los datos de la dificultad
            position = NEXT_LEVEL_ANIMATION;
            sleep(2);
            position = JUEGO;
			juego = init_game(*configuracion, pausa, next_level);	//Inicia el juego
			//Cuando concluye, devuelve la razón por la que se interrumpió el juego (perdió, ganó, pausa, etc)			
			//Esperamos a que se interrumpa el juego y analizamos la razón por la que se detuvo
            switch (juego)
            {
				case MUERTO:	//Si perdiste...
				{
					pausa = 0;	//Apagamos los flags para que se resetee todo
					next_level = 0;
					
					position = PUNTAJE;
                    sleep(2);
					
					int indice = process_score(jugador.puntaje, highscore_p);
					if (indice != -1)	//Si hizo un récord, lo guardamos en el .txt.
					{
						//El nombre por defecto es "raspberryPi" porque desde la raspi no se puede elegir un nombre
						char new_name[4] = "RBPI";

						#ifdef ALLEGRO

							for (int i = 0; i < 4; i ++)
							{
								receive_letter(new_name, i);
							}

						#endif /*ALLEGRO*/

                        shift_scores(jugador.puntaje, indice, highscore_p);
                        add_string_to_array(highscore_p->names, new_name, indice);
                        store_highscore(highscore_p);
                        #ifdef ALLEGRO
                        do
                            {
                                position = SHOW_HIGHSCORE;
                                inputs = input();
                            }
                            while ((inputs != ENTER));
                        find_highscore(highscore_p);
                        #endif // ALLEGRO
					}
                    position = PLAY;

                    configuracion->dificultad = defaultDifficulty;	//Por defecto, volvemos al nivel 1
					return MUERTO;
					break;	
				}

				case NEXT_LEVEL:
                {
                    next_level = 1;		//Aviso que al cargar el nuevo nivel, no se deben resetear las vidas ni el puntaje del jugador
                    pausa = 0;
                    
					/*No importa en qué dificultad arranque, cuando paso de nivel avanzo a la siguiente dificultad. 
                    Cuando llego a HELL, me mantengo ahi hasta perder.*/
                    if ((configuracion->dificultad) < HELL)	
				    {
					    configuracion->dificultad++;
				    }
					return NEXT_LEVEL;
				    break;	
                }
				case ENTER://El menú de pausa es el de inicio, con la diferencia de que al poner "PLAY" va a seguir desde donde lo dejó.
					pausa = 1;		//Avisamos que NO SE TIENE QUE RESETEAR EL NIVEL! Todo tiene que seguir exactamente como estaba
                    next_level = 0; //No pase de nivel.
                    position = PLAY;
                    return CONTINUE;
				    break;	

                case RESTART:	//Vuelvo al nivel inicial (ya sea EASY o el que haya seleccionado el jugador en el difficultyMenu)
                    pausa = 0;		//Reseteo el nivel
                    next_level = 0;	//Reseteo al jugador (su vida y su puntaje)
					configuracion->dificultad = defaultDifficulty;

                    return RESTART;
                    break;

				default: 
					return ERROR; 
				    break;	
            }
            break;
        }
		
        case HIGH_SCORES:
        {
            #ifdef ALLEGRO
                find_highscore(highscore_p);
                do
                {
                    position = SHOW_HIGHSCORE;
                    inputs = input();
                }
                while ((inputs != ENTER));
            #endif //ALLEGRO
            #ifdef RASPI
                position = SHOW_HIGHSCORE;
                sleep(2);
            #endif
            position = HIGH_SCORES;
            return HIGH_SCORES;	//Sólo el main puede comunicar a configs con menu, por eso volvemos.
            break;
        }
        case DIFICULTAD:
        {
            configuracion->dificultad = difficultyMenu();	//Selecciono dificultad para jugar
			defaultDifficulty = configuracion->dificultad;	//Se actualiza la dificultad por defecto
            return DIFICULTAD;
            break;
        }
        case QUIT:
        {
            return QUIT;    //Manda a que se apague todo
            break;
        }
        default:    	//Formalidad
            return ERROR;
            break;  
    }
}

#ifdef ALLEGRO

static void receive_letter(char string[], int i)
{
    int inputs;
    char caracter = 'A';
    position = 'A';
    
	do
    {
        inputs = input();
        if (inputs == DERECHA)
        {
            switch (position)
            {
                case 'A':
                    position = 'B';
					caracter = position;
                    break;
                case 'B':
                    position = 'C';
					caracter = position;
                    break;
                case 'C':
                    position = 'D';
					caracter = position;
                    break;
                case 'D':
                    position = 'E';
					caracter = position;
                    break;
                case 'E':
                    position = 'F';
					caracter = position;
                    break;
                case 'F':
                    position = 'G';
					caracter = position;
                    break;
                case 'G':
                    position = 'H';
					caracter = position;
                    break;
                case 'H':
                    position = 'I';
					caracter = position;
                    break;
                case 'I':
                    position = 'J';
					caracter = position;
                    break;
                case 'J':
                    position = 'K';
					caracter = position;
                    break;
                case 'K':
                    position = 'L';
					caracter = position; 
                    break;
                case 'L':
                    position = 'M';
					caracter = position;
                    break;
                case 'M':
                    position = 'N';
					caracter = position;
                    break;
                case 'N':
                    caracter = position;
                    position = 'O';
                    break;
                case 'O':
                    position = 'P';
                    caracter = position;
                    break;
                case 'P':
                    position = 'Q';
                    caracter = position;
                    break;
                case 'Q':
                    position = 'R';
                    caracter = position;
                    break;
                case 'R':
                    position = 'S';
                    caracter = position;
                    break;
                case 'S':
                    position = 'T';
                    caracter = position;
                    break;
                case 'T':
                    position = 'U';
                    caracter = position;
                    break;
                case 'U':
                    position = 'V';
                    caracter = position;
                    break;
                case 'V':
                    position = 'W';
                    caracter = position;
                    break;
                case 'W':
                    position = 'X';
                    caracter = position;
                    break;
                case 'X':
                    position = 'Y';
                    caracter = position;
                    break;
                case 'Y':
                    position = 'Z';
                    caracter = position;
                    break;
                case 'Z':
                    position = 'A';
                    caracter = position;
                    break;
                default:
                    printf("ERROR: No hay Ñ!\n");
                    break;
            }
        }
        if (inputs == IZQUIERDA)
        {
			switch (position)
			{
				case 'A':
                    position = 'Z';
                    caracter = position;
					break;
				case 'Z':
                    position = 'Y';
                    caracter = position;
					break;
				case 'Y':
                    position = 'X';
                    caracter = position;
					break;
				case 'X':
                    position = 'W';
                    caracter = position;
					break;
				case 'W':
                    position = 'V';
                    caracter = position;
					break;
				case 'V':
                    position = 'U';
                    caracter = position;
					break;
				case 'U':
                    position = 'T';
                    caracter = position;
					break;
				case 'T':
                    position = 'S';
                    caracter = position;
					break;
				case 'S':
                    position = 'R';
                    caracter = position;
					break;
				case 'R':
                    position = 'Q';
                    caracter = position;
					break;
				case 'Q':
                    position = 'P';
                    caracter = position;
					break;
				case 'P':
                    position = 'O';
                    caracter = position;
					break;
				case 'O':
                    position = 'N';
                    caracter = position;
					break;
				case 'N':
                    position = 'M';
                    caracter = position;
					break;
				case 'M':
                    position = 'L';
                    caracter = position;
					break;
				case 'L':
                    position = 'K';
                    caracter = position;
					break;
				case 'K':
                    position = 'J';
                    caracter = position;
					break;
				case 'J':
                    position = 'I';
                    caracter = position;
					break;
				case 'I':
                    position = 'H';
                    caracter = position;
					break;
				case 'H':
                    position = 'G';
                    caracter = position;
					break;
				case 'G':
                    position = 'F';
                    caracter = position;
					break;
				case 'F':
                    position = 'E';
                    caracter = position;
					break;
				case 'E':
                    position = 'D';
                    caracter = position;
					break;
				case 'D':
                    position = 'C';
                    caracter = position;
                	break;
				case 'C':
                    position = 'B';
                    caracter = position;
					break;
				case 'B':
                    position = 'A';
                    caracter = position;
					break;
				default:
                    printf("ERROR: No hay Ñ!\n");
                    break;
			}
        }
    }
    while (inputs != ENTER);
	string[i] = caracter;
    return;
}
#endif //ALLEGRO

static int difficultyMenu(void)
{
	//Arranco con el cursor sobre "EASY"
    int dificultad = EASY;
    position = EASY;
    int inputs;

    do
    {   
        inputs = input();	//Recibo un input
       
	    if(inputs == ABAJO)
		{
            switch (dificultad)
            {
                case EASY:	//Si estaba en EASY y me moví para abajo...
                {
                    position = MEDIUM;	//..entonces estoy en medium, y así sucesivamente.
                    dificultad = MEDIUM;
                    break;
                }
                case MEDIUM:
                {
                    position = HARD;
                    dificultad = HARD;
                    break;
                }
                default: 
                    break;
            }
		}

		if(inputs == ARRIBA)
		{
			switch (dificultad)
            {
                case MEDIUM:
                {
                    position = EASY;
                    dificultad = EASY;
                    break;
                }
                case HARD:
                {
                    position = MEDIUM;
                    dificultad = MEDIUM;
                    break;
                }
                default: break; 
            }
		}

		usleep(100*ONE_MS);//Para poder seleccionar bien las opciones.

	} while( inputs != ENTER );	//termina si se presiona el switch

	return dificultad;	//Indica qué dificultad seleccionó el jugador
}

