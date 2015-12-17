AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= rdiff file_tool
INCLUDES=-I../include/
rdiff_LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread
file_tool_LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread
librsync_so_LDFLAGS=-shared
module_PROGRAMS= librsync.so
module_LDADD= ../lib/libifs.so
moduledir= .
librsync_la_LDFLAGS = -module
librsync_so_SOURCES=rsync_core.cpp write_file_map.cpp file_map.cpp checksum.cpp checksum2.cpp \
				 checksum3.cpp

rdiff_SOURCES= rsync_core.cpp write_file_map.cpp file_map.cpp checksum.cpp checksum2.cpp \
			   checksum3.cpp rdiff.cpp

file_tool_SOURCES= write_file_map.cpp file_map.cpp file_tool.cpp
