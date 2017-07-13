//Notifications to user after booting RTChanger.

#include <stdio.h>
#include "mcu.h"

int main ()
{
  printf ( "Welcome to RTChanger! \n" );
  printf ( "Using this program, you can manually change the Raw RTC. \n" );
  printf ( "The Raw RTC is your System Clock. Editing this allows you to bypass timegates. \n" );
  printf ( "More information can be found at my GitHub. \n \n" ); //Filler text. No GitHub yet.
  printf ( "Press A to continue." );
    while ( aptmainloop() ) //Detects the input for the A button.
{
      hidscaninput();
      kDown == hidKeysDown(); //Detects is the A button was pressed.
      if (kDown == KEY_A)
{
        consoleClear();
        printf ( "Please change the time below. It does not match your current time you see on the Home Menu, this is normal. Change the time by however much you need necessary, changing the time backwards is not recommended." );

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
    
   }
  } 
 }
}
