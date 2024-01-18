#include "Board.hpp"

void Board::MovePiece(const Vector2i &start_pos, const Vector2i &end_pos)
{

    const unsigned short start_x = start_pos.x;
    const unsigned short start_y = start_pos.y;
    const unsigned short end_x = end_pos.x;
    const unsigned short end_y = end_pos.y;
    PlaySound(start_pos, end_pos);
    if ((board[start_y][start_x] & Piece::Pawn) == Piece::Pawn)
    {
        if ((board[start_y][start_x] & Piece::Black) && (end_y == 7))
        {
            need_promote_b = end_x;
            pawn_needs_promotion = 7;
        }
        if ((board[start_y][start_x] & Piece::White) && (end_y == 0))
        {
            need_promote_w = end_x;
            pawn_needs_promotion = 0;
        }
    }
    // for (int i = 0; i < 8; i++)
    // {
    //     for (int j = 0; j < 8; j++)
    //     {
    //         cout << board[i][j] << ' ';
    //         if (board[i][j] / 10 == 0)
    //             cout << ' ';
    //     }
    //     cout << '\n';
    // }
    board[end_y][end_x] = board[start_y][start_x];
    board[start_y][start_x] = 0;
    UpdateBoard();
}

void Board::PlaySound(const Vector2i &start_pos, const Vector2i &end_pos)
{
    const unsigned short start_x = start_pos.x;
    const unsigned short start_y = start_pos.y;
    const unsigned short end_x = end_pos.x;
    const unsigned short end_y = end_pos.y;
    if (board[end_y][end_x])
    {
        if ((board[end_y][end_x] & 7) != Piece::EnPassantTrace)
            sound[1].play(); // capture
        else if ((board[start_y][start_x] & 7) == 6)
            sound[1].play(); // capture
        else
            sound[0].play(); // move
    }
    else
        sound[0].play(); // move
}

void Board::SelectSquare(const Vector2i &pos)
{
    selected_square.setFillColor(SELECTED_SQ_COLOR);
    selected_square.setPosition(Vector2f(board_pos.x + pos.x * SQUARE_SIZE,
                                         board_pos.y + pos.y * SQUARE_SIZE));
}

void Board::DeSelectSquare()
{
    selected_square.setFillColor(Color::Transparent);
}

void Board::HighlightSquare(const unsigned short x, const unsigned short y)
{
    highlighted_square[y][x].setFillColor(LEGAL_MOVE);
}

void Board::DeHighlightSquare()
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            highlighted_square[i][j].setFillColor(Color::Transparent);
}

void Board::SetEnPassantTrace(const Vector2i &pos, const unsigned short side)
{
    board[(unsigned short)pos.y][(unsigned short)pos.x] = Piece::EnPassantTrace + side;
}

void Board::RemoveEnPassantTrace(unsigned short side)
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board[i][j] == Piece::EnPassantTrace + side)
                board[i][j] = 0;
}

