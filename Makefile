CC::=g++
CCFLAGS::=-std=c++17 -Wall -O0 -I. -g
PROG::=build/ccres
MKDIR::=mkdir
RMDIR::=rm -rf
OBJS::= \
	build/app.o \
	build/string.o \
	build/ctype.o \
	build/token.o \
	build/tokenizer.o \
	build/parser.o \
	build/renderer.o \

all: $(PROG) build/tests/tests

.PHONY: init
init:
	make clean
	$(MKDIR) build
	$(MKDIR) build/tests

.PHONY: clean
clean:
	$(RMDIR) build

build/tests/tests: build/tests/tests.o $(OBJS)
	$(CC) $(CCFLAGS) -o build/tests/tests build/tests/tests.o $(OBJS)

$(PROG): build/main.o $(OBJS)
	$(CC) $(CCFLAGS) -o $(PROG) build/main.o $(OBJS)

build/tests/tests.o: tests/tests.cc tests/tests.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/main.o: ccres/main.cc ccres/main.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/app.o: ccres/app.cc ccres/app.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/string.o: ccres/string.cc ccres/string.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/ctype.o: ccres/ctype.cc ccres/ctype.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/token.o: ccres/token.cc ccres/token.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/tokenizer.o: ccres/tokenizer.cc ccres/tokenizer.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/parser.o: ccres/parser.cc ccres/parser.h
	$(CC) $(CCFLAGS) -c -o $@ $<
build/renderer.o: ccres/renderer.cc ccres/renderer.h
	$(CC) $(CCFLAGS) -c -o $@ $<
