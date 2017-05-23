#CXX=g++
#See https://www.softprayog.in/programming/d-bus-tutorial for DBus example 
CXX_FLAGS=`pkg-config --cflags dbus-1` -I./  -std=c++11
LD_FALGS=`pkg-config --libs dbus-1` -lpthread

all: testWrapper

%.o: %.cc 
	$(CXX) -Wall -c  $< -o $@  ${CXX_FLAGS}

testWrapper: testWrapper.cc
	$(CXX) -o $@ $<  ${LD_FLAGS}

.PHONY: clean
clean:
	rm *.o testWrapper
	
config:
	sudo cp in.softprayog.add_server.conf /etc/dbus-1/system.d/in.softprayog.add_server.conf
	sudo cp in.softprayog.add_client.conf /etc/dbus-1/system.d/in.softprayog.add_client.conf
	sudo service dbus reload
  
