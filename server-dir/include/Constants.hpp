#pragma once

enum Commands
{
    Exit,
    Login,
    Register,
    Play,
    Search,
    Chat,
    Game_db
};

enum LogicCodes
{
    NextScreen,
    VoidInput,
    InvalidLoginInfo,
    InvalidRegInfo,
    Success,
    Error
};

enum DBCodes
{
    NotFound,
    AlreadyInDb
};