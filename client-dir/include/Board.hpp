#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include "ClientData.hpp"
#include "Piece.hpp"
#include "Button.hpp"

using namespace sf;
using namespace std;
class Board
{
    unsigned short need_promote_w;
    unsigned short need_promote_b;
    unsigned short pawn_needs_promotion;
    unsigned short board[8][8];
    Vector2f board_pos;
    Texture board_background_texture;
    SoundBuffer buffer[2];
    Sound sound[2];
    RectangleShape board_background;
    RectangleShape pieces[8][8];
    RectangleShape selected_square;
    RectangleShape highlighted_square[8][8];

    RectangleShape promote_bg;
    RectangleShape promote_Q;
    RectangleShape promote_R;
    RectangleShape promote_N;
    RectangleShape promote_B;

public:
    map<unsigned short, Texture> pieces_texture;
    void MovePiece(const Vector2i &start_pos, const Vector2i &end_pos);
    void PlaySound(const Vector2i &start_pos, const Vector2i &end_pos);
    void SelectSquare(const Vector2i &pos);
    void DeSelectSquare();
    void HighlightSquare(const unsigned short x, const unsigned short y);
    void DeHighlightSquare();
    void SetEnPassantTrace(const Vector2i &pos, const unsigned short side);
    void RemoveEnPassantTrace(const unsigned short side);
    void UpdateBoard();
    void PromotePawn(const unsigned short &id);
    Board(const String FEN_board, Vector2f board_pos);
    string BoardToFEN();
    void FENToBoard(string FEN);

    void SetPosition(Vector2f pos);
    bool Contains(Vector2f mouse_pos);
    bool ContainsPromBox(Vector2f mouse_pos);
    void TryCastling(const Vector2i &start_pos, const Vector2i &end_pos);
    void TryEnPassant(const Vector2i &start_pos, const Vector2i &end_pos);

    Vector2f GetPosition();
    Vector2f GetEndPosition();
    unsigned int GetChoice(Vector2f mouse_pos);

    void Draw(RenderWindow &window);
};