CC := g++
CFLAGS := -Wall -pthread -std=c++17
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: server client

server: 
	@$(CC) $(CFLAGS) $(LIBS) -I./server-dir/include/ ./server-dir/src/*.cpp -o ./server-dir/bin/server

run-server:
	@./server-dir/bin/server

client: 
	@$(CC) $(CFLAGS) $(LIBS) -I./client-dir/include/ ./client-dir/src/*.cpp -o ./client-dir/bin/client

run-client:
	@./client-dir/bin/client
clean:
	@rm -f server client
