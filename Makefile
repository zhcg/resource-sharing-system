DIR_INC = ./include 
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TARGET = server_main

BIN_TARGET = ${DIR_BIN}/${TARGET}

#CROSS = ~/workspace/git/9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclib
CC = $(CROSS)gcc
STRIP = $(CROSS)strip

CFLAGS = -g -Wall -I${DIR_INC}
LDFLAGS = -lpthread

${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ)  -o $@ $(LDFLAGS) 
	$(CC) client.c -o client 
	mv client bin/

${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean

clean:
	find ${DIR_OBJ} -name *.o -exec rm -rf '{}' \;
	rm ${DIR_BIN}/*;
