#include <stdio.h>
#include <3ds.h>
#include <string.h>
#include "mcu.h"
#include "banner_png.h"

typedef struct  
{
    u8 seconds;
    u8 minute;
    u8 hour;
    u8 something; //Unused value.
    u8 day;
    u8 month;
    u8 year;
} RTC;

const int cursorOffset[] = { //Sets a cursor below the selected value.
    19,
    16,
    13,
    0, //Unused offset.
    10,
    7,
    3
 };

void bcdfix(u8* wat)
{
    if((*wat & 0xF) == 0xF) *wat -= 6;
    if((*wat & 0xF) == 0xA) *wat += 6;
}

Result initServices(PrintConsole topScreen, PrintConsole bottomScreen){ //Initializes the services.
    gfxInit(GSP_RGB565_OES, GSP_BGR8_OES, false); //Inits both screens.
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
    
    unsigned char* image;
    unsigned width, height;
    
    GSPGPU_FlushDataCache(gpusrc, width*height*4);           //Ensures the 'banner.png' is in physical RAM.
    
    C3D_TexInit(&spritesheet_tex, width, height, GPU_RGBA8); //Loads the texture and bind it to the first texture unit.
    //Converts the image to 3DS tiled texture format.
    C3D_SafeDisplayTransfer ((u32*)gpusrc, GX_BUFFER_DIM(width,height), (u32*)spritesheet_tex.data, GX_BUFFER_DIM(width,height), TEXTURE_TRANSFER_FLAGS);
    gspWaitForPPF();
    C3D_TexSetFilter(&spritesheet_tex, GPU_LINEAR, GPU_NEAREST);
    C3D_TexBind(0, &spritesheet_tex);
    free(image);
    linearFree(gpusrc);
    
    lodepng_decode32(&image, &width, &height, banner_png, banner_png_size);
    Result res = mcuInit();
    return res;
}

void deinitServices(){
    mcuExit();
    gfxExit();
}

void mcuFailure(){
    printf("\n\nPress any key to exit...");
    while (aptMainLoop())
    {
        hidScanInput();
        if(hidKeysDown())
        {
            deinitServices();
            break;
        }
        gspWaitForVBlank();
    }
    return;
}

int main()
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
        mcuFailure();
        return -1;
    }
    printf ("\x1b[35m-\x1b[0m \x1b[31m-\x1b[0m \x1b[33m-\x1b[0m     "); //Mind the three printfs with seemingly garbage code. It just makes the bottom screen look pretty.
    printf ("  \x1b[32mRTChanger Version1.0\x1b[0m");
    printf ("\x1b[35m-\x1b[0m \x1b[31m-\x1b[0m \x1b[33m-\x1b[0m       \n \n");
    puts ("Welcome to RTChanger! \n");                                    //Notifications to user after booting RTChanger.
    puts ("Using this program, you can manually    change the Raw RTC."); //Extra spaces between words so that the screen doesn't separate them.
    puts ("The Raw RTC is your hidden System Clock.Editing this allows you to bypass       timegates.");
    puts ("As you may see, this Raw RTC is         different from the System Clock you have set.");
    puts ("More information can be found at my     GitHub."); 
    puts ("I highly recommend you view the README  if you haven't already.");
    puts ("Please change your time or START to     return to the Home Menu. \n \n \n");
    puts ("\x1b[36mhttps://www.github.com/Storm-Eagle20/RTChanger\x1b[0m");
    consoleSelect(&topScreen);
    
    RTC mcurtc;
    mcuReadRegister(0x30, &mcurtc, 7);
    RTC rtctime;
    
    u32 kDown = 0;
    u32 kHeld = 0;
    u32 kUp = 0;
    
    u8* buf = &rtctime;
    u8 offs = 0;
    while (aptMainLoop()) //Detects the input for the A button.
    {   
        printf ("\x1b[0;0H");
        puts ("Here you can change your time. Changing backwards is not recommended.");
        puts ("Change your time by however you may need.");
        puts ("The format is year, month, day, then hours,        minutes, and seconds.");
        puts ("When you are done setting the Raw RTC, press A to save the changes. \n");
        
        hidScanInput();               //Scans for input.
        kDown = hidKeysDown();        //Detects if the A button was pressed.
        kHeld = hidKeysHeld();        //Detects if the A button was held.
        kUp = hidKeysUp();            //Detects if the A button was just released.
        
        if(kHeld & KEY_START) break;  //User can choose to continue or return to the Home Menu.  
        
        if(kDown & (KEY_UP))          //Detects if the UP D-PAD button was pressed.
        {    
            buf[offs]++; //Makes an offset increasing the original value by one.
            switch(offs)
            {   
                case 0:  //seconds
                case 1:  //minutes
                    break;
                    
                case 2:  //hours
                    break;
                    
                case 4:  //days
                    break;
                    
                case 5:  //months
                    break;
                    
                case 6:  //years
                    break;
            }       
        }
        if(kDown & (KEY_DOWN))       //Detects if the UP D-PAD button was pressed.
        {    
            buf[offs]--; //Makes an offset decreasing the original value by one.
            switch(offs)
            {
                case 0:  //seconds
                case 1:  //minutes
                    break;
                    
                case 2:  //hours
                    break;
                    
                case 4:  //days
                    break;
                    
                case 5:  //months
                    break;
                    
                case 6:  //years
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
        
        if(kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) //The code for allowing the user to change the time.
        {
            bcdfix(buf + offs);
            printf("20%02X/%02X/%02X %02X:%02X:%02X\n", buf[6], buf[5], buf[4], buf[2], buf[1], buf[0]);
            printf("%*s\e[0K", cursorOffset[offs], "^^"); //The cursor.
        }
        if(kDown & KEY_A) //Allows the user to save the changes. Not implemented yet.
        {
        }
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    
    return 0;
}
