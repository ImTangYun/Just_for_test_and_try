AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= server main
INCLUDES=-I../include/
server_SOURCES= server.cpp server_handler.cpp
LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a
main_SOURCES= main.cpp client.cpp
