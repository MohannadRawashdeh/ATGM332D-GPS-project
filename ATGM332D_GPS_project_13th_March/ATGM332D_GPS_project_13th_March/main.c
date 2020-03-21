
#include <atmel_start.h>
/*add GPS uart library*/ 
#include "gps_uart_async.h"
#define DEBUG_SERCOM3_Status    0
/*add sting.h always to the main file */
#include <string.h>
uint8_t reset_gps_data = 0;
/*OLED display Library*/
#include "gfx_mono_main.h"
#include "gfx_mono_font_basic_6x7.h"

#define Display_Mode_Long_lat  0x01
struct OLED_DISPLAY_data 
{
	char Date[11];    /*MM/dd/yyyy*/
	char Time[9];     /*hh:mm:ss*/
	uint8_t Fixed_print_data;
	uint8_t print_nmea;/*0: no print , 1:print not available , 2:NMEA data*/
	uint8_t Fix_status;/* 0: not connected  1:connected*/
	uint8_t print_date;
	uint8_t print_time_hm;
}OLED_Data_disp;

struct Geo_Location_param
{
float gps_Longitude;
float gps_Latitude;
char  gps_Lat_str[16];	
char  gps_Lon_str[16];
}Geo_Location;
void start_OLED_display(void);
void display_OLED_results(uint8_t display_screen);
void Get_Long_Lat(char *dest_lat,char *dest_lon);
void start_OLED_display(void)
{    
		spi_m_sync_enable(&SPI_OLED);
		gfx_mono_init();
		for (uint16_t lines=0;lines<UG2832HSWEG04_LCD_HEIGHT;lines++)
		{
			gfx_mono_draw_horizontal_line(&MONOCHROME_GRAPHICS_0_desc,0,lines,UG2832HSWEG04_LCD_WIDTH-1,GFX_PIXEL_SET);
			delay_ms(10);
		}
		delay_ms(100);
		for (uint16_t lines=0;lines<UG2832HSWEG04_LCD_HEIGHT;lines++)
		{
			gfx_mono_draw_horizontal_line(&MONOCHROME_GRAPHICS_0_desc,0,lines,UG2832HSWEG04_LCD_WIDTH-1,GFX_PIXEL_CLR);
			delay_ms(10);
		}
		//draw a frame around the screen
		gfx_mono_draw_rect(&MONOCHROME_GRAPHICS_0_desc,0,0,UG2832HSWEG04_LCD_WIDTH,UG2832HSWEG04_LCD_HEIGHT,GFX_PIXEL_SET);
		OLED_Data_disp.Fixed_print_data = 1;
		OLED_Data_disp.print_nmea = 1;
		OLED_Data_disp.Fix_status = 2;
}
void display_OLED_results(uint8_t display_screen)
{
	if (display_screen == Display_Mode_Long_lat)
	{
		//OLED_Data_disp.print_date == 1
		/*check if there's any change from the previous reading*/
		if (OLED_Data_disp.Date[0]!=GPRMC.Date[2] || OLED_Data_disp.Date[1]!=GPRMC.Date[3] ||
		    OLED_Data_disp.Date[3]!=GPRMC.Date[0] || OLED_Data_disp.Date[4]!=GPRMC.Date[1] ||
	        OLED_Data_disp.Date[8]!=GPRMC.Date[4] || OLED_Data_disp.Date[9]!=GPRMC.Date[5])
		{		
		OLED_Data_disp.Date[0]=GPRMC.Date[2];
		OLED_Data_disp.Date[1]=GPRMC.Date[3];
		OLED_Data_disp.Date[2]='/';
		OLED_Data_disp.Date[3]=GPRMC.Date[0];
		OLED_Data_disp.Date[4]=GPRMC.Date[1];
		OLED_Data_disp.Date[5]='/';
		OLED_Data_disp.Date[6]='2';
		OLED_Data_disp.Date[7]='0';
		OLED_Data_disp.Date[8]=GPRMC.Date[4];
		OLED_Data_disp.Date[9]=GPRMC.Date[5];	
		OLED_Data_disp.print_date = 1;/*to update the display with new date*/	
		#if DEBUG_SERCOM3_Status == 1
		printf("mis-match date\r\n");
		#endif
		}
		/*time */
		if (OLED_Data_disp.Time[0]!=GPRMC.Fix_taken[0] || OLED_Data_disp.Time[1]!=GPRMC.Fix_taken[1] ||
		    OLED_Data_disp.Time[3]!=GPRMC.Fix_taken[2] || OLED_Data_disp.Time[4]!=GPRMC.Fix_taken[3])
		{		
		OLED_Data_disp.Time[0]=GPRMC.Fix_taken[0];
		OLED_Data_disp.Time[1]=GPRMC.Fix_taken[1];
		OLED_Data_disp.Time[2]=':';
		OLED_Data_disp.Time[3]=GPRMC.Fix_taken[2];
		OLED_Data_disp.Time[4]=GPRMC.Fix_taken[3];
		OLED_Data_disp.Time[5]=':';
		OLED_Data_disp.print_time_hm=1;
		#if DEBUG_SERCOM3_Status == 1
		printf("mis-match time\r\n");
		#endif
		}
		OLED_Data_disp.Time[6]=GPRMC.Fix_taken[4];
		OLED_Data_disp.Time[7]=GPRMC.Fix_taken[5];
		/*
		#if DEBUG_SERCOM3_Status == 1    
		printf("\r\nDate oled:");
		for(uint8_t dis=0;dis<10;dis++)
		{
			printf("%c",OLED_Data_disp.Date[dis]);
		}
		printf("\r\n");
		#endif
		*/
		//gfx_mono_text_draw_progmem_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) OLED_Data_disp.Date,2,2,&basic_6x7);		
		 if (GPRMC.status == 'A' && OLED_Data_disp.Fix_status != 1)
		 {
			 OLED_Data_disp.print_nmea=2;/*to print the nmea data*/
			 OLED_Data_disp.Fix_status=1;
		 }
		 else if (GPRMC.status == 'V' && OLED_Data_disp.Fix_status !=0)
		 {
			 OLED_Data_disp.print_nmea=1;/*to print not available msg*/
			 OLED_Data_disp.Fix_status=0;
		 }
		if (OLED_Data_disp.print_date == 1)
		{
			OLED_Data_disp.print_date=0;
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) OLED_Data_disp.Date,2,2,&basic_6x7);
		}
		uint8_t x_axis_pos=69;
		if (OLED_Data_disp.print_time_hm == 1)
		{
			OLED_Data_disp.print_time_hm=0;
			
			for (int8_t l=0;l<6;l++)
			{
				gfx_mono_text_draw_char(&MONOCHROME_TEXT_0_desc,OLED_Data_disp.Time[l],x_axis_pos,2,&basic_6x7);
				x_axis_pos+=(basic_6x7.width +1);
			}			
		}
		x_axis_pos=69+((basic_6x7.width +1)*6);
		gfx_mono_text_draw_char(&MONOCHROME_TEXT_0_desc,OLED_Data_disp.Time[6],x_axis_pos,2,&basic_6x7);
		x_axis_pos+=(basic_6x7.width +1);
		gfx_mono_text_draw_char(&MONOCHROME_TEXT_0_desc,OLED_Data_disp.Time[7],x_axis_pos,2,&basic_6x7);
		//gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) OLED_Data_disp.Time,69,2,&basic_6x7);
		if (OLED_Data_disp.Fixed_print_data == 1)
		{
			OLED_Data_disp.Fixed_print_data = 0;
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "Fix :",2,16,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "Long:",2,25,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "Lati:",2,34,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "SpOG:",2,43,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "NMEA:$GPRMC",2,52,&basic_6x7);
		}
		if (OLED_Data_disp.print_nmea == 2)
		{
			//OLED_Data_disp.print_nmea = 0;
			x_axis_pos=2 +((basic_6x7.width +1)*5);                       
			gfx_mono_text_draw_char(&MONOCHROME_TEXT_0_desc,(uint8_t )'A' ,x_axis_pos,16,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) Geo_Location.gps_Lon_str,x_axis_pos,25,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) Geo_Location.gps_Lat_str,x_axis_pos,34,&basic_6x7);			
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) GPRMC.speed_o_ground,x_axis_pos,43,&basic_6x7);
		}
		if (OLED_Data_disp.print_nmea == 1)
		{
			OLED_Data_disp.print_nmea = 0;
			x_axis_pos=2 +((basic_6x7.width +1)*5);
			gfx_mono_text_draw_char(&MONOCHROME_TEXT_0_desc, (uint8_t )'V' ,x_axis_pos,16,&basic_6x7);                        
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "Not Avai    " ,x_axis_pos,25,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "Not Avai    " ,x_axis_pos,34,&basic_6x7);
			gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,(uint8_t *) "0     " ,x_axis_pos,43,&basic_6x7);
		}
		
	}
}

