CC = gcc
CFLAGS = -g -Wall -Wshadow

MASTER_EXE = oss
MASTER_OBJ = oss.o
MASTER_DEPS = oss.c

SLAVE_EXE = user_proc
SLAVE_OBJ = user_proc.o
SLAVE_DEPS = user_proc.c

target: $(MASTER_EXE) $(SLAVE_EXE)

%.o: %.c
		$(CC) $(CFLAGS) -c -o $@ $^

$(MASTER_EXE): $(MASTER_OBJ)
		$(CC) $(CFLAGS) -o $(MASTER_EXE) $(MASTER_OBJ) -lm

$(SLAVE_EXE): $(SLAVE_OBJ)
		$(CC) $(CFLAGS) -o $(SLAVE_EXE) $(SLAVE_OBJ) -lm

.PHONY: clean

clean:
		-rm oss user_proc *.o