/*
 * gps_uart_async.c
 *
 * Created: 1/5/2020 11:51:05 PM
 *  Author: Mohannad Rawashdeh
 */ 
#include "hal_io.h"
#include <hpl_usart_async.h>
#include <utils_ringbuffer.h>
#include <atmel_start.h>
#include "gps_uart_async.h"
#include <string.h>
#include <stdio.h>

/*
  uart async  RX callback 
*/
 void GPS_RXD_cb(const struct usart_async_descriptor *const io_descr)
{	
	usart_async_get_status(&GPS_UART,&async_uart);
	//usart_async_get_status(&io_descr,&async_uart);	
	//ringbuffer_get(&GPS_UART.rx,rxdata);
	//printf("%c",rxdata[0]);
}

/*    
  initialize the Library  and enable the driver and the callback 
  function for uart async
*/

void gps_uart_async_init()
{
	GPGGA.message_type="$GPGGA";
	GPRMC.message_type="$GPRMC";
	GPGLL.message_type="$GPGLL";
	GPGGA.ID=1;
	GPRMC.ID=2;
	GPGLL.ID=3;
	GPGGA.Gcounter=0;
	GPRMC.Gcounter=0;
	GPGLL.Gcounter=0;
	byte_rx_buf=0;
	pstptr=0;             /*must be initialized as 0 in the cpp file*/
	pstmode=0;            /*must be initialized as 0 in the cpp file*/
	message_detected=0;   /*must be initialized as 0 in the cpp file*/
	info_msg_ID=0;        /*must be initialized as 0 in the cpp file*/
	rtn=0;                 /*must be initialized as 0 in the cpp file*/
	GPGGA.Enable=0;
	GPGLL.Enable=0;
	GPRMC.Enable=1;
	reset_gps_msg_info(GPGGA.ID);
	reset_gps_msg_info(GPGLL.ID);
	reset_gps_msg_info(GPRMC.ID);
}

uint8_t get_gps_msg_type(uint8_t *uart_msg,char *target_msg)
{
		/*any target message must be 6 bytes long including $ sign*/
		const int8_t msg_length=6;
		for (uint8_t tcount=3;tcount<=msg_length;tcount++)
		{
			if (uart_msg[tcount]==target_msg[tcount])
			{
				// the message are equal -> continue
			}
			else{ return 0;}//exit the function and return error
		}
		/*if the code reach this point the comparison was a success*/
		return 1;
}

uint8_t get_gps_sentence_type(uint8_t uart_rx_byte)
{
	if (uart_rx_byte == '$')//dollarchar)
	{
		pstmode=0x01;
		pstptr=0;
	}
	if (pstmode == 0x01)
	{
		Position_system_type[pstptr]=uart_rx_byte;
		pstptr++;
		if (pstptr >= 6)
		{
			pstmode=0;
			if(get_gps_msg_type(Position_system_type,GPGGA.message_type) && GPGGA.Enable>0)
			{
				message_detected=GPGGA.ID;
				//reset_gps_msg_info(GPGGA.ID);
				//printf("\n<GPGGA.ID>\n");
			}
			if(get_gps_msg_type(Position_system_type,GPRMC.message_type) && GPRMC.Enable>0)
			{
				message_detected=GPRMC.ID;
				//reset_gps_msg_info(GPRMC.ID);
				//printf("\n<GPRMC.ID>\n");
			}
			if(get_gps_msg_type(Position_system_type,GPGLL.message_type) && GPGLL.Enable>0)
			{
				message_detected=GPGLL.ID;
				//reset_gps_msg_info(GPGLL.ID);
				//printf("\n<GPGLL.ID>\n");
			}
		}
	 }
	return 1;
}

