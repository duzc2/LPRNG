#
# Makefile for LPRNG
#
include ./config

lprng.o:
	$(CC) -I$(LUA_INCLUDE) -O2 -fPIC -Wall -g -DBUILD_DLL -c lprng.c -o lprng.o

so: lprng.o
	$(CC) -shared -fPIC -Wl,--dll   -L$(LUA_LIB) -llua  -o $(SO) lprng.o 

all:so
	@echo finished

clean:
	$(RM) -f lprng.o
	$(RM) -f LPRNG.so
