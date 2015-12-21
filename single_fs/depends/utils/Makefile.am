AUTOMAKE_OPTIONS= foreign
noinst_LIBRARIES=libutils.a
bin_PROGRAMS=log_test
log_test_SOURCES=log.cpp log_test.cpp
libutils_a_SOURCES=time_utils.cpp log.cpp


libutils_so_LDFLAGS= -shared
module_PROGRAMS=libutils.so
moduledir= .
libutils_la_LDFLAGS= -module
libutils_so_SOURCES= log.cpp time_utils.cpp
