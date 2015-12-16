AUTOMAKE_OPTIONS= foreign
#bin_PROGRAMS= server client test cp
bin_PROGRAMS= rdiff file_tool
INCLUDES=-I../include/
#server_SOURCES= server.cpp server_handler.cpp
LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread
#client_SOURCES= main.cpp client.cpp

rdiff_SOURCES= rsync_core.cpp write_file_map.cpp file_map.cpp checksum.cpp checksum2.cpp \
			   checksum3.cpp rdiff.cpp

file_tool_SOURCES= write_file_map.cpp file_map.cpp file_tool.cpp
