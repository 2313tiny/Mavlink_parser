

.PHONY: all clean


all: comp	run

comp:
	@clear
	g++ -g -I../generated/include/mavlink/v2.0 -o smd task-1.cpp
run:
	@clear
	./smd

clean:	
	rm smd
	@clear

valgrind:
	valgrind ./smd
	

test:
	@clear
	g++ -g -I../generated/include/mavlink/v2.0 -o smd parser_test.cpp
	./smd
