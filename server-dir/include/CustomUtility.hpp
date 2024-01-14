#pragma once
#include <iostream>
#include <random>
using namespace std;

// DataBase Utility
int displayCallback(void * /*unused*/, int argc, char **argv, char **colNames);
int generateIntRandom(unsigned int &seed, const unsigned int min, const unsigned int max);