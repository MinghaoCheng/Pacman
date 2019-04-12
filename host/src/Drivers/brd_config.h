/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _BRD_CONFIG_H_
#define _BRD_CONFIG_H_

/*
 *  Pacman device param specification
 */
#define LED_MATRIX_ROW                      4
#define LED_MATRIX_COLUMN                   4
#define LED_GPIO_SPI_CHANNEL                1
#define LED_ROW_REFRESH_PERIOD_US           250 // matrix.fps = 1/row_refresh/row

#define LDR_GPIO_SPI_CHANNEL                0
#define LDR_GPIO_WIDTH                      8

#define TCP_SOCKET                          3210

#endif
