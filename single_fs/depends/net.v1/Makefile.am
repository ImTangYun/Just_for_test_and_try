AUTOMAKE_OPTIONS= foreign
noinst_LIBRARIES=libnetmachine.a
libnetmachine_a_SOURCES=listen_handler.cpp socket_context.cpp cthread.cpp communicate_loop.cpp \
						net_machine.cpp cthread_mutex.cpp listen_socket_context.cpp \
						stream_socket_context.cpp package.cpp
libnetmachine_a_LIBDADD=../../lib
INCLUDES=-I../../include/
#SUBDIRS=. app
SUBDIRS=. test

module_PROGRAMS= libnetmachine.so
moduledir= .
libnetmachine_so_SOURCES= listen_handler.cpp socket_context.cpp cthread.cpp communicate_loop.cpp \
						 net_machine.cpp cthread_mutex.cpp listen_socket_context.cpp \
						 stream_socket_context.cpp package.cpp
libnetmachine_so_LDFLAGS= -shared
libnetmachine_la_LDFLAGS= -module
