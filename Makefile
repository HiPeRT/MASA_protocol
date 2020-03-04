demo: server client
CFLAGS=-Wall -g -std=c++11
LDFLAGS=-lpthread -lrt
INCLUDE=-I./cereal/include -I./include

server: demo/server.cpp
	g++ ${CFLAGS} ${INCLUDE} demo/server.cpp -o server ${LDFLAGS}

client: demo/client.cpp
	g++ ${CFLAGS} ${INCLUDE} demo/client.cpp -o client ${LDFLAGS}

clean:
	rm server
	rm client