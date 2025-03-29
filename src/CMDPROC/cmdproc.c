/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include "cmdproc.h"


/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];
static uint16_t rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static uint16_t txBufLen = 0; 

 
/* Function implementation */

/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	bool SOF_found = false;
	int begin, end, sof_count = 0, eof_count = 0;
	unsigned char sid;
	uint8_t checksum_calculated = 0;
	uint8_t checksum_received = 0;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0) {
		sendWarningErrorResponse(-1);
		return -1; 
	}
	
	/* Find index of SOF */
	for(begin=0; begin < rxBufLen; begin++) {
		if(UARTRxBuffer[begin] == SOF_SYM) {
			SOF_found = true;
			break;
		}
	}

	/* Find index of EOF */
	for(end = begin; end < rxBufLen; end++) {
		if(UARTRxBuffer[end] == EOF_SYM) {
			break;
		}
	}

	for(int i = begin; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			sof_count++;
		}
		else if (UARTRxBuffer[i] == EOF_SYM){
			eof_count++;
		}
	}

	if(sof_count>eof_count) {
		/* Find index of SOF */
		for(int new_begin = begin+1; new_begin < rxBufLen; new_begin++) {
			if(UARTRxBuffer[new_begin] == SOF_SYM) {
				if(end>new_begin) {
					resetRxBufferCommand(begin,new_begin-1);
					begin = new_begin;
				}
				break;
			}
		}
	}
	else if(sof_count<eof_count) {
		/* Find index of EOF */
		for(int new_end = end; new_end < rxBufLen; new_end++) {
			if(UARTRxBuffer[new_end] == EOF_SYM) {
				if(begin>end) {
					resetRxBufferCommand(0,end);
					end = new_end;
				}
				break;
			}
		}
	}	
	
	/* If a SOF was found look for commands */
	if(end < rxBufLen) {
		switch(UARTRxBuffer[begin+1]) { 
			case 'A':
				/* Check for complete command*/
				if(end-begin+1 != 6) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-2);
					return -1;
				}

				/* Check checksum */
				for(int k = end-3; k<end; k++) {
					if(UARTRxBuffer[k] < '0' || UARTRxBuffer[k] > '9') {
						resetRxBufferCommand(begin,end);
						sendWarningErrorResponse(-4);
						return -4;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[begin+1], 1);
				checksum_received = 100*(UARTRxBuffer[begin+2] - '0') + 10*(UARTRxBuffer[begin+3] - '0') + (UARTRxBuffer[begin+4] - '0');
				if (checksum_calculated != checksum_received) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-3);
					return -3;
				}

				resetRxBufferCommand(begin,end);
				return 0;
			
			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   

				/* Check for complete command*/
				if(end-begin+1 != 7) {
					resetRxBufferCommand(begin,end);
					return -1;
				}
		
				/* Check sensor type */
				sid = UARTRxBuffer[begin+2];
				if(sid != 't' && sid != 'h' && sid != 'c') {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-2);
					return -2;
				}
				
				/* Check checksum */
				for(int k = end-3; k<end; k++) {
					if(UARTRxBuffer[k] < '0' || UARTRxBuffer[k] > '9') {
						resetRxBufferCommand(begin,end);
						sendWarningErrorResponse(-4);
						return -4;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[begin+1], 2);
				checksum_received = 100*(UARTRxBuffer[begin+3] - '0') + 10*(UARTRxBuffer[begin+4] - '0') + (UARTRxBuffer[begin+5] - '0');
				if(checksum_calculated != checksum_received) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-3);
					return -3;
				}

			
				/* Command is (is it? ... ) valid. Produce answer and terminate */ 
				// txChar('#');
				// txChar('p'); /* p is the reply to P 							*/	
				// txChar('t'); /* t indicate that it is a temperature 			*/
				// txChar('+'); /* This is the sensor reading. You should call a 	*/
				// txChar('2'); /*   function that emulates the reading of a 		*/
				// txChar('1'); /*   sensor value 	*/
				// txChar('1'); /* Checksum is 114 decimal in this case		*/
				// txChar('1'); /*   You should call a funcion that computes 	*/
				// txChar('4'); /*   the checksum for any command 				*/  
				// txChar('!');

				resetRxBufferCommand(begin,end);
				return 0;

			case 'L':
				/* Check for complete command*/
				if(end-begin+1 != 6) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-1);
					return -1;
				}

				/* Check checksum */
				for(int k = end-3; k<end; k++) {
					if(UARTRxBuffer[k] < '0' || UARTRxBuffer[k] > '9') {
						resetRxBufferCommand(begin,end);
						sendWarningErrorResponse(-4);
						return -4;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[begin+1], 1);
				checksum_received = 100*(UARTRxBuffer[begin+2] - '0') + 10*(UARTRxBuffer[begin+3] - '0') + (UARTRxBuffer[begin+4] - '0');
				if (checksum_calculated != checksum_received) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-3);
					return -3;
				}

				return 0;

			case 'R':
				/* Check for complete command*/
				if(end-begin+1 != 6) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-1);
					return -1;
				}

				/* Check checksum */
				for(int k = end-3; k<end; k++) {
					if(UARTRxBuffer[k] < '0' || UARTRxBuffer[k] > '9') {
						resetRxBufferCommand(begin,end);
						sendWarningErrorResponse(-4);
						return -4;
					}
				}
				checksum_calculated = calcChecksum(&UARTRxBuffer[begin+1], 1);
				checksum_received = 100*(UARTRxBuffer[begin+2] - '0') + 10*(UARTRxBuffer[begin+3] - '0') + (UARTRxBuffer[begin+4] - '0');
				if (checksum_calculated != checksum_received) {
					resetRxBufferCommand(begin,end);
					sendWarningErrorResponse(-3);
					return -3;
				}

				resetRxBufferCommand(begin,end);
				return 0;

			default:
				/* If code reaches this place, the command is not recognized */
				resetRxBufferCommand(begin,end);
				sendWarningErrorResponse(-2);
				return -2;				
		}
		
		
	}

	if(!SOF_found) {
		resetRxBuffer();
	}
	
	/* Cmd string not null and SOF or EOF not found */
	sendWarningErrorResponse(-1);
	return -1;

}

