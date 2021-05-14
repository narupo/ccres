CC::=g++
CCFLAGS::=-std=c++17 -Wall -O0 -I.
PROG::=build/ccres
MKDIR::=mkdir
RMDIR::=rm -rf
OBJS::= \
	build/main.o \
	build/app.o \
	build/string.o \
	build/token.o \
	build/tokenizer.o \

all: $(PROG)

.PHONY: init
init:
	$(MKDIR) build

.PHONY: clean
clean:
	$(RMDIR) build

$(PROG): $(OBJS)
	$(CC) $(CCFLAGS) -o $(PROG) $(OBJS)

build/main.o: ccres/main.cc ccres/main.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/app.o: ccres/app.cc ccres/app.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/string.o: ccres/string.cc ccres/string.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/token.o: ccres/token.cc ccres/token.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/tokenizer.o: ccres/tokenizer.cc ccres/tokenizer.h
	$(CC) $(CCFLAGS) -c -o $@ $<
