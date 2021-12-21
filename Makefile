
NAME=krycc
CC=clang++
CPPFLAGS= -O2 -I/usr/include/clang
LDFLAGS=`llvm-config --cxxflags --ldflags --libs core executionengine mcjit interpreter analysis native bitwriter --system-libs`

OBJS = \
	main.o \
	jit.o \
    predef.o \
    gmpredef.o \
    ../gmp/gmp-6.2.1-normalcc/.libs/libgmp.a

.SUFFIXES :
.SUFFIXES : .c .cpp .o

all: $(NAME).exe

clean:
	rm *.exe *.o

$(NAME).exe: $(OBJS)
	$(CC) -g -o $(NAME).exe $(OBJS) $(LDFLAGS) -lclang-cpp

.cpp.o:
	$(CC) -g -c $(CPPFLAGS) -o $*.o $<



