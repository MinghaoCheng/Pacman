![Logo](/Docs/pictures/pacman-logo.gif)
# Pacman
ENG5220, Embedded real-time programming project, Team19

|Author|Team 19|
|---|---
|E-mail|2359434c@student.gla.ac.uk

* The follwing link is two demostrations of playing the game, catched by the ghost and won the game
https://www.youtube.com/playlist?list=PLmhQhGby8E4eoPKThM3S1RnaBH7sxEcJI

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
      To build the software of car: on car raspberryPi, type ```./build_car.sh``` in terminal.
3. Execute the programme  
    1. Host  
      Type "sudo ./host_exe" in terminal. (Because this programme would access the kernel module, so it requires super_user)  
    2. Car  
      Type "./car_exe" in terminal.
4. Game rules  
      Once the game starts(After the background music), the Pacdots(green LEDs) would all be on. The Pacman(the Car) would follow the instructions of keyboard connected to the host. If the Pacman eats a Pacdot (car hits the green LED), the LDR would give feedback to the host and shut down the corresponding LED. Meanwhile, a sound would give the player a feedback. The player would win the game when all Pacdots are eaten (a winning music would be played). There is only one ghost(red LED), which would move randomly, if the Pacman hits the ghost, the player will lose (a losing music would be played).  
5. Have fun!  

## System overview<a name="system_overview"></a>
* This project contains a host and a car. The host does the following work:  
    ** Control a red LED matrix using a GPIO extender "MCP23S17" to display the location of the ghost
    ** Control a green LED matrix using a GPIO extender "MCP23S17" to display the pacdots
    ** Control a LDR matrix as the feedback of location of the car
    ** Handle a TCP/IP connection as the server, and send instructions to the car via the connection
    ** Connect with a simple keyboard as input
    ** Output sound through 3.5mm audio jack (Using function "system()" now, I would add ALSA sound driver later)  
* The car work as follow:
    ** Handle a TCP/IP connection as client to receive instructions from the host
    ** Control two motors(which drive the wheels of the car) through a dual H-bridge IC "L298N"
    ** The car sink current from 4xAA batteries and a 5V3A LDO is used.

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
* For PCBs of the host, connect them in the way as follow:  

<div align=center><img width="400" height="400" src="/Docs/pictures/host_brd0.jpg"/></div>  

<div align=center><img width="400" height="400" src="/Docs/pictures/host_brd1.jpg"/></div>  

* For LED&LDR matrices, put one green LED, one red LED and one LDR at a single hole on the panel(There are 4x4 holes). Note to put LDR above the LEDs.  

<div align=center><img width="400" height="400" src="/Docs/pictures/Pannel_0.jpg"/></div>  

* Solder green LED matrix/red LED matrix as follow, this schematic also can be found at [PCBs](/Docs/PCBs):

<div align=center><img width="400" height="400" src="/Docs/pictures/LED_matrix0.png"/></div> 

* Solder keyboard following the schematic

* Picture of the system set

<div align=center><img width="400" height="400" src="/Docs/pictures/System_0.jpg"/></div>

<div align=center><img width="400" height="400" src="/Docs/pictures/System_1.jpg"/></div>

## Software
* There are two source codes in this project. One works on the host and another one works on the car.  
* The software of the host contains a kernel module which would work as the driver of the GPIO(it handles GPIO interrupt and output of the GPIO used by the host), and it contains the main codes which works in the user space. There are several components of this software, see details at [Docs](/Docs/software/host)  
* The software of the car controls the motor using L298N through GPIO, and it also receives instructions from the host over TCP/IP through Wi-Fi connection. See details at [Docs](/Docs/software/car)   
* dependency:  
      * host:
      ```pthread, rt, ```
      * car:
      ```pthread```
## Found a bug<a name="found_a_bug">
* Hardware:  
      To isolate the fault of the hardware, the best way is to check the PCB-layouts and schematics.  
      Here is the definition of pins used by the host:  
      
|Raspi-pin|Function|
|---|---
|03|Reset of MCP23S17(s)
|05|INT B of MCP23S17 of LDR board
|07|INT A of MCP23S17 of LDR board
|19|MOSI of SPI bus
|21|MISO of SPI bus
|23|CLK of SPI bus
|24|CS_0 of SPI bus, LDR board
|26|CS_1 of SPI bus, LED board
|29|Keyboard, Left
|31|Keyboard, Up
|33|Keyboard, Right
|35|Keyboard, Down
|37|Keyboard, Reset
      
* Software:
      Remember to add "sudo" to install the host kernel module and execute the programme
      
* For other questions, you can send me an Email with description

## License
See license.md
