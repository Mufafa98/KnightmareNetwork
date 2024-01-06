#include "Engine.hpp"

Engine::Engine()
{
    pawn_needs_promotion = 8;
    turn_swapped = false;
    turn = 0;
    fifty_move_rule = 0;
    white_castling_rights = 3;
    black_castling_rights = 3;
    game_running = true;
    game_result = 0;
}

bool Engine::CheckMoveRook(unsigned short (&board_to_check)[8][8], const Vector2i &start_pos, const Vector2i &end_pos)
{
    const unsigned short start_x = start_pos.x;
    const unsigned short start_y = start_pos.y;
    const unsigned short end_x = end_pos.x;
    const unsigned short end_y = end_pos.y;
    if (end_x != start_x && end_y != start_y)
        return false;
    unsigned short left_limit = 0;
    unsigned short right_limit = 7;
    unsigned short up_limit = 0;
    unsigned short down_limit = 7;
    for (unsigned short i = 0; i < 8; i++)
    {
        if (i < start_x && i > left_limit && board_to_check[start_y][i] && (board_to_check[start_y][i] & 7) != Piece::EnPassantTrace)
            left_limit = i;
        else if (i > start_x && i < right_limit && board_to_check[start_y][i] && (board_to_check[start_y][i] & 7) != Piece::EnPassantTrace)
            right_limit = i;
        if (i < start_y && i > up_limit && board_to_check[i][start_x] && (board_to_check[i][start_y] & 7) != Piece::EnPassantTrace)
            up_limit = i;
        else if (i > start_y && i < down_limit && board_to_check[i][start_x] && (board_to_check[i][start_y] & 7) != Piece::EnPassantTrace)
            down_limit = i;
    }
    if (end_x < left_limit || end_x > right_limit)
        return false;
    if (end_y < up_limit || end_y > down_limit)
        return false;
    return true;
}

bool Engine::CheckMoveBishop(unsigned short (&board_to_check)[8][8], const Vector2i &start_pos, const Vector2i &end_pos)
{
    const unsigned short start_x = start_pos.x;
    const unsigned short start_y = start_pos.y;
    const unsigned short end_x = end_pos.x;
    const unsigned short end_y = end_pos.y;
    if (abs(short(end_x - start_x)) != abs(short(start_y - end_y)))
        return false;
    unsigned short left_up_limit = 0;
    unsigned short right_down_limit = 0;
    unsigned short right_up_limit = 0;
    unsigned short left_down_limit = 0;
    for (unsigned short i = 1; i < 8; i++)
    {
        if (short(start_x - i) < 0 || short(start_y - i) < 0)
            break;
        left_up_limit = i;
        if (board_to_check[start_y - i][start_x - i] && (board_to_check[start_y - i][start_x - i] & 7) != Piece::EnPassantTrace)
            break;
    }
    for (unsigned short i = 1; i < 8; i++)
    {
        if (short(start_x + i) > 7 || short(start_y + i) > 7)
            break;
        right_down_limit = i;
        if (board_to_check[start_y + i][start_x + i] && (board_to_check[start_y + i][start_x + i] & 7) != Piece::EnPassantTrace)
            break;
    }
    for (unsigned short i = 1; i < 8; i++)
    {
        if (short(start_x - i) < 0 || short(start_y + i) > 7)
            break;
        left_down_limit = i;
        if (board_to_check[start_y + i][start_x - i] && (board_to_check[start_y + i][start_x - i] & 7) != Piece::EnPassantTrace)
            break;
    }
    for (unsigned short i = 1; i < 8; i++)
    {
        if (short(start_x + i) > 7 || short(start_y - i) < 0)
            break;
        right_up_limit = i;
        if (board_to_check[start_y - i][start_x + i] && (board_to_check[start_y - i][start_x + i] & 7) != Piece::EnPassantTrace)
            break;
    }
    if (end_y < start_y - left_up_limit && end_x < start_x - left_up_limit)
        return false;
    if (end_y > start_y + right_down_limit && end_x > start_x + right_down_limit)
        return false;
    if (end_y > start_y + left_down_limit && end_x < start_x - left_down_limit)
        return false;
    if (end_y < start_y - right_up_limit && end_x > start_x + right_up_limit)
        return false;
    return true;
}

