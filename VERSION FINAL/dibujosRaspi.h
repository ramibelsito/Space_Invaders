/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef DIBUJOSRASPI_H
#define DIBUJOSRASPI_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "standard.h"

/**
 * @brief Asigna la matrix dibujo. 
 * @param letter Estructura de tipo dcoord_t, que contiene las coordenadas del punto al que se quiere escribir. 
 * @return Puntero a la matriz dibujo.
*/
int * detectMatrix (char letter);

/**
 * @brief Pinta en buffer el diujo enviado. 
 * @param coordenada Coordenada del objeto a dibujar. 
 * @param val Encendido o Apagado.
 * @param matriz	Dibujo.e
*/ 
void paint (dcoord_t coordenada, dlevel_t val, const int matriz[MAX_DISP_X][MAX_DISP_Y]);

/**
 * @brief Pinta en buffer el diujo asignado. 
 * @param coordenada Coordenada del objeto a dibujar. 
 * @param val Encendido o Apagado.
 * @param objeto Objeto a dibujar
 * @param tipo	Para asignar distintos dibujos a un objeto.
*/
void printObject (int posicion[2], dlevel_t valor, char objeto, int tipo);


/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_