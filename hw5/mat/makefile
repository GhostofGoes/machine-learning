BIN  = mat
#CFLAGS = -DCPLUSPLUS -g  # for use with C++ if file ext is .c

CC=g++
SHELL=/bin/sh

CPPFLAGS= -L /usr/lib/libp/  -pg
CPPFLAGS=-O3 -Wall
CPPFLAGS=-g -Wall
CFLAGS=$(CPPFLAGS)
LIBS = -lm

SRCS=\
mat.cpp\
rand.cpp\
main.cpp

HDRS=\
mat.h\
rand.h\
main.cpp

OBJS=\
mat.o\
rand.o\
main.o

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(BIN)

size:	$(HDRS)  $(SRCS) 
	wc -l $?

srcs:	$(HDRS)  $(SRCS) 
	echo $(HDRS)  $(SRCS) 

all:
	touch $(HDRS)  $(SRCS) 

clean:
#	make tar
#	mv *.tar Tars
	/bin/rm -f *.o $(BIN)*.tar *~ core gmon.out a.out

tar: makefile $(SRCS) $(HDRS)
	tar -cvf `timename $(BIN)-`.tar makefile $(SRCS) $(HDRS) $(DOCS)
	ls -l $(BIN)*tar

commit: 
	hg commit makefile $(SRCS) $(HDRS) $(DOCS)

add:
	hg add makefile $(SRCS) $(HDRS) $(DOCS)

status:
	hg status makefile $(SRCS) $(HDRS) $(DOCS)
