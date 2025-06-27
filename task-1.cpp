#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mavlink_types.h>   //mavlink_message_t
#include <common/mavlink.h>  //MAVLINK_STX 253 ver2.0



mavlink_parse_state_t  r_parse_state_status=MAVLINK_PARSE_STATE_IDLE;
static int count_payload; 

void my_mavlink_parse_char(uint8_t read_byte,
	       		  mavlink_message_t * message,
			  mavlink_framing_t * r_framing_status)
{
	//*r_framing_status = MAVLINK_FRAMING_OK;
	switch(r_parse_state_status ){
		case 	MAVLINK_PARSE_STATE_IDLE:
		     if(MAVLINK_STX == read_byte){
		    	r_parse_state_status = MAVLINK_PARSE_STATE_GOT_STX;
			printf("..1  0x%X \n", read_byte);
		     } 
		     *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case  MAVLINK_PARSE_STATE_GOT_STX:
			message->len = read_byte;	
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_LENGTH;
			printf("..2  %d \n", read_byte);
		//	*r_framing_status = MAVLINK_FRAMING_OK;
		     *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case MAVLINK_PARSE_STATE_GOT_LENGTH:
			message->incompat_flags = read_byte;	
			r_parse_state_status =	MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS;
			printf("..3  0x%X \n", read_byte);
		     *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS:
			message->compat_flags=read_byte;
			r_parse_state_status =	MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS;
		     *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
			printf("..4  0x%X \n", read_byte);
		break;
		
		case MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS:
			message->seq=read_byte;
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_SEQ; 
		        *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
			printf("..5  %d \n", read_byte);

		break;
		
		case MAVLINK_PARSE_STATE_GOT_SEQ:
			message->sysid=read_byte;
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_SYSID;
		        *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
			printf("..6  %d \n", read_byte);
		break;

		case MAVLINK_PARSE_STATE_GOT_SYSID:
			message->compid = read_byte;
			r_parse_state_status = 	MAVLINK_PARSE_STATE_GOT_COMPID;
		        *r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
			printf("..7  %d \n", read_byte);
		break;

		case MAVLINK_PARSE_STATE_GOT_COMPID:
		    	message->msgid = (uint32_t)(read_byte<<16); 
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_MSGID1;
			printf("..8  \n");
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID1:
			message->msgid = (message->msgid)| (uint32_t)(read_byte<<8);  //8
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_MSGID2;
			printf("..9   \n");
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID2:
		 	message->msgid =(uint32_t) (message->msgid << 0) | read_byte; //8
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_MSGID3;
			printf("..10 %d \n", message->msgid);
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID3:
			//message->msgid =(uint32_t)(message->msgid << 0) | read_byte;  //8
			r_parse_state_status = MAVLINK_PARSE_STATE_GOT_PAYLOAD;
			//	printf("..11  %d \n", message->msgid);
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		///////////////////// D O     N O T    T O U C H 

		case MAVLINK_PARSE_STATE_GOT_PAYLOAD:
			if (count_payload < message->len){
			printf("..12  %d, count_payload = %d\n", message->payload64, count_payload);
			} else {
				*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
				r_parse_state_status = MAVLINK_PARSE_STATE_GOT_CRC1;	
				count_payload = 0; 
			}
			++count_payload;
		break;

		case MAVLINK_PARSE_STATE_GOT_CRC1:
			
		break;

		case MAVLINK_PARSE_STATE_GOT_BAD_CRC1:
			*r_framing_status = MAVLINK_FRAMING_BAD_CRC;
		break;

		case MAVLINK_PARSE_STATE_SIGNATURE_WAIT:
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;
		break;

		default:
			r_parse_state_status=MAVLINK_PARSE_STATE_IDLE;
			*r_framing_status = MAVLINK_FRAMING_INCOMPLETE;

		break;
	} 
	
}

int main() {
  	FILE * file_ptr;
        int character; //Use int to hold the return, as EOF is an int
	uint8_t read_byte; //current value

	mavlink_message_t frame_v2;	

	//1.Open the file in read mode ("rb")	
	file_ptr = fopen("mavlink.bin", "rb");

	//2. Check if the file was opened seccesfully
	if (NULL == file_ptr){
		perror("[FAIL]...Error opening file"); 	
		return 1; 
	}

	//3. Parsing
	mavlink_framing_t r_framing_status;   //This holds the parser's internal state	
	
	while( EOF != (read_byte = fgetc(file_ptr))){
		my_mavlink_parse_char(read_byte, &frame_v2, &r_framing_status);
	}	
#if 10	
		if (MAVLINK_FRAMING_OK ==r_framing_status){
			printf("[OK]---Mavlink v2 message detect---\n");
			printf(" Payload length: %d", frame_v2.len);	

		} else if (MAVLINK_FRAMING_BAD_CRC ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD CRC \n"); 	
		} else if (MAVLINK_FRAMING_INCOMPLETE ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message INCOMPLETE \n"); 	
		} else if (MAVLINK_FRAMING_BAD_SIGNATURE ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD SIGNATURE \n "); 	
		}	
#endif

	//4.Close the file
	fclose(file_ptr); 
	printf("\nEnd of file reached \n");
	
	return 0;
}
