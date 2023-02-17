#include "entorno.h"
#define OBJETO(X,Y) ( (int) ( (X)+(Y) ) )

aliens_t alien [N +1];
bullet_t balas [N +1];
barrera_t barreras [M];
jugador_t jugador;
int aliensVivos;
int frecuencia_disparo;
int frecuencia_nave_nodriza;
float multiplicador_movimiento;

//Global asi se guarda el sentido anterior, antes siempre desp de pausa movia a la derecha.
int sentido[2] = {1,1};

//Falag para la ejecucion de los threads.
int stop = 1;

//Devuelve el tiempo transcurrido desde un "star_time".
static int get_elapsed_time(clock_t start_time);

//Basta con que un solo alien llegue al borde derecho, izquierdo, superior o inferior para que devuelva 1.
static int detect_limits (int eje);

//Desplaza a TODOS los aliens vivos y a la vez los hace disparar
static void aliens_move_shoot (int direccion, int sentido);

//Desplaza en la dirección y sentido indicadas al alien de índice i
static void move_alien(int i, int direccion, int sentido);

//Puedo impactar contra el final del mapa, contra un alien, contra una barrera, contra otra bala (proyectil) o contra el jugador
static int check_colisiones(aliens_t* alien, barrera_t* barrera, jugador_t* jugador, bullet_t* bala, int i, int * aliensVivos);

//Desactivar un proyectil significa: cambiar su campo "active", actualizar el campo "colisiones" y la vida del impactado
//La j es el índice del objeto contra el que chocó el proyectil
//La i es el índice de la bala o proyectil que estamos desactivando (le digo proyectil así reservo la "b" para "barrera")
static void desactivar_bala ( aliens_t* alien, barrera_t* barrera, jugador_t* jugador, bullet_t* bala, int i, char impacto, int j, int * aliensVivos);

//Me devuelve 1 si los objetos se superponen (chocan). 
static int colisiona_hitbox(float x1, float y1, float ancho1, float alto1, float x2, float y2, float ancho2, float alto2);

//Me devuelve 1 (verdadero) si los intervalos se intersecan.
static int intersection (float max1, float min1, float max2, float min2);


