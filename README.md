# RTChanger

RTChanger is a WIP homebrew application for the Nintendo 3DS system. RTChanger will allow you to change the Raw RTC of your system. This will be the only legally distributable open sourced Raw RTC changer! Changing the Raw RTC allows you to bypass most timegates in games. 

There are many applications in different games as some games have more than one time gate.

# Timegates?

Let's take Pokemon Sun and Moon for an example. You want to train your Pokemon in Poke Pelago. That can take two days or more. Then you want to grow berries which can take days. You also want to do the daily lottery in Festival Plaza, too. Imagine with a new homebrew application, you no longer have to wait. That is now a reality, and RTChanger can manually change your time ahead by any value you want. Almost every timegate in 3DS games will ge gone and you'll be able to rapidly train your Pokemon, grow thousands of berries within an hour, and get tons of stuff by the lottery.

Please note that there are some timegates which RTChanger will not be capable of bypassing. Some timegates are server based, meaning the server keeps the time on their end, and RTChanger cannot bypass these as it's client-side only and there is no possibility of server-based timegate bypassing.

# What is the Raw RTC?

Your system has a special hidden clock inside that continues to move forward just like the regular clock and cannot be changed outside of normal circumstances. Upon booting, many games check for differences in the RTC (Your normal system clock) and Raw RTC. Games can detect whether you changed your system clock. However, changing the Raw RTC prevents the games from detecting that if done properly.

# Q&A

Q: Wasn't it already possible to change the Raw RTC? Why was this developed?

A: Config, a SDK devkit tool which allows you to change your Raw RTC as well exists too. This tool however has been leaked on the internet far and wide. Due to being an official devkit tool, it was not intended to be leaked and was only for PANDA and SNAKE units (3DS and New 3DS developer consoles). As this is officially created by Nintendo itself, it is copyrighted and distribution of it can lead to DMCA takedown requests. 

As the Raw RTC was a value stored in the MCU, very few developers knew how to work around it. Sono, otherwise known as MarcusD, was one of these developers. There was little interest in dealing with a program which could do this, and as a new and aspiring developer, I wanted to create RTChanger especially considering reddit's /r/3DShacks community didn't have any way to change the Raw RTC, even with Boot9Strap! The only way to change it before now was to get a hold of Config.

---

Q: What is required for this?

A: This will be a .CIA and .3DSX file and requires Custom Firmware and Boot9Strap. The .3DSX also requires Luma3DS as it needs Rosalina to patch the Service Manager (SM) module. The preferred CFW for the .CIA is Luma3DS, but you can choose one at your own descretion. .CIA files are installed with FBI, as you should have gotten it installed in www.3ds.guide. 


.3DSX files are used with an entrypoint or Rosalina. The recommended entrypoint is the HBLauncher Loader. HBLauncher Loader is given in the link below.

https://github.com/yellows8/hblauncher_loader/releases

If you're unfamiliar with .3DSX files, you'll need fincs' Homebrew Launcher 2.0. Get the boot.3dsx and put it at the root of your SD card.

https://github.com/fincs/new-hbmenu/releases

# **You must patch Service Manager to use the .3DSX version.**

Open Rosalina [`L+Down+Select`], go to `Process patches menu`, and select `Patch SM for service checks.` You CANNOT use the .3DSX version of RTChanger without doing this first. The .CIA version doesn't require this, however.

While you can use any entrypoint you like, here is the release page for the HBLauncher_Loader .CIA, which will allow you to access userland to boot up the .3DSX if you don't have access to any other entrypoint.

If preferred, you can also use Rosalina with my program. This only requires the boot.3dsx for the Homebrew Launcher 2.0 by fincs. Open Download Play on the Home Menu, then open Rosalina [`L+Down+Select`] and patch SM if you haven't already by going to `Process patches menu` and selecting `Patch SM for service checks.` After that, go to `Miscellaneous options...` and select `Switch the hb. title to the current app`. **Do not attempt to take over any title except for Download Play. RTChanger will crash and any bug reports will be closed if the user did not use Download Play.**

If the patch succeeds, press B to exit Rosalina and exit Download Play by pressing the Home Menu button and closing it. Open Download Play again and you should be in the Homebrew Menu, and you can open my application provided you patched Service Manager. 

---

Q: Will there be a GodMode9 script?

A: I have reached out to d0k3, however it is his decision as to whether he wishes to implement RTC scripting on GodMode9. As of writing this I have not received a response as to whether he'll do anything. I can attempt to port RTChanger to GodMode9 when the project is finished, however there is no guarantees this can be ported and I am not going to disturb him more than once on this issue.

---

Q: This involves the MCU. Is this safe?

A: Yes. What you may have heard was that changing the color of the LED can brick the MCU, but that was not true as the brick yellows8 got was a NAND brick.

---

Q: Does this affect my regular system time?

A: Sadly, yes.

---

Q: Is this legally redistributable?

A: Yes! Feel free to share it to friends, but I recommend you send them to the GitHub first.

---

Q: Will this be shared on GBATemp?

A: I plan on it. I hope to release this to as many people as I can. I genuinely hope RTChanger will catch on in the community.

---

Q: Will there be any updates to this?

A: I don't honestly see the point of updating this once it's released. Unless a critical bug arises, there's really nothing else to be done with this application in particular.

# Disclaimer

I am not responsible for any bans or any ingame punishment of any sort from the use of RTChanger. You are highly unlikely to get any sort of ban, but there have been ban waves before. Quoting Plailect from his guide (if you have installed CFW recently you should already have this done):

>There have been reports of a wave of bans being handed out to CFW users by Nintendo. To protect yourself, please do the following steps before starting this guide:
>1.  Go to System Settings, then “Internet Settings”, then “SpotPass”, then “Sending of System Information”
>2.  Disable the “Sending of System Information” option
>3.  Exit the System Settings
>4.  Go to your Friend’s List (the Face Icon in the top row of your Home Menu)
>      * If you receive an error and are kicked out of the menu, the Friend’s List setting is already disabled
>6.  Go to the Friend’s List settings, then “Friend Notification Settings”, then “Show friends what you’re playing”
>7.  Disable the “Show friends what you’re playing” option
>8.  Exit your Friend’s List

Credits to Plailect (/u/Plailect), owner of www.3ds.guide. 

# Contact Information

I'm currently most active on Reddit. If you need something urgent, or would like to ask a few questions, please contact me on Reddit. /u/x-the-komujin is the name.

# Credits (thus far)

Sono (AKA MarcusD). I would not have even started this project if it weren't for him. I am very new to coding and he has and currently is helping me with this project. A very, very helpful person.

LiquidFenrir on Discord. I'm not joking when I say that he seriously assisted me in the entirety of my code around the binary coded decimals, which I was completely unfamiliar with. It would have taken weeks to get that code done without him.

The 3DSHacks #dev community on Discord. I had quite a few questions to ask there. I also desperately needed new Makefiles to create .CIA and .3DSX files.

Smealum for ctrulib. I really needed that library.
