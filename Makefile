SRC_DIR=src
OBJ_DIR=obj
DEP_DIR=$(SRC_DIR)
OUT_DIR=out
INC_DIR=src

# SRCS are all .c files in SRC_DIR (recursive find)
SRCS=$(shell find $(SRC_DIR) -type f -name '*.c')

# OBJS are all .o files in OBJ_DIR (complements SRCS)
OBJS=$(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:.c=.o))

# DEPS are all .h files in DEP_DIR
DEPS=$(shell find $(DEP_DIR) -type f -name '*.h')

# OUT is the final compiled file executable
OUT=$(OUT_DIR)/main

CC=gcc
LDFLAGS=

$(OUT): $(OBJS) | $(OUT_DIR)
	$(CC) -Wall -o $(OUT) $(OBJS) $(LDFLAGS)
	@printf "Done compiling files\n"

# compilation
$(OBJS):  $(SRCS) $(DEPS) | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) -Wall -c $(subst $(OBJ_DIR),$(SRC_DIR),$(@:.o=.c)) -o $@ -I$(INC_DIR)

# recreate dirs
$(OBJ_DIR) $(OUT_DIR):
	mkdir -p $@

run: $(OUT)
	@printf "Running:\n\n\n\n"
	@./$(OUT)

# cleaning
clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)