//Esta función NO debe llamarse si se hizo pausa porque resetea todo el "environment"!!
void setBackend (configs_t configuration, int nextLevelFlag)
{
	int i, j;	//índices y/o contadores

	//Seteamos configuraciones generales:
	aliensVivos = N;	//Todos los aliens arrancan estando vivitos y coleando.
	frecuencia_disparo = (configuration.frecuencia_disparo)*DISPARO_RASPI;
	frecuencia_nave_nodriza = configuration.frecuencia_nave_nodriza;
	multiplicador_movimiento = configuration.velocidad_aliens;
	
	//Seteamos los sentidos por defecto
	sentido[X] = 1;
	sentido[Y]= 1;
	
	//Seteamos a los aliens
	for (j = 0; j<J_FILAS; j++)
	{
		for (i=0 ; i<N_ALIENS ; i++)
		{
			alien[j*N_ALIENS + i].tipo = j+1; // va de alien tipo 1 a alien tipo 3
			alien[j*N_ALIENS + i].vida = configuration.vida_aliens;
			alien[j*N_ALIENS + i].posicion[X] = i*(HITBOX_ALIENS_X + (HITBOX_ALIENS_X/2));
			alien[j*N_ALIENS + i].posicion[Y] = j*(HITBOX_ALIENS_Y + HITBOX_ALIENS_Y/4 + 1) + HITBOX_NODRIZA_Y;
		}
	}
	
	alien[N].vida = 0;	//La nave nodriza arranca "muerta", SIEMPRE!
	alien[N].tipo = 4;	//Para asegurarnos de que no sea clasificada como un alien normal al matarla

		#ifdef ALLEGRO	//Las barreras se acomodan automáticamente

			//Distribuimos las barreras en el espacio:
			int resto = MAX_DISP_X%HITBOX_BARRERA_X;	//Lo que sobraría si quisiera poner la mayor cantidad de barreras posible
			int espacioConBarreras = M*HITBOX_BARRERA_X;
			int espacioSinBarreras = MAX_DISP_X-espacioConBarreras;	//Lo que realmente sobra de espacio

			//Si hay 5 barreras, hay 4 espacios entre barreras y dos espacios entre barreras y "paredes" (max_x y min_x)
			//Generalizando, si hay M barreras, hay M-1 espacios entre barreras y siempre dos entre barreras y paredes.
			int espacioEntreBarrerasTotal = espacioSinBarreras-resto;
			
			//Cada espacio entre barrera y barrera mide: totalEntreBarreras/(M-1)
			int espacioEntreBarreras = espacioEntreBarrerasTotal/(M-1);

			//La primera y la última se ubican a una distancia de "resto/2" de los extremos en X del display
			barreras[0].posicion[X] = (resto/2); //Cada espacio entre barrera y pared mide: resto/2
			barreras[M-1].posicion[X] = (MAX_DISP_X-HITBOX_BARRERA_X-(resto/2));//La coordenada está a la izquierda
			
			//"e" indica a dónde se ubica el borde derecho de la barrera anterior.
			int e = (barreras[0].posicion[X] + HITBOX_BARRERA_X);

			//La primera y la última ya se ubicaron antes de entrar al for
			for (i=1 ; i<M-1 ; i++)	
			{	//Su borde izquierdo se ubica a espacioEntreBarreras distancia respecto del borde derecho de la anterior
				barreras[i].posicion[X] = e + espacioEntreBarreras;	
				e = (barreras[i].posicion[X] + HITBOX_BARRERA_X); //actualizo la posición del borde derecho
			}	//Es importante recordar que la coordenada se encuentra sobre el borde izquierdo de la barrera!!

			//Termino de setear los demás parámetros de las barreras:
			for (i=0 ; i<M ; i++)
			{
				barreras[i].vida = configuration.vida_barreras;
				barreras[i].posicion[Y] = MAX_DISP_Y-SIZE_FONT_SCORE-HITBOX_JUGADOR_Y-BARRERA_POS_Y;	//Le dejamos un espacio entre el jugador y la barrera
			}

		#endif //ALLEGRO

		#ifdef RASPI

			//Asi las cargue en raspi.
			for (i = 0; i<M ; i++)
			{
				barreras[i].vida = configuration.vida_barreras;
				barreras[i].posicion [X] =i*(HITBOX_BARRERA_X+3);
				barreras[i].posicion[Y] = BARRERA_POS_Y;
			}
			
		#endif /*RASPI*/

		//Una vez seteadas las barreras, seteamos las balas de los aliens.
		for (i = 0; i<N; i++)
		{
			balas[i].active = OFF;  //Todas las balas arrancan desactivadas
			balas[i].direccion = DOWN; //Las balas de los aliens van para abajo
		}

		//La bala del jugador va para arriba y arranca apagada
		balas[N].active = OFF;
		balas[N].direccion = UP;    
		
		//Seteamos al jugador:
		jugador.posicion[X] = (MAX_DISP_X/2 - HITBOX_JUGADOR_X/2);
		jugador.posicion[Y] = (MAX_DISP_Y-HITBOX_JUGADOR_Y-1 - SIZE_FONT_SCORE);

		if(!nextLevelFlag)	//Si no pasó de nivel, significa que acaba de iniciar.
		{
			jugador.puntaje = 0;						//Arranca con cero puntos
			jugador.vida = configuration.vida_player;	//Le cargamos las vidas por primera vez
		}
		else	//Si paso de nivel, gana una vida y suma puntos. O sea, no reseteamos estos valores.
		{
			jugador.vida++;
			jugador.puntaje+=100;
		}

    return;
}

