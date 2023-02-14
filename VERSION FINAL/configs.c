#include "configs.h"

//Constantes privadas, o sea, que sólo se usan en este módulo:

#define LINE_LENGTH 100 // largo de la linea en el archivo txt
#ifdef ALLEGRO
#define DESFASAJE 0
#endif //ALLEGRO
#ifdef RASPI
#define DESFASAJE 1
#endif // RASPI
// posiciones de los valores de las distintas varibales en el archivo txt
#define FR_DISPARO_POS 29
#define FR_NODRIZA_POS 48
#define VEL_POS 64
#define VIDA_BARRERAS_POS 83
#define VIDA_PLAYER_POS 98

#define LINE_LENGTH_HIGHSCORE 30 // largo de la linea en el archivo txt
#define SCORE_POS 5 // posicion del primer score
#define NAME_SCORE_COMBO 10 // lo que ocupa un nombre mas su score en el archivo txt

static void set_line(unsigned int line_to_set, FILE * file); // mueve el "cursor" entre dificultades del txt

static void num_to_string(int num, int digits, char scores[digits+1]); // convierte de ints a strings numeros de cuantos digitos le mandes

#ifdef ALLEGRO
void input_name(char *new_name)
{
    return;
}
#endif // ALLEGRO

#ifdef RASPI
void input_name(char * new_name)
{
    return;
}
#endif // RASPI

void store_highscore(highscore_t * storage)
{
    int i , j;	//Índices
    char  scores_in_string[3][5];
    FILE * config = fopen("./config.txt", "r+");
    
	if ( !config)
        fprintf(stderr, "failed to write to config.txt\n");
   
    fseek(config, -31, SEEK_END);
   
    for (i = 0 ; i < 3 ; i++)
    {
        num_to_string(storage->scores[i], 4 , scores_in_string[i]);
    }

    for (i=0; i<3; i++)
    {
        scores_in_string[i][4] = '\0';
    }
    
	fputc((int)'\n', config);
    for ( i = 0 ; i < 3 ; i ++ )
    {
        for ( j = 0 ; j < 4; j ++ )
        {
            fputc( (int)(storage->names[i][j]), config);
        }
        fputc((int)' ', config);
		for ( j = 0 ; j < 4 ; j ++)
        {
            fputc((int)(scores_in_string[i][j]), config);
        }
        fputc((int)' ', config);
    }
    fclose(config);
    return;
}

void num_to_string(int num, int digits, char scores[digits+1])
{
    int i, k=1;
    for( i = 0 ; i<digits; i++)
    {
        k*=10;
    }
    
	for ( i = 0; i <digits; i++)
    {
        k /= 10;
        scores[i] = ((num/k)%10) + '0';
    }
    
	scores[digits+1] = '\0';
    return;
}


void add_string_to_array(char array[3][10], char* new_string, int index) 
{
    for (int i = 2; i>index; i--)
    {
        strcpy(array[i], array[i-1]);
    }
    
	array[3][0] = '\0';
    strcpy(array[index], new_string);
    return;
}

void shift_scores(int new_score, int indice, highscore_t * storage)
{
    int aux;
    for (int i = indice; i<3; i++)
    {
        aux = storage->scores[i];
        storage->scores[i] = new_score;
        new_score = aux;
    }
    return;
}

int process_score( int new_score, highscore_t * storage)
{
    int i; 
    for (i = 0; i < 3; i++)
    {
        if (new_score > storage->scores[i])
        {
            return i;
        }
    }
    return -1;
}

void find_highscore(highscore_t * storage)
{
    FILE * config = fopen("config.txt", "r");
    int i, j, k;
    for (i  = 0; i < 3; i ++)
    {
        storage->scores[i] = 0;
        storage->names[i][0] = '\0';
    }
    char info[LINE_LENGTH_HIGHSCORE];
    
	set_line(5,config); // seteo cursor en la linea de highscore
    fread(info, LINE_LENGTH_HIGHSCORE, 1, config); // guardo data en info
    
	for (i = 0; i<3 ; i++)
    {
        k = 10000; // multiplicador para guardar todo el char[4] en un solo int
        for (j = 0; j < 4; j++) // igual que arriba pero el puntaje tiene 4 digitos
        {
            k /= 10;
            storage->scores[i] += ((info[(j+SCORE_POS)+(NAME_SCORE_COMBO*i)] - '0') * k); // obtengo el numero en char y lo guardo en el int correspondiente
            storage->names[i][j] = info[j+(NAME_SCORE_COMBO*i)]; // guardo el nombre en le correspondiente string
        }
    }
    
	storage->names[i][4] = '\0';
   
    fclose(config);
    return;
}

//Recibe la dificultad que se busca y devuelve la configuración correspondiente.
configs_t set_difficulty(unsigned int dificultad)
{
    FILE * config = fopen("config.txt", "r");
    char config_params[LINE_LENGTH];
    configs_t configuration;
    set_line(dificultad-1, config);
    fread(config_params, sizeof(char), LINE_LENGTH, config);
	//Cargamos los datos en el struct
    configuration.frecuencia_disparo = (config_params[FR_DISPARO_POS] - '0')*10 + (config_params[FR_DISPARO_POS+1] - '0');
    configuration.frecuencia_nave_nodriza = (config_params[FR_NODRIZA_POS] - '0')*10 + (config_params[FR_NODRIZA_POS+1] - '0');
    configuration.velocidad_aliens = (config_params[VEL_POS] - '0') + (0.1*(config_params[VEL_POS+2] - '0'));
    configuration.vida_barreras = config_params[VIDA_BARRERAS_POS] - '0';
    configuration.vida_player = config_params[VIDA_PLAYER_POS] - '0';
	configuration.vida_aliens = dificultad;	
	configuration.dificultad = dificultad;
    configuration.velocidad_balas = VELOCIDAD_BALAS;

    fclose(config);
    return configuration;
}


static void set_line(unsigned int line_to_set, FILE * file)
{
    line_to_set *= LINE_LENGTH + DESFASAJE;
    fseek(file, line_to_set, SEEK_SET);
    return;
}
