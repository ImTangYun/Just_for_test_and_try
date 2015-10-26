PREFIX := ../../
LIB :=  -lpthread
INCS := $(PREFIX)extern_include

CPPFLAGS := -Wall -Werror -g -fPIC -std=c++0x
CC = g++

install:
	cp taskqueue.h $(PREFIX)/extern_include


MAIN := test

OBJS := test.o 
all: $(MAIN)

%.o:%.cpp
	$(CC) $(CPPFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS))  -c $< -o $@

$(MAIN): test.o  $(LIB)


clean:
	rm -f *.o
	rm -f $(MAIN)
