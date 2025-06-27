#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mavlink_types.h>   //mavlink_message_t
#include <common/mavlink.h>  //MAVLINK_STX 253 ver2.0



mavlink_parse_state_t  r_parse_state_status=MAVLINK_PARSE_STATE_IDLE;

void my_mavlink_parse_char(uint8_t read_byte,
	       		  mavlink_message_t * message,
			  mavlink_framing_t * r_framing_status)
{
	//*r_framing_status = MAVLINK_FRAMING_OK;
		
	switch(r_parse_state_status ){
		case 	MAVLINK_PARSE_STATE_IDLE:
			*r_framing_status = MAVLINK_FRAMING_OK;
		break;

		case  MAVLINK_PARSE_STATE_GOT_STX:
		break;

		case MAVLINK_PARSE_STATE_GOT_LENGTH:
		break;

		case MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS:
		break;
		
		case MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS:
		break;
		
		case MAVLINK_PARSE_STATE_GOT_SEQ:
		break;

		case MAVLINK_PARSE_STATE_GOT_SYSID:
		break;

		case MAVLINK_PARSE_STATE_GOT_COMPID:
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID1:
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID2:
		break;

		case MAVLINK_PARSE_STATE_GOT_MSGID3:
		break;

		case MAVLINK_PARSE_STATE_GOT_PAYLOAD:
		break;

		case MAVLINK_PARSE_STATE_GOT_CRC1:
		break;

		case MAVLINK_PARSE_STATE_GOT_BAD_CRC1:
		break;

		case MAVLINK_PARSE_STATE_SIGNATURE_WAIT:
		break;

		default:
			r_parse_state_status=MAVLINK_PARSE_STATE_IDLE;
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
	
		if (MAVLINK_FRAMING_OK ==r_framing_status){
			printf("[OK]---Mavlink v2 message detect---\n");
			printf(" Payload length: %d", frame_v2.len);	
		} else if (MAVLINK_FRAMING_BAD_CRC ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD CRC "); 	
		} else if (MAVLINK_FRAMING_INCOMPLETE ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message INCOMPLETE "); 	
		} else if (MAVLINK_FRAMING_BAD_SIGNATURE ==r_framing_status) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD SIGNATURE "); 	
		}	
	}	

	//4.Close the file
	fclose(file_ptr); 
	printf("\nEnd of file reached \n");
	
	return 0;
}