bool Engine::MoveResultsInCheck(const Vector2i &start_pos, const Vector2i &end_pos)
{
    unsigned short temp_board[8][8];
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            temp_board[i][j] = board_to_check[i][j];
    temp_board[(unsigned short)end_pos.y][(unsigned short)end_pos.x] = temp_board[(unsigned short)start_pos.y][(unsigned short)start_pos.x];
    temp_board[(unsigned short)start_pos.y][(unsigned short)start_pos.x] = 0;
    const unsigned short side = board_to_check[(unsigned short)start_pos.y][(unsigned short)start_pos.x] & 24;

    if (side == Piece::White)
    {
        Vector2i king_pos;
        for (unsigned short i = 0; i < 8; i++)
            for (unsigned short j = 0; j < 8; j++)
                if (temp_board[i][j] == Piece::White + Piece::King)
                {
                    king_pos = Vector2i(j, i);
                    break;
                }
        for (unsigned short i = 0; i < 8; i++)
            for (unsigned short j = 0; j < 8; j++)
                if (IsLegal(temp_board, Vector2i(j, i), king_pos, 3) && (temp_board[i][j] & 7) != Piece::EnPassantTrace)
                    return true;
    }
    else if (side == Piece::Black)
    {
        Vector2i king_pos;
        for (unsigned short i = 0; i < 8; i++)
            for (unsigned short j = 0; j < 8; j++)
                if (temp_board[i][j] == Piece::Black + Piece::King)
                {
                    king_pos = Vector2i(j, i);
                    break;
                }
        for (unsigned short i = 0; i < 8; i++)
            for (unsigned short j = 0; j < 8; j++)
                if (IsLegal(temp_board, Vector2i(j, i), king_pos, 3) && (temp_board[i][j] & 7) != Piece::EnPassantTrace)
                    return true;
    }
    return false;
}

bool Engine::IsAttackedBy(const unsigned short side, const Vector2i &pos)
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board_to_check[i][j] & side)
                if (IsLegal(board_to_check, Vector2i(j, i), pos, 3) && (board_to_check[i][j] & 7) != Piece::EnPassantTrace)
                    return true;
    return false;
}

bool Engine::IsChecked(const unsigned short side)
{
    Vector2i king_pos;
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board_to_check[i][j] == side + Piece::King)
            {
                king_pos = Vector2i(j, i);
                break;
            }
    unsigned short opposite_side;
    if (side == Piece::White)
        opposite_side = Piece::Black;
    else if (side == Piece::Black)
        opposite_side = Piece::White;
    return IsAttackedBy(opposite_side, king_pos);
}

bool Engine::HasLegalMoves(const unsigned short side)
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board_to_check[i][j] & side)
                for (unsigned short k = 0; k < 8; k++)
                    for (unsigned short t = 0; t < 8; t++)
                        if (IsLegal(board_to_check, Vector2i(j, i), Vector2i(t, k), 0))
                            return true;

    return false;
}

string Engine::CheckSidePieces(const unsigned short side)
{
    string result = "";
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board_to_check[i][j] & side)
            {
                switch (board_to_check[i][j] & 7)
                {
                case Piece::King:
                    result += "k";
                    break;
                case Piece::Queen:
                    result += "q";
                    break;
                case Piece::Bishop:
                    result += "b";
                    break;
                case Piece::Knight:
                    result += "n";
                    break;
                case Piece::Rook:
                    result += "r";
                    break;
                case Piece::Pawn:
                    result += "p";
                    break;
                default:
                    break;
                }
            }
    return result;
}

bool Engine::SelectSquare(const Vector2i &pos)
{
    const unsigned short piece_id = board_to_check[pos.y][pos.x];
    return piece_id != 0 && ((turn % 2 == 0 && (piece_id & 8)) || (turn % 2 && (piece_id & 16)));
}

