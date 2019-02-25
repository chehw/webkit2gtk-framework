TARGET=bin/webkit-test

DEBUG ?= 1

CC=gcc -std=gnu99
LINKER=gcc -std=gnu99
AR=ar crf

CFLAGS=-Wall -Iinclude -Iutils
LIBS=-lm -lpthread


CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
CFLAGS += $(shell pkg-config --cflags webkit2gtk-4.0)

LIBS += $(shell pkg-config --libs gtk+-3.0)
LIBS += $(shell pkg-config --libs webkit2gtk-4.0)

INCLUDE=include/common.h

ifeq ($(DEBUG),1)
	CFLAGS += -g -D_DEBUG
endif

SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:src/%.c=obj/%.o)

UTILS_SOURCES := $(wildcard utils/*.c)
UTILS_OBJECTS := $(UTILS_SOURCES:utils/%.c=utils/obj/%.o)

all: do_init $(TARGET)

$(TARGET): $(OBJECTS) $(UTILS_OBJECTS)
	$(LINKER) -o $@ $(OBJECTS) $(UTILS_OBJECTS) $(LIBS)
	
$(OBJECTS): obj/%.o : src/%.c  $(INCLUDE)
	$(CC) -o $@ -c $< $(CFLAGS)
	
$(UTILS_OBJECTS): utils/obj/%.o: utils/%.c

.PHONY: do_init clean test
clean:
	rm -f *.o obj/*.o $(TARGET) test/*.o test/obj/*.o utils/*.o utils/obj/*.o  obj/plugins/*.o
	
do_init:
	mkdir -p obj test/obj bin lib  obj/plugins 
		
test:
	cd test && ./make.sh
	
