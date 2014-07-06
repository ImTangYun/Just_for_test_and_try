PREFIX := ../
LIB := $(PREFIX)extern_lib/libthread.a $(PREFIX)extern_lib/libepoll.a $(PREFIX)extern_lib/libconfig.a $(PREFIX)/extern_lib/libutil.a -lpthread
INCS := $(PREFIX)extern_include
USER_MARCOS := #_FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE

CPPFLAGS := -Wall -Werror -g -fPIC -std=c++11
CC = g++

INSTALL := cp -p

MAIN := main

OBJS := main.o http_socket_handler.o task_queue.o response.o http_conf.o
all: $(MAIN)

%.o:%.cpp
	$(CC) $(CPPFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS))  -c $< -o $@

$(MAIN): main.o http_socket_handler.o task_queue.o response.o http_conf.o $(LIB)


clean:
	rm -f *.o
	rm -f $(MAIN)