void Board::UpdateBoard()
{
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            if (board[i][j] != 0 && board[i][j] != Piece::EnPassantTrace + Piece::White && board[i][j] != Piece::EnPassantTrace + Piece::Black)
            {
                pieces[i][j].setTexture(&pieces_texture.at(board[i][j]));
                pieces[i][j].setPosition(Vector2f(board_pos.x + j * SQUARE_SIZE,
                                                  board_pos.y + i * SQUARE_SIZE));
                pieces[i][j].setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            }
            else
                pieces[i][j] = RectangleShape();
    selected_square.setFillColor(Color::Transparent);
    if (need_promote_w < 8)
    {
        promote_bg.setPosition(Vector2f(need_promote_w * SQUARE_SIZE + board_pos.x, board_pos.y));
        promote_bg.setFillColor(PROMOTE_BG);

        promote_Q.setPosition(Vector2f(need_promote_w * SQUARE_SIZE + board_pos.x, board_pos.y));
        promote_Q.setTexture(&pieces_texture.at(Piece::White + Piece::Queen));

        promote_R.setPosition(Vector2f(need_promote_w * SQUARE_SIZE + board_pos.x, 1 * SQUARE_SIZE + board_pos.y));
        promote_R.setTexture(&pieces_texture.at(Piece::White + Piece::Rook));

        promote_N.setPosition(Vector2f(need_promote_w * SQUARE_SIZE + board_pos.x, 2 * SQUARE_SIZE + board_pos.y));
        promote_N.setTexture(&pieces_texture.at(Piece::White + Piece::Knight));

        promote_B.setPosition(Vector2f(need_promote_w * SQUARE_SIZE + board_pos.x, 3 * SQUARE_SIZE + board_pos.y));
        promote_B.setTexture(&pieces_texture.at(Piece::White + Piece::Bishop));
    }
    if (need_promote_b < 8)
    {
        const float temp_pos_y = board_pos.y + board_background.getSize().y - 4 * SQUARE_SIZE;
        promote_bg.setPosition(Vector2f(need_promote_b * SQUARE_SIZE + board_pos.x, temp_pos_y));
        promote_bg.setFillColor(PROMOTE_BG);

        promote_Q.setPosition(Vector2f(need_promote_b * SQUARE_SIZE + board_pos.x, temp_pos_y));
        promote_Q.setTexture(&pieces_texture.at(Piece::Black + Piece::Queen));

        promote_R.setPosition(Vector2f(need_promote_b * SQUARE_SIZE + board_pos.x, 1 * SQUARE_SIZE + temp_pos_y));
        promote_R.setTexture(&pieces_texture.at(Piece::Black + Piece::Rook));

        promote_N.setPosition(Vector2f(need_promote_b * SQUARE_SIZE + board_pos.x, 2 * SQUARE_SIZE + temp_pos_y));
        promote_N.setTexture(&pieces_texture.at(Piece::Black + Piece::Knight));

        promote_B.setPosition(Vector2f(need_promote_b * SQUARE_SIZE + board_pos.x, 3 * SQUARE_SIZE + temp_pos_y));
        promote_B.setTexture(&pieces_texture.at(Piece::Black + Piece::Bishop));
    }
}

void Board::PromotePawn(const unsigned short &id)
{
    if (id == Piece::King || id == Piece::Pawn)
        return;
    int x_pos;
    for (x_pos = 0; x_pos < 8; x_pos++)
        if ((board[pawn_needs_promotion][x_pos] & 7) == Piece::Pawn)
            break;
    if (pawn_needs_promotion == 0)
        board[pawn_needs_promotion][x_pos] = Piece::White + id;
    else if (pawn_needs_promotion == 7)
        board[pawn_needs_promotion][x_pos] = Piece::Black + id;
    need_promote_b = 8;
    need_promote_w = 8;
    pawn_needs_promotion = 8;
    UpdateBoard();
}

