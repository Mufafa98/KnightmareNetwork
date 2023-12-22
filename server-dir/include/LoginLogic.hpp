#pragma once
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include "Constants.hpp"
#include "ClientInfo.hpp"
#include "DataBaseLogic.hpp"

using namespace std;

void LoginLogic(ClientInfo &info);
int CheckLoginInfoInDb(void *data, int argc, char **argv, char **colNames);