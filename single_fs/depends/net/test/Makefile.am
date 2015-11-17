AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= client server# main
INCLUDES=-I../../../include/
client_SOURCES= client.cpp
server_SOURCES= server.cpp server_handler.cpp #file_utils.cpp
LDADD=../../../lib/libnetmachine.a #../lib/libifs.a
#server_LDADD=../lib/libnetmachine.a ../lib/libifs.a
