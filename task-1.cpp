// Task #1:

// 1. we have a data buffer in mavlink.bin

// 2. all information about mavlink protocol you can find here:
// https://mavlink.io/en/guide/
// this is start point.
// please note: 
// - we use Mavlink version 2.0;
// - we dont use the "SIGNATURE" part.

// 3. What we need?
// - need to parse buffer above;
// - you need to implement program on c/c++, which prints list of all messages;

// Example:
// msg length: number of messages in const char* data
// msg id: %u
// msg id: %u
// ...

// 4. Please be note: data buffer contains garbage bytes.

// 5. What we want to see:
// - task-1.c or task-1.cpp file with mavlink data buffer(see #1) and main() function which parse it and prints(printf() or cout, it doesnt matter) it.
// - if you will use git -> it would be better.
// - tests? maybe, why not.
// - build system: from scratch in command line, makefile, cmake - up to you.


//worked version
//#include <common/mavlink.h>



#include <stdio.h>
#include <stdlib.h>
#include <common/mavlink.h>
//#include </home/agmer/mavlink/generated/include/mavlink/v2.0/common/mavlink.h>
#include <mavlink_get_info.h>
#include <protocol.h>

void sf_readMessageFromFile(const char * filename){
	FILE * file = fopen(filename, "r");
	if(NULL == file){
		perror("Error opening file");
		return;	
	}

//	char buffer[253]; //Buffer to hold each line/ message
/*	while(NULL != fgets(buffer, sizeof(buffer), file)){
		//Process the message in buffer
		printf("Read message: %s \n", buffer);	
	}
*/

	//STX magic
	//char buffer[253]; //Buffer to hold each line/ message
	//uint8_t buffer[MAVLINK_STX]; //Buffer to hold each line/ message
	char buffer[MAVLINK_STX]; //Buffer to hold each line/ message
	fgets(buffer, sizeof(buffer), file);
	printf("magic: %s \n", buffer);	

	//LEN	
	fgets(buffer, sizeof(buffer), file);
	printf("len: %s \n", buffer);	


	//INCOMPAT_FLAG	
	fgets(buffer, sizeof(buffer), file);
	printf("inc_flags: %s\n", buffer);

	//COMPAT_FLAGs
	fgets(buffer, sizeof(buffer), file);
	printf("compat_flags: %s\n", buffer);


	//PACKET SEQUENCE NUMBER
	fgets(buffer, sizeof(buffer), file);
	printf("packet sequence number: %s\n", buffer);
	
	//SYSTEM ID (sender)
	fgets(buffer, sizeof(buffer), file);
	printf("System ID: %s\n", buffer);
	
	//COMPONENT ID (sender)
	fgets(buffer, sizeof(buffer), file);
	printf("Component ID: %s\n", buffer);
	

	//Message ID	
	fgets(buffer, sizeof(buffer), file);
	printf("Message ID1: %s\n", buffer);
	fgets(buffer, sizeof(buffer), file);
	printf("Message ID2: %s\n", buffer);
	fgets(buffer, sizeof(buffer), file);
	printf("Message ID3: %s\n", buffer);
	fgets(buffer, sizeof(buffer), file);
	printf("Message ID4: %s\n", buffer);

	


		



	
	fclose(file); //Close the file
}



int main() {
	printf("MAVLINK_STX = %d \n", MAVLINK_STX);	
//	printf("MAVLINK_WIRE_PROTOCOL_VERSION = %c \n ", MAVLINK_WIRE_PROTOCOL_VERSION);

	const char * filename = "mavlink.bin";
	sf_readMessageFromFile(filename);
	return 0;

    return 0;
}






