#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>
#include "Constants.hpp"
#include "ClientInfo.hpp"

#include "LoginLogic.hpp"
#include "RegisterLogic.hpp"

#include "DataBaseLogic.hpp"

#include "CustomUtility.hpp"

#define PORT 8080
#define MAX_CLIENTS 5

using namespace std;

void *handle_client(void *client_info_ptr)
{
    struct ClientInfo *client_info = static_cast<struct ClientInfo *>(client_info_ptr);
    int client_socket = client_info->socket;
    unsigned int client_command;

    cout << "New connection from: " << client_info->client_id << '\n';

    while (1)
    {
        read(client_socket, &client_command, sizeof(client_command));
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
        case Commands::Play:
        {
            cout << "Recived from " << client_info->client_id << " play command \n";
            break;
        }
        case Commands::Chat:
        {
            cout << "Recived from " << client_info->client_id << " chat command \n";
            break;
        }
        case Commands::Game_db:
        {
            cout << "Recived from " << client_info->client_id << " game_db command \n";
            break;
        }
        case Commands::Search:
        {
            cout << "Recived from " << client_info->client_id << " search command \n";
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
    // const char *query = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, password TEXT);";
    // // // test.CreateTable(query);
    // // query = "INSERT INTO users (username, password) VALUES ('TEST_USER', 'TEST_PASS');";
    // // test.InsertData(query);
    // query = "SELECT * FROM USERS";
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