//Actualización de aliens (sus disparos y su desplazamiento)
void * updateAliens()		
{
    srand(time(NULL));
	clock_t start_time = clock();
	float frecuencia_movimiento = FRECUENCIA_ALIENS/multiplicador_movimiento;
	int aumento_velocidad = 0;
	
    while(!stop)
    {
        aliens_move_shoot (X, sentido[X]);//Muevo la matriz (mientras tanto, disparan)
		usleep(ONE_MS*frecuencia_movimiento);
		
		//Basta con que un alien llegue al límite para que todos cambien de dirección y/o sentido en bloque
        if (detect_limits(X))
        {
            sentido[X] *= (-1);
            aliens_move_shoot(Y, sentido[Y]); //Muevo en Y (mientras tanto, disparan)
			usleep(ONE_MS*frecuencia_movimiento);

			//Basta con que un alien llegue al límite para que todos cambien de dirección y/o sentido en bloque
            if (detect_limits(Y))
            {
                sentido[Y] *= (-1);
            }
        } 

		//Analisis de aparicion de nave nodriza. Cuantos más aliens vivos haya, más probable es que aparezca
		if( ( (alien[N].disparar = (rand() % frecuencia_nave_nodriza) ) == 1 ) && (!alien[N].vida) && (aliensVivos > MIN_CANT_ALIENS_P_NODRIZA) )
		{
			alien[N].tipo = NODRIZA;
			alien[N].posicion[X] = DISP_MIN;
			alien[N].posicion[Y] = DISP_MIN;
			alien[N].vida = 1;
		}

		//Se acelera el movimiento de un nivel un maximo de 3 veces
		if ((get_elapsed_time(start_time) >= ACELERATION_TIME) && (aumento_velocidad <= ACELERATION_LIMIT))
		{
			frecuencia_movimiento *= 0.85;//Acelero.
			aumento_velocidad++;
		}
    }
    pthread_exit(NULL);	//Concluye el thread.
}

static int get_elapsed_time(clock_t start_time)
{
	clock_t current_time = clock();
	int elapsed_time = (current_time - start_time)/CLOCKS_PER_SEC;
	return elapsed_time;
}

//Desplaza a TODOS los aliens vivos y a la vez los hace disparar
static void aliens_move_shoot (int direccion, int sentido)
{
	int i;
	for (i = 0; i < N; i++)
	{
		//Si el alien está activo, lo movemos.
		if ( alien[i].vida > 0 )
		{
			move_alien(i,direccion,sentido);
		}
		//Posibilidad de que el alien dispare si está activo y su bala está desactivada.
		alien[i].disparar = rand()%(frecuencia_disparo);	//Cuantos menos aliens haya, más probable es que te disparen
		if ( (alien[i].disparar == 1) && (balas[i].active == OFF) && (alien[i].vida > 0) )
		{
			balas[i].posicion[X] = alien[i].posicion[X] + HITBOX_ALIENS_X/2;
			balas[i].posicion[Y] = alien[i].posicion[Y] + HITBOX_ALIENS_Y - 1;
			balas[i].active = ON;
		}		
	}
	if (alien[N].vida > 0)
	{
		move_alien(i, X, 1);
	}
}

//Desplaza en la dirección y sentido indicadas al alien de índice i
static void move_alien(int i, int direccion, int sentido)
{
	if (direccion == X)
	{
    	alien[i].posicion[direccion] += (sentido*(HITBOX_ALIENS_X/2));
	}

	if (direccion == Y)
	{
		alien[i].posicion[direccion] += (sentido*(HITBOX_ALIENS_Y/2));
	}

    return;
}


