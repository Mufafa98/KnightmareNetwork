#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>
#include <stack>
#include <ctime>
#include <chrono>
#include <string>
#include "Constants.hpp"
#include "ClientInfo.hpp"

#include "Logic.hpp"

#include "DataBaseLogic.hpp"

#include "CustomUtility.hpp"

#include "Engine.hpp"

#define PORT 8080
#define MAX_CLIENTS 5

using namespace std;

stack<int[2]> play_wait_list;

void *handle_client(void *client_info_ptr)
{
    Engine engine;
    GameInfo game;
    engine.FENToBoard(START_CHESS_POS);
    struct ClientInfo *client_info = static_cast<struct ClientInfo *>(client_info_ptr);
    int client_socket = client_info->socket;
    unsigned int client_command;

    cout << "New connection from: " << client_info->client_id << '\n';

    while (1)
    {
        recv(client_socket, &client_command, sizeof(client_command), 0);
        switch (client_command)
        {
        case Commands::Exit:
        {
            cout << "Recived from " << client_info->client_id << " exit command \n";
            write(client_socket, &client_command, sizeof(client_command));
            close(client_socket);
            delete client_info;
            return nullptr;
            break;
        }
        case Commands::Login:
        {
            cout << "[" << client_info->client_id << "] ";
            LoginLogic(*client_info);
            break;
        }
        case Commands::Register:
        {
            cout << "[" << client_info->client_id << "] ";
            RegisterLogic(*client_info);
            break;
        }
        case Commands::RequestUserData:
        {
            cout << "[" << client_info->client_id << "] ";
            ReqUserData(*client_info);
            break;
        }
        case Commands::Play:
        {
            cout << "[" << client_info->client_id << "] ";
            if (PlayLogic(*client_info, engine, game) == Commands::Exit)
            {
                cout << "Recived from " << client_info->client_id << " exit command \n";
                write(client_socket, &client_command, sizeof(client_command));
                close(client_socket);
                delete client_info;
                return nullptr;
                break;
            }
            break;
        }
        case Commands::BoardCommand:
        {
            cout << "[" << client_info->client_id << "] ";
            BoardCommandLogic(*client_info, engine, game);
            break;
        }
        case Commands::Search:
        {
            SocialLogic(*client_info);
            break;
        }
        default:
            break;
        }
    }

    cout << "Connection from: " << client_info->client_id << " closed" << '\n';

    close(client_socket);
    delete client_info;

    return nullptr;
}

int main()
{
    // DataBase test;
    // char query[200];
    // // query = "CREATE TABLE IF NOT EXISTS games ( game_id INTEGER PRIMARY KEY, white_player INTEGER, black_player INTEGER, game TEXT, FOREIGN KEY (white_player) REFERENCES users(id), FOREIGN KEY (black_player) REFERENCES users(id) );";
    // // // test.CreateTable(query);
    // // query = "INSERT INTO users (username, password) VALUES ('TEST_USER', 'TEST_PASS');";
    // strcpy(query, "INSERT INTO games (white_player, black_player, game) VALUES (1, 2, 'asdaasdassdasdasd');");
    // test.InsertData(query);
    // // test.CreateTable(query);
    // strcpy(query, "SELECT * FROM games;");
    // test.SelectData(query, displayCallback);
    // return 0;
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t thread_id;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("[Server]: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int on = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[Server]: Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("[Server]: Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << PORT << "..." << '\n';
    unsigned int client_id_local = 0;
    DataBase db;
    // string querry;
    // querry = "DROP TABLE friends;";
    // db.SelectData(querry.c_str(), displayCallback);
    // querry = "CREATE TABLE IF NOT EXISTS friends ( friendship_id INTEGER PRIMARY KEY, user_1 INTEGER, user_2 INTEGER, FOREIGN KEY (user_1) REFERENCES users(id), FOREIGN KEY (user_2) REFERENCES users(id) );";
    // db.CreateTable(querry.c_str());
    // querry = "SELECT * FROM friends;";
    // db.SelectData(querry.c_str(), displayCallback);
    // return 0;
    // querry = "DROP TABLE friends;";
    // db.SelectData(querry.c_str(), displayCallback);
    while (true)
    {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0)
        {
            perror("[Server]: Accept failed");
            continue;
        }

        struct ClientInfo *client_info = new struct ClientInfo;
        client_info->socket = client_socket;
        client_info->client_id = client_id_local;
        client_info->db = &db;
        client_info->seed = 314159265;
        client_id_local++;

        if (pthread_create(&thread_id, nullptr, handle_client, static_cast<void *>(client_info)) < 0)
        {
            perror("[Server]: Could not create thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);
    }

    close(server_socket);

    return 0;
}