int8_t get_gps_msg_info(uint8_t gps_msg_ID,uint8_t msg_to_read,uint8_t uart_rx_byte)
{
	if (uart_rx_byte == ',')
	{
		GPGGA.Gcounter=0;
		GPRMC.Gcounter=0;
		GPGLL.Gcounter=0;
		//printf("r-2 ");
		return -2;
	}
	else if (uart_rx_byte == '\n')// entire new message to read
	{
		GPGGA.Gcounter=0;
		GPRMC.Gcounter=0;
		GPGLL.Gcounter=0;
		//printf("r-3");
		return -3;
	}
	else if (uart_rx_byte == '*')//chksumsign)// checksum
	{
		GPGGA.Gcounter=0;
		GPRMC.Gcounter=0;
		GPGLL.Gcounter=0;
		info_msg_ID++;
		return 1;
	}
	if (gps_msg_ID == GPGGA.ID)
	{
		if (msg_to_read == 1)//Time message to
		{
			GPGGA.Time[GPGGA.Gcounter]=uart_rx_byte;
			//printf("[%d->%c]\n",GPGGA.Gcounter,GPGGA.Time[GPGGA.Gcounter]);
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 2)//latitude info
		{
			GPGGA.Latitude[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 3)//latitude deg
		{
			GPGGA.Latitude_deg=uart_rx_byte;
			//GPGGA.Gcounter++;
		}
		else if (msg_to_read == 4)//Longitude
		{
			GPGGA.Longitude[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 5)//Longitude deg
		{
			GPGGA.Longitude_deg=uart_rx_byte;
			//GPGGA.Gcounter++;
		}
		else if (msg_to_read == 6)//fix
		{
			GPGGA.fix=uart_rx_byte;
			//GPGGA.Gcounter++;
		}
		else if (msg_to_read == 7)//number_of_satallite
		{
			GPGGA.number_of_satallite[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 8)//HDOP
		{
			GPGGA.HDOP[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 9)//Altitude
		{
			GPGGA.Altitude[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 10)//Altitude_unit
		{
			GPGGA.Altitude_unit=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 11)//HOG
		{
			GPGGA.HOG[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 12)//HOG_unit
		{
			GPGGA.HOG_unit=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		else if (msg_to_read == 15)//Checksum
		{
			GPGGA.Checksum[GPGGA.Gcounter]=uart_rx_byte;
			GPGGA.Gcounter++;
		}
		return 1;
	}
	
	if (gps_msg_ID == GPRMC.ID)
	{
		if (msg_to_read == 1)//fix taken message to
		{
			GPRMC.Fix_taken[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 2)//status
		{
			GPRMC.status=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 3)//Latitude
		{
			GPRMC.Latitude[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 4)//Latitude deg
		{
			GPRMC.Latitude_deg=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 5)//Longitude
		{
			GPRMC.Longitude[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 6)//Longitude deg
		{
			GPRMC.Longitude_deg=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 7)//speed o gnd
		{
			GPRMC.speed_o_ground[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 8)//track angle deg
		{
			GPRMC.track_angle_deg[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 9)//Date
		{
			GPRMC.Date[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 10)//Magnatic_Variation
		{
			GPRMC.Magnatic_Variation[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 11)//Mag_Var_pole
		{
			GPRMC.Mag_Var_pole=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		else if (msg_to_read == 13)//Checksum
		{
			GPRMC.Checksum[GPRMC.Gcounter]=uart_rx_byte;
			GPRMC.Gcounter++;
		}
		return 1;
	}
	if (gps_msg_ID == GPGLL.ID)
	{
		if (msg_to_read == 1)//Latitude
		{
			GPGLL.Latitude[GPGLL.Gcounter]=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		if (msg_to_read == 2)//Latitude
		{
			GPGLL.Latitude_deg=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		if (msg_to_read == 3)//Longitude
		{
			GPGLL.Longitude[GPGLL.Gcounter]=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		if (msg_to_read == 4)//Longitude_deg
		{
			GPGLL.Longitude_deg=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		else if (msg_to_read == 5)//Fix_taken
		{
			
			GPGLL.Fix_taken[GPGLL.Gcounter]=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		else if (msg_to_read == 6)//Data_active
		{
			
			GPGLL.Data_active=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		else if (msg_to_read == 8 || msg_to_read == 7)//Checksum need to check
		{
			
			GPGLL.Checksum[GPGLL.Gcounter]=uart_rx_byte;
			GPGLL.Gcounter++;
		}
		return 1;
	}
	return -1;
}

void reset_gps_msg_info(int8_t message_ID)
{
	if (message_ID == GPGGA.ID)
	{
		memset(GPGGA.Time,'0',strlen(GPGGA.Time));
		memset(GPGGA.Latitude,'0',strlen(GPGGA.Latitude));
		memset(GPGGA.Longitude,'0',strlen(GPGGA.Longitude));
		GPGGA.fix='0';
		GPGGA.Latitude_deg =' ';
		GPGGA.Longitude_deg=' ';
	}
	else if (message_ID == GPGLL.ID)
	{
		memset(GPGLL.Latitude,'0',strlen(GPGLL.Latitude));
		memset(GPGLL.Longitude,'0',strlen(GPGLL.Longitude));
		memset(GPGLL.Fix_taken,'0',strlen(GPGLL.Fix_taken));
		GPGLL.Data_active='V';
		GPGLL.Latitude_deg =' ';
		GPGLL.Longitude_deg=' ';
	}
	else if (message_ID == GPRMC.ID)
	{
		memset(GPRMC.Latitude,'0',strlen(GPRMC.Latitude));
		memset(GPRMC.Longitude,'0',strlen(GPRMC.Longitude));
		memset(GPRMC.Fix_taken,'0',strlen(GPRMC.Fix_taken));
		memset(GPRMC.Date,'0',strlen(GPRMC.Date));
		GPRMC.status='V';
		GPRMC.Latitude_deg =' ';
		GPRMC.Longitude_deg=' ';
	}
}

uint8_t get_gps_ringbuffer_byte()
{
	if (usart_async_is_rx_not_empty(&GPS_UART)>0)
	{
		ringbuffer_get(&GPS_UART.rx,rxdata);
		return 1;
	}
	return 0;
}
/*these function to get the information from the sentence we want
  add the same function if you want to read another sentence
*/
static void get_GAA_info()
{
			rtn=get_gps_msg_info(GPGGA.ID,info_msg_ID,rxdata[0]);
			 if (rtn < 0)
			 {
				 if (rtn == -3)// new message
				 {
					 message_detected = 0;
					 info_msg_ID=0;
					 pstmode=0x01;
					 pstptr=0;
					 GPGGA.Ready = 0x01;
					 /*
					 printf("\nGGA time:%s\n",GPGGA.Time);
					 printf("GGA Lat:%s\n" ,GPGGA.Latitude);
					 printf("GGA Lat deg:%c\n" ,GPGGA.Latitude_deg);
					 printf("GGA Lon:%s\n" ,GPGGA.Longitude);
					 printf("GGA Lon deg:%c\n" ,GPGGA.Longitude_deg);
					 printf("GGA fix:%c\n" ,GPGGA.fix);					 
					 printf("GGA # of sat:%s\n" ,GPGGA.number_of_satallite);
					 */
					 /*
					 printf("GGA HDOP:%s\n" ,GPGGA.HDOP);
					 printf("GGA Altitude:%s\n" ,GPGGA.Altitude);
					 printf("GGA Altitude unit:%c\n" ,GPGGA.Altitude_unit);
					 printf("GGA HOG:%s\n" ,GPGGA.HOG);
					 printf("GGA HOG_unit:%c\n" ,GPGGA.HOG_unit);
					 printf("GGA checksum:%s\n" ,GPGGA.Checksum);
					 */					 
				 }
				 else if (rtn == -2 ){info_msg_ID++;}//comma sign or checksum sign
				 else if (rtn == -1){ }//error
			 }
			 /*end of 0x01*/
}
static void get_RMC_info()
{
	rtn=get_gps_msg_info(GPRMC.ID,info_msg_ID,rxdata[0]);
	 if (rtn < 0)
	 {
		 if (rtn == -3)// new message
			 {
				message_detected = 0;
				info_msg_ID=0;
			    pstmode=0x01;
				pstptr=0;
				GPRMC.Ready=0x01;
				/*
				printf("\nRMC Fix_taken:%s\n",GPRMC.Fix_taken);
				printf("RMC status:%c\n" ,GPRMC.status);
				printf("RMC Lat:%s\n" ,GPRMC.Latitude);
				printf("RMC Lat deg:%c\n" ,GPRMC.Latitude_deg);
				printf("RMC Lon:%s\n" ,GPRMC.Longitude);
				printf("RMC Lon deg:%c\n" ,GPRMC.Longitude_deg);
					 // printf("RMC speed_o_ground:%s\n" ,GPRMC.speed_o_ground);
					 // printf("RMC track_angle_deg:%s\n" ,GPRMC.track_angle_deg);
				printf("RMC Date:%s\n" ,GPRMC.Date);
					 // printf("RMC Magnatic_Variation:%s\n" ,GPRMC.Magnatic_Variation);
					 // printf("RMC Mag_Var_pole:%c\n" ,GPRMC.Mag_Var_pole);
					 // printf("RMC Checksum:%s\n" ,GPRMC.Checksum);
					 */
				 }
				 else if (rtn == -2){info_msg_ID++;}//comma sign
				 else if (rtn == -1){ }//error
			 }
			 // end of 0x02
}
static void get_GLL_info()
{
rtn=get_gps_msg_info(GPGLL.ID,info_msg_ID,rxdata[0]);
if (rtn < 0)
{
	if (rtn == -3)// new message
	{
		message_detected = 0;
		info_msg_ID=0;
		pstmode=0x01;
		pstptr=0;
		GPGLL.Ready=0x01;
	}
	else if (rtn == -2){info_msg_ID++;}//comma sign
	else if (rtn == -1){ }//error
    }
}

/* if there is any data coming from the gps uart port , analyze it 
   here and get the result if Ready = 1 is available
*/
uint8_t analyze_gps_data()
{
	switch (message_detected)
		 {
		 case 0x00:
				get_gps_sentence_type(rxdata[0]);
				 //rtn=0;
		 break;		 
		 case 0x01:
			 /*start 0x01*/		 
		    if (GPGGA.Enable == 1)
		    {
			  get_GAA_info();
	        }		 
			 /*end of 0x01*/
		  break;
		  case 0x02:
		  /*start 0x02*/
		  if (GPRMC.Enable == 1)
		  {
			  get_RMC_info();
		  }
		  /*end of 0x02*/
		  break;
		  case 0x03:
		  /*start 0x03*/
		  if (GPGLL.Enable == 1)
		  {
			  get_GLL_info();
		  }
		  /*end of 0x03*/
		  break;
		  default:
		  message_detected = 0;
		  break;
         }
		 return 0;
}

/*
  library version 4 byte
 */

uint32_t gps_get_version()
{
	//return 12018?;
	return 1;
	/*  day+month*10+year*100+version*(10000)
	 first release -> 12,018? ( 8 Jan 2020 version 1.0)
	*/
	
}