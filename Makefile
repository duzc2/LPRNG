#
# Makefile for LPRNG
#
include ./config

lprng.o:
	$(CC) -O2 -fPIC -Wall -g -DBUILD_DLL -I$(LUA_INCLUDE) -c lprng.c -o lprng.o

all: lprng.o
	$(CC) -shared -fPIC -Wl,--dll  -static-libgcc  -L$(LUA_LIB_PATH) -llua  -o $(SO) lprng.o 

clean:
	$(RM) -f lprng.o
	$(RM) -f lprng.so