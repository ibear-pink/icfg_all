#!/bin/ksh

ICFG_INCLUDE=-I${IBF_PATH}/icfg_all/icfg/include
ICFG_LIB=-L${IBF_PATH}/icfg_all/icfg/lib -licfg

IFILE_INCLUDE=-I${IBF_PATH}/ifile_all/ifile/include
IFILE_LIB=-L${IBF_PATH}/ifile_all/ifile/lib -lifile

#SDFS_INCLUDE=-I${SDFS_HOME}/include -I${DSC_PATH}/include
#SDFS_LIB=-L${SDFS_HOME}/lib -lsdfs -L${DSC_PATH}/lib -ldsc

IFLAGS=-DSS_64BIT_SERVER -D_REENTRANT -D_DEBUG_INFO_

INCLUDE=-I. ${ICFG_INCLUDE} ${SDFS_INCLUDE} ${IFILE_INCLUDE}

LIB=-L. ${ICFG_LIB} ${SDFS_LIB} ${IFILE_LIB}

.SUFFIXES:.cpp .o

CFLAGS = ${IFLAGS} ${INCLUDE}

CC=g++ -m64 -g  -lpthread  -ldl -O0 -fpermissive

OBJ = demo.o

BIN=demo

all:${BIN} install clean

.cpp.o:
	echo "Compiling the $<"
	$(CC) -c ${CFLAGS} $<
.c.o:
	echo "Compiling the $<"
	$(CC) -c ${CFLAGS} $<

clean:
	rm -f demo.o

######################################################################

${BIN}: $(OBJ)
	$(CC) ${CFLAGS} -o $@ $? ${LIB}

######################################################################

install:

#mv ${BIN} ${ACCT_BIN}


