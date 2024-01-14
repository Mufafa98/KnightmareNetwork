#include "PlayScreen.hpp"

PlayScreen::PlayScreen()
{
    opponent_found = false;
    turn = 0;
    need_to_promote = false;
    game_ended = false;
    click_count = 0;
    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);
    board = new Board(START_CHESS_POS, Vector2f(0, 0));

    waiting_bg.SetPosition(Vector2f(board->GetEndPosition().x + 10, WINDOW_HEIGHT / 2 - 50));
    waiting_bg.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 100));
    waiting_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    waiting_bg.SetCornerSize(10);

    waiting_text.SetPosition(waiting_bg.GetPosition());
    waiting_text.SetString("Searching opponent.");
    waiting_text.Center(waiting_bg.GetPosition(), waiting_bg.GetSize());
    waiting_animation.restart();

    chat_bg.SetPosition(Vector2f(board->GetEndPosition().x + 10, 10));
    chat_bg.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 6 * (WINDOW_HEIGHT - 45) / 10.0));
    chat_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    chat_bg.SetCornerSize(10);

    opponent_color.setPosition(chat_bg.GetPosition() + Vector2f(10, 20));
    opponent_color.setRadius(10);
    opponent_color.setFillColor(Color::Black);

    opponent_username.SetPosition(opponent_color.getPosition() +
                                  Vector2f(2 * opponent_color.getRadius() + 10, -opponent_color.getRadius() + 5));
    opponent_username.SetString("Username");

    chat_buffer = "";

    chat_text.SetPosition(opponent_color.getPosition() + Vector2f(10, 30));
    chat_text.SetString("");

    message_bg.SetPosition(Vector2f(chat_bg.GetPosition().x + 10, chat_bg.GetEndPosition().y - 10 - 50));
    message_bg.SetSize(Vector2f(chat_bg.GetSize().x - 30 - 50, 50));
    message_bg.SetFillColor(THIRD_BACKGROUND_COLOR);
    message_bg.SetCornerSize(10);

    message_text.SetString("Message");
    message_text.SetPosition(message_bg.GetPosition() + Vector2f(10, 10));
    message_text.SetFillColor(TEXT_FONT_COLOR);
    message_text.SetType(InputBox::Type::Message);

    send_message.SetSize(Vector2f(50, 45));
    send_message.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    send_message.SetPosition(Vector2f(message_bg.GetEndPosition().x + 10, message_bg.GetPosition().y));
    send_message.SetCornerSize(5);

    send_message_shade.SetSize(Vector2f(50, 50));
    send_message_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    send_message_shade.SetPosition(send_message.GetPosition());
    send_message_shade.SetCornerSize(5);

    send_message_text.SetString(">");
    send_message_text.SetCharacterSize(40);
    send_message_text.Center(send_message.GetPosition(), send_message.GetSize() - Vector2f(0, 7));

    back_to_menu.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 1 * (WINDOW_HEIGHT - 45) / 10.0));
    back_to_menu.SetFillColor(BUTTON_BACKGROUND_COLOR);
    back_to_menu.SetPosition(Vector2f(chat_bg.GetPosition().x, chat_bg.GetEndPosition().y + 10));
    back_to_menu.SetCornerSize(10);

    back_to_menu_shade.SetSize(back_to_menu.GetSize() + Vector2f(0, 5));
    back_to_menu_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    back_to_menu_shade.SetPosition(back_to_menu.GetPosition());
    back_to_menu_shade.SetCornerSize(10);

    back_to_menu_text.SetString("Resign & Menu");
    back_to_menu_text.SetCharacterSize(40);
    back_to_menu_text.Center(back_to_menu.GetPosition(), back_to_menu.GetSize());

    board_possitions.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 3 * (WINDOW_HEIGHT - 45) / 10.0));
    board_possitions.SetFillColor(SECOND_BACKGROUND_COLOR);
    board_possitions.SetPosition(Vector2f(back_to_menu_shade.GetPosition().x, back_to_menu_shade.GetEndPosition().y + 10));
    board_possitions.SetCornerSize(10);
}

