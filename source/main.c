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

#define UNITS_AMOUNT 7

enum {
    SECONDS_OFFSET = 0,
    MINUTES_OFFSET,
    HOURS_OFFSET,
    UNUSED_OFFSET,
    DAY_OFFSET,
    MONTH_OFFSET,
    YEAR_OFFSET
};

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

const int offsNext[] = {
    1,
    2,
    6,
    0, //Unused offset.
    0,
    4,
    5
};

const int offsPrevious[] = {
    4,
    0,
    1,
    0, //Unused offset.
    5,
    6,
    2
};

const int cursorOffset[] = { //Supposed to set a cursor below the highlighted number. Doesn't work currently.
    19,
    16,
    13,
    0, //Unused offset.
    2,
    5,
    10
};

u8 maxValue[] = { //Supposed to set the maximum value. Doesn't work properly.
    60,
    60,
    24,
    0, //Unused offset.
    32,
    13,
    100
};

const u8 minValue[] = { //Supposed to set the minimum value. Doesn't work properly.
    0,
    0,
    0,
    0, //Unused offset.
    1,
    1,
    0
};

void setMaxDayValue(RTC rtctime)
{
    int year = rtctime.year+2000;
    int maxDayValue = 30;
    
    //30, 31, 30 gets shifted after august
    maxDayValue += (rtctime.month % 2) ^ (rtctime.month >= 8);
    
    //leap years and february
    if(rtctime.month == 2)
    {
        maxDayValue -= 2;
        if (year % 4 == 0)
        {
            if (year % 100 == 0)
            {
                if (year % 400 == 0) maxDayValue++;
            }
            else maxDayValue++;
        }
    }
    
    maxValue[DAY_OFFSET] = maxDayValue+1;
}

void handleOverflow(RTC * rtctime) //Handles overflows to presumably prevent users from going above 59 seconds as an example.
{
    u8 * bufs = (u8*)rtctime;
    for(int i = 0; i < UNITS_AMOUNT; i++)
    {
        if(bufs[i] >= maxValue[i] && i+1 < UNITS_AMOUNT)
        {
            if(i == UNUSED_OFFSET) continue;
            bufs[i] = minValue[i];
            int offset = 1;
            if (i+offset == UNUSED_OFFSET) offset++;
            bufs[i+offset]++;
        }
            
    }
}

void changeRTCValue(RTC * rtctime, int offset, int change)
{
    u8 * bufs = (u8*)rtctime;
    bufs[offset] += change;
    if(bufs[offset] < minValue[offset] || bufs[offset] > maxValue[offset])
        bufs[offset] = maxValue[offset]-1;
}
void bcdfix(u8* wat)
{
    if((*wat & 0xF) == 0xF) *wat -= 6;
    if((*wat & 0xF) == 0xA) *wat += 6;
}

void BCDtoByte(u8 * BCD, u8 * Byte)
{
   for (int i = 0; i < UNITS_AMOUNT; i++)
    {
        Byte[i] = (BCD[i] & 0xF) + (10 * (BCD[i] >> 4));
    }
}

void ByteToBCD(u8 * Byte, u8 * BCD)
{
    for (int i = 0; i < UNITS_AMOUNT; i++)
    {
        int units = Byte[i] % 10;
        int tens = (Byte[i] - units)/10;
        BCD[i] = (tens << 4 | units);
    }
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
    puts ("Welcome to RTChanger! \n");                                    //Notifications to user after booting RTChanger.
    puts ("Using this program, you can manually    change the Raw RTC."); //Extra spaces between words so that the screen doesn't separate them.
    puts ("The Raw RTC is your hidden System Clock.Editing this allows you to bypass       timegates.");
    puts ("As you may see, this Raw RTC is         different from the System Clock you have set.");
    puts ("More information can be found at my     GitHub."); 
    puts ("I highly recommend you view the README  if you haven't already.");
    puts ("Please change your time or START to     return to the Home Menu. \n \n \n");
    puts ("\x1b[36mhttps://www.github.com/Storm-Eagle20/RTChanger\x1b[0m");
    consoleSelect(&topScreen);
    
    u8 bcdRTC[UNITS_AMOUNT] = {0};
    mcuReadRegister(0x30, bcdRTC, UNITS_AMOUNT);
    RTC rtctime;
    
    u32 kDown = 0;
    u32 kHeld = 0;
    u32 kUp = 0;
    
    u8* buf = &rtctime;
    u8 offs = 0;
    while (aptMainLoop())                           //Detects the input for the A button.
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
        
        if(kDown & (KEY_UP))     //Detects if the UP D-PAD button was pressed.
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
        if(kDown & (KEY_DOWN))    //Detects if the UP D-PAD button was pressed.
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
        
        if(kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))
        {
            bcdfix(buf + offs);
            printf("20%02X/%02X/%02X %02X:%02X:%02X\r", buf[6], buf[5], buf[4], buf[2], buf[1], buf[0]);
        }
        if(kDown & KEY_A)
        {
            ByteToBCD((u8*)&rtctime, bcdRTC);
            mcuWriteRegister(0x30, bcdRTC, UNITS_AMOUNT);
        }
        
        setMaxDayValue(rtctime);
        handleOverflow(&rtctime);
        setMaxDayValue(rtctime);
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    killswitch:
    
    mcuExit();
    gfxExit();
    
    return 0;
}
