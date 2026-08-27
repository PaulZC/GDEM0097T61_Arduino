#include <SPI.h>
//EPD
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "Ap_29demo.h"  

void setup() {
   pinMode(A14, INPUT);  //BUSY
   pinMode(A15, OUTPUT); //RES 
   pinMode(A16, OUTPUT); //DC   
   pinMode(A17, OUTPUT); //CS   
   //SPI
   SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
   SPI.begin ();  
}

//Tips//
/*
1.Flickering is normal when EPD is performing a full screen update to clear ghosting from the previous image so to ensure better clarity and legibility for the new image.
2.There will be no flicker when EPD performs a partial update.
3.Please make sue that EPD enters sleep mode when update is completed and always leave the sleep mode command. Otherwise, this may result in a reduced lifespan of EPD.
4.Please refrain from inserting EPD to the FPC socket or unplugging it when the MCU is being powered to prevent potential damage.)
5.Re-initialization is required for every full screen update.
6.When porting the program, set the BUSY pin to input mode and other pins to output mode.
*/
void loop() {
   unsigned char i,j,k;

#if 0 //Full screen update and fast update
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s. 
    /************Full display(2s)*******************/
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen update.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s. 

    /************Fast update mode(2s)*******************/
    EPD_HW_Init_Fast(); //Fast update initialization.
    EPD_WhiteScreen_ALL_Fast(gImage_2); //To display one image using fast update.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif

#if 0   
    /************4 Gray  update mode(2s)*******************/		
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.

    EPD_HW_Init_4Gray(); //Fast update initialization.
    EPD_WhiteScreen_ALL_4Gray(gImage_4G1); //To display one image using fast update.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s. 
#endif

#if 1 //GoodDisplay's original Partial update demostration.
  //Partial update demo support displaying a clock at 5 locations with 00:00.  If you need to perform partial update more than 5 locations, please use the feature of using partial update at the full screen demo.
  //After 5 partial updatees, implement a full screen update to clear the ghosting caused by partial updatees.
  //////////////////////Partial update time demo/////////////////////////////////////
      EPD_HW_Init(); //Electronic paper initialization. 
      EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.
      for(i=0;i<6;i++)
      EPD_Dis_Part_Time(32,56+24*0,Num[i],         //x-A,y-A,DATA-A
                        32,56+24*1,Num[0],         //x-B,y-B,DATA-B
                        32,56+24*2,gImage_numdot, //x-C,y-C,DATA-C
                        32,56+24*3,Num[0],        //x-D,y-D,DATA-D
                        32,56+24*4,Num[1],24,32); //x-E,y-E,DATA-E,Resolution 24*32
          

      EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.
      EPD_HW_Init(); //Full screen update initialization.
      EPD_WhiteScreen_White(); //Clear screen function.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.
#endif  
  
#if 1 //Partial update demostration.
    //Partial update demo support displaying a blinking clock at 5 locations with 00:00.
    //Paul's note: this only works because all five digits are overwritten simultaneously
    //////////////////////Partial update time demo/////////////////////////////////////
    EPD_HW_Init(); //Electronic paper initialization. 
    EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.
    for(i=0;i<19;i++)
        EPD_Dis_Part_Time(32,56+24*0,i % 2 == 0 ? Num[i / 2] : gImage_space,    //x-A,y-A,DATA-A
                          32,56+24*1,i % 2 == 0 ? Num[i / 2] : gImage_space,    //x-B,y-B,DATA-B
                          32,56+24*2,i % 2 == 0 ? gImage_numdot : gImage_space, //x-C,y-C,DATA-C
                          32,56+24*3,i % 2 == 0 ? Num[i / 2] : gImage_space,    //x-D,y-D,DATA-D
                          32,56+24*4,i % 2 == 0 ? Num[i / 2] : gImage_space,    //x-E,y-E,DATA-E,
                          24,32); //Resolution 24*32
    EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif  
  
#if 1 //Paul's partial update demostration.
    // Ping-pong is enabled by default and causes problems.
    // We can only appear to shift a digit along by one if
    // we fully erase the space occupied by all digits and
    // then add in the new digit.
    EPD_HW_Init(); //Electronic paper initialization. 
    EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.
    for(i=0;i<3;i++)
    {
        for(j=0;j<7;j++)
        {
          for(k=0;k<7;k++)
          {
            // Erase all previous digits
            EPD_Dis_Part_RAM(32,32+(24*k),gImage_space,24,32); //Resolution 24*32
          }

          // Add new digit
          EPD_Dis_Part_RAM(32,32+(24*j),Num[6-j],24,32); //Resolution 24*32

          EPD_Part_Update();
        }
    }

    // Erase last digit by erasing all previous digits
    for(k=0;k<7;k++)
    {
      EPD_Dis_Part_RAM(32,32+(24*k),gImage_space,24,32); //Resolution 24*32
    }
    EPD_Part_Update();
    
    EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
    
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif  
  
#if 1 //Paul's partial update demostration.
    // Ping-pong is enabled by default and causes problems.
    // We can only appear to shift a digit along by one if
    // we fully erase the space occupied by all digits and
    // then add in the new digit.
    // In this demo only the previous digit is erased,
    // resulting in much alternate digit weirdness!
    EPD_HW_Init(); //Electronic paper initialization. 
    EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.
    for(j=0;j<3;j++)
    {
        for(i=0;i<7;i++)
        {
          if (i > 0)
          {
            // Erase previous digit
            EPD_Dis_Part_RAM(32,32+(24*(i - 1)),gImage_space,24,32); //Resolution 24*32
          }
          else if (j > 0)
          {
            // Erase previous last digit
            EPD_Dis_Part_RAM(32,32+(24*6),gImage_space,24,32); //Resolution 24*32
          }

          // Add new digit
          EPD_Dis_Part_RAM(32,32+(24*i),Num[6-i],24,32); //Resolution 24*32

          EPD_Part_Update();
        }
    }

    EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
    
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif  
  
#if 1 //Paul's partial update demostration.
    // Ping-pong is enabled by default and causes problems.
    // If we want to only erase the previous digit when
    // adding the new digit, we can write and update twice
    // so both halves of the RAM are updated.
    EPD_HW_Init(); //Electronic paper initialization. 
    EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.
    for(j=0;j<3;j++)
    {
      for(i=0;i<7;i++)
      {
        for(k=0;k<2;k++) // Do each erase and add twice!
        {
          if (i > 0)
          {
            // Erase previous digit
            EPD_Dis_Part_RAM(32,32+(24*(i - 1)),gImage_space,24,32); //Resolution 24*32
          }
          else if (j > 0)
          {
            // Erase previous last digit
            EPD_Dis_Part_RAM(32,32+(24*6),gImage_space,24,32); //Resolution 24*32
          }

          // Add new digit
          EPD_Dis_Part_RAM(32,32+(24*i),Num[6-i],24,32); //Resolution 24*32

          EPD_Part_Update();
        }
      }
    }

    // Erase last digit
    EPD_Dis_Part_RAM(32,32+(24*6),gImage_space,24,32); //Resolution 24*32
    EPD_Part_Update();
    
    EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
    
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif  
  
#if 1 //Paul's partial update demostration.
    // Ping-pong is enabled by default and causes problems.
    // Just for giggles and just to prove it works:
    // we can instead do a single write and erase the
    // previous previous digit when adding the new digit
    EPD_HW_Init(); //Electronic paper initialization. 
    EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.
    for(j=0;j<3;j++)
    {
      for(i=0;i<7;i++)
      {
        if ((i + j*7) >= 2) // If we have printed at least 2 digits
        {
          k = (i + 5) % 7; // Subtract 2 from i (unsigned char)
          EPD_Dis_Part_RAM(32,32+(24*k),gImage_space,24,32); // Erase previous previous digit
        }

        EPD_Dis_Part_RAM(32,32+(24*i),Num[6-i],24,32); // Add new digit

        EPD_Part_Update();
      }
    }

    k = (i + 5) % 7; // Subtract 2 from i (unsigned char)
    EPD_Dis_Part_RAM(32,32+(24*k),gImage_space,24,32); // Erase previous previous digit
    EPD_Part_Update();
    
    EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
    
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif  
  
#if 0 //Demo of using partial update to update the full screen, to enable this feature, please change 0 to 1.
    //After 5 partial updatees, implement a full screen update to clear the ghosting caused by partial updatees.
    //////////////////////Partial update time demo/////////////////////////////////////
    EPD_HW_Init(); //E-paper initialization 
    EPD_SetRAMValue_BaseMap(gImage_p1); //Please do not delete the background color function, otherwise it will cause an unstable display during partial update.
    EPD_Dis_PartAll(gImage_p1); //Image 1
    EPD_Dis_PartAll(gImage_p2); //Image 2
    EPD_Dis_PartAll(gImage_p3); //Image 3
    EPD_Dis_PartAll(gImage_p4); //Image 4
    EPD_Dis_PartAll(gImage_p5); //Image 5 
    EPD_DeepSleep();//Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s. 
    EPD_HW_Init(); //Full screen update initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif
  
#if 0 //Demonstration of full screen update with 180-degree rotation, to enable this feature, please change 0 to 1.
    /************Full display(2s)*******************/
    EPD_HW_Init_180(); //Full screen update initialization.
    EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen update.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s.
#endif        

 while(1);  // The program stops here   
}




//////////////////////////////////END//////////////////////////////////////////////////
