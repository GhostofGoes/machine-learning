BIN  = goes
CC   = g++
SRCS = $(BIN).cpp matrix.h matrix.cpp
LIBS = -lm $(BIN).cpp matrix.cpp
CCFLAGS = -g -std=gnu++0x

$(BIN): $(SRCS)
	$(CC) $(CCFLAGS) $(OBJS) $(LIBS) -o $(BIN)

all:    
	touch $(SRCS)
	make

clean:
	rm -f $(BIN) $(BIN).tar *~ $(BIN).output

tar:
	tar -cvf $(BIN).tar $(SRCS) makefile