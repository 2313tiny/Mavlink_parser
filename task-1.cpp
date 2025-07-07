#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>   // PRIu64, PRIx64



#include <mavlink_types.h>  //mavlink_message_t
#include <common/mavlink.h>  //MAVLINK_STX  (0xFD = 253) ver2.0


//#define TEST_CASE 33

mavlink_parse_state_t  r_parse_state_status=MAVLINK_PARSE_STATE_IDLE;
static int count_payload = 0; 
static int count_checksum = 0;
static int count_signature = 0;
static uint16_t crcTmp = 0; 


mavlink_message_t GOLDEN_output;


void my_mavlink_parse_char(uint8_t read_byte,
	       		  mavlink_message_t * message,
			  mavlink_framing_t * r_framing_status)
{
#if 10
 	*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
	switch(r_parse_state_status ){
		case 	MAVLINK_PARSE_STATE_IDLE:
		     if(MAVLINK_STX == read_byte){
		    	r_parse_state_status = MAVLINK_PARSE_STATE_GOT_STX;
			count_payload = 0;
			count_checksum = 0; 
			count_signature = 0;	
			message->msgid = 0; 
			crcTmp = 0; 	

			//	GOLDEN_output.ck[0]= 0;
			//GOLDEN_output.ck[1]= 0;
			//CRC16 = 0; 
			crc_init(&crcTmp);	
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		//        mavlink_start_checksum(&GOLDEN_output);		
		//	printf("..Protocol Magic Marker:  0x%X \n", read_byte); //visual debug
		     } 
		    *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case  MAVLINK_PARSE_STATE_GOT_STX:
			message->len = read_byte;	
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_LENGTH;
		  	*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		 //       mavlink_update_checksum(&GOLDEN_output, read_byte);		
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		//	printf("..Payload Length:  %d \n", read_byte);
		break;

		case MAVLINK_PARSE_STATE_GOT_LENGTH:
			message->incompat_flags = read_byte;	
			r_parse_state_status =	MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS;
		    	*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		  //      mavlink_update_checksum(&GOLDEN_output, read_byte);		
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		//	printf("..Incomp. Flags:  0x%X \n", read_byte);
		break;

		case MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS:
			message->compat_flags=read_byte;
			r_parse_state_status =	MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS;
		     	*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		   //     mavlink_update_checksum(&GOLDEN_output, read_byte);		
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		//	printf("..Comp Flags:  0x%X \n", read_byte);
		 break;
		
		case MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS:
			message->seq= read_byte;   //UNDER CONSTRUCTION ?????
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_SEQ; 
		        *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
	           //	printf("..Sequence:  0x%X \n", message->seq);
		        //mavlink_update_checksum(&GOLDEN_output, read_byte);		
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		break;
		
		case MAVLINK_PARSE_STATE_GOT_SEQ:
			message->sysid=read_byte;
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_SYSID;
		        *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		       // mavlink_update_checksum(&GOLDEN_output, read_byte);		
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		//	printf("..System ID:  %d \n", read_byte);
		break;

		case MAVLINK_PARSE_STATE_GOT_SYSID:
			message->compid = read_byte;
			r_parse_state_status = 	MAVLINK_PARSE_STATE_GOT_COMPID;
		        *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		       // mavlink_update_checksum(&GOLDEN_output, read_byte);		
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		//	printf("..Component ID:  %d \n", read_byte);
		break;

		case MAVLINK_PARSE_STATE_GOT_COMPID:
			//little endian 
		    	//message->msgid = message->msgid| (uint32_t)read_byte << 0; 
		    	message->msgid = (uint32_t)read_byte << 0; 
		       	printf(" read_byte0 =  0x%X\n", read_byte);
		    	//message->msgid = message->msgid | (uint32_t)read_byte ; 
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_MSGID1;
		 	crc_accumulate(read_byte, &crcTmp);
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		        //mavlink_update_checksum(&GOLDEN_output, read_byte);		
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID1:
			message->msgid = message->msgid | (uint32_t)read_byte << 8 ;  //16
		       	printf(" read_byte1 =  0x%X\n", read_byte);
			//message->msgid = message->msgid << 8 | (uint32_t)read_byte  ;  //16
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_MSGID2;
			//printf("..9   \n");
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		        //mavlink_update_checksum(&GOLDEN_output, read_byte);		
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID2:
		 	message->msgid =message->msgid  | ((uint32_t)read_byte << 16); //8
		       	printf(" read_byte3 =  0x%X\n", read_byte);
		 	//message->msgid =message->msgid << 8  | ((uint32_t)read_byte); //8
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_MSGID3;
		//	printf("..Message ID: %d \n", message->msgid);
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		       // mavlink_update_checksum(&GOLDEN_output, read_byte);		
	//	break;  //UNDER CONSTRUCTION

		case MAVLINK_PARSE_STATE_GOT_MSGID3:
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_PAYLOAD;
		//	printf("..11  %d \n", message->msgid);
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		        //mavlink_update_checksum(&GOLDEN_output, read_byte);		
		break;

		case MAVLINK_PARSE_STATE_GOT_PAYLOAD:
		if (count_payload < message->len){
			message->payload64[count_payload % 7] = read_byte;    
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		        mavlink_update_checksum(&GOLDEN_output, read_byte);		
			} else {
				*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
				r_parse_state_status = MAVLINK_PARSE_STATE_GOT_CRC1;	
				count_payload = 0; 
			}
			++count_payload;
		break;

		case MAVLINK_PARSE_STATE_GOT_CRC1:
			if(count_checksum < 2) {
				message->ck[count_checksum] = read_byte;				
			//	printf("..13-14 %d \n,", message->ck[count_checksum]);
				*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
				r_parse_state_status = MAVLINK_PARSE_STATE_SIGNATURE_WAIT;

			} //else {
			//	r_parse_state_status = MAVLINK_PARSE_STATE_GOT_BAD_CRC1; 
			//	if (message->incompat_flags & MAVLINK_IFLAG_SIGNED){
		 	//		r_parse_state_status = MAVLINK_PARSE_STATE_SIGNATURE_WAIT; 
			//	} else {
			//		r_parse_state_status = MAVLINK_PARSE_STATE_GOT_BAD_CRC1; 
			//	}	
			
		 	crc_accumulate(read_byte, &crcTmp);
			printf("..crcTmp:  0x%X \n", crcTmp); //visual debug
		      	printf("GOLDEN_output..%d...%d  \n", GOLDEN_output.ck[0], GOLDEN_output.ck[1]);
		break;

		case MAVLINK_PARSE_STATE_GOT_BAD_CRC1:
			*r_framing_status = MAVLINK_FRAMING_BAD_CRC;
			return ;	
		//	printf("..  CRC bad case ....  \n,");
		break;

		case MAVLINK_PARSE_STATE_SIGNATURE_WAIT:
			if (count_signature < MAVLINK_SIGNATURE_BLOCK_LEN){
				message->signature[count_signature]= read_byte;
				*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
			}

			if (count_signature == MAVLINK_SIGNATURE_BLOCK_LEN){
				*r_framing_status = MAVLINK_FRAMING_OK;
			//	printf("...End of packet \n\n");	
				r_parse_state_status = MAVLINK_PARSE_STATE_IDLE;	

			}
			++count_signature;
		break;

		default:
			r_parse_state_status=MAVLINK_PARSE_STATE_IDLE;
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;
	} 
#endif
}

