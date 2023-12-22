#pragma once
#include <iostream>
using namespace std;

//DataBase Utility
int displayCallback(void* /*unused*/, int argc, char** argv, char** colNames) {
    for (int i = 0; i < argc; ++i)
        cout << colNames[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\t";
    cout << '\n';
    return 0;
}