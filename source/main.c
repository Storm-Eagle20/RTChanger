#include <stdio.h>
#include <3ds.h>
#include "mcu.h"

#define hangmacro() \
({\
    puts("Press a key to exit...");\
    while(aptMainLoop())\
    {\
        hidScanInput();\
        if(hidKeysDown())\
        {\
            goto killswitch;\
        }\
        gspWaitForVBlank();\
    }\
})

typedef struct  
{
    u8 seconds;
    u8 minute;
    u8 hour;
    u8 something;
    u8 day;
    u8 month;
    u8 year;   
} RTC;

void bcdfix(u8* wat)
{
    if((*wat & 0xF) == 0xF) *wat -= 6;
    if((*wat & 0xF) == 0xA) *wat += 6;
}

int main ()
{
    gfxInit(GSP_RGB565_OES, GSP_BGR8_OES, false); //Inits both screens.
    PrintConsole topScreen, bottomScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
    consoleSelect(&bottomScreen);
    Result res = mcuInit();
    if(res < 0)
    {
        printf("Failed to init MCU: %08X\n", res);
        puts("This .3DSX was likely opened without    Luma3DS or a SM patch.");
        puts("\x1b[30;41mYou cannot use this application without Luma3DS and Boot9Strap.\x1b[0m");
        puts("If you have Luma3DS 8.0 and up, just    ignore the above message and patch SM.  Restart the application afterwards.");
        puts("If you are confused, please visit my    GitHub and view the README.\n \n \n");
        puts("\x1b[36mhttps://www.github.com/Storm-Eagle20/RTChanger\x1b[0m");
        hangmacro();
    }
    puts ("Welcome to RTChanger! \n");            //Notifications to user after booting RTChanger.
    puts ("Using this program, you can manually    change the Raw RTC."); //Extra spaces between words so that the screen doesn't separate them.
    puts ("The Raw RTC is your hidden System Clock.Editing this allows you to bypass      timegates.");
    puts ("More information can be found at my     GitHub."); 
    puts ("I highly recommend you view the README  if you haven't already.");
    puts ("Please change your time or START to     return to the Home Menu. \n \n \n");
    puts ("\x1b[36mhttps://www.github.com/Storm-Eagle20/RTChanger\x1b[0m");
    consoleSelect(&topScreen);
    puts("Here you can change your time. Changing backwards is not recommended.");
    puts("Change your time by however you may need. \n");
    RTC rtctime;
    u32 kDown = 0;
    u32 kHeld = 0;
    u32 kUp = 0;
    u8* buf = &rtctime;
    u8 offs = 0;
    while (aptMainLoop())                           //Detects the input for the A button.
    {   
        hidScanInput();
        kDown = hidKeysDown();        //Detects if the A button was pressed.
        kHeld = hidKeysHeld();        //Detects if the A button was held.
        kUp = hidKeysUp();            //Detects if the A button was just released.
        
        if(kHeld & KEY_START) break;  //User can choose to continue or return to the Home Menu.  
        
        if(kDown & (KEY_UP))          //Detects if the UP D-PAD button was pressed.
        {    
            buf[offs]++; //Makes an offset increasing the original value by one.
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
        if(kDown & (KEY_DOWN))        //Detects if the UP D-PAD button was pressed.
        {    
            buf[offs]--; //Makes an offset decreasing the original value by one.
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
        if(kDown & KEY_LEFT)
        {
            if(offs == 2) offs = 4;
            else if(offs < 6) offs++;
        }
        if(kDown & KEY_RIGHT)
        {
            if(offs == 4) offs = 2;
            else if(offs) offs--;
        }
        
        if(kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))
        {
            bcdfix(buf + offs);
            printf("20%02X/%02X/%02X %02X:%02X:%02X\r", buf[6], buf[5], buf[4], buf[2], buf[1], buf[0]);
        }
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    killswitch:
    
    mcuExit();
    gfxExit();
    
    return 0;
}
