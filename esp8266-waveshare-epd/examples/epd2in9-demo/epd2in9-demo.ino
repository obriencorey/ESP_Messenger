/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

/* Entry point ----------------------------------------------------------------*/
void setup()
{
  printf("EPD_2IN9_test Demo\r\n");
  DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN9_Init(EPD_2IN9_FULL);
    EPD_2IN9_Clear();
    DEV_Delay_ms(100);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1)) * EPD_2IN9_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 270, WHITE);

#if 1   // Drawing on the image
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");

    Paint_DrawString_EN(0, 4, "AAAAAAAAAAAA", &Font24, WHITE, BLACK);
    Paint_DrawString_EN(10, 28, "test2", &Font24, WHITE, BLACK);
    Paint_DrawString_EN(20, 52, "test3", &Font24, WHITE, BLACK);
    Paint_DrawString_EN(30, 76, "test4", &Font24, WHITE, BLACK);
    Paint_DrawString_EN(40, 100, "test5", &Font24, WHITE, BLACK);
    
    EPD_2IN9_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time        

#endif
    printf("Clear...\r\n");
    EPD_2IN9_Init(EPD_2IN9_FULL);
    EPD_2IN9_Clear();

    printf("Goto Sleep...\r\n");
    EPD_2IN9_Sleep();
    free(BlackImage);
    BlackImage = NULL;
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