void Get_Long_Lat(char *dest_lat,char *dest_lon)
{
	    //03551.9128
		int16_t lon_dd=(dest_lon[0]-48)*100 + ((dest_lon[1]-48)*10)+ (dest_lon[2]-48);
		float lon_tt_int=((dest_lon[3]-48)*10) + ((dest_lon[4]-48)*1);
		float lon_tt_f=(dest_lon[6]-48)*1000 + (dest_lon[7]-48)*100 + (dest_lon[8]-48)*10 + (dest_lon[9]-48)*1;
		lon_tt_f=lon_tt_f/10000;
		lon_tt_int+=lon_tt_f;
		lon_tt_int/=60.0000;
		Geo_Location.gps_Longitude=0;
		Geo_Location.gps_Longitude=lon_dd+lon_tt_int;
		// read the E/W char
		if (GPRMC.Longitude_deg == 'W')
		{
			Geo_Location.gps_Longitude*=(-1); // ADD negative sign
		}
		// lat 3204.0719
		int16_t lat_dd=(dest_lat[0]-48)*10 + ((dest_lat[1]-48)*1);
		float lat_tt_int=((dest_lat[2]-48)*10) + ((dest_lat[3]-48)*1);
		float lat_tt_f=(dest_lat[5]-48)*1000 + (dest_lat[6]-48)*100 + (dest_lat[7]-48)*10 + (dest_lat[8]-48)*1;
		lat_tt_f=lat_tt_f/10000;
		lat_tt_int+=lat_tt_f;
		lat_tt_int/=60.0000;
		Geo_Location.gps_Latitude=0;
		Geo_Location.gps_Latitude=lat_dd+lat_tt_int;
		
		if (GPRMC.Latitude_deg == 'S')
		{
			Geo_Location.gps_Latitude*=(-1); // ADD negative sign
		}
		/*Copy the data to char array for OLED display*/
		sprintf(Geo_Location.gps_Lat_str,"%2.5f",Geo_Location.gps_Latitude);
		sprintf(Geo_Location.gps_Lon_str,"%2.5f",Geo_Location.gps_Longitude);
		#if DEBUG_SERCOM3_Status == 1
         printf("gps_Latitude,gps_Longitude= [%2.5f,%2.4f]\r\n",Geo_Location.gps_Latitude,Geo_Location.gps_Longitude);
		 printf("gps_Lat_str,gps_Lon_str= [%s,%s]\r\n",(char *)Geo_Location.gps_Lat_str,(char *)Geo_Location.gps_Lon_str);
        #endif
}
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	start_OLED_display();
    gps_uart_async_init();
    /*usart async driver and enable callback function*/
    usart_async_register_callback(&GPS_UART, USART_ASYNC_RXC_CB, GPS_RXD_cb);
    usart_async_get_io_descriptor(&GPS_UART, &gps_uart_io);
    usart_async_enable(&GPS_UART);
    /*in default , only GPRMC message is enabled*/
    GPGGA.Enable=0;/*0 if no need for this message*/
    GPRMC.Enable=1;/*0 if no need for this message*/
    GPGLL.Enable=0;/*0 if no need for this message*/

	/* Replace with your application code */
	while (1) 
	{
		 if (get_gps_ringbuffer_byte() == 1)
		 {
			 /*the byte is available and ready to use*/
			 /*print this for debugging purpose only*/
			 //printf("%c",rxdata[0]);
			 analyze_gps_data();
		 }//end if
		 
		 /*check if the sentence is available to use*/
		   if (GPRMC.Ready ==0x01)
		  {
			  #if DEBUG_SERCOM3_Status == 1            
			  printf("\n**** RMC ****\nFix_taken:%s\n",GPRMC.Fix_taken);
			  printf("status:%c\n" ,GPRMC.status);
			  printf("Lat:%s" ,GPRMC.Latitude);
			  printf(" deg:%c\n" ,GPRMC.Latitude_deg);
			  printf("Lon:%s" ,GPRMC.Longitude);
			  printf(" deg:%c\n" ,GPRMC.Longitude_deg);
			  printf("Date:%s\n" ,GPRMC.Date);
			  printf("SOG:%s\n" ,GPRMC.speed_o_ground);
			  printf("TA:%s\n" ,GPRMC.track_angle_deg);
			  printf("Mag_Var:%s\n" ,GPRMC.Magnatic_Variation);
			  printf("MVP:%c\n" ,GPRMC.Mag_Var_pole);
			  printf("chk:%s\n" ,GPRMC.Checksum);
			  #endif
			  if (GPRMC.status == 'V' && reset_gps_data == 1)
			  {
				  reset_gps_data = 0;
				  reset_gps_msg_info(GPRMC.ID);
				  reset_gps_msg_info(GPGLL.ID);
				  reset_gps_msg_info(GPGGA.ID);
			  }
			  else if (GPRMC.status == 'A' && reset_gps_data == 0)
			  {
				  reset_gps_data = 1;
			  }
			  
			  if (GPRMC.status == 'A')
			  {
				  gpio_set_pin_level(LED2_Grn,true);delay_ms(20);
				  gpio_set_pin_level(LED2_Grn,false);
				  Get_Long_Lat(GPRMC.Latitude,GPRMC.Longitude);/*only here calculate the value of the Long/Lat*/
			  }
			  else if (GPRMC.status == 'V')
			  {
				  gpio_set_pin_level(LED3_Oge,true);delay_ms(20);
				  gpio_set_pin_level(LED3_Oge,false);
			  }			  
			   display_OLED_results(Display_Mode_Long_lat);
			   GPRMC.Ready=0;
		  }
	}
}
