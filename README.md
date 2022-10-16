sc2-map-analyzer
================

StarCraft 2 Map Analyzer

An attempt by a non-programmer in October 2022 to get the SC2 Map Analyzer program to work on modern maps.


Assumptions:
jnmaloney's fork of RFDaemoniac's copy of Dimfish's source code is the most up to date version of this program to start from (https://github.com/jnmaloney/sc2-map-analyzer).
The compiled version seen on this web page is from the source code of previously mentioned fork (https://tl.net/forum/sc2-maps/414326-map-analyser-tool).


Current Status:
The version of the program from the TL.net forum is able to open maps saved by the modern editor, however at the moment it is unable to properly parse all the data (most importantly it cannot currently see ramps and LoSB). 

Problems:
Ramps are not read.
LoSBs are not read.
Photon Overcharge is no longer relevant.
Vespine inefficiencies are no longer as relevant.
Only 3 bases are recognized per side.
"Buildable" checkbox does nothing, possibly build grid is not being read.
"Pathable" checkbox does nothing,
No distance measurement provided with pathing paths enabled.


Take everything state here with a grain of salt. I know nothing of C++ and barely anything of programming in general.
I am taking on this challenge because I am a SC2 mapmaker and would love to see this thing working again.
