PREFIX := ../../
#PANDORA := $(PREFIX)extern_lib/
#LIB := -Wl,-rpath,../extern_lib -L$(PANDORA) $(PANDORA)/libpandora.a -Wl,-rpath,/opt/apps/gcc-4.7.3/lib64,-rpath,/opt/apps/gcc-4.7.3/lib

INCS := $(PREFIX) $(PREFIX)/extern_include
USER_MARCOS := _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE

CPPFLAGS := -Wall -Werror -g -fPIC -std=c++0x
CC = g++

INSTALL_LIB := $(PREFIX)/extern_lib
INSTALL_INCLUDE := $(PREFIX)/extern_include
INSTALL := cp -p

THREAD_LIB_TAGS := libthread.a
THREAD_LIB_SO_TAGS := libthread.so


SOURCE := $(wildcard  *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCE))

all: $(THREAD_LIB_TAGS) $(THREAD_LIB_SO_TAGS)
$(THREAD_LIB_TAGS): $(OBJS)
	$(AR) -crv $@ $(OBJS)

$(THREAD_LIB_SO_TAGS): $(OBJS)
	$(CC) --shared -fPIC -o $@ $(OBJS)

%.o:%.cpp
	$(CC) $(CPPFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(CLIENT_LIB_TAGS) $(CLIENT_LIB_SO_TAGS)

install: all
	$(INSTALL) $(THREAD_LIB_TAGS) $(INSTALL_LIB)
	$(INSTALL) $(THREAD_LIB_SO_TAGS) $(INSTALL_LIB)
	cp *.h $(INSTALL_INCLUDE)
	rm -rf *.o $(THREAD_LIB_TAGS) $(THREAD_LIB_SO_TAGS)
