

CC=gcc
SRC_DIR=core
BUILD_DIR=build
INC_DIRS=include argparse
INC_FLAGS=$(addprefix -I,$(INC_DIRS))
SRC := $(shell find $(SRC_DIR) -name '*.c')
SRC+=argparse/argparse.c
OBJ := $(SRC:%=$(BUILD_DIR)/%.o)
LDFLAGS=-Os
CFLAGS=-Os $(INC_FLAGS)
EXEC = lizcheneyd

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJ) $(ARGPARSE_STATIC)
