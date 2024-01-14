#pragma once
#include "DataBaseLogic.hpp"

struct ClientInfo
{
    int socket;
    int thread_socket;
    unsigned int client_id;
    unsigned int perm_id;
    unsigned int seed;
    char username[20];
    DataBase *db;
};