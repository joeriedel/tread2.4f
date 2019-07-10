************************************************************************
			TREAD3D2 VERSION 1.0 README
************************************************************************

Table of Contents
1. Introduction
2. System Requirements
3. Installation
4. Configuring Tread for the First Run
5. Running Tread for the First Time
7. Known Bugs
8. Unknown Bugs
9. Options Dialog
10. Tread and support of MODs

*****************
*1. Introduction*
*****************
Welcome to Tread3D2 Version 1.0.  This file will help you through the installation and basic features of Tread.  

************************
*2. System Requirements*
************************
- Windows 98/NT with IE5, Windows 95 and non IE5 support has not been tested
- Pentium Equivalent or Greater 
- A minimum of 128 megs of ram, 256 megs recommended.
- 10 MB Hard Disk space for installation 
- 15 MB Hard Disk space once installed 
- 800x600 minimum resolution, 1024x768 or greater is highly recommended
- 16bit color or higher recommended 
- Quake/Quake2/Hexen2 (The games are the whole reason you want a map editor right?) 

***************** 
*3. Installation*
*****************
Before running setup for Tread please make sure you have Quake or Quake2 or Hexen2 installed.  This is not a requirement but it will make your life a little easier since Tread automatically finds the games install directories during setup.  Since you are reading this file we assume you have already unzipped the zip file that you downloaded from the Internet.  Now simply double click setup.exe in that same directory this Readme.txt is located and you'll be on your way.

****************************************
*4. Configuring Tread for the First Run*
****************************************
Because Quake's BSP program QBSP requires wad files, Quake does not come with everything needed to build levels.

For your convenience we have created a program called ExtPak2. ExtPak2 will extract the Quake textures and update Tread's settings. Do not run ExtPak2 while Tread is running. Settings written by ExtPak2 will be lost if you do. This program should be located on the Start Menu under the Tread group.

Once the installation has completed, preliminary settings for Tread should be in place. Tread's installation searches for supported games and sets up the necessary settings. The installation will also check for supported texture packages.

*************************************
*5. Running Tread for the First Time*
*************************************
Once you have successfully run Tread's installation program and optionally extracted Quake textures using ExtPak2, you can launch Tread by clicking on its icon under the start menu.

Tread should give you a message notifying you that it has detected that this is the first time you have run the program. Click "OK". Tread will now display a dialog showing the current configurations. You can look them over and change them if necessary. Once you have finished, click "Ok". Tread will now inform you that you must restart.

Restart Tread by clicking on its icon under the start menu. This time, Tread should load a splash screen and print out it's progress as it performs start-up tasks. If nothing goes wrong, the main application window should open maximized, and you should be on your way. Enjoy!

***************
*6. Known Bugs*
***************
Below are known bugs, so that makes them features.

1. You can attempt to build maps during a currently executing build. You can really screw things up doing this, so don't say we didn't warn you.

2. You can occasionally resize things into oblivion.

*****************
*7. Unknown Bugs*
*****************
Oh, dear God!

Find em', mail em: jmriedel@planetquake.com

BTW: We need ALL the information possible: 
- What you were doing at the time. 
- A short list of steps to reproduce the problem. 
- What happened (crash, assertion, something funny). If Tread did crash, what module did it crash in? 
- Give us as many details as you can squeeze from your brain.

Bear in mind that if we cannot reproduce the problem, the report is useless to us. If you cannot reproduce a bug, let us know what happened, but be aware that we may not be able to fix it without more information.

*******************
*8. Options Dialog*
*******************
Game configurations Tab:
This section of the options dialog allows you to configure build programs and game executables for all the games that Tread supports.

You can select the game you wish to configure by selecting a game from the game list. 

Game Directory: 
Specifies the full path of the directory where the game's executable is located.

Game Executable: 
Specifies the full path of the games executable file.

Compiled Map Directory: 
Specifies full path where map building and intermediate building should take place. BSP files are also placed in this directory.

BSP/LIGHT/VIS: 
The full path of the BSP, light or vis executable used to do map processing.

*******************************
*9. Tread and support of MODs*
*******************************
We love Quake and Quake2 and one of the single biggest goals of Tread is to facilitate the use of MODs and TC's. That's the only reason Tread has a scripting language. We have included a script editor called GDFEdit. 


We will be releasing full documentation on Tread shortly.

*****************
*11. Legal Stuff*
*****************
Tread is Copyright © 1999 Prospect Vector, All Rights Reserved.

Other product and company names herein may be trademarks of their respective owners.