void Engine::FENToBoard(string FEN)
{
    unsigned short x_pos = 0;
    unsigned short y_pos = 0;
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            board_to_check[i][j] = 0;
    for (size_t i = 0; i < FEN.size(); i++)
    {
        switch (FEN[i])
        {
        case 'K':
        {
            board_to_check[y_pos][x_pos] = Piece::White + Piece::King;
            x_pos++;
            break;
        }
        case 'Q':
        {
            board_to_check[y_pos][x_pos] = Piece::White + Piece::Queen;
            x_pos++;
            break;
        }
        case 'B':
        {
            board_to_check[y_pos][x_pos] = Piece::White + Piece::Bishop;
            x_pos++;
            break;
        }
        case 'N':
        {
            board_to_check[y_pos][x_pos] = Piece::White + Piece::Knight;
            x_pos++;
            break;
        }
        case 'R':
        {
            board_to_check[y_pos][x_pos] = Piece::White + Piece::Rook;
            x_pos++;
            break;
        }
        case 'P':
        {
            board_to_check[y_pos][x_pos] = Piece::White + Piece::Pawn;
            x_pos++;
            break;
        }
        case 'k':
        {
            board_to_check[y_pos][x_pos] = Piece::Black + Piece::King;
            x_pos++;
            break;
        }
        case 'q':
        {
            board_to_check[y_pos][x_pos] = Piece::Black + Piece::Queen;
            x_pos++;
            break;
        }
        case 'b':
        {
            board_to_check[y_pos][x_pos] = Piece::Black + Piece::Bishop;
            x_pos++;
            break;
        }
        case 'n':
        {
            board_to_check[y_pos][x_pos] = Piece::Black + Piece::Knight;
            x_pos++;
            break;
        }
        case 'r':
        {
            board_to_check[y_pos][x_pos] = Piece::Black + Piece::Rook;
            x_pos++;
            break;
        }
        case 'p':
        {
            board_to_check[y_pos][x_pos] = Piece::Black + Piece::Pawn;
            x_pos++;
            break;
        }
        default:
        {
            if (FEN[i] >= '0' && FEN[i] <= '9')
                x_pos += FEN[i] - '0';
            break;
        }
        }
        if (x_pos >= 8)
        {
            x_pos = 0;
            y_pos++;
        }
    }
}

void Engine::DeSelectSquare()
{
    // deselect square
}

void Engine::HighlightLegalMoves(const Vector2i &pos)
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (IsLegal(board_to_check, pos, Vector2i(j, i), 0))
                highlighted_sq[i][j] = 1;
            else
                highlighted_sq[i][j] = 0;
}

void Engine::DeHighlightLegalMoves()
{
    // dehightlight square
}

void Engine::CheckEndConditions()
{
    turn_swapped = false;
    unsigned short side;
    if (turn % 2 == 0)
        side = Piece::White;
    else
        side = Piece::Black;

    // stalemate and checkmate
    if (!HasLegalMoves(side))
    {
        game_running = false;
        if (IsChecked(side))
        {
            if (side == Piece::White)
                game_result = 3;
            else
                game_result = 2;
        }
        else
            game_result = 0;
    }
    // fifty move rule
    if (fifty_move_rule == 50)
    {
        game_running = false;
        game_result = 1;
    }
    // Insufficient material
    const string black_pieces = CheckSidePieces(Piece::Black);
    const string white_pieces = CheckSidePieces(Piece::White);
    const unsigned short black_size = black_pieces.size();
    const unsigned short white_size = white_pieces.size();
    if (black_size == white_size && white_size == 1)
    {
        game_running = false;
        game_result = 0;
    }
    else if (black_size == 1 && white_size == 2)
    {
        if (white_pieces.find('n') || white_pieces.find('b'))
        {
            game_running = false;
            game_result = 0;
        }
    }
    else if (black_size == 2 && white_size == 1)
    {
        if (black_pieces.find('n') || black_pieces.find('b'))
        {
            game_running = false;
            game_result = 0;
        }
    }
    // threefold
    if (game_past_possitions.GetFrecuency(BoardToFEN()) == 3)
    {
        game_running = false;
        game_result = 0;
    }
}

