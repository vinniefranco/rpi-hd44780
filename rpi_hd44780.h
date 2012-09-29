#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h> // http://www.open.com.au/mikem/bcm2835/index.html

/*
 *
 */
#define CNULL_TERM (char *) NULL

/*
 * PIN mappings for HD44780
 * http://www.open.com.au/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939
 */
#define H_RS RPI_GPIO_P1_26 // CE1
#define H_E  RPI_GPIO_P1_24 // CE0
#define H_D4 RPI_GPIO_P1_22 // #25
#define H_D5 RPI_GPIO_P1_18 // #24
#define H_D6 RPI_GPIO_P1_16 // #23
#define H_D7 RPI_GPIO_P1_12 // #18

/*
 * Cursor addresses
 */
#define LINE1_START 0x80
#define LINE2_START 0xC0
#define LINE3_START 0x94
#define LINE4_START 0xD4

/*
 * Timing
 */
#define P_NARROW 200   // Min delay 120
#define P_WIDE   5500  // Pulse width
#define P_15MS   15500 // >15ms

/*
 *
 */
int HD_PINS[6] = {
  H_RS, H_E,  H_D4,
  H_D5, H_D6, H_D7
};

/*
 *
 */
int HD_DATAPINS[4] = {
  H_D4, H_D5, H_D6, H_D7
};

/*
 *
 */
int  lcd_set_gpio_pins ( void );

/*
 *
 */
int  lcd_init ( void );

/*
 *
 */
void lcd_clear_screen ( void );

/*
 *
 */
void lcd_set_cursor ( unsigned char u8_line );

/*
 *
 */
void lcd_send_char ( const char u8_char );

/*
 *
 */
int  lcd_send_string ( const char *str );

/*
 *
 */
void lcd_send_byte ( const unsigned char u8_byte, int mode );

/*
 *
 */
void lcd_epulse ( int narrow );

