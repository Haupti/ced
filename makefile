# user config
CC:=clang
BUILD_DIR:=build
SRC_DIR:=src
TARGET:=./$(BUILD_DIR)/main
CFLAGS:= -Wall -Werror -xc -std=c11 

# os specific
ifeq ($(PLATFORM_WINDOWS),)
	SOURCES:=$(shell dir /b /s *.c | Sort)
	HEADERS:=$(shell dir /b /s *.h | Sort)
else
	SOURCES:=$(shell find . -name "*.c" | sort)
	HEADERS:=$(shell find . -name "*.h" | sort)
endif
.PHONY: all $(TARGET)
all: $(TARGET)
$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) $(HEADERS) -o $(TARGET)
run: $(TARGET)
	./$(TARGET)
clean:
	rm -rf ./$(BUILD_DIR)/*