#ifndef TEST_CASE

int main() {
  	FILE * file_ptr;
        int read_character; //Use int to hold the return, as EOF is an int
	uint8_t read_byte; //current value
	const char * fname = "mavlink.bin";

	mavlink_message_t frame_v2;	


	//1.Open the file in read mode ("rb")	
	file_ptr = fopen( fname,  "rb");

	//2. Check if the file was opened seccesfully
	if (NULL == file_ptr){
		perror("[FAIL]...Error opening file"); 	
		return 1; 
	}

	//3. Parsing
       	//This holds the parser's internal state	
	mavlink_framing_t r_framing_status = MAVLINK_FRAMING_INCOMPLETE; 
	printf("...  Starting Mavlink v2 parsing ...\n");
				
	// only provide the next raw byte from the stream
	while( EOF != (read_character = fgetc(file_ptr))){    
	read_byte =(uint8_t)read_character;
		
		my_mavlink_parse_char(read_byte, &frame_v2, &r_framing_status); //State Machine Processor

		if (MAVLINK_FRAMING_OK ==r_framing_status){
			printf("---Found Mavlink v2 Magic Marker: --- \n");
			printf(" Payload Length: %d\n", frame_v2.len);	
			printf(" Incomp. Flags:  0x%X\n", frame_v2.incompat_flags);
			printf(" Comp Flags:     0x%X\n", frame_v2.compat_flags);
			printf(" Sequence:       %d\n", frame_v2.seq);
			printf(" System ID:      %d\n", frame_v2.sysid);
			printf(" Component ID:   %d\n", frame_v2.compid);
			printf(" Message ID: 	 %d\n", frame_v2.msgid);
		
			printf(" Reading %d bytes: ",frame_v2.len);
			for(int i = 0; i < frame_v2.len; ++i){
				printf("0x%02X ", frame_v2.payload64[i]);	
				//printf("% " PRIx64 "  ", frame_v2.payload64[i]);	
			}
			printf("\n");	
			
			printf(" Checksum:	 %d %d\n", frame_v2.ck[0], frame_v2.ck[1]);
	
			if (frame_v2.incompat_flags & MAVLINK_IFLAG_SIGNED){
				printf(" Skipping %d bytes for Signature...\n",MAVLINK_SIGNATURE_BLOCK_LEN);	
			} else {
				printf(" No signature block expected\n");	
			}	
		
			printf("---End of packet ---\n\n");
		
		} else if (MAVLINK_FRAMING_BAD_CRC ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD CRC \n"); 	
		} else if (MAVLINK_FRAMING_INCOMPLETE ==r_framing_status) {
		//	fprintf(stderr,"[FAIL]....Mavlink v2 message INCOMPLETE \n"); 	
		} else if (MAVLINK_FRAMING_BAD_SIGNATURE ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD SIGNATURE \n "); 	
		}	
	}

	//4.Close the file
	fclose(file_ptr); 
	printf("\nEnd of file reached \n");
	
	return 0;
}
#endif
