AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= server main
INCLUDES=-I../include/
server_SOURCES= server.cpp server_handler.cpp
main_LDADD=../lib/libnetmachine.a ../lib/libifs.a
server_LDADD=../lib/libnetmachine.a ../lib/libifs.a
main_SOURCES= main.cpp client.cpp