Board::Board(const String FEN_board, Vector2f board_pos)
{
    need_promote_b = 8;
    need_promote_w = 8;
    pawn_needs_promotion = 8;
    this->board_pos = board_pos;
    for (unsigned short side : {Piece::White, Piece::Black})
    {
        string first;
        if (side == Piece::White)
            first = "w";
        else
            first = "b";
        for (unsigned short type : {Piece::King, Piece::Queen, Piece::Bishop, Piece::Knight, Piece::Rook, Piece::Pawn})
        {
            string second;
            switch (type)
            {
            case Piece::King:
                second = "K.png";
                break;
            case Piece::Queen:
                second = "Q.png";
                break;
            case Piece::Bishop:
                second = "B.png";
                break;
            case Piece::Knight:
                second = "N.png";
                break;
            case Piece::Rook:
                second = "R.png";
                break;
            case Piece::Pawn:
                second = "P.png";
                break;
            default:
                break;
            }
            pieces_texture[side + type] = Texture();
            pieces_texture[side + type].loadFromFile(string("assets/board_assets/pieces/" + first + second));
            pieces_texture[side + type].setSmooth(true);
        }
    }

    pieces_texture.insert(pair(Piece::Black + Piece::Pawn, Texture()));
    unsigned short x_pos = 0;
    unsigned short y_pos = 0;
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            board[i][j] = 0;
    for (size_t i = 0; i < FEN_board.getSize(); i++)
    {
        switch (FEN_board[i])
        {
        case 'K':
        {
            board[y_pos][x_pos] = Piece::White + Piece::King;
            x_pos++;
            break;
        }
        case 'Q':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Queen;
            x_pos++;
            break;
        }
        case 'B':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Bishop;
            x_pos++;
            break;
        }
        case 'N':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Knight;
            x_pos++;
            break;
        }
        case 'R':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Rook;
            x_pos++;
            break;
        }
        case 'P':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Pawn;
            x_pos++;
            break;
        }
        case 'k':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::King;
            x_pos++;
            break;
        }
        case 'q':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Queen;
            x_pos++;
            break;
        }
        case 'b':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Bishop;
            x_pos++;
            break;
        }
        case 'n':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Knight;
            x_pos++;
            break;
        }
        case 'r':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Rook;
            x_pos++;
            break;
        }
        case 'p':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Pawn;
            x_pos++;
            break;
        }
        default:
        {
            if (FEN_board[i] >= '0' && FEN_board[i] <= '9')
                x_pos += FEN_board[i] - '0';
            break;
        }
        }
        if (x_pos >= 8)
        {
            x_pos = 0;
            y_pos++;
        }
    }

    for (unsigned short i = 0; i < 8; i++)
    {
        for (unsigned short j = 0; j < 8; j++)
        {
            if (board[i][j] != 0 && board[i][j] != Piece::EnPassantTrace + Piece::White && board[i][j] != Piece::EnPassantTrace + Piece::Black)
            {
                pieces[i][j].setTexture(&pieces_texture.at(board[i][j]));
                pieces[i][j].setPosition(Vector2f(board_pos.x + j * SQUARE_SIZE,
                                                  board_pos.y + i * SQUARE_SIZE));
                pieces[i][j].setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            }
            highlighted_square[i][j].setPosition(Vector2f(board_pos.x + j * SQUARE_SIZE,
                                                          board_pos.y + i * SQUARE_SIZE));
            highlighted_square[i][j].setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            highlighted_square[i][j].setFillColor(Color::Transparent);
        }
    }
    if (!board_background_texture.loadFromFile("assets/board_assets/Board.png"))
    {
        perror("LoadFromFile");
        exit(errno);
    }
    if (!buffer[0].loadFromFile("assets/board_assets/sounds/Move.mp3"))
    {
        perror("LoadFromFile");
        exit(errno);
    }
    sound[0].setBuffer(buffer[0]);
    if (!buffer[1].loadFromFile("assets/board_assets/sounds/Capture.mp3"))
    {
        perror("LoadFromFile");
        exit(errno);
    }
    sound[1].setBuffer(buffer[1]);
    board_background.setTexture(&board_background_texture);
    board_background.setSize(Vector2f(BOARD_WIDTH, BOARD_HEIGHT));
    board_background.setPosition(board_pos);
    selected_square.setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    selected_square.setFillColor(Color::Transparent);

    promote_bg.setSize(Vector2f(SQUARE_SIZE, 4 * SQUARE_SIZE));
    promote_Q.setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    promote_R.setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    promote_N.setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    promote_B.setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
}

string Board::BoardToFEN()
{
    string raw_pos = "";
    for (unsigned short i = 0; i < 8; i++)
    {
        for (unsigned short j = 0; j < 8; j++)
        {
            char piece = '1';
            switch (board[i][j] & 7)
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
            if (board[i][j] & Piece::White && (board[i][j] & 7) != Piece::EnPassantTrace)
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

void Board::FENToBoard(string FEN)
{
    unsigned short x_pos = 0;
    unsigned short y_pos = 0;
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            board[i][j] = 0;
    for (size_t i = 0; i < FEN.size(); i++)
    {
        switch (FEN[i])
        {
        case 'K':
        {
            board[y_pos][x_pos] = Piece::White + Piece::King;
            x_pos++;
            break;
        }
        case 'Q':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Queen;
            x_pos++;
            break;
        }
        case 'B':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Bishop;
            x_pos++;
            break;
        }
        case 'N':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Knight;
            x_pos++;
            break;
        }
        case 'R':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Rook;
            x_pos++;
            break;
        }
        case 'P':
        {
            board[y_pos][x_pos] = Piece::White + Piece::Pawn;
            x_pos++;
            break;
        }
        case 'k':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::King;
            x_pos++;
            break;
        }
        case 'q':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Queen;
            x_pos++;
            break;
        }
        case 'b':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Bishop;
            x_pos++;
            break;
        }
        case 'n':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Knight;
            x_pos++;
            break;
        }
        case 'r':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Rook;
            x_pos++;
            break;
        }
        case 'p':
        {
            board[y_pos][x_pos] = Piece::Black + Piece::Pawn;
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
    UpdateBoard();
}

void Board::SetPosition(Vector2f pos)
{
    board_pos = pos;
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
            pieces[i][j].setPosition(Vector2f(board_pos.x + j * SQUARE_SIZE,
                                              board_pos.y + i * SQUARE_SIZE));
    board_background.setPosition(board_pos);
}

