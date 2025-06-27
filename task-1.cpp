//worked version
//#include <common/mavlink.h>



#include <stdio.h>
#include <stdlib.h>
#include <common/mavlink.h>
//#include </home/agmer/mavlink/generated/include/mavlink/v2.0/common/mavlink.h>

void sf_readMessageFromFile(const char * filename){
	FILE * file = fopen(filename, "r");
	if(NULL == file){
		perror("Error opening file");
		return;	
	}

	fclose(file); //Close the file
}



int main() {
	printf("MAVLINK_STX = %d \n", MAVLINK_STX);	
	printf("MAVLINK_WIRE_PROTOCOL_VERSION = %c \n ", MAVLINK_WIRE_PROTOCOL_VERSION);

	const char * filename = "mavlink.bin";
	sf_readMessageFromFile(filename);
	return 0;

    return 0;
}