bool Engine::MakeMove(const Vector2i &start_pos, const Vector2i &end_pos)
{
    if (IsLegal(board_to_check, start_pos, end_pos, 0))
    {
        turn_swapped = true;
        fifty_move_rule++;
        const unsigned short start_x = start_pos.x;
        const unsigned short start_y = start_pos.y;
        const unsigned short end_x = end_pos.x;
        const unsigned short end_y = end_pos.y;
        if (board_to_check[end_y][end_x])
            fifty_move_rule = 0;
        if ((board_to_check[start_y][start_x] & 7) == Piece::Pawn)
        {
            fifty_move_rule = 0;
            if (board_to_check[start_y][start_x] & Piece::White)
            {
                if (start_y == 6 && start_y - end_y == 2 && !board_to_check[start_y - 1][end_x])
                {
                    RemoveEnPassantTrace(Piece::White);
                    SetEnPassantTrace(Vector2i(end_x, start_y - 1), Piece::White);
                }
            }
            else if (board_to_check[start_y][start_x] & Piece::Black)
            {
                if (start_y == 1 && end_y - start_y == 2 && !board_to_check[start_y + 1][end_x])
                {
                    RemoveEnPassantTrace(Piece::Black);
                    SetEnPassantTrace(Vector2i(end_x, start_y + 1), Piece::Black);
                }
            }

            if (board_to_check[start_y][start_x] & Piece::White)
            {
                if (board_to_check[end_y][end_x] == Piece::EnPassantTrace + Piece::Black)
                    board_to_check[end_y + 1][end_x] = 0;
            }
            else if (board_to_check[start_y][start_x] & Piece::Black)
            {
                if (board_to_check[end_y][end_x] == Piece::EnPassantTrace + Piece::White)
                    board_to_check[end_y - 1][end_x] = 0;
            }

            if (end_y == 7 || end_y == 0)
                pawn_needs_promotion = end_y;
        }
        else if ((board_to_check[start_y][start_x] & 7) == Piece::King)
        {
            if (board_to_check[start_y][start_x] & Piece::White)
            {
                white_castling_rights = 0;
            }
            else if (board_to_check[start_y][start_x] & Piece::Black)
            {
                black_castling_rights = 0;
            }
            if (end_x == start_x - 2)
            {
                MovePiece(Vector2i(start_x - 4, start_y), Vector2i(start_x - 1, start_y));
            }
            else if (end_x == start_x + 2)
            {
                MovePiece(Vector2i(start_x + 3, start_y), Vector2i(start_x + 1, start_y));
            }
        }
        else if ((board_to_check[start_y][start_x] & 7) == Piece::Rook)
        {
            if (board_to_check[start_y][start_x] & Piece::White)
            {
                if (start_y == 7)
                {
                    if (start_x == 0)
                    {
                        white_castling_rights &= 1;
                    }
                    else if (start_x == 7)
                    {
                        white_castling_rights &= 2;
                    }
                }
            }
            else if (board_to_check[start_y][start_x] & Piece::Black)
            {
                if (start_y == 0)
                {
                    if (start_x == 0)
                    {
                        black_castling_rights &= 1;
                    }
                    else if (start_x == 7)
                    {
                        black_castling_rights &= 2;
                    }
                }
            }
        }

        if (white_castling_rights && end_y == 7)
        {
            if (end_x == 0)
                white_castling_rights &= 1;
            else if (end_x == 7)
                white_castling_rights &= 2;
        }
        if (black_castling_rights && end_y == 0)
        {
            if (end_x == 0)
                black_castling_rights &= 1;
            else if (end_x == 7)
                black_castling_rights &= 2;
        }

        MovePiece(start_pos, end_pos);
        if (board_to_check[end_y][end_x] & 8)
            RemoveEnPassantTrace(Piece::Black);
        else if (board_to_check[end_y][end_x] & 16)
            RemoveEnPassantTrace(Piece::White);
        game_past_possitions.Insert(BoardToFEN());
        turn++;
        return true;
    }
    return false;
}