bool Board::Contains(const Vector2f mouse_pos)
{
    bool temp = mouse_pos.x >= board_pos.x && mouse_pos.y >= board_pos.y &&
                mouse_pos.x < board_pos.x + board_background.getSize().x &&
                mouse_pos.y < board_pos.y + board_background.getSize().y;
    return temp;
}

bool Board::ContainsPromBox(Vector2f mouse_pos)
{
    Vector2f start = promote_bg.getPosition();
    Vector2f end = start + promote_bg.getSize();
    bool temp = mouse_pos.x >= start.x && mouse_pos.y >= start.y &&
                mouse_pos.x < start.x + end.x &&
                mouse_pos.y < start.y + end.y;
    return temp;
}

void Board::TryCastling(const Vector2i &start_pos, const Vector2i &end_pos)
{
    if ((board[start_pos.y][start_pos.x] & 7) == Piece::King)
    {
        if (end_pos.x == start_pos.x - 2)
        {
            MovePiece(Vector2i(start_pos.x - 4, start_pos.y), Vector2i(start_pos.x - 1, start_pos.y));
        }
        else if (end_pos.x == start_pos.x + 2)
        {
            MovePiece(Vector2i(start_pos.x + 3, start_pos.y), Vector2i(start_pos.x + 1, start_pos.y));
        }
    }
}

void Board::TryEnPassant(const Vector2i &start_pos, const Vector2i &end_pos)
{
    if ((board[start_pos.y][start_pos.x] & 7) == Piece::Pawn)
    {
        if (board[start_pos.y][start_pos.x] & Piece::White)
        {
            RemoveEnPassantTrace(Piece::White);
            if (start_pos.y == 6 && start_pos.y - end_pos.y == 2 && !board[start_pos.y - 1][end_pos.x])
            {
                SetEnPassantTrace(Vector2i(end_pos.x, start_pos.y - 1), Piece::White);
            }
        }
        else if (board[start_pos.y][start_pos.x] & Piece::Black)
        {
            RemoveEnPassantTrace(Piece::Black);
            if (start_pos.y == 1 && end_pos.y - start_pos.y == 2 && !board[start_pos.y + 1][end_pos.x])
            {
                SetEnPassantTrace(Vector2i(end_pos.x, start_pos.y + 1), Piece::Black);
            }
        }

        if (board[start_pos.y][start_pos.x] & Piece::White)
        {
            if (board[end_pos.y][end_pos.x] == Piece::EnPassantTrace + Piece::Black)
                board[end_pos.y + 1][end_pos.x] = 0;
        }
        else if (board[start_pos.y][start_pos.x] & Piece::Black)
        {
            if (board[end_pos.y][end_pos.x] == Piece::EnPassantTrace + Piece::White)
                board[end_pos.y - 1][end_pos.x] = 0;
        }
    }
}

Vector2f Board::GetPosition()
{
    return board_pos;
}

Vector2f Board::GetEndPosition()
{
    return board_pos + board_background.getSize();
}

unsigned int Board::GetChoice(Vector2f mouse_pos)
{
    unsigned int start = promote_bg.getPosition().y;
    if (mouse_pos.y >= start && mouse_pos.y < start + SQUARE_SIZE)
        return Piece::Queen;
    if (mouse_pos.y >= start + 1 * SQUARE_SIZE && mouse_pos.y < start + 2 * SQUARE_SIZE)
        return Piece::Rook;
    if (mouse_pos.y >= start + 2 * SQUARE_SIZE && mouse_pos.y < start + 3 * SQUARE_SIZE)
        return Piece::Knight;
    if (mouse_pos.y >= start + 3 * SQUARE_SIZE && mouse_pos.y < start + 4 * SQUARE_SIZE)
        return Piece::Bishop;
    return 0;
}

void Board::Draw(RenderWindow &window)
{
    window.draw(board_background);
    window.draw(selected_square);
    for (unsigned short i = 0; i < 8; i++)
        for (unsigned short j = 0; j < 8; j++)
        {
            window.draw(highlighted_square[i][j]);
            window.draw(pieces[i][j]);
        }
    if (need_promote_b < 8 || need_promote_w < 8)
    {
        /*
            problem: when in a match with someone if he/she wants to
            promote a pawn, the promotion box shows on both clients
        */
        window.draw(promote_bg);
        window.draw(promote_Q);
        window.draw(promote_R);
        window.draw(promote_N);
        window.draw(promote_B);
    }
}
