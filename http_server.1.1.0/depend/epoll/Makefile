PREFIX := ../../
LIB := $(PREFIX)/extern_lib/libthread.a -lpthread

INCS := $(PREFIX) $(PREFIX)/extern_include
USER_MARCOS := _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE

CPPFLAGS := -Wall -Werror -g -fPIC -std=c++0x
CC = g++

INSTALL_LIB := $(PREFIX)/extern_lib
INSTALL_INCLUDE := $(PREFIX)/extern_include
INSTALL := cp -p

EPOLL_LIB_TAGS := libepoll.a
EPOLL_LIB_SO_TAGS := libepoll.so


SOURCE := $(wildcard  *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCE))

all: $(EPOLL_LIB_TAGS) $(EPOLL_LIB_SO_TAGS)
$(EPOLL_LIB_TAGS): $(OBJS)
	$(AR) -crv $@ $(OBJS)

$(EPOLL_LIB_SO_TAGS): $(OBJS)
	$(CC) --shared -fPIC -o $@ $(OBJS)

%.o:%.cpp
	$(CC) $(CPPFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) $(addprefix -L,$(LIB)) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(EPOLL_LIB_TAGS) $(EPOLL_LIB_SO_TAGS)

install: all
	$(INSTALL) $(EPOLL_LIB_TAGS) $(INSTALL_LIB)
	$(INSTALL) $(EPOLL_LIB_SO_TAGS) $(INSTALL_LIB)
	cp *.h $(INSTALL_INCLUDE)
	rm -rf *.o $(EPOLL_LIB_TAGS) $(EPOLL_LIB_SO_TAGS)