static int detect_limits (int eje)
{
	int i,borde = 0;	//índice y flag
    switch (eje)
    {
        case X:
        {
            for (i = 0; i < N; i++)	//si el alien está activo y además llegó al límite, prendemos el flag
            {
				if ( (alien[i].vida > 0) && ( (alien[i].posicion[X] > (MAX_DISP_X - HITBOX_ALIENS_X -1 - ALLEGRO_CORRECTION_X)) || (alien[i].posicion[X] <= DISP_MIN ) ))
				{
					borde = 1;	//Si llegó al límite, prendemos el flag
				}
            }
			break;
        }
        
		case Y:
        {
            for (i = 0; i < N; i++)	//si el alien está activo y además llegó al límite, prendemos el flag
            {
				if ( (alien[i].vida > 0) && (alien[i].posicion[Y] >= (MAX_DISP_Y/2 - 2) || alien[i].posicion[Y] <= (DISP_MIN + HITBOX_NODRIZA_Y)) )
				{
					borde = 1;	//Si llegó al límite, prendemos el flag
				}
            }
			break;
        }
		default: break;	//Formalidad
    }
	if ((alien[N].vida) && ((alien[N].posicion[X] >= MAX_DISP_X - HITBOX_NODRIZA_X) || (alien[N].posicion[X] <= DISP_MIN)))
		alien[N].vida = 0;
	
	return borde;	//Informamos el estado final del flag
}

void * updateBullets() //Actualización de balas (desplazamiento)
{
    int i; //índice
    while ( !stop )
    {
        for (i = 0; i <= N; i++)		//Actualizo TODAS, las balas. Aliens y jugador
        {
            if (balas[i].active == ON)	//Si esta activa
            {
                if (balas[i].direccion == UP)
                {
                    if (balas[i].posicion[Y] > DISP_MIN+1)
                    {
                        balas[i].posicion[Y] -= (HITBOX_BALAS_Y/6 + 1);//Actualizo bala
                    }
                    else	//Desactivo la bala para no sacarla del espacio jugable
                    {
                        balas[i].active = OFF;
						balas[i].colisiones = OBJETO(MAPA,0);	//Llegó al "techo"
                    }
                }
                else if (balas[i].direccion == DOWN)
                {
                    if (balas[i].posicion[Y] < (MAX_DISP_Y - 1))
                    {
                        balas[i].posicion[Y] += (HITBOX_BALAS_Y/6 + 1);//Actualizo bala
                    }
                    else
                    {
                        balas[i].active = OFF;
						balas[i].colisiones = OBJETO(MAPA,1);	//Llegó al "piso"
                    }
                }
            }
        }
		usleep(VELOCIDAD_BALAS);		//Cooldown para que las balas se desplacen de forma controlada y no en un flash
    }
    pthread_exit(NULL);	//Termina el thread
}


void* colitions()	 //Actualización de balas (colisiones y sus consecuencias)
{

    while( !stop )
    {
		//Recordar que la bala n+1 tiene índice n en el arreglo (la primera tiene índice 0, no 1)
		for  ( int i = 0 ; i < N+1 ; i++ )	//La bala N+1 es la del jugador
		{
			if ( balas[i].active )
			{
				check_colisiones(alien, barreras, &jugador, balas, i, &aliensVivos);
			}
		}
    }

    pthread_exit(NULL);		//Concluye el thread cuando stop == 0
}


