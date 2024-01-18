#pragma once
#include <iostream>
#include <sqlite3.h>
#include "Constants.hpp"

using namespace std;

class DataBase
{
    sqlite3 *db;

public:
    DataBase();

    void CreateTable(const char *query);
    void InsertData(const char *query);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), unsigned int &result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), int &result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), char *result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), QueueGDI *result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), QueueUDI *result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), QueueS *result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), string *result);
    void SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **));
};