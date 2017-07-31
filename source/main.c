#include <stdio.h>
#include <3ds.h>
#include <string.h>

#include "mcu.h"

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
    u8 something; //Unused offset.
    u8 day;
    u8 month;
    u8 year;
} RTC;

const int offsNext[] = {
    1,
    2,
    6,
    0, //UNUSED_OFFSET
    0,
    4,
    5
};

const int offsPrevious[] = {
    4,
    0,
    1,
    0, //UNUSED_OFFSET
    5,
    6,
    2
};

const int cursorOffset[] = { //Sets a cursor below the selected value.
    19,
    16,
    13,
    0, //Unused offset.
    10,
    7,
    3
};

u8 maxValue[] = {
    60,
    60,
    24,
    0, //UNUSED_OFFSET
    32,
    13,
    100
};

const u8 minValue[] = {
    0,
    0,
    0,
    0, //UNUSED_OFFSET
    1,
    1,
    0
};

void setMaxDayValue(RTC * rtctime)
{
    int year = rtctime->year+2000;
    int maxDayValue = 30;
    
    //30, 31, 30 gets shifted after August.
    maxDayValue += (rtctime->month % 2) ^ (rtctime->month >= 8);
    
    //Accounts for leap years and February.
    if(rtctime->month == 2)
    {
        maxDayValue -= 2;
        if(year % 4 == 0)
        {
            if(year % 100 == 0)
            {
                if(year % 400 == 0) maxDayValue++;
            }
            else maxDayValue++;
        }
    }
    u8 previousMax = maxValue[DAY_OFFSET]-1;
    maxValue[DAY_OFFSET] = maxDayValue+1;
    if(rtctime->day == previousMax) rtctime->day = maxDayValue;
}

Result initServices(PrintConsole topScreen){ //Initializes the services.
    consoleInit(GFX_TOP, &topScreen);
    Result ret = mcuInit();
    return ret;
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

void RTC_to_BCD(RTC * rtctime)
{
    u8 * bufs = (u8*)rtctime;
    for (int i = 0; i < UNITS_AMOUNT; i++)
    {
        u8 units = bufs[i] % 10;
        u8 tens = (bufs[i] - units) / 10;
        bufs[i] = (tens << 4) | units;
    }
}

void BCD_to_RTC(RTC * rtctime)
{
    u8 * bufs = (u8*)rtctime;
    for (int i = 0; i < UNITS_AMOUNT; i++)
    {
        u8 units = bufs[i] & 0xF;
        u8 tens = bufs[i] >> 4;
        bufs[i] = (10 * tens) + units;
    }
}

int main ()
{
    gfxInit(GSP_RGB565_OES, GSP_BGR8_OES, false); //Inits both screens.
    PrintConsole topScreen, bottomScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
    consoleSelect(&bottomScreen);
    Result ret = mcuInit();
    if(R_FAILED(ret))
    {
        consoleSelect(&topScreen);
        printf("Failed to init MCU: %08X\n", ret);
        puts("If this error persists, use Rosalina to patch\n Service Manager.\n");
        mcuFailure();
        return -1;
    }
    
    puts ("\x1b[35m-\x1b[0m\x1b[31m-\x1b[0m\x1b[33m-\x1b[0m       \x1b[32mRTChanger Version1.0\x1b[0m       \x1b[35m-\x1b[0m\x1b[31m-\x1b[0m\x1b[33m-\x1b[0m");
    puts ("Welcome to RTChanger! \n"); //Notifications to user after booting RTChanger.
    puts ("Using this program, you can manually\nchange the Raw RTC.");
    puts ("The Raw RTC is your hidden System Clock.Editing this allows you to bypass       timegates.");
    puts ("As you may see, this Raw RTC is \ndifferent from the System Clock you have set.");
    puts ("More information can be found at my \nGitHub."); 
    puts ("I highly recommend you view the README \nif you haven't already.");
    puts ("Please change your time or START to \nreturn to the Home Menu. \n \n \n");
    puts ("\x1b[36mhttps://www.github.com/Storm-Eagle20/RTChanger\x1b[0m");
    
    consoleSelect(&topScreen);
    
    printf ("\x1b[0;0H");
    puts ("Here you can change your time. Changing backwards is not recommended.");
    puts ("Change your time by however you may need.");
    puts ("The format is year, month, day, then hours, \nminutes, and seconds.");
    puts ("When you are done setting the Raw RTC, press A to save the changes. \n");
    
    RTC rtctime = {0};
    mcuReadRegister(0x30, &rtctime, UNITS_AMOUNT);
    BCD_to_RTC(&rtctime);
    
    u32 kDown = 0;
    u32 kHeld = 0;
    u32 kUp = 0;
    
    u8 * bufs = (u8*)&rtctime;
    int offs = 0;
    
    while (aptMainLoop()) //Detects the user input.
    {
        hidScanInput();               //Scans for input.
        kDown = hidKeysDown();        //Detects if the A button was pressed.
        kHeld = hidKeysHeld();        //Detects if the A button was held.
        kUp = hidKeysUp();            //Detects if the A button was just released.
        
        printf("\x1b[0;0H");
        printf("\n\n\n\n\n\n\n\n%4.4u/%2.2u/%2.2u %2.2u:%2.2u:%2.2u\n", rtctime.year+2000, rtctime.month, rtctime.day, rtctime.hour, rtctime.minute, rtctime.seconds);
        printf("%*s\e[0K\e[1A\e[99D", cursorOffset[offs], "^^"); //Displays the cursor and time.
        
        if(kHeld & KEY_START) break;  //User can choose to continue or return to the Home Menu.  
        
        if(kDown & (KEY_UP))          //Detects if the UP D-PAD button was pressed.
        {
            bufs[offs]++;
            if(bufs[offs] == maxValue[offs]) bufs[offs] = minValue[offs];
        }
        if(kDown & (KEY_DOWN)) //Detects if the DOWN D-PAD button was pressed.
        {
            bufs[offs]--;
            if(bufs[offs] < minValue[offs] || bufs[offs] >= maxValue[offs]) bufs[offs] = maxValue[offs]-1;
        }
        if(kDown & KEY_LEFT) //Detects if the left button was pressed.
        {
            offs++;
            if(offs == UNUSED_OFFSET) offs++;
            if(offs > YEAR_OFFSET) offs = SECONDS_OFFSET;
        }
        if(kDown & KEY_RIGHT) //Detects if the right buttn was pressed.
        {
            offs--;
            if(offs == UNUSED_OFFSET) offs--;
            if(offs < SECONDS_OFFSET) offs = YEAR_OFFSET;
        }
        
        if(kDown & KEY_A) //Allows the user to save the changes.
        {
            RTC_to_BCD(&rtctime);
            ret = mcuWriteRegister(0x30, &rtctime, UNITS_AMOUNT);
            BCD_to_RTC(&rtctime);
            
            mcuExit();
            gfxExit();
            
            APT_HardwareResetAsync();
        }
        
        setMaxDayValue(&rtctime);
        
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    
    mcuExit();
    gfxExit();
    
    return 0;
}
