/*
 * M324_GPGGA02.c
 * MCU : 324/644
 * Created: 01/07/2016 09:25:26
 * Author : Rafou1
  $GPGGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M, , *42
			1  ,   2    ,2,    3    ,3,4,5, 6  ,7,8  ,9,  10     
  01 - 123519 = Acquisition du FIX à 12:35:19 UTC
  02 - 4807.038,N = Latitude 48°07.038' N
  03 - 01131.324,E = Longitude 11°31.324' E
  04 - 1 = Fix qualification : (0 = non valide, 1 = Fix GPS, 2 = Fix DGPS)
  05 - 08 = Nombre de satellites en poursuite.
  06 - 0.9 = DOP (Horizontal dilution of position) Dilution horizontale.
  07 - 545.4,M = Altitude, en Mètres, au dessus du MSL (mean see level) niveau moyen des Océans.
  08 - 46.9,M = Correction de la hauteur de la géoïde en Mètres par rapport à l'éllipsoïde WGS84 (MSL).
  09 - (Champ vide) = nombre de secondes écoulées depuis la dernière mise à jour DGPS.
  10 - (Champ vide) = Identification de la station DGPS.
  11 - *42 = Checksum
  12 - Non représentés CR et LF.

 */ 


#include <math.h>
#include <avr/io.h>
#include <util/delay.h> // uses F_CPU to achieve us and ms delays
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "HD44780.h"	//Header file for the LCD Module driver.
#include "define.h"


#define BAUD 9600//38400
#define MYUBRR F_CPU/BAUD/16 -1

void USART_Init(unsigned int ubrr);

unsigned char usart_receive(void);

void get_GPGGA(void);

uint8_t GPGGA_lock, gps_temp, count_j, count_k, latitude_n_s, longitude_w_e;

uint8_t gps_time[10], latitude[14], longitude[14], sat_numb[5], DOP[5], alt[8];

int main (void) {

	DDRD |= (1 << PD4);
	cli();
	lcd_init(LCD_DISP_ON_CURSOR);
	USART_Init(MYUBRR);
	sei();
	lcd_init(LCD_DISP_ON);			//LCD Initialisation,  LCD turned on with cursor off.
	lcd_clrscr();					//LCD clear display routine.
	lcd_gotoxy(0,0);
	lcd_puts("HORLOGE - GPS");
	lcd_gotoxy(0,1);
	lcd_puts("GPGGA-V5.00");
	_delay_ms(2000);
	lcd_clrscr();
	
	while(1)
	{
		for(count_k=0;count_k<=3;count_k++) //for(count_k=0;count_k<=40;count_k++)
		{
			get_GPGGA();
			// #1
			// TIME /*
			lcd_gotoxy(0,0);
			for(count_j=0;count_j<=1;count_j++){
				lcd_putc(gps_time[count_j]);
			}
			lcd_putc(':');
			for(count_j=2;count_j<=3;count_j++){
				lcd_putc(gps_time[count_j]);
			}
			lcd_putc(':');
			for(count_j=4;count_j<=9;count_j++){
				lcd_putc(gps_time[count_j]);
			}
			lcd_puts(" GMT");
			// FIN de TIME 
			
			// Début SATELITES
			lcd_gotoxy(0,1);
			lcd_puts("SATELITES: ");
			for(count_j=0;sat_numb[count_j]!=',';count_j++){
				lcd_putc(sat_numb[count_j]);
			}
			// FIN de SATELITES
			_delay_ms(25);
		}
		// #2
		lcd_clrscr();
		lcd_puts("LAT : ");
		for(count_j=0;count_j<=1;count_j++){ 
			lcd_putc(latitude[count_j]);
		}
		lcd_putc(223);//symbol "°"
		for(count_j=2;count_j<=10;count_j++){ //for(count_j=2;count_j<=9;count_j++){
			lcd_putc(latitude[count_j]);
		}
		lcd_putc(latitude_n_s);
		 //Fin latitude
		 
		 //Début longitude
		lcd_gotoxy(0,1);
		lcd_puts("LONG: ");
		for(count_j=0;count_j<=1;count_j++){
			lcd_putc(longitude[count_j]);
		}
		lcd_putc(223);//symbol "°"
		for(count_j=2;count_j<=10;count_j++){//for(count_j=2;count_j<=9;count_j++){
			lcd_putc(longitude[count_j]);
		}
		lcd_putc(longitude_w_e);
		_delay_ms(6000);
		 //Fin longitude	
		// #3
		// Début DOP
		lcd_clrscr();
		lcd_puts("DOP: ");
		for(count_j=0;DOP[count_j]!=',';count_j++){
			lcd_putc(DOP[count_j]);
		}
		// fin de DOP
		//début ATLTITUDE
		lcd_gotoxy(0,1);
		lcd_puts("Alt: ");
		for(count_j=0;alt[count_j]!=',';count_j++){
			lcd_putc(alt[count_j]);
		}
		lcd_gotoxy(10,1);
		lcd_puts(" m");
		
		_delay_ms(2500);
		//FIN ATLTITUDE
		lcd_clrscr();
	}
}

void get_GPGGA(void){
	GPGGA_lock = 1;
	while(GPGGA_lock){
		gps_temp = usart_receive();
		if(gps_temp == '$'){
		gps_temp = usart_receive();
			if(gps_temp == 'G'){
			gps_temp = usart_receive();
				if(gps_temp == 'P'){
					gps_temp = usart_receive();
					if(gps_temp == 'G'){
						gps_temp = usart_receive();
						if(gps_temp == 'G'){
							gps_temp = usart_receive();
							if(gps_temp == 'A'){
								gps_temp = usart_receive();
								if(gps_temp == ','){
									// Get time
									gps_temp = usart_receive();
									gps_time[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										gps_time[count_j] = gps_temp;									
									}

									// ****************  Get latitude  ******************
									gps_temp = usart_receive();
									latitude[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){ //for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										latitude[count_j] = gps_temp;								
									}
								
									// N/S
									gps_temp = usart_receive();
									latitude_n_s = gps_temp;
									// Jump ,
									gps_temp = usart_receive();gps_temp = usart_receive();
									
									// ************  Get longitude  **************
									gps_temp = usart_receive();
									longitude[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										longitude[count_j] = gps_temp;								
									}
									// W/E
									gps_temp = usart_receive();
									longitude_w_e = gps_temp;
									// jump 1,
									gps_temp = usart_receive();gps_temp = usart_receive();gps_temp = usart_receive();
									// Sat
									gps_temp = usart_receive();
									sat_numb[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										sat_numb[count_j] = gps_temp;
									}
									
									// DOP
									gps_temp = usart_receive();
									DOP[0] = gps_temp;
									for(count_j=1;gps_temp!=',';count_j++){
										gps_temp = usart_receive();
										DOP[count_j] = gps_temp;
									}
									// Alt
									gps_temp = usart_receive();
									alt[0] = gps_temp;
									for(count_j=1;gps_temp!=',';count_j++){
										gps_temp = usart_receive();
										alt[count_j] = gps_temp;
									}
									GPGGA_lock = 0;
								}
							}
						}
					}
				}		
			}
		}
	}
}

unsigned char usart_receive( void ){
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

void USART_Init( unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
}

