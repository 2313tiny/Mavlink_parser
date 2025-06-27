#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mavlink_types.h>   //mavlink_message_t
#include <common/mavlink.h>  //MAVLINK_STX 253 ver2.0



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

	//3. Read charachters one by one until EOF is reached
	// EOF = -1 , 
	//while (EOF != (character = fgetc(file_ptr))){
	if  (EOF != (read_byte = fgetc(file_ptr)) ) {
 	printf("---Mavlink v2 protocol magic marker"  ); //print the character

		// STX2 frame detect
			
		if ( MAVLINK_STX == read_byte ){ //protocol magic marker
			frame_v2.magic = read_byte;		
		} 
		
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.len = read_byte;
		}
		
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.incompat_flags = read_byte;	
		}
	
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.compat_flags = read_byte;	
		}

		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.seq = read_byte;	
		}
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.sysid = read_byte;	
		}

		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.compid = read_byte;	
		}

		//Reconstruct the 24-bit msgid	
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.msgid = read_byte;	
		}
		
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.msgid = read_byte;	
		}
		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.msgid = read_byte;	
		}
		

		if (EOF != (read_byte = fgetc(file_ptr))){
			frame_v2.payload64[0] = read_byte;	
		}

	



	printf(" length  = %u \n", frame_v2.len);
	printf(" incompat_flags  = 0x%x \n", frame_v2.incompat_flags);
	printf(" compat_flags  = 0x%x \n", frame_v2.compat_flags);
	printf(" seq  = %u \n", frame_v2.seq);
	printf(" sysid  = %u \n", frame_v2.sysid);

	printf("---End of frame ---\n\n ", character); //print the character
	} else {
		//read_byte				
	}




	//4.Close the file
	fclose(file_ptr); 
	printf("\nEnd of file reached \n");
	
	
	
	return 0;
}
