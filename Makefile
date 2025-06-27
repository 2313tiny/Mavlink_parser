

.PHONY: all clean


all: comp	run

comp:
	@clear
	g++ -g -I/home/agmer/mavlink/generated/include/mavlink/v2.0 -o smd task-1.cpp

run:
	@clear
	./smd

clean:	
	rm smd
	@clear

valgrind:
	valgrind ./smd