PlayScreen::~PlayScreen()
{
    delete board;
}

void PlayScreen::UpdateOnFileDescriptor(int fd)
{
    waiting_animation.restart();
    this->fd = fd;

    int flags = fcntl(this->fd, F_GETFL, 0);
    fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);
}

void PlayScreen::Animate(RenderWindow &window)
{
    if (!opponent_found)
    {
        unsigned int server_response;
        if (!opponent_found && recv(fd, &server_response, sizeof(server_response), 0) > 0)
        {
            if (server_response == InternalLogic::OpponentFound)
            {
                opponent_found = true;
                int flags = fcntl(fd, F_GETFL, 0);
                fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
                recv(fd, &server_response, sizeof(server_response), 0); // culoarea
                if (!server_response)
                    opponent_color.setFillColor(Color::White);
                char username[20];
                recv(fd, username, sizeof(username), 0);
                opponent_username.SetString(username);
            }
        }
        unsigned int time = waiting_animation.getElapsedTime().asMilliseconds();
        if (time >= 490 && time <= 510)
        {
            waiting_text.SetString("Searching opponent.");
            waiting_text.Center(waiting_bg.GetPosition(), waiting_bg.GetSize());
        }
        else if (time >= 990 && time <= 1010)
        {
            waiting_text.SetString("Searching opponent..");
            waiting_text.Center(waiting_bg.GetPosition(), waiting_bg.GetSize());
        }
        else if (time >= 1490 && time <= 1510)
        {
            waiting_text.SetString("Searching opponent...");
            waiting_text.Center(waiting_bg.GetPosition(), waiting_bg.GetSize());
        }
        else if (time >= 1990 && time <= 2010)
        {
            waiting_text.SetString("Searching opponent....");
            waiting_text.Center(waiting_bg.GetPosition(), waiting_bg.GetSize());
            waiting_animation.restart();
        }
    }
    unsigned int server_response;
    int flags = fcntl(this->fd, F_GETFL, 0);
    fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);
    if (recv(fd, &server_response, sizeof(server_response), MSG_PEEK) > 0)
    {
        flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
        if (server_response == BoardCommands::MovePieceAnimate)
        {
            recv(fd, &server_response, sizeof(server_response), 0);
            int x1, x2, y1, y2;
            recv(fd, &x1, sizeof(x1), 0);
            recv(fd, &y1, sizeof(y1), 0);
            recv(fd, &x2, sizeof(x2), 0);
            recv(fd, &y2, sizeof(y2), 0);
            board->TryEnPassant(Vector2i(x1, y1), Vector2i(x2, y2));
            board->TryCastling(Vector2i(x1, y1), Vector2i(x2, y2));
            board->MovePiece(Vector2i(x1, y1), Vector2i(x2, y2));
            recv(fd, &server_response, sizeof(server_response), 0);
        }
        else if (server_response == BoardCommands::PromotePawnAnimate)
        {
            recv(fd, &server_response, sizeof(server_response), 0);
            unsigned int piece_id;
            recv(fd, &piece_id, sizeof(piece_id), 0);
            board->PromotePawn(piece_id);
        }
        else if (server_response == BoardCommands::GameOver)
        {
            recv(fd, &server_response, sizeof(server_response), 0);
            back_to_menu_text.SetString("Menu");
            back_to_menu_text.Center(back_to_menu.GetPosition(), back_to_menu.GetSize());
            game_ended = true;
        }
        else if (server_response == Commands::Chat)
        {
            recv(fd, &server_response, sizeof(server_response), 0);
            char message[130];
            recv(fd, message, sizeof(message), 0);
            chat_buffer += message;
            chat_buffer += "\n";
            chat_text.SetString(chat_buffer.c_str());
        }
    }
    if (opponent_found)
    {
        flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
    }
    Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));
    if (back_to_menu.Contains(mouse_pos))
        back_to_menu.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.9));
    else
        back_to_menu.SetFillColor(BUTTON_BACKGROUND_COLOR);
    if (send_message.Contains(mouse_pos))
        send_message.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.9));
    else
        send_message.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (back_to_menu.Contains(mouse_pos))
            back_to_menu.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.8));
        else
            back_to_menu.SetFillColor(BUTTON_BACKGROUND_COLOR);
        if (send_message.Contains(mouse_pos))
            send_message.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.8));
        else
            send_message.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    }
}