//Puedo impactar contra el final del mapa, contra un alien, contra una barrera, contra otra bala (proyectil) o contra el jugador
static int check_colisiones(aliens_t* alien, barrera_t* barrera, jugador_t* jugador, bullet_t* bala, int i, int * aliensVivos)
{
	int colition_flag = 0;	//Si el proyectil ya colisionó, entonces se activa y se concluye el análisis
	
	int j;					//Índice del objeto contra el que podría estar colisionando
	
	float ancho_obj;		//Tamaño del objeto contra el que podría estar colisionando
	float alto_obj;		
	
	float obj_x;			//Coordenadas del objeto contra el que podría estar colisionando
	float obj_y;

	float pos_x = (bala[i]).posicion[X];	//Coordenadas de la bala
	float pos_y = (bala[i]).posicion[Y];


	float ancho_bala = HITBOX_BALAS_X;		//Tamaño de la bala
	float alto_bala  = HITBOX_BALAS_Y;

	
	//Dejo comentado esto porque en su momento no tuve en cuenta los hitboxes (lo voy a rehacer):

	/*El piso y el techo del mapa tienen "hitbox alto" = 1, y un "hitbox ancho" = X_MAX. 
	Las coordenadas del piso son ( X_MAX/2 ; Y_MAX ) y las del techo son ( X_MAX/2 ; 0 )
	Recordar siempre que en el display el eje Y apunta hacia abajo y NO hacia arriba */

	/*
	//Esto ya lo chequea pipe pero #programaciónALaDefensiva
	//Para este caso es más prolijo poner el return de una y no usar el colition_flag.
	switch ( (int)bala[i].posicion[1] )	//Chequeo su coordenada Y
	{//el casteo a int trunca? Si redondea entonces está mal. El switch no permite floats, una paja

		//La M de MAPA indica que impactó contra el final del espacio jugable.
		case ALTURA: j=1; desactivar_bala(bala, i, MAPA, j);return;	//La colisión "mapa 1" es chocar contra el piso
		case 0: j=0; desactivar_bala(bala, i, MAPA, j);		return;	//La colisión "mapa 0" es chocar contra el techo
		default: break;
	}	//Los índices respetan la convención de que nuestro display tiene el eje Y apuntando para abajo
		//Otra opción más "limpia" es analizar que el hitbox coincida con el límite del mapa
	*/	


	if ( bala[i].direccion == UP )	//Los aliens no se pegan entre sí.
	{	
		ancho_obj = HITBOX_ALIENS_X-1;	//Tamaño del alien
		alto_obj  = HITBOX_ALIENS_Y;

		//Si la bala proviene del jugador, chequeo si colisionó contra un alien. La nave nodirza es el alien N+1
		for ( j = 0 ; (j < N) && (colition_flag == 0) ; j++ )	//Recordar que el alien n tiene índice j=n-1. 
		{	
			//Si el alien está activo, analizo:
			if ( alien[j].vida > 0 )
			{
				//Determino la posición del alien en cuestión
				obj_x = alien[j].posicion[X];
				obj_y = alien[j].posicion[Y];

				/*
				//Determino el tamaño del alien en cuestión
				ancho_obj = alien[j].hitbox[X];
				alto_obj  = alien[j].hitbox[Y];
				(Por ahora, todos los aliens tienen el mismo hitbox)*/	

				//Con las dimensiones de cada objeto, veo si sus hitboxes se intersecan
				if ( colisiona_hitbox(pos_x,pos_y,ancho_bala,alto_bala,obj_x,obj_y,ancho_obj,alto_obj) )
				{	//colisiona_hitbox devuelve 1 si hay colisión, y 0 si no la hay.
					
					colition_flag = 1;		//Aviso que ha habido una colisión. Desde ya, muchas gracias. Atte: juani
					desactivar_bala(alien, barrera, jugador, bala, i, ALIEN, j, aliensVivos);

					return colition_flag;	//Notar que la bala puede impactar contra UN SOLO OBJETO.
				}
			}
		}

		ancho_obj = HITBOX_NODRIZA_X-1;	//Tamaño de la nave nodriza
		alto_obj  = HITBOX_NODRIZA_Y;

		//Si la nave nodriza está activa, analizo:
		if (alien[N].vida > 0 )
		{
			//Determino la posición de la nave nodriza
				obj_x = alien[N].posicion[X];
				obj_y = alien[N].posicion[Y];

				//Con las dimensiones de cada objeto, veo si sus hitboxes se intersecan
				if ( colisiona_hitbox(pos_x,pos_y,ancho_bala,alto_bala,obj_x,obj_y,ancho_obj,alto_obj) )
				{	//colisiona_hitbox devuelve 1 si hay colisión, y 0 si no la hay.
					
					colition_flag = 1;		//Aviso que ha habido una colisión. Desde ya, muchas gracias. Atte: juani
					desactivar_bala(alien, barrera, jugador, bala, i, ALIEN, N, aliensVivos);

					return colition_flag;	//Notar que la bala puede impactar contra UN SOLO OBJETO.
				}
		}
	}


	ancho_obj = HITBOX_BARRERA_X -1;	//Tamaño de la barrera (-1 pixel para la raspi.)
	alto_obj  = HITBOX_BARRERA_Y;

	//Chequeo si colisionó contra una barrera 
	for ( j = 0 ; (j < M) && (colition_flag == 0) ; j++ )	//Recordar que la barrera m tiene índice j=m-1
	{
		//Si la barrera está activa, analizo:
		if ( barrera[j].vida > 0 )
		{
			//Determino la posición de la barrera en cuestión
			obj_x = barrera[j].posicion[X];
			obj_y = barrera[j].posicion[Y];

			/*
			//Determino el tamaño de la barrera en cuestión
			ancho_obj = barrera[j].hitbox[X];
			alto_obj  = barrera[j].hitbox[Y];
			Al final todas las barreras tienen el mismo hitbox*/

			//Con las dimensiones de cada objeto, veo si sus hitboxes se intersecan
			if ( colisiona_hitbox(pos_x,pos_y,ancho_bala,alto_bala,obj_x,obj_y,ancho_obj,alto_obj) )
			{	//colisiona_hitbox devuelve 1 si hay colisión, y 0 si no la hay.

				colition_flag = 1;	//Aviso que ha habido una colisión. Saludos cordiales.
				desactivar_bala(alien, barrera, jugador, bala, i, BARRERA, j, aliensVivos);

				return colition_flag;	//Notar que la bala puede impactar contra UN SOLO OBJETO.
			}	
		}
	}


	ancho_obj = HITBOX_JUGADOR_X-1;	//Tamaño del jugador
	alto_obj  = HITBOX_JUGADOR_Y;

	jugador_t * jugadorp = jugador;	//Multiplayer (?)

	//El jugador no se pega a sí mismo... A menos que en un nivel ultra hardcore haya más de un jugador y ent- Juani sh
	if ( bala[i].direccion == DOWN )
	{
		//Chequeo si colisionó contra el jugador. Por ahora solo hay un jugador XD por eso j<1
		for  ( j = 0 ; (j < 1) && (colition_flag == 0 ) ; j++)	//Dejo la puerta abierta a un multiplayer jejejejeje
		{
			//Si el jugador está activo...  B R U H
			if ( jugadorp[j].vida > 0 )
			{
				//Determino la posición del jugador en cuestión
				obj_x = jugadorp[j].posicion[X];
				obj_y = jugadorp[j].posicion[Y];

				/*
				//Determino el tamaño del jugador en cuestión
				ancho_obj = jugador[j].hitbox[X];
				alto_obj  = jugador[j].hitbox[Y];*/

				//Con las dimensiones de cada objeto, veo si sus hitboxes se intersecan
				if ( colisiona_hitbox(pos_x,pos_y,ancho_bala,alto_bala,obj_x,obj_y,ancho_obj,alto_obj) )
				{	//colisiona_hitbox devuelve 1 si hay colisión, y 0 si no la hay.

					colition_flag = 1;	//Aviso que ha habido una colisión
					desactivar_bala(alien, barrera, jugador, bala, i, JUGADOR, j, aliensVivos);

					return colition_flag;	//Notar que la bala puede impactar contra UN SOLO OBJETO.
				}	
			}
		}
	}


	ancho_obj = HITBOX_BALAS_X;		//Tamaño de las balas
	alto_obj  = HITBOX_BALAS_Y;

	//Por último, chequeo si colisionó contra otro proyectil.
	for  ( j = 0 ; (j < N+1) && (colition_flag == 0 ) ; j++)	//Recordar que la bala N+1 tiene índice j=N
	{
		//Si la bala está activa, analizo:
		if ( bala[j].active )
		{
			//Determino la posición de la bala en cuestión
			obj_x = bala[j].posicion[X];
			obj_y = bala[j].posicion[Y];

			/*
			//Determino el tamaño de la bala en cuestión
			ancho_obj = bala[j].hitbox[X];
			alto_obj  = bala[j].hitbox[Y];*/

			//Con las dimensiones de cada objeto, veo si sus hitboxes se intersecan
			if ( ( colisiona_hitbox(pos_x,pos_y,ancho_bala,alto_bala,obj_x,obj_y,ancho_obj,alto_obj) ) && (j!=i) )
			{	//La bala no colisiona contra sí misma, por eso j!=i

				colition_flag = 1;	//Aviso que ha habido una colisión, estimado.
				desactivar_bala(alien, barrera, jugador, bala, i, PROYECTIL, j, aliensVivos);

				return colition_flag;	//Notar que la bala puede impactar contra UN SOLO OBJETO.
			}
		}
	}

	/*Es prácticamente un copy paste. Me viene a la cabeza la frase de
	Nico de que si hay mucho copy paste es porque probablemente hay otra
	forma de hacerlo.*/
	
	return colition_flag;	//Si no colisionó contra nada, lo informo
}


