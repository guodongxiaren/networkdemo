SRC=$(wildcard *.c)
BIN=$(patsubst %.c,%,$(SRC))
CFLAG=-I ../include
ALL: $(BIN)

%: %.c
	gcc $(CFLAG) $^ -o $@
.PHONY:clean
clean:
	-rm $(BIN)