void PlayScreen::UpdateOnKeyPressed(unsigned int code)
{
    if (message_text.GetSelected())
    {
        message_text.UpdateText(code, 100);
        unsigned int nl_count;
        nl_count = message_text.FitWidth(message_bg.GetPosition().x + 10, message_bg.GetEndPosition().x - 10);
        nl_count++;
        if (nl_count == 1)
        {
            message_bg.SetPosition(Vector2f(message_bg.GetPosition().x, chat_bg.GetEndPosition().y - 10 - nl_count * 50));
            message_bg.SetSize(Vector2f(message_bg.GetSize().x, nl_count * 50));
        }
        else
        {
            message_bg.SetPosition(Vector2f(message_bg.GetPosition().x, chat_bg.GetEndPosition().y - 10 - nl_count * 38));
            message_bg.SetSize(Vector2f(message_bg.GetSize().x, nl_count * 38));
        }
        message_text.SetPosition(message_bg.GetPosition() + Vector2f(10, 10));
    }
}

void PlayScreen::UpdateOnClick(Vector2f mouse_pos)
{
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (message_bg.Contains(mouse_pos))
            message_text.Select();
        else
            message_text.DeSelect();
    }
}

void PlayScreen::Restart()
{
    delete board;
    opponent_found = false;
    turn = 0;
    need_to_promote = false;
    game_ended = false;
    click_count = 0;
    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);
    board = new Board(START_CHESS_POS, Vector2f(0, 0));

    waiting_bg.SetPosition(Vector2f(board->GetEndPosition().x + 10, WINDOW_HEIGHT / 2 - 50));
    waiting_bg.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 100));
    waiting_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    waiting_bg.SetCornerSize(10);

    waiting_text.SetPosition(waiting_bg.GetPosition());
    waiting_text.SetString("Searching opponent.");
    waiting_text.Center(waiting_bg.GetPosition(), waiting_bg.GetSize());
    waiting_animation.restart();

    chat_bg.SetPosition(Vector2f(board->GetEndPosition().x + 10, 10));
    chat_bg.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 6 * (WINDOW_HEIGHT - 45) / 10.0));
    chat_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    chat_bg.SetCornerSize(10);

    opponent_color.setPosition(chat_bg.GetPosition() + Vector2f(10, 20));
    opponent_color.setRadius(10);
    opponent_color.setFillColor(Color::Black);

    opponent_username.SetPosition(opponent_color.getPosition() +
                                  Vector2f(2 * opponent_color.getRadius() + 10, -opponent_color.getRadius() + 5));
    opponent_username.SetString("Username");

    chat_buffer = "";

    chat_text.SetPosition(opponent_color.getPosition() + Vector2f(10, 30));
    chat_text.SetString("");

    message_bg.SetPosition(Vector2f(chat_bg.GetPosition().x + 10, chat_bg.GetEndPosition().y - 10 - 50));
    message_bg.SetSize(Vector2f(chat_bg.GetSize().x - 30 - 50, 50));
    message_bg.SetFillColor(THIRD_BACKGROUND_COLOR);
    message_bg.SetCornerSize(10);

    message_text.SetString("Message");
    message_text.SetPosition(message_bg.GetPosition() + Vector2f(10, 10));
    message_text.SetFillColor(TEXT_FONT_COLOR);
    message_text.SetType(InputBox::Type::Message);

    send_message.SetSize(Vector2f(50, 45));
    send_message.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    send_message.SetPosition(Vector2f(message_bg.GetEndPosition().x + 10, message_bg.GetPosition().y));
    send_message.SetCornerSize(5);

    send_message_shade.SetSize(Vector2f(50, 50));
    send_message_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    send_message_shade.SetPosition(send_message.GetPosition());
    send_message_shade.SetCornerSize(5);

    send_message_text.SetString(">");
    send_message_text.SetCharacterSize(40);
    send_message_text.Center(send_message.GetPosition(), send_message.GetSize() - Vector2f(0, 7));

    back_to_menu.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 1 * (WINDOW_HEIGHT - 45) / 10.0));
    back_to_menu.SetFillColor(BUTTON_BACKGROUND_COLOR);
    back_to_menu.SetPosition(Vector2f(chat_bg.GetPosition().x, chat_bg.GetEndPosition().y + 10));
    back_to_menu.SetCornerSize(10);

    back_to_menu_shade.SetSize(back_to_menu.GetSize() + Vector2f(0, 5));
    back_to_menu_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    back_to_menu_shade.SetPosition(back_to_menu.GetPosition());
    back_to_menu_shade.SetCornerSize(10);

    back_to_menu_text.SetString("Resign & Menu");
    back_to_menu_text.SetCharacterSize(40);
    back_to_menu_text.Center(back_to_menu.GetPosition(), back_to_menu.GetSize());

    board_possitions.SetSize(Vector2f(WINDOW_WIDTH - board->GetEndPosition().x - 20, 3 * (WINDOW_HEIGHT - 45) / 10.0));
    board_possitions.SetFillColor(SECOND_BACKGROUND_COLOR);
    board_possitions.SetPosition(Vector2f(back_to_menu_shade.GetPosition().x, back_to_menu_shade.GetEndPosition().y + 10));
    board_possitions.SetCornerSize(10);
}

