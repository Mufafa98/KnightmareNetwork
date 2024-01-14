#pragma once
#include <SFML/Graphics.hpp>

#define START_CHESS_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
// #define START_CHESS_POS "7k/RQ6/8/8/8/8/8/7K"
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
    BoardCommand,
    BoardCommandMatch,
    SaveMatch
};

enum BoardCommands
{
    FirstPos,
    SecondPos,
    SelectSquare,
    HighlightLegalMoves,
    MovePiece,
    MovePieceAnimate,
    UnableToMovePiece,
    NeedPromotion,
    PromotePawn,
    PromotePawnAnimate,
    NotYourTurn,
    YourTurn,
    GameOver,
    GameSaved,
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

enum InternalLogic
{
    Nothing,
    OpponentFound
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

struct GameInfo
{
    sf::Vector2i first_pos;
    sf::Vector2i second_pos;
    unsigned int color;
    /*
        0 - white
        1 - black
    */
};