CC := g++
CFLAGS := -Wall -pthread -std=c++17
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: client run

client: 
	@$(CC) $(CFLAGS) $(LIBS) -I./include/ ./src/*.cpp -o ./bin/client

run:
	@./bin/client
clean:
	@rm -f server client
