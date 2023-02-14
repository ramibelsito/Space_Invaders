
#include "dibujosRaspi.h"
#include "disdrv.h"

/********************************************* **********************************
 * ROM CONST VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
//Se que son chars, lo dejo asi por ahora.
 int matSPACEINVADERS[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1'},
  {' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' '},
  {'1', ' ', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', '1'},
  {' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', ' ', ' ', '1', ' ', '1', '1', '1', ' ', '1', ' ', ' ', ' '},
  {'1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int playMat [MAX_DISP_X][MAX_DISP_Y] = {
  {'1', '1', '1', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', '1', '1', ' ', '1', ' ', ' ', '1', '1', '1', ' ', '1', ' ', '1', ' '},
  {' ', '1', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', '1', ' ', '1', '1', '1', ' '},
  {' ', '1', '1', '1', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', '1', ' ', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', '1', '1', '1', '1', ' ', '1', ' ', '1', ' ', ' ', ' '}};

 int hScoreMat [MAX_DISP_X][MAX_DISP_Y] = {
  {' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', ' ', ' ', '1', '1', ' ', ' ', '1', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', '1', '1', ' ', '1', '1', ' ', '1', '1', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', '1', '1', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', ' ', '1', '1', ' ', '1', '1', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

  int difficultyMat [MAX_DISP_X][MAX_DISP_Y] = {
  {' ', '1', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', ' ', '1', '1', ' ', '1', ' ', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matRESTART[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', ' ', ' ', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', '1', ' ', '1', '1', '1', '1', '1', '1', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', '1', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', '1', '1', ' ', ' ', '1', '1', '1', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', ' ', '1', '1', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int exitMat [MAX_DISP_X][MAX_DISP_Y] = {
  {' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', '1', ' ', '1', '1', ' ', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', ' ', ' '},
  {' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', ' ', ' '},
  {' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', '1', '1', '1', '1', '1', '1', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', '1', '1', ' ', '1', ' ', ' ', '1', ' ', '1', ' ', '1', '1', '1', ' '},
  {' ', '1', ' ', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' '},
  {' ', '1', '1', ' ', ' ', ' ', '1', '1', ' ', ' ', '1', ' ', ' ', '1', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', '1', ' ', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' '},
  {' ', '1', '1', '1', ' ', '1', ' ', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' '}};

   int matEASY[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', '1', '1', ' ', '1', '1', '1', ' ', '1', '1', ' ', '1', ' ', '1',' ' },
  {' ', '1', ' ', ' ', ' ', '1', ' ', '1', '1', ' ', ' ', ' ', '1', '1', '1',' ' },
  {' ', '1', '1', ' ', ' ', '1', '1', '1', ' ', '1', '1', ' ', ' ', '1', ' ',' ' },
  {' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' ', '1', ' ',' ' },
  {' ', '1', '1', '1', ' ', '1', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ',' ' },
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

   int matMEDIUM[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', ' ', '1', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', '1', ' ', '1', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

    int matHARD[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', '1', ' ', '1', ' ', '1', '1', '1', ' ', '1', '1', ' ', ' ', '1', '1',' ' },
  {' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ','1' },
  {' ', '1', '1', '1', ' ', '1', '1', '1', ' ', '1', '1', '1', ' ', '1', ' ','1' },
  {' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', '1', ' ', ' ', '1', ' ','1' },
  {' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', '1', '1',' ' },
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matNEXTLEVEL_1[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matNEXTLEVEL_2[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matNEXTLEVEL_3[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matNEXTLEVEL_4[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matNEXTLEVEL_5[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', '1', '1', '1', ' ', ' ', '1', ' ', ' ', '1', '1', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

 int matSHOW_HIGHSCORE[MAX_DISP_X][MAX_DISP_Y] = {
  {' ', '1', ' ', ' ', '1', ' ', ' ', '1', '1', ' ', ' ', '1', '1', '1', ' ', ' '},
  {' ', '1', '1', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' ', '1', ' ', ' ', ' '},
  {' ', '1', ' ', '1', '1', ' ', '1', ' ', ' ', '1', ' ', ' ', '1', ' ', ' ', ' '},
  {' ', '1', ' ', ' ', '1', ' ', ' ', '1', '1', ' ', ' ', ' ', '1', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', '1', '1', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', '1', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', '1', ' ', '1', ' ', ' ', '1', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', '1', ' ', '1', '1', '1', ' ', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', '1', ' ', ' ', '1', '1', '1', ' ', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', '1', ' ', '1', ' ', '1', ' ', ' ', ' ', '1', ' ', ' ', ' ', ' '}};



 int  matNAVE [MAX_DISP_X][MAX_DISP_Y] = { 
                                                            [0][1] = 1,
                                                [1][0] = 1, [1][1] = 1, [1][2] = 1};

 int  matAliens1 [MAX_DISP_X][MAX_DISP_Y] = { 
                                                            [0][1] = 1,
                                                [1][0] = 1,              [1][2] = 1}; 

 int  matAliens2 [MAX_DISP_X][MAX_DISP_Y] = { 
                                                [0][0] = 1, [0][1] = 1,  [0][2] = 1,
                                                [1][0] = 1,              [1][2] = 1};  

 int  matAliens3 [MAX_DISP_X][MAX_DISP_Y] = { 
                                                [0][0] = 1, [0][1] = 1,  [0][2] = 1,
                                                            [1][1] = 1};  

 int  matNODRIZA [MAX_DISP_X][MAX_DISP_Y] = {[0][0] = 1, [0][1] = 1}; 

 int matBARRERAS4 [MAX_DISP_X][MAX_DISP_Y] = {[0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 1, [1][2] = 1};

 int matBARRERAS3 [MAX_DISP_X][MAX_DISP_Y] = {[0][0] = 0, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 1, [1][2] = 1};

 int matBARRERAS2 [MAX_DISP_X][MAX_DISP_Y] = {[0][0] = 0, [0][1] = 0, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 1, [1][2] = 1};

 int matBARRERAS1 [MAX_DISP_X][MAX_DISP_Y] = {[0][0] = 0, [0][1] = 0, [0][2] = 0,
                                                   [1][0] = 1, [1][1] = 1, [1][2] = 1};

 int matCERO[NUMBER_Y][NUMBER_X] = {            [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 1, [2][1] = 0, [2][2] = 1,
                                                   [3][0] = 1, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 1, [4][1] = 1, [4][2] = 1};       

 int matUNO[NUMBER_Y][NUMBER_X] = {       [0][0] = 0, [0][1] = 0, [0][2] = 1,
                                                   [1][0] = 0, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 0, [2][1] = 0, [2][2] = 1,
                                                   [3][0] = 0, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 0, [4][1] = 0, [4][2] = 1};

 int matDOS[NUMBER_Y][NUMBER_X] = {       [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 0, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 1, [2][1] = 1, [2][2] = 1,
                                                   [3][0] = 1, [3][1] = 0, [3][2] = 0,
                                                   [4][0] = 1, [4][1] = 1, [4][2] = 1};
                    
 int matTRES[NUMBER_Y][NUMBER_X] = {             [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 0, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 0, [2][1] = 1, [2][2] = 0,
                                                   [3][0] = 0, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 1, [4][1] = 1, [4][2] = 1};
                                                
 int matCUATRO[NUMBER_Y][NUMBER_X] = {    [0][0] = 1, [0][1] = 0, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 1, [1][2] = 1,
                                                   [2][0] = 0, [2][1] = 0, [2][2] = 1,
                                                   [3][0] = 0, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 0, [4][1] = 0, [4][2] = 1};
                                                
 int matCINCO[NUMBER_Y][NUMBER_X] = {     [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 0, [1][2] = 0,
                                                   [2][0] = 1, [2][1] = 1, [2][2] = 1,
                                                   [3][0] = 0, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 1, [4][1] = 1, [4][2] = 1};

 int matSEIS[NUMBER_Y][NUMBER_X] = {      [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 0, [1][2] = 0,
                                                   [2][0] = 1, [2][1] = 1, [2][2] = 1,
                                                   [3][0] = 1, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 1, [4][1] = 1, [4][2] = 1};
                                                
 int matSIETE[NUMBER_Y][NUMBER_X] = {     [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 0, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 0, [2][1] = 0, [2][2] = 1,
                                                   [3][0] = 0, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 0, [4][1] = 0, [4][2] = 1};
                                                
 int matOCHO[NUMBER_Y][NUMBER_X] = {      [0][0] = 0, [0][1] = 1, [0][2] = 0,
                                                   [1][0] = 1, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 0, [2][1] = 1, [2][2] = 0,
                                                   [3][0] = 1, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 1, [4][1] = 1, [4][2] = 1};

 int matNUEVE[NUMBER_Y][NUMBER_X] = {     [0][0] = 1, [0][1] = 1, [0][2] = 1,
                                                   [1][0] = 1, [1][1] = 0, [1][2] = 1,
                                                   [2][0] = 1, [2][1] = 1, [2][2] = 1,
                                                   [3][0] = 0, [3][1] = 0, [3][2] = 1,
                                                   [4][0] = 0, [4][1] = 0, [4][2] = 1};


 int * detectMatrix (char letter)
{
    letter = MAY_MIN(letter);
    switch (letter) 
    {        
        case 1 + BUSQUEDA_MATRIZ_DIFICULTADES:
        {
            return &matNEXTLEVEL_1[0][0];
            break;

        }   
         case 2+BUSQUEDA_MATRIZ_DIFICULTADES:
        {
            return &matNEXTLEVEL_2[0][0];
            break;

        } 
         case 3+BUSQUEDA_MATRIZ_DIFICULTADES:
        {
            return &matNEXTLEVEL_3[0][0];
            break;

        } 
         case 4+BUSQUEDA_MATRIZ_DIFICULTADES:
        {
            return &matNEXTLEVEL_4[0][0];
            break;

        } 
         case 5+BUSQUEDA_MATRIZ_DIFICULTADES:
        {
            return &matNEXTLEVEL_5[0][0];
            break;

        } 
        case '0':
		{
		    return &matCERO[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}    
		case '1':
		{
		    return &matUNO[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '2':
		{
		    return &matDOS[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
		case '3':
		{
		    return &matTRES[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '4':
		{
		    return &matCUATRO[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '5':
		{
		    return &matCINCO[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
		case '6':
		{
		    return &matSEIS[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '7':
		{
		    return &matSIETE[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '8':
		{
		    return &matOCHO[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '9':
		{
		    return &matNUEVE[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case (char)PLAY:
		{
		    return &playMat[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case (char)HIGH_SCORES:
		{
		    return &hScoreMat[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case (char)DIFICULTAD:
        {
            return &difficultyMat[0][0];
            break;
        }
        case (char)QUIT:
		{
		    return &exitMat[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case (char)SPACE_INVADERS:
		{
		    return &matSPACEINVADERS[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case (char)RESTART:
		{
		    return &matRESTART[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
          case (char)EASY:
		{
		    return &matEASY[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
          case (char)MEDIUM:
		{
		    return &matMEDIUM[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
          case (char)HARD:
		{
		    return &matHARD[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
          case (char)SHOW_HIGHSCORE:
		{
		    return &matSHOW_HIGHSCORE[0][0]; //Si querria letra A, me devuelve puntero a matriz
		    break;
		}
        case '\0':
        {
            printf("Terminador\n");
            return 0;
        }

		default:
		{
		   // printf("Error. Lo que quiere dibujar no existe. Pruebe usando otra palabra al llamar a printMenu()\n");
		    return NULL;
		    break;
		}
    }
}

//Me había olvidado de printObject y paint. Tengo que agregar lo de la ruta de información acá también.
void printObject (int posicion[2], dlevel_t valor, char objeto, int tipo)
{
    dcoord_t coordenadas;
    coordenadas.x = posicion[0];
    coordenadas.y = posicion[1];
    // printf("Llegue a printObject.\n");
    switch (objeto)
    {
        case PROYECTIL:
        {
            //printf ("Coordenadas BALA = [ %d, %d]\n", coordenadas.x, coordenadas.y);
            disp_write (coordenadas, valor);
            break;
        }
        case ALIEN:
        {
            //printf ("Coordenadas ALIEN = [ %d, %d]\n", coordenadas.x, coordenadas.y);
            switch (tipo)
            {
                case 1:
                    paint (coordenadas, valor, matAliens1);
                    break;
                case 2:
                    paint (coordenadas,valor, matAliens2);
                    break;
                case 3:
                    paint (coordenadas,valor, matAliens3);
                    break;
                case NODRIZA:
                    paint (coordenadas,valor, matNODRIZA);
            }
            break;
        }
        case JUGADOR:
        {
            //printf ("Coordenadas NAVE = [ %d, %d]\n", coordenadas.x, coordenadas.y);
            paint(coordenadas, valor, matNAVE);
            break;
        }
        case BARRERA:
        {
            
            if (tipo == 1)
            {
                paint (coordenadas, valor, matBARRERAS1);
            }
            else if (tipo == 2)
            {
                paint(coordenadas,valor,matBARRERAS2);
            }
            else if (tipo == 3)
            {
                paint(coordenadas, valor, matBARRERAS3);
            }
            else
            {
                paint (coordenadas, valor, matBARRERAS4);
            }
            break;
        }
        default: printf("default.\n"); break;
    }
    
}


void paint (dcoord_t coordenada, dlevel_t val, const int matriz[MAX_DISP_X][MAX_DISP_Y])//Coordenada es el punto, no el hitbox.
{
    int i, j;
    dcoord_t coords;
    coords.x = coordenada.x ;
    coords.y = coordenada.y;
    for (i = 0; i < HITBOX_ALIENS_Y; i++)
    {
        for (j = 0; j < HITBOX_ALIENS_X; j++)
        {
            if (matriz[i][j] == ON)//La parte que quiero copiar
            {
                disp_write(coords, val);
            }
            coords.x++;
        }
        coords.x = coordenada.x;
        coords.y ++;
    }
}

