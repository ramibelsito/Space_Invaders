#include "player.h"
#include "entorno.h"
#include <pthread.h>




#ifdef RASPI
	#include "disdrv.h"         //Display
    #include "joydrv.h"         //inputs
	#include "dibujosRaspi.h"
#endif /*RASPI*/

#ifdef ALLEGRO
	#include <allegro5/allegro.h>
	#include <allegro5/allegro_image.h>
	#include <allegro5/allegro_font.h>
	#include <allegro5/allegro_ttf.h>
	#include <allegro5/allegro_acodec.h> 
#endif // ALLEGRO

#define IN_ALPHABET(X) ( ((X) >= 'A') && ((X) <= 'Z') )


static int printMenu(int opcion, void* frontend, int* pausa, highscore_t * highscore_p, unsigned int *dificultad);


#ifdef ALLEGRO
	static int verify_bitmap_creation(ALLEGRO_DISPLAY *, ALLEGRO_BITMAP *);
	static void tittle_animation(ALLEGRO_BITMAP * space_invaders_tittle, ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * display_background);
	static float resize(float source_w, float source_h, float dest_w, float dest_h, int width_or_height);
	static void dibujar_alien(ALLEGRO_BITMAP * alien_drawing, int i, int tinted);
	static void print_alphabet(char bold_key, ALLEGRO_FONT * font24, ALLEGRO_FONT * font36); 
	
	
    void * updateDisplay(void * argumentosVarios)
	{
		//Interpretamos el puntero como un puntero a display_t
		display_t * thData_p = (display_t*)argumentosVarios;
		
		int * position_p = thData_p->position;
		int * pausa = thData_p->pausa;
		unsigned int * dificultad = thData_p->dificultad;
		highscore_t * highscore_p =  thData_p->highscore_p;
		int previous_position = PLAY;
		ALLEGRO_DISPLAY * display = NULL;	

		if (!al_init())
		{
			fprintf(stderr, "failed to initialize allegro!\n");
			display = NULL;
		}
		
		display = al_create_display(MAX_DISP_X, MAX_DISP_Y);
		
    	if (!display)
    	{
        	fprintf(stderr, "failed to create display!\n");
        	al_uninstall_system();
        	display = NULL;
			stop = -1;
    	}
		else if (!al_init_image_addon())
		{
			fprintf(stderr, "unable to start image addon\n");
			al_uninstall_system();
			al_shutdown_image_addon();
			display = NULL;
			stop = -1;
		}
		else if (!al_install_keyboard())
		{
			fprintf(stderr, "unable to initiate keyboard\n");
			display = NULL;
			stop = -1;
		}

		ALLEGRO_BITMAP * alien_drawing_1;
		ALLEGRO_BITMAP * alien_drawing_2;
		ALLEGRO_BITMAP * alien_drawing_3;
		ALLEGRO_BITMAP * background;
		ALLEGRO_BITMAP * barreras_drawing_1;
		ALLEGRO_BITMAP * barreras_drawing_2;
		ALLEGRO_BITMAP * barreras_drawing_3;
		ALLEGRO_BITMAP * bullet_drawing;
		ALLEGRO_BITMAP * nave_nodriza_drawing;
		ALLEGRO_BITMAP * player_drawing;
		ALLEGRO_BITMAP * player_vida_drawing;
		
		alien_drawing_1 = al_load_bitmap("./assets/alien1.png");
		alien_drawing_2 = al_load_bitmap("./assets/alien2.png");
		alien_drawing_3 = al_load_bitmap("./assets/alien3.png");
		background = al_load_bitmap("./assets/background.png");
		barreras_drawing_1 = al_load_bitmap("./assets/barreras1.png");
		barreras_drawing_2 = al_load_bitmap("./assets/barreras2.png");
		barreras_drawing_3 = al_load_bitmap("./assets/barreras3.png");
		bullet_drawing = al_load_bitmap("./assets/bullet.png");
		nave_nodriza_drawing = al_load_bitmap("./assets/nave_nodriza.png");
		player_drawing = al_load_bitmap("./assets/player.png");
		player_vida_drawing = al_load_bitmap("./assets/vida_player.png");

		al_convert_mask_to_alpha(alien_drawing_1, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(alien_drawing_2, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(alien_drawing_3, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(barreras_drawing_1, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(barreras_drawing_2, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(barreras_drawing_3, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(bullet_drawing, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(nave_nodriza_drawing, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(player_drawing, al_map_rgb(0,0,0));
		al_convert_mask_to_alpha(player_vida_drawing, al_map_rgb(0,0,0));
		
		if (verify_bitmap_creation(display, alien_drawing_1) == -1
				|| verify_bitmap_creation(display, alien_drawing_2) == -1
				|| verify_bitmap_creation(display, alien_drawing_3) == -1
				|| verify_bitmap_creation(display, background) == -1
				|| verify_bitmap_creation(display, barreras_drawing_1) == -1
				|| verify_bitmap_creation(display, barreras_drawing_2) == -1
				|| verify_bitmap_creation(display, barreras_drawing_3) == -1
				|| verify_bitmap_creation(display, bullet_drawing) == -1
				|| verify_bitmap_creation(display, nave_nodriza_drawing) == -1
				|| verify_bitmap_creation(display, player_drawing) == -1
				|| verify_bitmap_creation(display, player_vida_drawing) == -1)
		{
			stop = 1;
		}

        if (al_init_font_addon() == 0)
        {
            fprintf(stderr, "error initializing font addon\n");
            display = NULL;
			stop = -1;
        }
        
        if (al_init_ttf_addon() == 0)
        {
            fprintf(stderr, "error initializing font addon\n");
            display = NULL;
			stop = -1;
        }
		
		al_set_display_icon(display, alien_drawing_3);
		al_set_window_title(display, " -- Space Invaders -- ");
		al_set_new_display_refresh_rate(165);
		al_inhibit_screensaver(true);

		al_draw_scaled_bitmap(background, 0,0,
		al_get_bitmap_width(background),
		al_get_bitmap_height(background), 0 ,0 ,
		al_get_display_width(display), al_get_display_height(display), 0);
		
		al_flip_display();
		printMenu(SPACE_INVADERS, (void*)display, pausa, highscore_p, dificultad);
		
		ALLEGRO_FONT * fontScore;
		fontScore = al_load_ttf_font("assets/font_rellena.ttf", SIZE_FONT_SCORE, 0);
		while (*position_p != EXIT_PROGRAM)
		{
			if (*position_p == JUEGO)
			{
				while( !stop )
				{
					al_draw_scaled_bitmap(background, 0,0,
						al_get_bitmap_width(background),
						al_get_bitmap_height(background), 0 ,0 ,
						al_get_display_width(display), al_get_display_height(display), 0);
					
					for (int i = 0; i < N; i++)			
					{
				
						
						if ((alien[i].vida > 1) || ((*dificultad == EASY) && (alien[i].vida > 0)))
						{
							switch(alien[i].tipo)
							{
								case 1:
									
									dibujar_alien(alien_drawing_1, i, 0);
									break;
								case 2:
									dibujar_alien(alien_drawing_2, i, 0);
									break;
								case 3: 
									dibujar_alien(alien_drawing_3, i, 0);
									break;
								default:
								break;
							}
						}
						else if ((alien[i].vida == 1) && (*dificultad != EASY))
						{
							switch (alien[i].tipo)
							{
								case 1:
									dibujar_alien(alien_drawing_1, i , 1);
									break;
								case 2:
									dibujar_alien(alien_drawing_2, i, 1);
									break;
								case 3:
									dibujar_alien(alien_drawing_3, i , 1);
									break;
								default:
									break;
							}
						}
					}
					for (int i = 0; i < (N+1) ; i++)	//Una bala por cada alien + la bala del jugador.
					{
						if (balas[i].active == ON)
						{
							al_draw_scaled_bitmap(bullet_drawing, 0, 0,
								al_get_bitmap_width(bullet_drawing),
								al_get_bitmap_height(bullet_drawing),
								balas[i].posicion[X], balas[i].posicion[Y], 
								HITBOX_BALAS_X, HITBOX_BALAS_Y, 0);
						}
					}
					for (int i = 0; i < M; i++)
					{
						switch(barreras[i].vida)
						{
							case 3:
							case 4:
							case 5:
								al_draw_scaled_bitmap(barreras_drawing_3, 0, 0,
									al_get_bitmap_width(barreras_drawing_3),
									al_get_bitmap_height(barreras_drawing_3),
									barreras[i].posicion[X], barreras[i].posicion[Y],
									HITBOX_BARRERA_X, HITBOX_BARRERA_Y, 0);
								break;
							case 2:
								al_draw_scaled_bitmap(barreras_drawing_2, 0, 0,
									al_get_bitmap_width(barreras_drawing_2),
									al_get_bitmap_height(barreras_drawing_2),
									barreras[i].posicion[X], barreras[i].posicion[Y],
									HITBOX_BARRERA_X, HITBOX_BARRERA_Y, 0);
								break;
							case 1:
								al_draw_scaled_bitmap(barreras_drawing_1, 0, 0,
									al_get_bitmap_width(barreras_drawing_1),
									al_get_bitmap_height(barreras_drawing_1),
									barreras[i].posicion[X], barreras[i].posicion[Y],
									HITBOX_BARRERA_X, HITBOX_BARRERA_Y, 0);
								break;
							default: 
								break;
						}
					}

					if (alien[N].vida>0)	//El alien N+1 tiene índice N y es la nave nodriza
					{
						al_draw_scaled_bitmap(nave_nodriza_drawing, 0, 0,
							al_get_bitmap_width(nave_nodriza_drawing),
							al_get_bitmap_height(nave_nodriza_drawing),
							alien[N].posicion[X], alien[N].posicion[Y],
							HITBOX_NODRIZA_X, HITBOX_NODRIZA_Y, 0);
					}

					for (int i = 0; i<jugador.vida; i++)
					{
						al_draw_scaled_bitmap(player_vida_drawing, 0, 0,
							al_get_bitmap_width(player_vida_drawing),
							al_get_bitmap_height(player_vida_drawing),
							(i*SIZE_HEART) + 10, MAX_DISP_Y - (SIZE_HEART+5), SIZE_HEART, SIZE_HEART, 0);
					}
					
					if (jugador.vida)
						al_draw_scaled_bitmap(player_drawing, 0, 0,
							al_get_bitmap_width(player_drawing), al_get_bitmap_height(player_drawing),
							jugador.posicion[X], jugador.posicion[Y], HITBOX_JUGADOR_X, HITBOX_JUGADOR_Y, 0);
					al_draw_textf(fontScore, al_map_rgb(255,255,255), MAX_DISP_X-SIZE_FONT_SCORE, MAX_DISP_Y-SIZE_FONT_SCORE-2, ALLEGRO_ALIGN_RIGHT, "%d", jugador.puntaje);
					al_flip_display();
				}
				usleep(100*ONE_MS); // para que se muestre el menu correcto (si es pausa = 1 o no)
				if (*position_p != NEXT_LEVEL_ANIMATION)
				{
					printMenu(PLAY, (void*)display, pausa, highscore_p, dificultad);
				}
				previous_position = PLAY;
			}
			else
			{
				while(stop && ((*position_p) != EXIT_PROGRAM))
				{
					switch(*position_p)//Dependiendo de donde estoy parado en el menu, printeo la "foto" correspondiente
					{
						case PLAY:
						{	
							if (previous_position != PLAY)
							{
								printMenu(PLAY, (void*)display, pausa, highscore_p, dificultad);
								previous_position = PLAY;
							}
						break;
						}
						case HIGH_SCORES:
						{
							if (previous_position != HIGH_SCORES)
							{
								printMenu(HIGH_SCORES, (void*)display, pausa, highscore_p, dificultad);
								previous_position = HIGH_SCORES;
							}
							break;
						}
						case DIFICULTAD:
						{
							if (previous_position != DIFICULTAD)
							{
								printMenu(DIFICULTAD, (void*)display, pausa, highscore_p, dificultad);
								previous_position = DIFICULTAD;
							}
							break;
						}
						case QUIT:
						{
							if (previous_position != QUIT)
							{
								printMenu(QUIT, (void*)display, pausa, highscore_p, dificultad);
								previous_position = QUIT;
							}
							break;
						}
						case RESTART:
						{
							if (previous_position != RESTART)
							{
								printMenu(RESTART, (void*)display, pausa, highscore_p, dificultad);
								previous_position = RESTART;
							}
							break;
						}
						case SHOW_HIGHSCORE:
						{
							if (previous_position != SHOW_HIGHSCORE)
							{
								printMenu(SHOW_HIGHSCORE, (void*)display, pausa, highscore_p, dificultad);
								previous_position = SHOW_HIGHSCORE;
							}
							break;
						}
						case EASY:
						{
							if (previous_position != EASY)
							{
								printMenu(EASY, (void*)display, pausa, highscore_p, dificultad);
								previous_position = EASY;
							}
							break;
						}
						case MEDIUM:
						{
							if (previous_position != MEDIUM)
							{
								printMenu(MEDIUM, (void*)display, pausa, highscore_p, dificultad);
								previous_position = MEDIUM;
							}
							break;
						}
						case HARD:
						{
							if (previous_position != HARD)
							{
								printMenu(HARD, (void*)display, pausa, highscore_p, dificultad);
								previous_position = HARD;
							}
							break;
						}
						case NEXT_LEVEL_ANIMATION:
						{
							if (previous_position != NEXT_LEVEL_ANIMATION)
							{
								printMenu(NEXT_LEVEL_ANIMATION, (void*)display, pausa, highscore_p, dificultad);
								previous_position = NEXT_LEVEL_ANIMATION;
							}
							break;
						}
						case PUNTAJE:
						{
							if (previous_position != PUNTAJE)
							{
								printMenu(PUNTAJE, (void*)display, pausa, highscore_p, dificultad);
								previous_position = PUNTAJE;
							}
							break;
						}
						default:
							break;
					}
					if (IN_ALPHABET(*position_p))
					{
						while (previous_position != (*position_p))
						{
							printMenu(*position_p, (void *)display, pausa, highscore_p, dificultad);
							previous_position = (*position_p);
						}
					}
				}
			}
		}
		al_destroy_bitmap(alien_drawing_1);
		al_destroy_bitmap(alien_drawing_2);
		al_destroy_bitmap(alien_drawing_3);
		al_destroy_bitmap(background);
		al_destroy_bitmap(barreras_drawing_1);
		al_destroy_bitmap(barreras_drawing_2);
		al_destroy_bitmap(barreras_drawing_3);
		al_destroy_bitmap(bullet_drawing);
		al_destroy_bitmap(nave_nodriza_drawing);
		al_destroy_bitmap(player_drawing);
		pthread_exit(NULL);
	}

	static void dibujar_alien(ALLEGRO_BITMAP * alien_drawing, int i, int tinted)
	{
		float * alien_width = (float*)malloc(sizeof(float));
		float * alien_height = (float*)malloc(sizeof(float));
		* alien_width = al_get_bitmap_width(alien_drawing);
		* alien_height = al_get_bitmap_height(alien_drawing);
		if (!tinted)
		{
			al_draw_scaled_bitmap(alien_drawing, 0,0,
									*alien_width, *alien_height,
									alien[i].posicion[X],alien[i].posicion[Y],
									resize(*alien_width,*alien_height,HITBOX_ALIENS_X, HITBOX_ALIENS_Y, 0),
									resize(*alien_width,*alien_height,HITBOX_ALIENS_X, HITBOX_ALIENS_Y, 1), 0);
		}
		else 
		{
			al_draw_tinted_scaled_bitmap(alien_drawing, al_map_rgb(255, 150, 200), 
									0, 0, *alien_width, *alien_height,
									alien[i].posicion[X], alien[i].posicion[Y],
									resize(*alien_width, * alien_height, HITBOX_ALIENS_X, HITBOX_ALIENS_Y,0),
									resize(*alien_width, *alien_height, HITBOX_ALIENS_X, HITBOX_ALIENS_Y,1), 0);
		}
		free(alien_width);
		free(alien_height);
		return;
	}

	static void print_alphabet(char bold_key, ALLEGRO_FONT * font24, ALLEGRO_FONT * font36) 
	{
		char letra = 'A';
		int row, col, x, y;
		int x_margin = 100, y_margin = 50;
		int cell_width = (MAX_DISP_X - 2 * x_margin) / 9;
		int cell_height = (529 - 2 * y_margin) / 3;
		for (row = 0; row < 3; row++) {
			for (col = 0; col < 9; col++) {
				letra = row * 9 + col + 'A';
				x = x_margin + col * cell_width + cell_width / 2;
				y = MAX_DISP_Y - y_margin - (3 - row) * cell_height + cell_height / 2;
				if ((letra != bold_key) && (letra <= 'Z'))
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTER, "%c", letra);
				}
				else if (letra == bold_key)
				{
					y-=7; // compensacion de letra mas grande
					al_draw_textf(font36, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTER, "%c", letra);
				}
			}
		}
		al_flip_display();
		return;
	}

	// se le pasa el tamano de la imagen, el tamano deseado y la coordenada deseada( width = 0, height = 1) 
	// devuelve el valor que tiene que tener la coordenada deseada 
	static float resize(float source_w, float source_h, float dest_w, float dest_h, int width_or_height)
	{
		float aspect_ratio = source_w/source_h;
		if (aspect_ratio > 1)
		{
			dest_h = dest_w/aspect_ratio;
		}
		else
		{
			dest_w = dest_h * aspect_ratio;
		}
		switch(width_or_height)
		{
			case 0:
				return dest_w;
				break;
			case 1:
				return dest_h;
				break;
		}
		return ERROR;
	}

	static int verify_bitmap_creation(ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * image)
    {       
        if (!(image))
        {
            fprintf(stderr, "unable to load image\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_destroy_display(display);
            return -1;
        }
        return 0;
    }

	static int printMenu(int opcion, void* frontend, int* pausa, highscore_t * highscore_p, unsigned int *dificultad)
    {
        ALLEGRO_DISPLAY * display = (ALLEGRO_DISPLAY*)frontend;
		ALLEGRO_FONT * font36;
        ALLEGRO_FONT * font24;
		ALLEGRO_FONT * font42;
		font42 = al_load_ttf_font("assets/font_rellena.ttf", 42, 0);
        font36 = al_load_ttf_font("assets/font_rellena.ttf", 36, 0);
        font24 = al_load_ttf_font("assets/font_rellena.ttf", 24, 0);
        ALLEGRO_BITMAP * background = al_load_bitmap("./assets/background.png");
        ALLEGRO_BITMAP * space_invaders_tittle = al_load_bitmap("./assets/space_invaders_tittle.png");
		al_convert_mask_to_alpha(space_invaders_tittle, al_map_rgb(0,0,0));

        switch(opcion)
        {
            case SPACE_INVADERS:	//Animación de inicio
                al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
                usleep(25*ONE_MS);  //Para que se vea el background unos segundos
                tittle_animation(space_invaders_tittle, display, background);	//Dónde se inicializa "space_invaders_tittle"?
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "difficulty");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
                al_flip_display();

            //De los cuatro textos, agrando el que el jugador está seleccionando (feedback), antes de que aprete ENTER
            case PLAY:
                al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
                al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
						al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				if (!(*pausa))
                {
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
                	al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
                	al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "difficulty");
                	al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				}
				else
				{
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
                	al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
                	al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "RESTART");
                	al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				}
				al_flip_display();
                break;
            case HIGH_SCORES:
                al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
                al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
						al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				if (!(*pausa))
				{
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "difficulty");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				}
				else
				{
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "RESTART");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				}
                al_flip_display();
                break;
            case DIFICULTAD:
                al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
                al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
						al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
                al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "difficulty");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
                al_flip_display();
                break;
            case QUIT:
                al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
                al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
						al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				if (!(*pausa))
                {
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "difficulty");
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				}
				else
				{
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
					al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "RESTART");
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				}
                al_flip_display();
                break;
			case RESTART:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "PLAY");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "highscore");
                al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "RESTART");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 225, ALLEGRO_ALIGN_CENTRE, "QUIT");
				al_flip_display();
				break;
			case SHOW_HIGHSCORE:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				for (int i = 0; i < 3; i++)
				{
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2 - 50, MAX_DISP_Y/3 + i*100, ALLEGRO_ALIGN_CENTER, "%s  ", highscore_p->names[i]);
					al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2 + 50, MAX_DISP_Y/3 + i*100, ALLEGRO_ALIGN_CENTER, "%d", highscore_p->scores[i]);
				}
				al_flip_display();
				break;
			case PUNTAJE:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "SCORE\n");
				al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 50, ALLEGRO_ALIGN_CENTRE, "%d\n", jugador.puntaje);
				al_flip_display();
				break;
			case EASY:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "EASY");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "MEDIUM");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "HARD");
				al_flip_display();
				break;
			case MEDIUM:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "EASY");
                al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "MEDIUM");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "HARD");
				al_flip_display();
				break;
			case HARD:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "EASY");
                al_draw_textf(font24, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 75, ALLEGRO_ALIGN_CENTRE, "MEDIUM");
                al_draw_textf(font36, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3 + 150, ALLEGRO_ALIGN_CENTRE, "HARD");
				al_flip_display();
				break;
			case NEXT_LEVEL_ANIMATION:
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
                al_flip_display();
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				al_draw_textf(font42, al_map_rgb(255,255,255), MAX_DISP_X/2, MAX_DISP_Y/3, ALLEGRO_ALIGN_CENTRE, "LEVEL: %d\n", *dificultad);
				al_flip_display();
				break;
			// TODO EL ALFABETO (MENOS LA ENIE)
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
				al_draw_scaled_bitmap(background, 0,0,
                al_get_bitmap_width(background),
                al_get_bitmap_height(background), 0 ,0 ,
                al_get_display_width(display), al_get_display_height(display), 0);
				al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
				al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
						362, 91, 550, 100, 0); // dibujo el titulo
				print_alphabet(opcion, font24, font36);
				al_flip_display();
				break;
            default:
                break;
        }
        return 0;
		al_destroy_bitmap(background);
		al_destroy_bitmap(space_invaders_tittle);
    }


	static void tittle_animation(ALLEGRO_BITMAP * space_invaders_tittle, ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * display_background)
	{
		int tittle_size_x = 700;
		int tittle_size_y = 250;
		int tittle_pos_x = (MAX_DISP_X-tittle_size_x)/2;
		int tittle_pos_y = (MAX_DISP_Y-tittle_size_y)/2;
		while (tittle_pos_y > 100)
		{
			al_draw_scaled_bitmap(display_background, 0,0,
						al_get_bitmap_width(display_background),
						al_get_bitmap_height(display_background), 0 ,0 ,
						al_get_display_width(display),al_get_display_height(display), 
						0);
			al_draw_scaled_bitmap(space_invaders_tittle, 0, 0,
					al_get_bitmap_width(space_invaders_tittle), al_get_bitmap_height(space_invaders_tittle),
					tittle_pos_x, tittle_pos_y, tittle_size_x, tittle_size_y, 0); // dibujo el titulo
			if (tittle_size_x > 280 && tittle_size_y > 100)
			{
				tittle_size_x -= 25;// reduzco el titulo en la eje x
				tittle_size_y -= 25; // reduzco el titulo en el eje y
				tittle_pos_x += 25/2; // corrijo el desfasaje del centro del display en eje x
			}
			tittle_pos_y -= 25/2; // subo titulo en eje y hasta la posicion deseada
			al_flip_display();
			usleep(ONE_MS*ONE_MS/24);
		}
		return;
	}


    int input()
	{
		ALLEGRO_EVENT_QUEUE * event_queue;
		event_queue = al_create_event_queue(); // inicializo lista de eventos
		if (!event_queue) {
			fprintf(stderr, "failed to create event_queue!\n");
			return -1;
		}
		ALLEGRO_TIMER * timer;
		timer = al_create_timer(1.0 / 2); //crea el timer pero NO empieza a correr
		if (!timer) {
			fprintf(stderr, "failed to create timer!\n");
			return -1;
		}
		al_register_event_source(event_queue, al_get_keyboard_event_source()); //Fuente de eventos: el teclado
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_start_timer(timer); //Recien aca EMPIEZA el timer
		int movement = 0, previous_movement = 0;
		
		while (!movement) // mientras no haya movimiento continúo chequeando
		{
			ALLEGRO_EVENT event;
			if (al_get_next_event(event_queue, &event)) //si hay un nuevo evento
			{
				if (event.type == ALLEGRO_EVENT_KEY_DOWN) // y es de tipo tecla presionada
				{
					switch(event.keyboard.keycode) // switch con la letra que se presiono
					{
						case ALLEGRO_KEY_W: 
						case ALLEGRO_KEY_SPACE: 
						case ALLEGRO_KEY_UP:
							movement = ARRIBA; // guardo el movimiento
							previous_movement = movement;
							break;
						case ALLEGRO_KEY_S: 
						case ALLEGRO_KEY_DOWN:
							movement = ABAJO;
							previous_movement = movement;
							break;
						case ALLEGRO_KEY_A: 
						case ALLEGRO_KEY_LEFT:
							movement = IZQUIERDA;
							previous_movement = movement;
							break;
						case ALLEGRO_KEY_D: 
						case ALLEGRO_KEY_RIGHT:
							movement = DERECHA;
							previous_movement = movement;
							break;
						case ALLEGRO_KEY_ENTER: 
						case ALLEGRO_KEY_ESCAPE:	//Tanto para seleccionar como para hacer pausa
							movement = ENTER;
							previous_movement = movement;
							break;
						default:
							movement = 0; // sino, especifico que no hubo movimiento
							previous_movement = movement;
							break;
					}
				}
				else if (event.type == ALLEGRO_EVENT_KEY_UP)
					movement = -1;
				else if (event.type == ALLEGRO_EVENT_TIMER)
				{
					movement = -1;
				}
			}
		}
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		return movement;
	} 
