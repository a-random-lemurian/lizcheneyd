

CC                = gcc

SRC_DIR           = core
BUILD_DIR         = build

INC_DIRS          = include argparse
INC_FLAGS         = $(addprefix -I,$(INC_DIRS))

SRC              := $(shell find $(SRC_DIR) -name '*.c')
SRC              += argparse/argparse.c
OBJ              := $(SRC:%=$(BUILD_DIR)/%.o)

LINK_LIB_NAMES    = crypto uuid ssl curl
LINK_LIB_FLAGS    = $(addprefix -l,$(LINK_LIB_NAMES))

ifndef DEBUG
DEBUG_FLAGS=-g3
endif

LDFLAGS           = -Os $(LINK_LIB_FLAGS) $(DEBUG_FLAGS)
CFLAGS            = -Os $(INC_FLAGS) $(DEBUG_FLAGS)

EXEC              = lizcheneyd

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJ) $(ARGPARSE_STATIC)
