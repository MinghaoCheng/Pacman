/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, April 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

#include "Drivers/brd_config.h"

#define GAME_PANEL_MATRIX_ROW                   LED_MATRIX_ROW
#define GAME_PANEL_MATRIX_COLUMN                LED_MATRIX_COLUMN
#define GHOST_UPDATE_PERIOD_NS                  3000000000

#define GAME_START_SOUND_FILE                   "./host_sound_files/pacman_beginning.wav"
#define GAME_DOT_EATEN_FILE                     "./host_sound_files/pacman_eatfruit.wav"
#define GAME_LOSING_FILE                        "./host_sound_files/pacman_death.wav"
#define GAME_WINNING_FILE                       "./host_sound_files/pacman_intermission.wav"

#define KEY_UP                                  0x01
#define KEY_DOWN                                0x02
#define KEY_LEFT                                0x03
#define KEY_RIGHT                               0x04
#define KEY_RESET                               0x05

#endif
