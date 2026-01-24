CC = gcc
CFLAGS = -g -O2 -Wall -Wextra -Werror
AR = ar rcs

SRC_DIR = src
UTILS_DIR = utils

SRCS = $(wildcard $(SRC_DIR)/*.c)
BINS = $(patsubst $(SRC_DIR)/%.c, %, $(SRCS))
UTILS = $(wildcard $(UTILS_DIR)/*.c)
UTIL_OBJS = $(patsubst $(UTILS_DIR)/%.c, %.o, $(UTILS))
UTIL_LIB = utils
READ_DIR = get_next_line
READ_LIB = gnl

INCLUDE = -I$(READ_DIR)
LIBS = -L. -l$(UTIL_LIB) -L$(READ_DIR) -l$(READ_LIB)

all: $(BINS) $(UTIL_LIB)

%: $(SRC_DIR)/%.c $(UTIL_LIB) $(READ_LIB)
	@$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBS) -o $@

$(UTIL_LIB): $(UTIL_OBJS)
	@$(AR) lib$(UTIL_LIB).a $^

$(READ_LIB):
	@make -C $(READ_DIR) all clean --no-print-directory

%.o: $(UTILS_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(BINS)
	@rm -f lib$(UTIL_LIB).a
	@rm -f *.o
	@rm -f $(READ_DIR)/lib$(READ_LIB).a
