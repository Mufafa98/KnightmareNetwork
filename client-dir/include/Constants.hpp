#pragma once
#include <SFML/Graphics.hpp>

#define START_CHESS_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

enum Commands
{
    Exit,
    Login,
    Register,
    RequestUserData,
    Play,
    Search,
    Chat,
    Game_db,
    BoardCommand
};

enum BoardCommands
{
    FirstPos,
    SecondPos,
    SelectSquare,
    HighlightLegalMoves,
    MovePiece,
    NeedPromotion,
    PromotePawn,
    None
};

enum LogicCodes
{
    NextScreen,
    VoidInput,
    InvalidLoginInfo,
    InvalidRegInfo,
    Exit_req,
    Success,
    Error
};

enum DBCodes
{
    NotFound,
    AlreadyInDb
};

struct user_info
{
    unsigned int id;
    char username[20];
};

struct game_info
{
    sf::Vector2i first_pos;
    sf::Vector2i second_pos;
};