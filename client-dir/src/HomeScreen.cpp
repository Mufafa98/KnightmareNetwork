#include "HomeScreen.hpp"

HomeScreen::HomeScreen()
{
    turn = 0;
    need_to_promote = false;
    click_count = 0;
    const float section_hight_space = 10;

    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    profile_background.SetSize(Vector2f(WINDOW_WIDTH - 20 - 800,
                                        (WINDOW_HEIGHT - 4 * section_hight_space) / 4));
    profile_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    profile_background.SetPosition(Vector2f(10, section_hight_space));
    profile_background.SetCornerSize(10);

    profile_picture.SetSize(Vector2f(100, 100));
    profile_picture.SetFillColor(THIRD_BACKGROUND_COLOR);
    profile_picture.SetPosition(profile_background.GetPosition() + Vector2f(10, 10));
    profile_picture.SetCornerSize(20);

    username.SetString("TEST USERNAME");
    username.Center(Vector2f(profile_picture.GetEndPosition().x, profile_picture.GetPosition().y),
                    Vector2f(profile_background.GetSize().x - profile_picture.GetSize().x - 20,
                             profile_picture.GetSize().y));
    username.SetFillColor(TEXT_FONT_COLOR);

    id_background.SetPosition(Vector2f(20, profile_picture.GetEndPosition().y + 10));
    id_background.SetSize(Vector2f((profile_background.GetSize().x - 30) / 2,
                                   profile_background.GetSize().y - profile_picture.GetEndPosition().y - 10));
    id_background.SetFillColor(THIRD_BACKGROUND_COLOR);
    id_background.SetCornerSize(10);

    elo_background.SetPosition(Vector2f(id_background.GetEndPosition().x + 10,
                                        profile_picture.GetEndPosition().y + 10));
    elo_background.SetSize(Vector2f((profile_background.GetSize().x - 30) / 2,
                                    profile_background.GetSize().y - profile_picture.GetEndPosition().y - 10));
    elo_background.SetFillColor(THIRD_BACKGROUND_COLOR);
    elo_background.SetCornerSize(10);

    id_text.SetString("Id: #1234");
    id_text.Center(id_background.GetPosition(), id_background.GetSize());
    id_text.SetFillColor(TEXT_FONT_COLOR);

    stat_text.SetString("Status: Online");
    stat_text.Center(elo_background.GetPosition(), elo_background.GetSize());
    stat_text.SetFillColor(TEXT_FONT_COLOR);

    menu_background.SetSize(Vector2f(WINDOW_WIDTH - 20 - 800,
                                     (WINDOW_HEIGHT - 4 * section_hight_space) / 4 * 2));
    menu_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    menu_background.SetPosition(Vector2f(10, profile_background.GetEndPosition().y + section_hight_space));
    menu_background.SetCornerSize(10);

    const float button_height_space = 15;
    const float button_height = (menu_background.GetSize().y - 5 * button_height_space) / 4;

    play_button.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height - 5));
    play_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    play_button.SetPosition(menu_background.GetPosition() + Vector2f(10, button_height_space));
    play_button.SetCornerSize(10);

    play_button_shade.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height));
    play_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    play_button_shade.SetPosition(play_button.GetPosition());
    play_button_shade.SetCornerSize(10);

    search_button.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height - 5));
    search_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    search_button.SetPosition(play_button_shade.GetEndPosition() +
                              Vector2f(-play_button_shade.GetSize().x, button_height_space));
    search_button.SetCornerSize(10);

    search_button_shade.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height));
    search_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    search_button_shade.SetPosition(search_button.GetPosition());
    search_button_shade.SetCornerSize(10);

    options_button.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height - 5));
    options_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    options_button.SetPosition(search_button_shade.GetEndPosition() +
                               Vector2f(-search_button_shade.GetSize().x, button_height_space));
    options_button.SetCornerSize(10);

    options_button_shade.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height));
    options_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    options_button_shade.SetPosition(options_button.GetPosition());
    options_button_shade.SetCornerSize(10);

    exit_button.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height - 5));
    exit_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    exit_button.SetPosition(options_button_shade.GetEndPosition() +
                            Vector2f(-options_button_shade.GetSize().x, button_height_space));
    exit_button.SetCornerSize(10);

    exit_button_shade.SetSize(Vector2f(menu_background.GetSize().x - 20, button_height));
    exit_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    exit_button_shade.SetPosition(exit_button.GetPosition());
    exit_button_shade.SetCornerSize(10);

    play_text.SetString("Play");
    play_text.SetCharacterSize(40);
    play_text.Center(play_button.GetPosition(), play_button.GetSize());

    search_text.SetString("Search");
    search_text.SetCharacterSize(40);
    search_text.Center(search_button.GetPosition(), search_button.GetSize());

    options_text.SetString("Options");
    options_text.SetCharacterSize(40);
    options_text.Center(options_button.GetPosition(), options_button.GetSize());

    exit_text.SetString("Exit");
    exit_text.SetCharacterSize(40);
    exit_text.Center(exit_button.GetPosition(), exit_button.GetSize());

    analysis_menu_bg.SetSize(Vector2f(WINDOW_WIDTH - 20 - 800,
                                      (WINDOW_HEIGHT - 4 * section_hight_space) / 4));
    analysis_menu_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    analysis_menu_bg.SetPosition(Vector2f(10, menu_background.GetEndPosition().y + section_hight_space));
    analysis_menu_bg.SetCornerSize(10);

    analysis_board = new Board(START_CHESS_POS, Vector2f(profile_background.GetEndPosition().x + 10, 0));
}

HomeScreen::~HomeScreen()
{
    delete analysis_board;
}

