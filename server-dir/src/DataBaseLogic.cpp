#include "DataBaseLogic.hpp"

DataBase::DataBase()
{
    int sqlite3_result_code = sqlite3_open("./assets/KnightmareNetwork.db", &db);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Open Database]");
        sqlite3_close(db);
    }
}

void DataBase::CreateTable(const char *query)
{
    int sqlite3_result_code = sqlite3_exec(db, query, 0, 0, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Create Database]");
        sqlite3_close(db);
    }
}

void DataBase::InsertData(const char *query)
{
    int sqlite3_result_code = sqlite3_exec(db, query, 0, 0, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Insert Database]");
        sqlite3_close(db);
    }
}

void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), unsigned int &result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, &result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), int &result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, &result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), char *result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), QueueGDI *result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), QueueUDI *result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), QueueS *result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **), string *result)
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, result, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
void DataBase::SelectData(const char *query, int (*CallBackFunction)(void *, int, char **, char **))
{
    int sqlite3_result_code = sqlite3_exec(db, query, CallBackFunction, 0, 0);
    if (sqlite3_result_code != SQLITE_OK)
    {
        perror("[Select Database]");
        sqlite3_close(db);
    }
}
