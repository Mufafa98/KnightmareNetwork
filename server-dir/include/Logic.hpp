#pragma once
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include "Constants.hpp"
#include "ClientInfo.hpp"
#include "DataBaseLogic.hpp"
#include "Engine.hpp"

using namespace std;

void LoginLogic(ClientInfo &info);
int CheckLoginInfoInDb(void *data, int argc, char **argv, char **colNames);

void RegisterLogic(ClientInfo &info);
int CheckRegisterInfoInDb(void *data, int argc, char **argv, char **colNames);

void ReqUserData(ClientInfo &info);
int CheckDataReq(void *data, int argc, char **argv, char **colNames);

void BoardCommandLogic(ClientInfo &info, Engine &engine, game_info &game);