//Desactivar un proyectil significa: cambiar su campo "active", actualizar el campo "colisiones" y la vida del impactado
//La j es el índice del objeto contra el que chocó el proyectil
//La i es el índice de la bala o proyectil que estamos desactivando (le digo proyectil así reservo la "b" para "barrera")
static void desactivar_bala ( aliens_t* alien, barrera_t* barrera, jugador_t* jugador, bullet_t* bala, int i, char impacto, int j, int * aliensVivos)
{
	static int colisiones_to_nodriza = 0;
	//Indico contra qué impactó la bala...
	switch (impacto)
	{
		case 'M': //Impactó contra el final del Mapa
			bala[i].colisiones = OBJETO(MAPA,j);		
		break;	

		case 'A': //Impactó contra un Alien
			bala[i].colisiones = OBJETO(ALIEN,j);
			alien[j].vida--;	//El alien pierde vida.
			
			if (alien[j].vida <= 0)	
			{
				(*aliensVivos)--;
				switch (alien[j].tipo)
				{
					case 1:	//Es el alien más fuerte
						jugador->puntaje+=15*multiplicador_movimiento;
						break;
					case 2:
						jugador->puntaje += 10*multiplicador_movimiento;	
						break;
					case 3:	//Es el alien más débil
						jugador->puntaje += 5*multiplicador_movimiento;
					default: break;
				}
			}
			
			if ( bala[i].colisiones == OBJETO(ALIEN,N) )	//Chequeo si la colisión fue contra la nave nodriza
			{
				jugador->puntaje += 100;	//Si el alien resultó ser la nave nodriza, gana 50 puntos en total.
				jugador->vida++;		//Y aparte gana una vida iey! Felisiteishonz
				(*aliensVivos)++;		//Aunque la nave nodriza no cuenta como un alien muerto para pasar de nivel!
				colisiones_to_nodriza++;
			}

		break;	

		case 'B': //Impactó contra una Barrera
			bala[i].colisiones = OBJETO(BARRERA,j);	
			barrera[j].vida--;	//La barrera se rompe un poco. Cambio el dibujo?
			//Sonido de barrera impactada. Si se rompió toda, sonido de barrera desaparecida
		break;	

		case 'J': //Impactó contra otra el Jugador
			bala[i].colisiones = OBJETO(JUGADOR,j);	
			
			if(jugador[j].vida>3 && colisiones_to_nodriza >= 1)
			{
				jugador[j].puntaje -=50;
				colisiones_to_nodriza--;
			}

			jugador[j].vida--;	
			
			//Sonido de jugador impactado. Si el jugador muere, sonido de... game over?
		break;	

		case 'P': //Impactó contra otro Proyectil
			bala[i].colisiones = OBJETO(PROYECTIL,j);	//La bala i impactó contra la jota 
			bala[i].colisiones = OBJETO(PROYECTIL,i);	//...y viceversa
			jugador->puntaje++; //El jugador gana un poquito de puntaje si neutraliza una bala (para un posible desempate en los récords)
			bala[j].active = OFF;	//Si dos proyectiles chocaron entre sí, tengo que asegurarme de que ambos hayan sido desactivados.
			//Sonido de balas neutralizándose
		break;

		default:
		break;
	}

	//...y la desactivo.
	bala[i].active = OFF;

	//Indico contra que bala "i" impactó el objeto "j"
	switch (impacto)
	{
		case 'A': alien[j].colisiones = OBJETO(PROYECTIL,i);	break;	//Impactó contra un Alien
		case 'B': barrera[j].colisiones = OBJETO(PROYECTIL,i);	break;	//Impactó contra una Barrera
		case 'J': jugador[j].colisiones = OBJETO(PROYECTIL,i);	break;	//Impactó contra otra el Jugador
		default: break;
	}	//(Es info que cargo por las dudas)

	return;
}


