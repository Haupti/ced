# user config
CC:=gcc
BUILD_DIR:=build
SRC_DIR:=src
TARGET:=./$(BUILD_DIR)/main
CFLAGS:= -Wall -Werror -xc -std=c11 -D_DEFAULT_SOURCE -lm

SOURCES:=$(shell find . -name "*.c" | sort)
HEADERS:=$(shell find . -name "*.h" | sort)

.PHONY: all $(TARGET)

all: $(TARGET)
$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) $(HEADERS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf ./$(BUILD_DIR)/*
