
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <mavlink_types.h>
#include <common/mavlink.h>


#include "task-1.cpp"

extern mavlink_parse_state_t r_parse_state_status;
extern int count_payload;
extern int count_checksum;
extern int count_signature;
extern mavlink_message_t GOLDEN_output; //This is used for checksum calculation

extern void my_mavlink_parse_char(uint8_t read_byte,
	       			mavlink_message_t * message,
			       	mavlink_framing_t * r_framing_status);

//Resets the global state of the parser for a new test run
void reset_parser_state()
{
	r_parse_state_status = MAVLINK_PARSE_STATE_IDLE;
	count_payload = 0;
	count_checksum = 0;
	count_signature = 0;
	//memset(&GOLDEN_output, 0, sizeof(mavlink_message_t)); //Clear all fields
}


void send_bytes_to_parser(const uint8_t * data,
	       	 	  size_t len,
			  mavlink_message_t * msg,
	       	          mavlink_framing_t * framing_status)
{
	for (size_t i = 0; i < len; ++i){
		my_mavlink_parse_char(data[i], msg, framing_status);
	}
}


void test_garbage_before_stx()
{
	mavlink_message_t msg;
	mavlink_framing_t framing_status;
	
	uint8_t data[] = {
		0xAA, 0xBB, 0xCC,
		MAVLINK_STX,
	       	0x7, 0x11, 0x22, //len, incompat_flags, compat_flags
		0x77, //seq
		0x44, 0x55, //sysid, compid
		0x33, 0x22, 0x11,  //msgid
		0x99, 0x88, 0x77,  //payload
		0x66, 0x55, 0x44, 
		0x33,   //0x22, 0x11, 
		0x55, 0x44  //crc
	};


	//Send garbage bytes one by one
	my_mavlink_parse_char(0xAA, &msg, &framing_status);
	assert(r_parse_state_status == MAVLINK_PARSE_STATE_IDLE);
	my_mavlink_parse_char(0xBB, &msg, &framing_status);
	assert(r_parse_state_status == MAVLINK_PARSE_STATE_IDLE);
	my_mavlink_parse_char(0xCC, &msg, &framing_status);
	assert(r_parse_state_status == MAVLINK_PARSE_STATE_IDLE);
	

	//Send the rest of the valid packet 
	send_bytes_to_parser(&data[3], sizeof(data)-3, &msg, &framing_status);

	assert(msg.len == 0x7);	//ok
	assert(msg.incompat_flags == 0x11);//ok
	assert(msg.compat_flags == 0x22);  //ok
	assert(msg.seq == 0x77); //ok 
	assert(msg.sysid == 0x44);  //ok
	assert(msg.compid == 0x55); //ok
	assert(msg.msgid == 0x112233);  //ok

	//assert(msg.payload64[0] == 0x0033445566778899);	
	//assert(msg.payload64[0] == 0x0099887766554433);	
	//assert(msg.payload64[0] == 0x000033445566778899);	
	//assert(msg.payload64[0] == 0x112233445566778899);	
	//assert(msg.payload64[0] == 0x998877665544332211);	
#if 0
	assert(msg.payload64[0] == 0x99);//ok
	assert(msg.payload64[1] == 0x88);//ok
	assert(msg.payload64[2] == 0x77);//ok
	assert(msg.payload64[3] == 0x66);//ok
	assert(msg.payload64[4] == 0x55);//ok
	assert(msg.payload64[5] == 0x44);//ok
	assert(msg.payload64[6] == 0x33);//ok
	assert(msg.payload64[7] == 0x22);//ok
	assert(msg.payload64[8] == 0x11);//ok

	assert(msg.ck[0] == 0x55);//??
	assert(msg.ck[1] == 0x44);//??

#endif
	//	assert(framing_status == MAVLINK_FRAMING_OK);
}



//A simple test runner macro/function
#define RUN_TEST(test_func)\
	do{\
		printf(" Running test %s ... \n", #test_func);\
		reset_parser_state(); \
		test_func(); \
		printf(" [OK].....Test: %s \n\n", #test_func);\
	} while(0)





int main()
{
	printf("--- Running Mavlink Parser Test ---\n\n");

	RUN_TEST(test_garbage_before_stx);
	return 0;
}
