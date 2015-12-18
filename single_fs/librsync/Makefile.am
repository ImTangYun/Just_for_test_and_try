AUTOMAKE_OPTIONS= foreign


INCLUDES=-I../include/

#gen librsync.so
librsync_so_LDFLAGS=-shared
module_LDADD= ../lib/libifs.so
moduledir= .
module_PROGRAMS= librsync.so
librsync_la_LDFLAGS = -module
librsync_so_SOURCES=rsync_core.cpp write_file_map.cpp file_map.cpp checksum.cpp checksum2.cpp \
					checksum3.cpp
librsync_so_LDADD= -lssl 

#gen executable prog ：rdiff、 file_tool 、rdiff_so
#and the compile order is decided as follow
bin_PROGRAMS= rdiff file_tool librsync.so rdiff_so 

rdiff_LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread

rdiff_so_LDADD= -L. -lrsync ../lib/libutils.a -lssl -L../lib -lifs
file_tool_LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread
				 checksum3.cpp

rdiff_SOURCES= rsync_core.cpp write_file_map.cpp file_map.cpp checksum.cpp checksum2.cpp \
			   checksum3.cpp rdiff.cpp
rdiff_so_SOURCES= rdiff.cpp

file_tool_SOURCES= write_file_map.cpp file_map.cpp file_tool.cpp