/* 
 * calcChecksum
 */ 
uint8_t calcChecksum(unsigned char * buf, int nbytes) {
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
 * sendWarningErrorResponse
 */
void sendWarningErrorResponse(int error_code) {
	resetTxBuffer();
	unsigned char message[UART_TX_SIZE - 10] = {0};
	unsigned char formattedMessage[UART_TX_SIZE] = {0};
	uint8_t checksum;
	switch (error_code) {
		case -1:
			strcpy((char *)message,"empty string or incomplete commmand");
			checksum = calcChecksum(message, strlen((const char*)message));

			txChar(SOF_SYM);
		    snprintf((char *)formattedMessage, sizeof(formattedMessage), "W%s%03d", message, checksum);
			for(size_t i = 0; i < strlen((const char*)formattedMessage); i++) {
				if (formattedMessage[i] != '\0') {
					txChar(formattedMessage[i]);
				}
            }
			txChar(EOF_SYM);
			break;
		case -2:
			strcpy((char *)message,"invalid command was found");
			checksum = calcChecksum(message, strlen((const char*)message));

			txChar(SOF_SYM);
		    snprintf((char *)formattedMessage, sizeof(formattedMessage), "E%s%03d", message, checksum);
			for(size_t i = 0; i < strlen((const char*)formattedMessage); i++) {
				if (formattedMessage[i] != '\0') {
					txChar(formattedMessage[i]);
				}
            }
			txChar(EOF_SYM);
			break;
		case -3:
			strcpy((char *)message,"checksum error was detected");
			checksum = calcChecksum(message, strlen((const char*)message));

			txChar(SOF_SYM);
		    snprintf((char *)formattedMessage, sizeof(formattedMessage), "E%s%03d", message, checksum);
			for(size_t i = 0; i < strlen((const char*)formattedMessage); i++) {
				if (formattedMessage[i] != '\0') {
					txChar(formattedMessage[i]);
				}
            }
			txChar(EOF_SYM);
			break;
		case -4:
			strcpy((char *)message,"string format is wrong");
			checksum = calcChecksum(message, strlen((const char*)message));

			txChar(SOF_SYM);
			snprintf((char *)formattedMessage, sizeof(formattedMessage), "E%s%03d", message, checksum);
			for(size_t i = 0; i < strlen((const char*)formattedMessage); i++) {
				if (formattedMessage[i] != '\0') {
					txChar(formattedMessage[i]);
				}
			}
			txChar(EOF_SYM);
			break;
		
		default:
			break;
	}
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
	memset(UARTRxBuffer, 0, UART_RX_SIZE);
	rxBufLen = 0;
}

/*
 * resetRxBuffer
 */
void resetRxBufferCommand(int begin, int end)
{
	for(;begin <= end;begin++) {
		UARTRxBuffer[begin] = 0;
	}
	for(int i=0;i<UART_RX_SIZE;i++) {
		if(UARTRxBuffer[begin] != 0) {
			return;
		}
	}
	rxBufLen = 0;
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
	memset(UARTTxBuffer, 0, UART_TX_SIZE);
	txBufLen = 0;
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
}


