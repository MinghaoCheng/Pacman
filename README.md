![Logo](/Docs/pictures/pacman-logo.gif)
# Pacman
ENG5220, Embedded real-time programming project, Team19

|Author|Team 19|
|---|---
|E-mail|2359434c@student.gla.ac.uk

## Motivation& Introduction

As a classic computer game, Pacman impressed lots of people. Now we can do a Pacman game in real life!

This project uses a game panel which contains two 4x4 LED matrices. One of them is in green working as Pacdots, and the other one is in red to represent the Ghost. There is also a 4x4 LDR matrix to give feedback to the host. Three matrices are on the same place and controlled by the host raspberryPi.

There is a car controlled by a raspberryPi which acts as Pacman. It can move around upon the pannel, eat Pacdots and hide from the Ghost.

## Table of contents
* [Getting started](#getting_started)
* [System overview](#system_overview)
* [Required components(BOM)](#required_components(BOM))
* [Assemble](#assemble)
* [Software](#software)
* [Found a bug](#found_a_bug)
* [License](#license)
## Getting started <a name="getting_started"></a>
1. Buiding hardware  
      Hardware contains PCBs, mechanics, sodering, see [Hardware](#hardware) for details.  
2. Built& install software  
    1. Host  
      To build the software of host: on host raspberryPi, type ```./build_host.sh``` in terminal.  
      To install the software of host:type ```sudo ./install_host.sh``` in terminal. (Only the kernel module needs to be installed)  
    2. Car  
      ~~To be added~~  
3. Execute the programme  
    1. Host  
      type "sudo ./host" in terminal. (Because this programme would access the kernel module, so it requires super_user)  
    2. Car  
      ~~To be added~~  
4. Game rules  
      Once the game starts(After the background music), the Pacdots(green LEDs) would all be on. The Pacman(the Car) would follow the instructions of keyboard connected to the host. If the Pacman eats a Pacdot (car hits the green LED), the LDR would give feedback to the host and shut down the corresponding LED. Meanwhile, a sound would give the player a feedback. The player would win the game when all Pacdots are eaten (a winning music would be played). There is only one ghost(red LED), which would move randomly, if the Pacman hits the ghost, the player will lose (a losing music would be played).  
5. Have fun!  

## System overview<a name="system_overview"></a>

## Required components(BOM)<a name="required_components(BOM)"></a>
* host_0 board

|Component|Quantity|Farnell code|
|---|---|---
|4816P-T02-222LF|1|2908136
|LM2901D|4|1648673
|Resistor,2kΩ,R0805|1|1469887
|Resistor,10kΩ,R0805|2|1469856
|MCP23S17T-E/SS|1|2775058
|pin header 2x8, bended, 2.54mm|2|2984492
|Capacitor, 0.1uF, C0603|1|2280842
|M20-6102045,Connector|1|1569230

* host_1 board

|Component|Quantity|Farnell code|
|---|---|---
|MCP23S17T-E/SS|1|2775058
|BCW66GLT1G|4|2317524
|BCW68GLT1G|4|2317538
|4816P-1-102LF|2|1612549
|M20-6102045,Connector|1|1569230
|pin header 2x8, bended, 2.54mm|1|2984492
|Capacitor, 0.1uF, C0603|1|2280842
|Resistor,200Ω,R0805*|8|1894175

    *:optional,depends on the impedence of the LEDs  

* car_0 board

|Component|Quantity|Farnell code|
|---|---|---
|L298N|1|403295
|NCP59301DS50R4G|1|2774711
|BYG10M-E3/TR|8|9549170
|Capacitor, 0.1uF, C0805|2|

* Others

|Component|Quantity|Farnell code|
|---|---|---
|RapsberryPi model 3B|2|
|Green LED*|16|
|Red LED*|16|
|LDR|16|3168335
|Wires|N/A|
|Panel**|1|

    *:could use any LEDs under 100mA, but the resistors on host_1 board needs to be adjusted  
    **:In this project, the panel was made of a cardboard.
## Assemble
## Software

## Found a bug<a name="found_a_bug">
     
## License

dependency:
~~sudo apt-get install alsa-utils~~
~~sudo apt-get install libasound2-dev~~
