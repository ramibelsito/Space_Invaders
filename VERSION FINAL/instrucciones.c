//Este archivo se compila y ejecuta automáticamente si invocamos al makefile sin definir el switch

#include <stdio.h>

int main (void)
{
    printf("Es necesario que en la command desk ejecute el comando 'make RASPI=1' si desea instalar el juego en su RaspberryPi o 'make ALLEGRO=1' si lo que usted quiere es la versión de Allegro.\nLuego, inicie la aplicación escribiendo el comando './Space_Invaders_App' en su command desk.\n");
}

/*Conociendo el comando "echo" de makefile (es como un printf pero de lenguaje make), este
archivo está bastante al pedo, porque podríamos meter el mensaje directamente en el makefile. 
(antes no lo conocía)*/