/*
 * GPS_uart_async.h
 *
 * Created: 1/5/2020 10:09:07 PM
 *  Author: Mohannad Rawashdeh
 */ 
/*
   GPS Modules Library based on UART async mode
   
*/
/* 
for Version 0.1.0  5th Jan 2020

 -> Support for GPGGA/GNGGA senetence
 -> Support for GPRMC/GNRMC senetence
 -> Support for GPGLL/GNGLL senetence
 -> Async mode with ringbuffer capability
*/
#include <atmel_start.h>
#ifndef GPS_UART_ASYNC_H_
#define GPS_UART_ASYNC_H_

#define    dollarchar  '$'
#define    commasign   ','
#define    newLinesign '\n'
#define    CRsign      '\r'
#define    chksumsign   '*'

#define GPS_UART_BUFFER_SIZE_byte  32
uint8_t rxdata[GPS_UART_BUFFER_SIZE_byte];
volatile uint16_t byte_rx_buf;
uint8_t Position_system_type[7];
uint8_t pstptr;             /*must be initialized as 0 in the cpp file*/
uint8_t pstmode;            /*must be initialized as 0 in the cpp file*/
uint8_t message_detected;   /*must be initialized as 0 in the cpp file*/
uint8_t info_msg_ID;        /*must be initialized as 0 in the cpp file*/
int8_t rtn;                 /*must be initialized as 0 in the cpp file*/
struct usart_async_status async_uart;/*only if you are using async uart driver*/
struct io_descriptor *gps_uart_io;
struct ringbuffer rx_buf;
struct GPGGA_data
{
	uint8_t ID;
	uint16_t Gcounter;
	char* message_type;
	char Time[12];
	char Latitude[16];
	char Latitude_deg;
	char Longitude[16];
	char Longitude_deg;
	char fix;
	char number_of_satallite[3];
	char HDOP[4];
	char Altitude[8];
	char Altitude_unit;
	char HOG[5];
	char HOG_unit;
	char Checksum[5];
	int8_t Ready;/*if the data is ready to read  0 or 1*/
	int8_t Enable; /*if the user need this message , this variable 
	must be 1 , otherwise 0*/
}GPGGA;
struct GPRMC_data
{
	uint8_t ID;
	uint16_t Gcounter;
	char* message_type;
	char Fix_taken[16];
	char status;// A or V
	char Latitude[16];
	char Latitude_deg;
	char Longitude[16];
	char Longitude_deg;
	char speed_o_ground[8]; //knots
	char track_angle_deg[8];//
	char Date[8];
	char Magnatic_Variation[8];
	char Mag_Var_pole;
	char Checksum[4];
	int8_t Ready;/*if the data is ready to read 0 or 1*/
	int8_t Enable; /*if the user need this message , this variable 
	must be 1 , otherwise 0*/
}GPRMC;
struct GPGLL_data
{
	uint8_t ID;
	uint16_t Gcounter;
	char* message_type;
	char Latitude[16];
	char Latitude_deg;
	char Longitude[16];
	char Longitude_deg;
	char Fix_taken[16];
	char Data_active;
	char Checksum[4];
	int8_t Ready;/*if the data is ready to read  0 or 1*/
	int8_t Enable; /*if the user need this message , this variable 
	must be 1 , otherwise 0*/
}GPGLL;

/**************************functions*****************************/

/*
  uart async  RX callback 
*/
void GPS_RXD_cb(const struct usart_async_descriptor *const io_descr);

/*    
  initialize the Library  and enable the driver and the callback 
  function for uart async
*/

void gps_uart_async_init();
/*
get the results as 1 if the target message and the message came from UART is equal
otherwise return 0
*/
uint8_t get_gps_msg_type(uint8_t *uart_msg,char *target_msg);

/*
 check gps sentence type , and return the type of the sentence detected
 you need to call this function and call the byte to read with it
 
 return the sentence type , otherwise return 0 
*/

uint8_t get_gps_sentence_type(uint8_t uart_rx_byte);

/*
 reset the message parameters depend on the sentence type
 recommended to erase all the  information  before starting a new reading for the 
 first time
*/
void reset_gps_msg_info(int8_t message_ID);

/*
 get the gps message info based on the sentence detected earlier.
 call the message ID ,and the type of message to time/date/long/lat..etc
 and the byte received by the UART async function.
 
 return:
 -1 : if fail for unknown reason
 -2 : if the rx_byte is comma 
 -3 : if the rx_byte is new line \n
  1 : if rx_byte is OK 
  
*/
int8_t get_gps_msg_info(uint8_t gps_msg_ID,uint8_t msg_to_read,uint8_t uart_rx_byte);

/*
  read the uart async and update the ringbuffer by reading 1 byte 
  return 0 if no data in the uart async buffer array
  return 1 if data in the ringbuffer available 
*/

uint8_t get_gps_ringbuffer_byte();
/* process & analyze the gps data
   if there is any data coming from the gps uart port , analyze it 
   here and get the result if Ready = 1 is available
*/
uint8_t analyze_gps_data();

/*
  library version 4 byte
  day+month*10+year*100+version*(10000)
  
 */

uint32_t gps_get_version();

#endif /* GPS_UART_ASYNC_H_ */