AUTOMAKE_OPTIONS= foreign
bin_PROGRAMS= server client
INCLUDES=-I../include/

LDADD=../lib/libnetmachine.a ../lib/libifs.a ../lib/libutils.a -lssl -lpthread

server_SOURCES= main.cpp server.cpp chat_process.cpp server_handler.cpp \
				user_connection.cpp client.cpp

client_SOURCES= client_main.cpp client.cpp
