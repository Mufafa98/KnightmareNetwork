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

void RegisterLogic(ClientInfo &info);
int CheckRegisterInfoInDb(void *data, int argc, char **argv, char **colNames);