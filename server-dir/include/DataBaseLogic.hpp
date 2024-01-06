#pragma once
#include <iostream>
#include <sqlite3.h>

using namespace std;

class DataBase
{
    sqlite3 *db;

public:
    DataBase();

    void CreateTable(const char *query);
    void InsertData(const char *query);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), unsigned int &result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), char *result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **));
};