#pragma once
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <stack>
#include <mutex>
#include <fcntl.h>
#include <chrono>
#include <ctime>
#include "Constants.hpp"
#include "ClientInfo.hpp"
#include "DataBaseLogic.hpp"
#include "Engine.hpp"
#include "CustomUtility.hpp"

using namespace std;

void LoginLogic(ClientInfo &info);
int CheckLoginInfoInDb(void *data, int argc, char **argv, char **colNames);

void RegisterLogic(ClientInfo &info);
int CheckRegisterInfoInDb(void *data, int argc, char **argv, char **colNames);

void ReqUserData(ClientInfo &info);
int CheckDataReq(void *data, int argc, char **argv, char **colNames);

void BoardCommandLogic(ClientInfo &info, Engine &engine, GameInfo &game);

unsigned int PlayLogic(ClientInfo &info, Engine &engine, GameInfo &game);

void SocialLogic(ClientInfo &info);
int GetGamesInStack(void *data, int argc, char **argv, char **colNames);
int GetUsersInStack(void *data, int argc, char **argv, char **colNames);
int GetFriendsInStack(void *data, int argc, char **argv, char **colNames);
int GetLineNo(void *data, int argc, char **argv, char **colNames);
int GetGamePos(void *data, int argc, char **argv, char **colNames);
