#ifndef _BRD_CONFIG_H_
#define _BRD_CONFIG_H_

/*
 *  Pacman device param specification
 */
#define LED_matrix_Row                      4
#define LED_matrix_Column                   4
#define LED_GPIO_SPI_CHANNEL                1
#define LED_row_refresh_period_us           250 // matrix.fps = 1/row_refresh/row

#define LDR_GPIO_SPI_CHANNEL                0

#endif
