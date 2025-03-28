/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "cmdproc.h"
#include "../SENSORS/sensors.h"

/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static unsigned char txBufLen = 0; 

 
/* Function implementation */

/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	int i;
	unsigned char sid;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
		
				/* Check sensor type */
				sid = UARTRxBuffer[i+2];
				if(sid != 't' && sid != 'h' && sid != 'c') {
					return -2;
				}
				
				/* Check checksum */
				int checksum_calculated = calcChecksum(&(UARTRxBuffer[i+1]),2);
				int checksum_received = 0;

				for (int i = 3; i <= 5; i++) {
					checksum_received += (int)(UARTRxBuffer[i]-48)*pow(10,5-i);
				}
			
				if(checksum_calculated%255 != checksum_received) {
					return -3;
				}
				
				/* Check EOF */
				if(UARTRxBuffer[i+6] != EOF_SYM) {
					return -4;
				}
			
				/* Command is (is it? ... ) valid. Produce answer and terminate */ 
				txChar('#');
				txChar('p'); /* p is the reply to P 							*/	
				txChar('t'); /* t indicate that it is a temperature 			*/
				txChar('+'); /* This is the sensor reading. You should call a 	*/
				txChar('2'); /*   function that emulates the reading of a 		*/
				txChar('1'); /*   sensor value 	*/
				txChar('1'); /* Checksum is 114 decimal in this case		*/
				txChar('1'); /*   You should call a funcion that computes 	*/
				txChar('4'); /*   the checksum for any command 				*/  
				txChar('!');
				
				/* Here you should remove the characters that are part of the 		*/
				/* command from the RX buffer. I'm just resetting it, which is not 	*/
				/* a good solution, as a new command could be in progress and		*/
				/* resetting  will generate errors									*/
				rxBufLen = 0;
				
				return 0;

			case 'A':
				if(UARTRxBuffer[rxBufLen-1] != EOF_SYM) return -1;
				if(rxBufLen != i+6) return -4;

				for(int j = i+2; j < rxBufLen; j++) {
					if((j == rxBufLen-1 && UARTRxBuffer[j] != EOF_SYM) || (j != rxBufLen-1 && (UARTRxBuffer[j] < '0' || UARTRxBuffer[j] > '9'))) {
						return -2;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[i+1], 1);
				checksum_received = 100*(UARTRxBuffer[i+2] - '0') + 10*(UARTRxBuffer[i+3] - '0') + (UARTRxBuffer[i+4] - '0');
				if (checksum_calculated != checksum_received) { // VER O %255 AQUI
					return -3;
                }
				return 0;
			case 'L':
				if(UARTRxBuffer[rxBufLen-1] != EOF_SYM) return -1;
				if(rxBufLen != i+7) return -4;

				sid = UARTRxBuffer[i+2];
				if(sid != 't' && sid != 'h' && sid != 'c') return -2;

				for(int j = i+3; j < rxBufLen; j++) {
					if((j == rxBufLen-1 && UARTRxBuffer[j] != EOF_SYM) || (j != rxBufLen-1 && (UARTRxBuffer[j] < '0' || UARTRxBuffer[j] > '9'))) {
						return -2;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[i+1], 2);
				checksum_received = 100*(UARTRxBuffer[i+3] - '0') + 10*(UARTRxBuffer[i+4] - '0') + (UARTRxBuffer[i+5] - '0');
				if (checksum_calculated != checksum_received) { // VER O %255 AQUI
					return -3;
				}
				return 0;
			case 'R':
				if(UARTRxBuffer[rxBufLen-1] != EOF_SYM) return -1;
				if(rxBufLen != i+6) return -4;

				for(int j = i+2; j < rxBufLen; j++) {
					if((j == rxBufLen-1 && UARTRxBuffer[j] != EOF_SYM) || (j != rxBufLen-1 && (UARTRxBuffer[j] < '0' || UARTRxBuffer[j] > '9'))) {
						return -2;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[i+1], 1);
				checksum_received = 100*(UARTRxBuffer[i+2] - '0') + 10*(UARTRxBuffer[i+3] - '0') + (UARTRxBuffer[i+4] - '0');
				if (checksum_calculated != checksum_received) { // VER O %255 AQUI
					return -3;
				}
				return 0;
			default:
				/* If code reaches this place, the command is not recognized */
				return -2;				
		}
		
		
	}
	
	/* Cmd string not null and SOF not found */
	return -4;

}

/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
	/* Here you are supposed to compute the modulo 256 checksum */
	/* of the first n bytes of buf. Then you should convert the */
	/* checksum to ascii (3 digitas/chars) and compare each one */
	/* of these digits/characters to the ones in the RxBuffer,	*/
	/* positions nbytes, nbytes + 1 and nbytes +2. 				*/
	
	/* That is your work to do. In this example I just assume 	*/
	/* that the checksum is always OK.							*/	

	int checksum = 0;

	for (int i = 0; i < nbytes; i++) {
		checksum += (int)buf[i];
	}

	return checksum;
}

/*
 * rxChar
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) {
		UARTRxBuffer[rxBufLen] = car;
		rxBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/*
 * txChar
 */
int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufLen < UART_TX_SIZE) {
		UARTTxBuffer[txBufLen] = car;
		txBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/*
 * resetRxBuffer
 */
void resetRxBuffer(void)
{
	rxBufLen = 0;		
	return;
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
	txBufLen = 0;		
	return;
}

/*
 * getTxBuffer
 */
void getTxBuffer(unsigned char * buf, int * len)
{
	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return;
}


