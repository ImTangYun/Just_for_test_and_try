AUTOMAKE_OPTIONS= foreign
#bin_PROGRAMS= server client test cp
bin_PROGRAMS= test cp scan simple_rsync
INCLUDES=-I../include/
#server_SOURCES= server.cpp server_handler.cpp
LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread
#client_SOURCES= main.cpp client.cpp
test_SOURCES= checksum.cpp test.cpp checksum1.cpp checksum2.cpp checksum3.cpp
cp_SOURCES= cp.cpp checksum.cpp checksum1.cpp checksum2.cpp checksum3.cpp file_map.cpp
scan_SOURCES= checksum.cpp scan.cpp checksum1.cpp checksum2.cpp checksum3.cpp
simple_rsync_SOURCES= checksum.cpp main.cpp checksum1.cpp checksum2.cpp \
					  rsync_service.cpp server_handler.cpp server_processor.cpp \
					  server.cpp client.cpp checksum3.cpp file_map.cpp