bool Engine::IsLegal(unsigned short (&board_to_check)[8][8], const Vector2i &start_pos, const Vector2i &end_pos, const int options)
{
    const unsigned short start_x = start_pos.x;
    const unsigned short start_y = start_pos.y;
    const unsigned short end_x = end_pos.x;
    const unsigned short end_y = end_pos.y;

    const unsigned short start_sq_id = board_to_check[start_y][start_x];
    const unsigned short end_sq_id = board_to_check[end_y][end_x];
    if (start_sq_id == 0)
        return false;
    if (start_x == end_x && start_y == end_y) // verify if it is on the same square
        return false;
    if (!((turn % 2 == 0 && (start_sq_id & 8)) || (turn % 2 == 1 && (start_sq_id & 16))) && !(options & 1)) // verify turn
        return false;
    if (((start_sq_id & 8) && (end_sq_id & 8) && ((end_sq_id & 7) != Piece::EnPassantTrace || start_sq_id == Piece::Pawn + 8)) ||
        ((start_sq_id & 16) && (end_sq_id & 16) && ((end_sq_id & 7) != Piece::EnPassantTrace || start_sq_id == Piece::Pawn + 16))) // verify if we want to move on a piece of ours
        return false;
    if (!(options & 2) && MoveResultsInCheck(start_pos, end_pos))
        return false;
    switch (start_sq_id & 7)
    {
    case 1: // King
    {
        if (abs(short(start_y - end_y)) > 1)
            return false;
        bool able_to_castle = false;
        if (abs(short(start_x - end_x)) == 2)
        {
            if ((board_to_check[start_y][start_x] & Piece::White) && end_y == 7)
            {
                if (end_x < start_x && (white_castling_rights & 2))
                {
                    if (!IsAttackedBy(Piece::Black, Vector2i(start_x - 1, start_y)) && !IsAttackedBy(Piece::Black, Vector2i(start_x - 2, start_y)))
                    {
                        if (!board_to_check[start_y][start_x - 1] && !board_to_check[start_y][start_x - 2] && !board_to_check[start_y][start_x - 3])
                            able_to_castle = true;
                    }
                }
                else if (end_x > start_x && (white_castling_rights & 1))
                {
                    if (!IsAttackedBy(Piece::Black, Vector2i(start_x + 1, start_y)) && !IsAttackedBy(Piece::Black, Vector2i(start_x + 2, start_y)))
                    {
                        if (!board_to_check[start_y][start_x + 1] && !board_to_check[start_y][start_x + 2])
                            able_to_castle = true;
                    }
                }
            }
            else if ((board_to_check[start_y][start_x] & Piece::Black) && end_y == 0)
            {
                if (end_x < start_x && (black_castling_rights & 2))
                {
                    if (!IsAttackedBy(Piece::White, Vector2i(start_x - 1, start_y)) && !IsAttackedBy(Piece::White, Vector2i(start_x - 2, start_y)))
                    {
                        if (!board_to_check[start_y][start_x - 1] && !board_to_check[start_y][start_x - 2] && !board_to_check[start_y][start_x - 3])
                            able_to_castle = true;
                    }
                }
                else if (end_x > start_x && (black_castling_rights & 1))
                {
                    if (!IsAttackedBy(Piece::White, Vector2i(start_x + 1, start_y)) && !IsAttackedBy(Piece::White, Vector2i(start_x + 2, start_y)))
                    {
                        if (!board_to_check[start_y][start_x + 1] && !board_to_check[start_y][start_x + 2])
                            able_to_castle = true;
                    }
                }
            }
        }
        if (abs(int(start_x - end_x)) > 1 && !able_to_castle)
            return false;
        break;
    }
    case 2: // Queen
    {
        if (!CheckMoveRook(board_to_check, start_pos, end_pos) && !CheckMoveBishop(board_to_check, start_pos, end_pos))
            return false;
        break;
    }
    case 3: // Bishop
    {
        if (!CheckMoveBishop(board_to_check, start_pos, end_pos))
            return false;
        break;
    }
    case 4: // Knight
    {
        const short dx[] = {-1, 1, 2, 2, 1, -1, -2, -2};
        const short dy[] = {2, 2, 1, -1, -2, -2, -1, 1};
        bool legal = false;
        for (int i = 0; i < 8; i++)
        {
            if (end_x == start_x + dx[i] && end_y == start_y + dy[i])
            {
                if (end_x < 0 || end_x >= 8 || end_y < 0 || end_y >= 8)
                    return false;
                legal = true;
            }
        }
        if (!legal)
            return false;
        break;
    }
    case 5: // Rook
    {
        if (!CheckMoveRook(board_to_check, start_pos, end_pos))
            return false;
        break;
    }
    case 6: // Pawn
    {
        if (start_sq_id & 8)
        {
            if (end_y > start_y)
                return false;
            if (start_y == 6 && start_y - end_y > 2)
                return false;
            if (start_y == 6 && start_y - end_y == 2 && board_to_check[start_y - 1][end_x])
                return false;
            if (start_y != 6 && start_y - end_y > 1)
                return false;
            if (start_x != end_x)
            {
                if (end_y != start_y - 1)
                    return false;
                if (end_x == start_x - 1 || end_x == start_x + 1)
                {
                    if (!board_to_check[end_y][end_x])
                        return false;
                }
                else
                    return false;
            }
            else
            {
                if (board_to_check[end_y][end_x])
                    return false;
            }
        }
        else if (start_sq_id & 16)
        {
            if (end_y < start_y)
                return false;
            if (start_y == 1 && end_y - start_y > 2)
                return false;
            if (start_y == 1 && end_y - start_y == 2 && board_to_check[start_y + 1][end_x])
                return false;
            if (start_y != 1 && end_y - start_y > 1)
                return false;
            if (start_x != end_x)
            {
                if (end_y != start_y + 1)
                    return false;
                if (end_x == start_x - 1 || end_x == start_x + 1)
                {
                    if (!board_to_check[end_y][end_x])
                        return false;
                }
                else
                    return false;
            }
            else
            {
                if (board_to_check[end_y][end_x])
                    return false;
            }
        }
        break;
    }
    default:
        break;
    }
    return true;
}

