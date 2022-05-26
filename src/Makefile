CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -pedantic -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 

CMD_SRC = assemble.c emulate.c
LIB_SRC = mask.c dis.c dis_br.c dis_dp.c dis_mul.c

BUILD_DIR = build

# Internal, you shouldn't need to change anythong below this

MAKEFLAGS += --no-builtin-rules # No Implict rules
.SECONDARY: # Don't remove object files

CFLAGS += -I include

CMD_OBJ = $(CMD_SRC:%.c=$(BUILD_DIR)/cmd/%.o)
CMD_BIN = $(CMD_SRC:%.c=$(BUILD_DIR)/bin/%)
CMD_DEPS = $(CMD_OBJ:%.o=%.d)

LIB_OBJ = $(LIB_SRC:%.c=$(BUILD_DIR)/lib/%.o)
LIB_DEPS = $(LIB_OBJ:%.o=%.d)

TIMESTAMP = $(BUILD_DIR)/.timestamp

DEPS = $(CMD_DEPS) $(LIB_DEPS)

all: $(CMD_BIN)

$(BUILD_DIR)/%.o: %.c $(TIMESTAMP)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(BUILD_DIR)/bin/%: $(BUILD_DIR)/cmd/%.o $(LIB_OBJ)
	$(CC) $(CFLAGS) -MMD -MP $^ -o $@

$(TIMESTAMP): Makefile
	mkdir -p $(BUILD_DIR)/cmd
	mkdir -p $(BUILD_DIR)/bin
	mkdir -p $(BUILD_DIR)/lib
	touch $@

lib/mask.c: maskgen.py 
	python3 maskgen.py

debug:
	@echo "CMD_SRC = $(CMD_SRC)"
	@echo "CMD_OBJ = $(CMD_OBJ)"
	@echo "CMD_BIN = $(CMD_BIN)"

clean:
	rm -rf $(BUILD_DIR)
	rm -f lib/mask.c
	rm -f include/mask.h

.PHONY: clean debug

-include $(DEPS)