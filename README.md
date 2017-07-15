# RTChanger

RTChanger is a WIP homebrew application for the Nintendo 3DS system. RTChanger will allow you to change the Raw RTC of your system. This will be the only legally distributable open sourced Raw RTC changer! Changing the Raw RTC allows you to bypass most timegates in games. 

There are many applications in different games as some games have more than one time gate.

# Timegates?

Let's take Pokemon Sun and Moon for an example. You want to train your Pokemon in Poke Pelago. That can take two days or more. Then you want to grow berries which can take days. You also want to do the daily lottery in Festival Plaza, too. Imagine with RTChanger, you no longer have to wait. That is now a reality, and RTChanger can manually change your time ahead by any value you want. Almost every timegate in 3DS games will ge gone and you'll be able to rapidly train your Pokemon, grow thousands of berries within an hour, and get tons of stuff by the lottery.

Please note that there are some timegates which RTChanger will not be capable of bypassing. Some timegates are server based, meaning the server keeps the time on their end, and RTChanger cannot bypass these as it's client-side only and there is no possibility of server-based timegate bypassing.

# What is the Raw RTC?

Your system has a special hidden clock inside that continues to move forward just like the regular clock and cannot be changed outside of normal circumstances. Upon booting, many games check for differences in the RTC (Your normal system clock) and Raw RTC. Games can detect whether you changed your system clock. However, changing the Raw RTC prevents the games from detecting that if done properly.

# Q&A

Q: Wasn't it already possible to change the Raw RTC? Why was this developed?

A: Config, a SDK devkit tool which allows you to change your Raw RTC as well exists too. This tool however has been leaked on the internet far and wide. Due to being an official devkit tool, it was not intended to be leaked and was only for PANDA and SNAKE units (3DS and New 3DS developer consoles). As this is officially created by Nintendo itself, it is copyrighted and distribution of it can lead to DMCA takedown requests. 

As the Raw RTC was a value stored in the MCU, very few developers knew how to work around it. Sono, otherwise known as MarcusD, was one of these developers. There was little interest in dealing with a program which could do this, and as a new and aspiring developer, I wanted to create RTChanger especially considering reddit's /r/3DShacks community didn't have any way to change the Raw RTC, even with Boot9Strap! The only way to change it before now was to get a hold of Config.

---

Q: What is required for this?

A: This is a .CIA file and requires Custom Firmware and Boot9Strap. The preferred CFW is Luma3DS, but you can choose one at your own descretion. .CIA files are installed with FBI, as you should have gotten it installed in www.3ds.guide. 

---

Q: Will there be a .3DSX version?

A: I will consider making one, but be aware that Rosalina (and therefore Luma3DS) will still be required due to the escalated permissions needed.

---

Q: This involves the MCU. Is this safe?

A: Yes. What you may have heard was that changing the color of the LED can brick the MCU, but even then that was only for firmware versions below 8.0.0. You're safe to use this application.

---

Q: Does this affect my regular system time?

A: Sadly, yes.

---

Q: Is this legally redistributable?

A: Yes!

---

Q: Will this be shared on GBATemp?

A: I plan on it. I hope to release this to as many people as I can. I genuinely hope RTChanger will catch on in the community.

---

Q: Will there be any updates to this?

A: I don't honestly see the point of updating this once it's released. Unless a critical bug arises, there's really nothing else to be done with this application in particular.

# Contact Information

I'm currently most active on Reddit. If you need something urgent, or would like to ask a few questions, please contact me on Reddit. /u/x-the-komujin is the name.

# Credits (thus far)

Sono (AKA MarcusD). I would not have even started this project if it weren't for him. I am very new to coding and he has and currently is helping me with this project. A very, very helpful person.

The 3DSHacks #dev community on Discord. I had quite a few questions to ask there.

Smealum for ctrulib. I really needed that library.
