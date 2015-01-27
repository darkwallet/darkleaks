CXXFLAGS=$(shell pkg-config --cflags libbitcoin) -I lib/
LIBS=$(shell pkg-config --libs libbitcoin)

LIBRARY_FILES=\
    lib/aes256.c \
    lib/darkleaks.hpp \
    lib/prove.cpp \
    lib/secrets.cpp \
    lib/start.cpp \
    lib/unlock.cpp \
    lib/utility.hpp

LIBRARY_OBJS=\
    lib/aes256.o \
    lib/prove.o \
    lib/secrets.o \
    lib/start.o \
    lib/unlock.o

TOOL_FILES=\
    tools/dl_prove.cpp \
    tools/dl_secrets.cpp \
    tools/dl_start.cpp \
    tools/dl_unlock.cpp \
    tools/utility.hpp

TOOL_BINS=\
    bin/dl_prove \
    bin/dl_secrets \
    bin/dl_start \
    bin/dl_unlock

default: all

tools/dl_prove.o: tools/dl_prove.cpp tools/utility.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
bin/dl_prove: tools/dl_prove.o
	mkdir -p bin/
	$(CXX) -o $@ tools/dl_prove.o lib/darkleaks.a $(LIBS)

tools/dl_secrets.o: tools/dl_secrets.cpp tools/utility.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
bin/dl_secrets: tools/dl_secrets.o
	mkdir -p bin/
	$(CXX) -o $@ tools/dl_secrets.o lib/darkleaks.a $(LIBS)

tools/dl_start.o: tools/dl_start.cpp tools/utility.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
bin/dl_start: tools/dl_start.o
	mkdir -p bin/
	$(CXX) -o $@ tools/dl_start.o lib/darkleaks.a $(LIBS)

tools/dl_unlock.o: tools/dl_unlock.cpp tools/utility.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
bin/dl_unlock: tools/dl_unlock.o
	mkdir -p bin/
	$(CXX) -o $@ tools/dl_unlock.o lib/darkleaks.a $(LIBS)

lib/*.o: $(LIBRARY_FILES)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

lib/darkleaks.a: $(LIBRARY_OBJS)
	ar rvs lib/darkleaks.a $(LIBRARY_OBJS)

python/darkleaks/_darkleaks.so: python/darkleaks/darkleaks.cpp
	$(CXX) -fPIC -I/usr/include/python2.7/ -Ilib/ -c python/darkleaks/darkleaks.cpp -o python/darkleaks/darkleaks.o
	$(CXX) -shared -Wl,-soname,_darkleaks.so python/darkleaks/darkleaks.o lib/darkleaks.a -lpython2.7 -lboost_python `pkg-config --libs libbitcoin` -lboost_thread -o python/darkleaks/_darkleaks.so

python_bindings: python/darkleaks/_darkleaks.so

all: lib/darkleaks.a $(TOOL_BINS) python_bindings

clean:
	rm -f lib/*.o tools/*.o

