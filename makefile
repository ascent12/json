CC = cc -std=c11
CFLAGS = -g -Wall -Wextra -pedantic

AR = ar

SRC = array.c object.c value.c json.c
OBJ = $(SRC:.c=.o)
LIB = json.a

TEST1_SRC = test/test.c
TEST1_EXE = test

TEST2_SRC = test/test2.c
TEST2_EXE = test2

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	$(CC) $(TEST1_SRC) $(LIB) -o $(TEST1_EXE) $(CFLAGS)
	$(CC) $(TEST2_SRC) $(LIB) -o $(TEST2_EXE) $(CFLAGS)

clean:
	@rm -f $(OBJ) $(LIB) $(TEST1_EXE) $(TEST2_EXE)
