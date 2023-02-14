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

 int * detectMatrix (char letter);//Asigna la matriz dibujo .
void paint (dcoord_t coordenada, dlevel_t val, const int matriz[MAX_DISP_X][MAX_DISP_Y]);
void printObject (int posicion[2], dlevel_t valor, char objeto, int tipo);


/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_