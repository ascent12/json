CC = cc -std=c11
CFLAGS = -g -Wall -Wextra -pedantic

AR = ar

SRC = array.c object.c value.c json.c
OBJ = $(SRC:.c=.o)
LIB = json.a

TEST_SRC = test.c
TEST_EXE = test

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	$(CC) $(TEST_SRC) $(LIB) -o $(TEST_EXE) $(CFLAGS)

clean:
	@rm -f $(OBJ) $(LIB) $(TEST_EXE)