bool Engine::GameRunning()
{
    return game_running;
}

bool Engine::PawnNeedsPromotion()
{
    return pawn_needs_promotion != 8;
}

void Engine::PromotePawn(const unsigned short &id)
{
    if (id == Piece::King || id == Piece::Pawn)
        return;
    int x_pos;
    for (x_pos = 0; x_pos < 8; x_pos++)
        if ((board_to_check[pawn_needs_promotion][x_pos] & 7) == Piece::Pawn)
            break;
    if (pawn_needs_promotion == 0)
        board_to_check[pawn_needs_promotion][x_pos] = Piece::White + id;
    else if (pawn_needs_promotion == 7)
        board_to_check[pawn_needs_promotion][x_pos] = Piece::Black + id;
    pawn_needs_promotion = 8;
    // board->UpdateBoard();
    CheckEndConditions();
}

unsigned short Engine::GetGameResult()
{
    return game_result;
}

unsigned short Engine::TurnSwapped()
{
    return turn_swapped;
}

void Engine::GetHighlightedArray(bool (&array)[8][8])
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            array[i][j] = highlighted_sq[i][j];
}

string Engine::BoardToFEN()
{
    string raw_pos = "";
    for (unsigned short i = 0; i < 8; i++)
    {
        for (unsigned short j = 0; j < 8; j++)
        {
            char piece = '1';
            switch (board_to_check[i][j] & 7)
            {
            case Piece::King:
                piece = 'k';
                break;
            case Piece::Queen:
                piece = 'q';
                break;
            case Piece::Bishop:
                piece = 'b';
                break;
            case Piece::Knight:
                piece = 'n';
                break;
            case Piece::Rook:
                piece = 'r';
                break;
            case Piece::Pawn:
                piece = 'p';
                break;
            default:
                break;
            }
            if (board_to_check[i][j] & Piece::White && (board_to_check[i][j] & 7) != Piece::EnPassantTrace)
                piece = _toupper(piece);
            raw_pos += piece;
        }
        raw_pos += '/';
    }
    string pos = "";
    unsigned short counter = 0;
    for (size_t i = 0; i < raw_pos.size(); i++)
    {
        if (raw_pos[i] != '1')
        {
            if (counter != 0)
                pos += '0' + counter;
            pos += raw_pos[i];
            counter = 0;
        }
        else
            counter++;
    }
    return pos;
}

void Engine::RemoveEnPassantTrace(unsigned short side)
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board_to_check[i][j] == Piece::EnPassantTrace + side)
                board_to_check[i][j] = 0;
}

void Engine::SetEnPassantTrace(const Vector2i &pos, const unsigned short side)
{
    board_to_check[pos.y][pos.x] = Piece::EnPassantTrace + side;
}

void Engine::MovePiece(const Vector2i &start_pos, const Vector2i &end_pos)
{
    board_to_check[end_pos.y][end_pos.x] = board_to_check[start_pos.y][start_pos.x];
    board_to_check[start_pos.y][start_pos.x] = 0;
}