void HomeScreen::UpdateOnFileDescriptor(int fd)
{
    cout.flush();
    this->fd = fd;
    unsigned int client_command = Commands::RequestUserData;
    send(fd, &client_command, sizeof(client_command), 0);
    user_info server_response;
    recv(fd, &server_response, sizeof(server_response), 0);
    string temp = "Id: #";
    temp += to_string(server_response.id);
    id_text.SetString(temp.c_str());
    id_text.Center(id_background.GetPosition(), id_background.GetSize());
    username.SetString(server_response.username);
    username.Center(Vector2f(profile_picture.GetEndPosition().x, profile_picture.GetPosition().y),
                    Vector2f(profile_background.GetSize().x - profile_picture.GetSize().x - 20,
                             profile_picture.GetSize().y));
}

void HomeScreen::Animate(RenderWindow &window)
{
    Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));
    if (play_button.Contains(mouse_pos))
        play_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.9));
    else
        play_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    if (search_button.Contains(mouse_pos))
        search_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.9));
    else
        search_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    if (options_button.Contains(mouse_pos))
        options_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.9));
    else
        options_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    if (exit_button.Contains(mouse_pos))
        exit_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.9));
    else
        exit_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);

    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (play_button.Contains(mouse_pos))
            play_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.8));
        else
            play_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
        if (search_button.Contains(mouse_pos))
            search_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.8));
        else
            search_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        if (options_button.Contains(mouse_pos))
            options_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.8));
        else
            options_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        if (exit_button.Contains(mouse_pos))
            exit_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.8));
        else
            exit_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    }
}

void HomeScreen::UpdateOnKeyPressed(unsigned int code)
{
}

void HomeScreen::UpdateOnClick(Vector2f mouse_pos)
{
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
    }
}

unsigned int HomeScreen::LogicOnClick(Vector2f mouse_pos)
{
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (need_to_promote)
        {
            if (analysis_board->ContainsPromBox(mouse_pos))
            {
                unsigned int choice = analysis_board->GetChoice(mouse_pos);
                unsigned int client_command = Commands::BoardCommand;
                send(fd, &client_command, sizeof(client_command), 0);
                client_command = BoardCommands::PromotePawn;
                send(fd, &client_command, sizeof(client_command), 0);
                analysis_board->PromotePawn(choice);
                send(fd, &choice, sizeof(choice), 0);
                unsigned int server_response;
                recv(fd, &server_response, sizeof(server_response), 0);
                need_to_promote = false;
            }
        }
        else if (analysis_board->Contains(mouse_pos))
        {
            int x = (mouse_pos.x - analysis_board->GetPosition().x) / 100;
            int y = (mouse_pos.y - analysis_board->GetPosition().y) / 100;
            unsigned int client_command = Commands::BoardCommand;
            send(fd, &client_command, sizeof(client_command), 0);
            if (click_count == 0)
                client_command = BoardCommands::FirstPos;
            else
                client_command = BoardCommands::SecondPos;
            if (client_command == BoardCommands::FirstPos || client_command == BoardCommands::SecondPos)
            {

                analysis_board->DeSelectSquare();
                analysis_board->DeHighlightSquare();
                send(fd, &client_command, sizeof(client_command), 0);
                send(fd, &x, sizeof(x), 0);
                send(fd, &y, sizeof(y), 0);
                unsigned int server_response;
                recv(fd, &server_response, sizeof(server_response), 0);
                if (client_command == BoardCommands::FirstPos)
                    game.first_pos = Vector2i(x, y);
                else if (client_command == BoardCommands::SecondPos)
                    game.second_pos = Vector2i(x, y);
                if (server_response == BoardCommands::SelectSquare)
                {
                    analysis_board->SelectSquare(Vector2i(x, y));
                    bool selected_sq[8][8];
                    recv(fd, selected_sq, sizeof(selected_sq), 0);
                    for (unsigned short i = 0; i < 8; i++)
                        for (unsigned short j = 0; j < 8; j++)
                            if (selected_sq[i][j])
                                analysis_board->HighlightSquare(j, i);
                }
                if (server_response == BoardCommands::MovePiece)
                {
                    turn++;
                    turn %= 2;
                    analysis_board->TryEnPassant(game.first_pos, game.second_pos);
                    analysis_board->TryCastling(game.first_pos, game.second_pos);
                    analysis_board->MovePiece(game.first_pos, game.second_pos);
                    recv(fd, &server_response, sizeof(server_response), 0);
                    if (server_response == BoardCommands::NeedPromotion)
                        need_to_promote = true;
                    if (need_to_promote)
                        turn--;
                }
                if (!need_to_promote) // TO DO pawn promotion
                {
                    click_count++;
                    click_count %= 2;
                }
            }
        }
        else
        {
            if (exit_button.Contains(mouse_pos))
            {
                return LogicCodes::Exit_req;
            }
        }
    }
    return -1;
}

void HomeScreen::Draw(RenderWindow &window)
{
    window.draw(screen_background);
    profile_background.Draw(window);
    profile_picture.Draw(window);
    username.Draw(window);
    id_background.Draw(window);
    elo_background.Draw(window);

    id_text.Draw(window);
    stat_text.Draw(window);

    menu_background.Draw(window);

    play_button_shade.Draw(window);
    play_button.Draw(window);
    search_button_shade.Draw(window);
    search_button.Draw(window);
    options_button_shade.Draw(window);
    options_button.Draw(window);
    exit_button_shade.Draw(window);
    exit_button.Draw(window);

    play_text.Draw(window);
    search_text.Draw(window);
    options_text.Draw(window);
    exit_text.Draw(window);

    analysis_menu_bg.Draw(window);

    analysis_board->Draw(window);
}
