/*
 * Joystick Driver
 *
 * Copyright 2019  Catedra Programacion I - ITBA
 *
 * Author(s):
 *  Daniel Jacoby
 *  Nicolas Magliola
 *  Pablo Vacatello
 */

#ifndef JOYDRV_H
#define JOYDRV_H


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//Las coordenadas del joystick varían entre -128 y 127 para cada coordenada
#define JOY_MAX_POS     127		
#define JOY_MAX_NEG     -128


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
    int8_t x;
    int8_t y;
} jcoord_t;

typedef enum {J_NOPRESS, J_PRESS} jswitch_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa el joystick. Debe llamarse UNA vez antes de empezar a usar el joystick.
*/
void joy_init(void);

/**
 * @brief Actualiza los valores medidos del joystick. Por lo tanto, debe llamarse antes de usar joy_get_coord() o joy_get_switch()
*/
int joy_update(void);		//void

/**
 * @brief Devuelve las coordenadas del joystick medidas al momento del último joy_update()
 * @return Una estructura de tipo jcoord_t con las coordenadas del joystick.
*/
jcoord_t joy_get_coord(void);

/**
 * @brief Es análogo a joy_get_coord, en este caso devuelve el estado del switch del joystick
 * @return Una variable de tipo jswich_t, es decir, J_NOPRESS o J_PRESS.
*/
jswitch_t joy_get_switch(void);


/*******************************************************************************
 ******************************************************************************/

#endif // JOYDRV_H

//No tenemos los .c de estas funciones, tenemos directamente los .o