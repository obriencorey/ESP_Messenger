/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

UBYTE* BlackImage;
char* input = "Hello there";
char* input2 = "Goodbye there!.83";

/* Entry point ----------------------------------------------------------------*/
void setup()
{
    /*
    BlackImage = EinkInitialize();
    EinkPuts(40,52,"String 1",24);
    EinkFinalize(BlackImage);

    BlackImage = EinkInitialize();
    EinkPuts(40,52,"String 2",24);
    EinkFinalize(BlackImage);

    BlackImage = EinkInitialize();
    EinkPuts(40,52,"String 3",24);
    EinkFinalize(BlackImage);
    */
    BlackImage = EinkInitialize();
    EinkPuts(40,52,input,24);
    EinkFinalize(BlackImage,0);
    delay(5000);
    BlackImage = EinkInitialize();
    EinkPuts(40,52,input2,24);
    EinkFinalize(BlackImage,1);
}

/* The main loop -------------------------------------------------------------*/
void loop(){

}

void EinkPuts(int x, int y, char* str, int fontsize){
  /*
   * int x -        how many pixels to the right the text is placed
   * int y -        how many pixels down the text is placed
   * char* str -    the string to be displayed
   * int fontsize - the fontsize (can only be 8,12,16,20,24)
   */
  switch(fontsize){
    case 8:
            Paint_DrawString_EN(x, y, str, &Font8, WHITE, BLACK);
            break;
    case 12:
            Paint_DrawString_EN(x, y, str, &Font12, WHITE, BLACK);
            break;
    case 16:
            Paint_DrawString_EN(x, y, str, &Font16, WHITE, BLACK);
            break;
    case 20:
            Paint_DrawString_EN(x, y, str, &Font20, WHITE, BLACK);
            break;
    case 24:
            Paint_DrawString_EN(x, y, str, &Font24, WHITE, BLACK);
            break;
    
  }
  
}

UBYTE* EinkInitialize(){
    DEV_Module_Init();
    EPD_2IN9_Init(EPD_2IN9_FULL);

    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1)) * EPD_2IN9_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        while(1);
    }
    
    Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 270, WHITE);
    Paint_Clear(WHITE);

    return(BlackImage);
  
}

void EinkFinalize(UBYTE* BlackImage, int toClear){
    //Run this function after displaying something
    //int toClear - 0 if you want the string to persist
    EPD_2IN9_Display(BlackImage);


    EPD_2IN9_Init(EPD_2IN9_FULL);
    if(toClear){
      EPD_2IN9_Clear();
    }

    EPD_2IN9_Sleep();
    free(BlackImage);
    BlackImage = NULL;
  
}