//Me devuelve 1 si los objetos se superponen (chocan). Ya fue testeada.
static int colisiona_hitbox(float x1, float y1, float ancho1, float alto1, float x2, float y2, float ancho2, float alto2)
{	//La coordenada está arriba a la izquierda del hitbox

	int colition_flag = 0;	//Si los hitboxes de ambos objetos se superponen, entonces se activa.

	//Busco los extremos en el intervalo Y de cada objeto para ver si se intersecan. 
	float max1 = y1+alto1;
	float min1 = y1;
	float max2 = y2+alto2;
	float min2 = y2;

	//Analizar primero los intervalos Y ahorra recursos, ya que es menos normal que se intersequen sin superponer los hitbox.

	colition_flag = intersection(max1,min1,max2,min2);	//Intersection me devuelve 1 (verdadero) si se intersecan.
	

	//Si los intervalos Y se intersecan, analizo los intervalos X.
	if (colition_flag)	
	{
		max1 = x1+ancho1;
		min1 = x1;
		max2 = x2+ancho2;
		min2 = x2;

		colition_flag = intersection(max1,min1,max2,min2);	//Me molesta tanto no poder poner tildes, que escribo en inglés JAJAJA
	}

	//Si ambos intervalos (X e Y) se intersecan, significa que sus hitboxes se superponen, entonces devuelvo 1. Sino, devuelvo 0.
	return colition_flag;
}