#endif /*ALLEGRO*/


#ifdef RASPI
	//Puntaje del jugador se imprime en pantalla.
	static void printPuntaje ( int puntaje );

	//Se copia en buffer una matriz a partir de las coordenadas (x,y) enviadas.
	static void copy_matrix( const int * matrix, int x, int y);

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
							break;
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

static int printMenu (int representacion, void * frontend, int* pausa, highscore_t * highscores_p, unsigned int * dificultad_p) 
{
	disp_clear();
	int i, j;
	const int * matriz;
	dcoord_t myPoint; 
	if ( dificultad_p != NULL)//Busco el "dibujo", segun el nivel.
	{
		matriz = detectMatrix ((char)(*dificultad_p + BUSQUEDA_MATRIZ_DIFICULTADES) );
	}
	else
	{
		matriz = detectMatrix ((char) representacion);//Busco la matriz correspondionte.
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
	int i, x = 1, y = 5;
	int * matrix;
	char puntaje_chars[5];
	
	sprintf(puntaje_chars, "%04d", puntaje); //Guardo el numero como string y relleno de 0 si es necesario.
	
	for (i = 0; i < 4; i++)
	{
		matrix = detectMatrix(puntaje_chars[i]); //Busco la matriz asignada al numero.
		copy_matrix (matrix,x,y);
		x+=4;
	}
	disp_update();

}

static void copy_matrix( const int * matrix, int x, int y) 
{
    int i, j;
	dcoord_t coords;
    for (i = 0; i < NUMBER_Y; i++) 
	{
        for (j = 0; j < NUMBER_X; j++) 
		{
			coords.x = j+x;
			coords.y = i+y;
			if (*(matrix + i*NUMBER_X + j) == 1)
			{
				disp_write(coords, D_ON);
			}
        }
    }
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

	
#endif /*RASPI*/



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