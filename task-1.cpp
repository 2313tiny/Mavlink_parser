#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mavlink_types.h>   //mavlink_message_t
#include <common/mavlink.h>  //MAVLINK_STX 253 ver2.0


uint8_t my_mavlink_parse_char(uint8_t c,
	       		  	  mavlink_message_t * message,
				  mavlink_status_t * r_mavlink_status)
{
	return MAVLINK_FRAMING_OK;

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
	mavlink_status_t r_mavlink_status;   //This holds the parser's internal state	
	while( EOF != (read_byte = fgetc(file_ptr))){
		uint8_t pars_result = my_mavlink_parse_char(read_byte, &frame_v2, &r_mavlink_status);
	
		if (MAVLINK_FRAMING_OK == pars_result){
			printf("[ok]...\n");			
		} else if (MAVLINK_FRAMING_BAD_CRC == pars_result) {
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD CRC "); 	
		} else if (MAVLINK_FRAMING_INCOMPLETE == pars_result){
			fprintf(stderr,"[FAIL]....Mavlink v2 message INCOMPLETE "); 	
		} else if (MAVLINK_FRAMING_BAD_SIGNATURE == pars_result){
			fprintf(stderr,"[FAIL]....Mavlink v2 message BAD SIGNATURE "); 	
		}	
	}	

	//4.Close the file
	fclose(file_ptr); 
	printf("\nEnd of file reached \n");
	
	return 0;
}
