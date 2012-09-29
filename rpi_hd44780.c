/*
 * =====================================================================================
 *
 *       Filename:  rpi_hd44780.c
 *
 *    Description:  Raspberry Pi GPIO HD44780 Driver
 *                  Requires:
 *                  C library for Broadcom BCM 2835 as used in Raspberry Pi
 *                  available at: http://www.open.com.au/mikem/bcm2835/
 *
 *        Version:  0.2
 *        Created:  09/27/2012 22:12:11
 *       Compiler:  gcc
 *
 *         Author:  Vincent Franco
 *
 * =====================================================================================
 */
#include "rpi_hd44780.h"


int lcd_set_gpio_pins( void )
{
  int i;

  // bcm could not initalize return status
  if ( ! bcm2835_init() )
  {
    return 1;
  }

  for ( i=0; i<6; ++i )
  {
    // Set all pins as outputs
    bcm2835_gpio_fsel(HD_PINS[i], BCM2835_GPIO_FSEL_OUTP);
    usleep(1);

    // Set all pins LOW
    bcm2835_gpio_write(pins[i], 0);
  }

  return 0;
}


void lcd_send_byte( const unsigned char u8_byte, int mode )
{
  int i;

  bcm2835_gpio_write(H_RS, mode);

  for ( i=0; i<8; ++i )
  {
    int p = i%4, b = (i>>2 == 0)? i+4 : i-4;
    bcm2835_gpio_write(HD_DATAPINS[p], ((u8_byte>>b) & 0x01));

    if ( i == 3 )
    {
      lcd_epulse(mode);
    }

  }

  lcd_epulse(mode);
}


void lcd_epulse ( int narrow )
{
  bcm2835_gpio_write(E, 1);
  usleep(1);

  bcm2835_gpio_write(E, 0);
  (narrow)? usleep(P_NARROW) : usleep(P_WIDE);
}


int lcd_init( void )
{
  int i;

  // Init commands
  unsigned char u8_cmds[5] = {
    0x28, 0x08, 0x01, 0x06, 0x0C
  };

  // Set all pins low;
  lcd_set_gpio_pins();

  // Wait more than 15ms after VCC rises to 4.5V
  usleep(P_15MS);

  bcm2835_gpio_write(H_D4, 1);
  bcm2835_gpio_write(H_D5, 1);
  bcm2835_gpio_write(H_D6, 0);
  bcm2835_gpio_write(H_D7, 0);
  lcd_epulse(0);

  // Wait for more than 4.1ms
  for ( i=0; i<2; ++i )
  {
    lcd_epulse(1); // Pulse twice
  }

  // Interface to 4-bit
  bcm2835_gpio_write(H_D4, 0);
  bcm2835_gpio_write(H_D5, 1);
  bcm2835_gpio_write(H_D6, 0);
  bcm2835_gpio_write(H_D7, 0);
  lcd_epulse(0);

  for ( i=0; i<5; ++i )
  {
    lcd_send_byte(u8_cmds[p], 0);
  }
}


void lcd_clear_screen( void )
{
  lcd_send_byte(0x01, 0);
}


void lcd_set_cursor( unsigned char u8_line )
{
  lcd_send_byte(u8_line, 0);
}


void lcd_send_char( const char u8_char )
{
  lcd_send_byte(u8_char, 1);   // Send char
  bcm2835_gpio_write(H_RS, 0); // Set  RS LOW
}


int lcd_send_string( const char *str )
{
  if ( str == CNULL_TERM )
  {
    return 1; // Nothing was passed
  }

  while ( *str != '\0' )
  {
    lcd_send_byte( *str++, 1 );
  }

  bcm2835_gpio_write(H_RS, 0);

  return 0;
}

