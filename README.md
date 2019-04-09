# Pacman
ENG5220, Embedded real-time programming project, Team19
****
|Author|Team 19|
|---|---
|E-mail|2359434c@student.gla.ac.uk
****

# Motivation& Introduction

As a classic computer game, Pacman impressed lots of people. Now we can do a Pacman game in real life!

This project uses a game panel which contains two 4x4 LED matrices. One of them is in green working as Pacdots, and the other one is in red to represent the Ghost. There is also a 4x4 LDR matrix to give feedback to the host. Three matrices are on the same place and controlled by the host raspberryPi.

There is a car controlled by a raspberryPi which acts as Pacman. It can move around upon the pannel, eat Pacdots and hide from the Ghost.

## Table of contents
* [Getting started](#getting started)
* [Hardware](#hardware)
* [Software](#software)
### Getting started
1. Buiding hardware
    Hardware contains PCBs, mechanics, sodering, see [Hardware](#hardware) for details.
2. Built& install software
    2.1 Host
    To build the software of host: on host raspberryPi, type "./build_host.sh" in terminal.
    To install the software of host:type "sudo ./install_host.sh" in terminal.
        (Only the kernel module needs to be installed)
    2.2 Car
    ~~To be added~~
3. Execute the programme
    3.1 Host
    type "sudo ./host" in terminal. (Because this programme would access the kernel module, so it requires super_user)
    3.2 Car
    ~~To be added~~
4. Game rules
    Once the game starts(After the background music), the Pacdots(green LEDs) would all be on. The Pacman(the Car) would follow the instructions of keyboard connected to the host. If the Pacman eats a Pacdot (car hits the green LED), the LDR would give feedback to the host and shut down the corresponding LED. Meanwhile, a sound would give the player a feedback. The player would win the game when all Pacdots are eaten (a winning music would be played). There is only one ghost(red LED), which would move randomly, if the Pacman hits the ghost, the player will lose (a losing music would be played).
5. Have fun!
### Hardware

### Software

dependency:
~~sudo apt-get install alsa-utils~~
~~sudo apt-get install libasound2-dev~~
