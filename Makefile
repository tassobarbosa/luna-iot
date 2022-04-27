CC	= g++
CPPFLAGS = -Iinclude
CFLAGS   = -Wall
LDFLAGS  = -Llib
LDLIBS   = -lm
ARGS = $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS)

all: server client

server: server.cpp
	g++ $(ARGS) -o server server.cpp src/*.cpp

client: client.cpp
	g++ $(ARGS) -o client client.cpp src/*.cpp

clean:
	$(RM) server client
	