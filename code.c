#include <stdio.h>
#include <3ds.h>
#include "mcu.h"

typedef struct  
{
    u8 seconds;
    u8 minute;
    u8 hour;
    u8 something;
    u8 day;
    u8 month;
    u8 year   
} RTC;

void bcdfix(u8* wat)
{
    if((*wat & 0xF) == 0xF) *wat -= 6;
    if((*wat & 0xF) == 0xA) *wat += 6;
}

int main ()
{
    gfxInit(GSP_RGB565_OES, GSP_BGR888_OES, false); //Inits both screens.
    PrintConsole console;
    consoleInit(GFX_TOP, &console);
    printf ("Welcome to RTChanger! \n");            //Notifications to user after booting RTChanger.
    printf ("Using this program, you can manually change the Raw RTC. \n");
    printf ("The Raw RTC is your hidden System Clock. Editing this allows you to bypass timegates. \n");
    printf ("More information can be found at my GitHub. \n"); 
    printf ("I highly recommend you view the README if you haven't already. \n \n");
    printf ("Press A to continue or START to return to the Home Menu. \n \n \n \n");
    printf ("\x1b[36mhttps://www.github.com/Storm-Eagle20/RTChanger\x1b[0m");
    RTC rtctime;
    u8* buf = &rtctime;
    u8 offs = 0;
    while (aptmainloop())                           //Detects the input for the A button.
    {    
         hidscaninput();
         kDown = hidKeysDown();        //Detects if the A button was pressed.
         kHeld = hidKeysHeld();        //Detects if the A button was held.
         kUp = hidKeysUp();            //Detects if the A button was just released.
        
         if(kHeld & KEY_START) break;  //User can choose to continue or return to the Home Menu.  
        
         if(kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))
         {
             bcdfix(buf + offs);
             printf("20%08X/%08X/%08X %08X:%08X:%08X\r", buf[6], buf[5], buf[4], buf[2], buf[1], buf[0]);
             if(kDown & (KEY_UP));
             {
                switch(offs)
                {
                    case 0: //seconds
                    case 1: //minutes
                        break;
                     
                    case 2: //hours
                        break;
                     
                    case 4: //days
                        break;
                     
                    case 5: //months
                        break;
                     
                    case 6: //years
                        break;
             }
             if(kDown & (KEY_UP));
             {
                 switch(offs)
                 {
                    case 0: //seconds
                    case 1: //minutes
                        break;
                    
                    case 2: //hours
                        break;
                    
                    case 4: //days
                        break;
                     
                    case 5: //months
                        break;
                    
                    case 6: //years
                        break;
             }
         }
        
         if (kDown & KEY_A);
         {
             consoleClear(); //Clears the screen of text.
             printf ("Please change the time below. It does not match your current time you see on the Home Menu, this is normal. /n"); //Text shown at the second screen.
             printf ("Change the time by however much you need necessary, \x1b[31mchanging the time backwards is not recommended.\x1b[0m"); 
         }
        
         gfxFlushBuffers();
         gfxSwapBuffers();
         gspWaitForVBlank();
    }
    gfxExit();
    return 0;
}
