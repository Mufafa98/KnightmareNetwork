#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Piece.hpp"
#include "DigitalTree.hpp"

using namespace sf;

class Engine
{
    bool game_running;
    bool turn_swapped;
    unsigned short game_result; // 1-draw   2-white won   3-black won
    unsigned short pawn_needs_promotion;
    unsigned short black_castling_rights;
    unsigned short white_castling_rights;
    unsigned int turn;
    unsigned int fifty_move_rule;
    DigitalTree game_past_possitions;
    unsigned short board_to_check[8][8];
    bool highlighted_sq[8][8];

    bool IsLegal(unsigned short (&board_to_check)[8][8], const Vector2i &start_pos, const Vector2i &end_pos, const int options);
    bool CheckMoveRook(unsigned short (&board_to_check)[8][8], const Vector2i &start_pos, const Vector2i &end_pos);
    bool CheckMoveBishop(unsigned short (&board_to_check)[8][8], const Vector2i &start_pos, const Vector2i &end_pos);
    bool MoveResultsInCheck(const Vector2i &start_pos, const Vector2i &end_pos);
    bool IsAttackedBy(const unsigned short side, const Vector2i &pos);
    bool IsChecked(const unsigned short side);
    bool HasLegalMoves(const unsigned short side);
    string CheckSidePieces(const unsigned short side);
    string BoardToFEN();

    void RemoveEnPassantTrace(unsigned short side);
    void SetEnPassantTrace(const Vector2i &pos, const unsigned short side);
    void MovePiece(const Vector2i &start_pos, const Vector2i &end_pos);

public:
    Engine();
    bool SelectSquare(const Vector2i &pos);
    void FENToBoard(string FEN);
    void DeSelectSquare(); // e nevoie?
    void HighlightLegalMoves(const Vector2i &pos);
    void DeHighlightLegalMoves();
    void CheckEndConditions();
    bool MakeMove(const Vector2i &start_pos, const Vector2i &end_pos);
    bool GameRunning();
    bool PawnNeedsPromotion();
    void PromotePawn(const unsigned short &id);
    unsigned short GetGameResult();
    unsigned short TurnSwapped();

    void GetHighlightedArray(bool (&array)[8][8]);
};

//  options for legal move
//  0 - none
//  1 - ignore turn
//  2 - ignore checks
//  3 - ignore turn and checks