unsigned int PlayScreen::LogicOnClick(Vector2f mouse_pos)
{
    if (!opponent_found)
        return ScreenCommand::NoneS;
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (need_to_promote)
        {
            if (board->ContainsPromBox(mouse_pos))
            {
                unsigned int server_response;
                unsigned int choice = board->GetChoice(mouse_pos);
                unsigned int client_command = Commands::BoardCommandMatch;
                send(fd, &client_command, sizeof(client_command), 0);
                recv(fd, &server_response, sizeof(server_response), 0);
                client_command = BoardCommands::PromotePawn;
                send(fd, &client_command, sizeof(client_command), 0);
                board->PromotePawn(choice);
                send(fd, &choice, sizeof(choice), 0);
                need_to_promote = false;
            }
        }
        else if (board->Contains(mouse_pos) && !game_ended)
        {
            int x = (mouse_pos.x - board->GetPosition().x) / 100;
            int y = (mouse_pos.y - board->GetPosition().y) / 100;
            unsigned int client_command = Commands::BoardCommandMatch;
            unsigned int server_response;
            send(fd, &client_command, sizeof(client_command), 0);
            recv(fd, &server_response, sizeof(server_response), 0);
            if (server_response == BoardCommands::YourTurn)
            {
                if (click_count == 0)
                    client_command = BoardCommands::FirstPos;
                else
                    client_command = BoardCommands::SecondPos;
                if (client_command == BoardCommands::FirstPos || client_command == BoardCommands::SecondPos)
                {
                    board->DeSelectSquare();
                    board->DeHighlightSquare();
                    send(fd, &client_command, sizeof(client_command), 0);
                    send(fd, &x, sizeof(x), 0);
                    send(fd, &y, sizeof(y), 0);
                    recv(fd, &server_response, sizeof(server_response), 0);
                    if (client_command == BoardCommands::FirstPos)
                        game.first_pos = Vector2i(x, y);
                    else if (client_command == BoardCommands::SecondPos)
                        game.second_pos = Vector2i(x, y);
                    if (server_response == BoardCommands::SelectSquare)
                    {
                        board->SelectSquare(Vector2i(x, y));
                        bool selected_sq[8][8];
                        recv(fd, selected_sq, sizeof(selected_sq), 0);
                        for (unsigned short i = 0; i < 8; i++)
                            for (unsigned short j = 0; j < 8; j++)
                                if (selected_sq[i][j])
                                    board->HighlightSquare(j, i);
                    }
                    if (server_response == BoardCommands::MovePiece)
                    {
                        int x1, x2, y1, y2;
                        recv(fd, &x1, sizeof(x1), 0);
                        recv(fd, &y1, sizeof(y1), 0);
                        recv(fd, &x2, sizeof(x2), 0);
                        recv(fd, &y2, sizeof(y2), 0);
                        turn++;
                        turn %= 2;
                        board->TryEnPassant(Vector2i(x1, y1), Vector2i(x2, y2));
                        board->TryCastling(Vector2i(x1, y1), Vector2i(x2, y2));
                        board->MovePiece(Vector2i(x1, y1), Vector2i(x2, y2));
                        recv(fd, &server_response, sizeof(server_response), 0);
                        if (server_response == BoardCommands::NeedPromotion)
                        {
                            need_to_promote = true;
                            click_count++;
                            click_count %= 2;
                        }
                        else if (server_response == BoardCommands::GameOver)
                        {
                            back_to_menu_text.SetString("Menu");
                            back_to_menu_text.Center(back_to_menu.GetPosition(), back_to_menu.GetSize());
                            game_ended = true;
                        }
                    }
                    if (server_response == BoardCommands::UnableToMovePiece)
                    {
                        send(fd, &x, sizeof(x), 0);
                        send(fd, &y, sizeof(y), 0);
                        recv(fd, &server_response, sizeof(server_response), 0);
                        if (server_response == BoardCommands::SelectSquare)
                        {
                            board->SelectSquare(Vector2i(x, y));
                            bool selected_sq[8][8];
                            recv(fd, selected_sq, sizeof(selected_sq), 0);
                            for (unsigned short i = 0; i < 8; i++)
                                for (unsigned short j = 0; j < 8; j++)
                                    if (selected_sq[i][j])
                                        board->HighlightSquare(j, i);
                        }
                        click_count++;
                        click_count %= 2;
                    }
                    if (!need_to_promote)
                    {
                        click_count++;
                        click_count %= 2;
                    }
                }
            }
        }
        else if (send_message.Contains(mouse_pos))
        {
            // TO DO: scroll bar for chat and better width fit
            //(don t separate a word in two.. put\n only in ' ')
            unsigned int client_command = Commands::Chat;
            send(fd, &client_command, sizeof(client_command), 0);
            char message[130];
            strcpy(message, message_text.GetString().c_str());
            send(fd, message, sizeof(message), 0);
            recv(fd, message, sizeof(message), 0);
            chat_buffer += message;
            chat_buffer += "\n";
            chat_text.SetString(chat_buffer.c_str());
            message_text.SetString("Message");
            message_text.Reset();
        }
        else if (back_to_menu.Contains(mouse_pos))
        {
            unsigned int client_command = Commands::SaveMatch;
            send(fd, &client_command, sizeof(client_command), 0);
            return ScreenCommand::MenuS;
        }
    }
    return ScreenCommand::NoneS;
}

void PlayScreen::Draw(RenderWindow &window)
{
    window.draw(screen_background);

    if (!opponent_found)
    {
        waiting_bg.Draw(window);
        waiting_text.Draw(window);
    }
    else
    {
        chat_bg.Draw(window);
        message_bg.Draw(window);
        window.draw(opponent_color);
        opponent_username.Draw(window);
        chat_text.Draw(window);
        message_text.Draw(window);
        send_message_shade.Draw(window);
        send_message.Draw(window);
        send_message_text.Draw(window);
        back_to_menu_shade.Draw(window);
        back_to_menu.Draw(window);
        back_to_menu_text.Draw(window);
        board_possitions.Draw(window);
    }
    board->Draw(window);
}
