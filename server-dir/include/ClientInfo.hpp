#pragma once
#include "DataBaseLogic.hpp"

struct ClientInfo
{
    int socket;
    unsigned int client_id;
    unsigned int perm_id;
    DataBase *db;
};