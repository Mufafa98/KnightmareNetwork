CC := g++
CFLAGS := -Wall -pthread -std=c++17
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsqlite3

all: server run

server: 
	@$(CC) $(CFLAGS) $(LIBS) -I./include/ ./src/*.cpp -o ./bin/server

run:
	@./bin/server

clean:
	@rm -f server client
