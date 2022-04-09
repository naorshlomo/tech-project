CC = g++
OBJS = src/network/network.o src/utils/utils.o src/main.o src/worker.o
EXEC = avalanche
DEBUG_FLAG = -g
COMP_FLAG = -Wall -Werror -pedantic-errors -std=c++11
INCLUDE_FLAG = -I.

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

src/network/network.o: src/network/network.cpp src/network/network.h src/utils/utils.h
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE_FLAG) $(COMP_FLAG) $*.cpp -o $@

src/utils/utils.o: src/utils/utils.cpp src/utils/utils.h
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE_FLAG) $(COMP_FLAG) $*.cpp -o $@

src/worker.o: src/worker.cpp src/worker.h
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE_FLAG) $(COMP_FLAG) $*.cpp -o $@

src/main.o: src/main.cpp
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE_FLAG) $(COMP_FLAG) $*.cpp -o $@

clean :
	rm -f $(OBJS) $(EXEC)