//Me devuelve 1 (verdadero) si los intervalos se intersecan.
static int intersection (float max1, float min1, float max2, float min2)
{
	/*Que los intervalos se intersequen significa
	que uno está dentro del otro o que uno de sus
	extremos está encerrado por los del otro*/

	int intersection_flag = 0;
	
	//Les puse números romanos para que no piensen que están relacionadas al hitbox 1 o 2. Sólo son vars auxiliares.
	float auxi;
	float auxii;
	float auxiii;
	float auxvi;

	//El signo de la diferencia me indica de que lado está un punto respecto del otro.
	/*EJEMPLO: El 2 está a la izquierda del 3 porque 2-3 es negativo.
	El 3 está a la derecha del 2 porque 3-2 es positivo*/

	//max1 está encerrado por los extremos max2 y min2?
	auxi = max1-max2;
	auxii = max1-min2;
	//min1 está encerrado por los extremos max2 y min2?
	auxiii = min1-max2;
	auxvi = min1-min2;
	
	/*Observen que si min1 está entre max2 y min2 es porque max2 está entre min1 y max1,
	y así. Es decir, ya cubrimos todos los casos de intersección.
	Solo falta considerar el caso en que el intervalo 2 sea subconjunto del 1, que ya
	está contemplado directamente en el if en la parte de "auxvi*auxi<=0".
	Si no me creen, hagan un dibujo en una hoja jajajaja */

	//Si tienen distinto signo o si son cero es porque hubo intersección en al menos un punto.
	if ( (auxi*auxii<=0) || (auxiii*auxvi<=0) || (auxvi*auxi<=0) ) 
	{
		intersection_flag = 1; 
	}

	return intersection_flag;
}

