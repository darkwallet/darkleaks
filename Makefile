CXXFLAGS=$(shell pkg-config --cflags libbitcoin libbitcoin-client) -ggdb
LIBS=$(shell pkg-config --libs libbitcoin libbitcoin-client)

default: all

dl_check_addr.o: dl_check_addr.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
dl_check_addr: dl_check_addr.o
	$(CXX) -o $@ dl_check_addr.o $(LIBS)

dl_unlock.o: dl_unlock.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
dl_unlock: dl_unlock.o aes256.o
	$(CXX) -o $@ dl_unlock.o aes256.o $(LIBS)

dl_prove.o: dl_prove.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
dl_prove: dl_prove.o
	$(CXX) -o $@ dl_prove.o $(LIBS)

aes256.o: aes256.c
	$(CXX) -o $@ -c $<

dl_secrets.o: dl_secrets.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
dl_secrets: dl_secrets.o
	$(CXX) -o $@ dl_secrets.o $(LIBS)

dl_start.o: dl_start.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
dl_start: dl_start.o aes256.o
	$(CXX) -o $@ dl_start.o aes256.o $(LIBS)

all: dl_start dl_prove dl_unlock dl_secrets dl_check_addr

clean:
	rm -f dl_start dl_prove dl_unlock dl_secrets dl_check_addr 
	rm -f *.o

