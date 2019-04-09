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
* [Build&Install](#Build&Install)
* [Hardware](#Hardware)
* [Software](#software)



### Build&Install


### Hardware

### Software

host - control the game logic, led matrix, LDR matrix, car (through Wi-Fi)

dependency:
sudo apt-get install alsa-utils
sudo apt-get install libasound2-dev
car  - work as Pacman
