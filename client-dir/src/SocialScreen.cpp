#include "SocialScreen.hpp"

void SocialScreen::DefaultGameSearch()
{
    unsigned int client_command;
    unsigned int server_response;
    client_command = Commands::Search;
    send(fd, &client_command, sizeof(client_command), 0);
    client_command = SearchCommands::GetGames;
    send(fd, &client_command, sizeof(client_command), 0);
    while (true)
    {
        recv(fd, &server_response, sizeof(server_response), 0);
        if (server_response == SearchCommands::LastElement)
            break;
        GameDbInfo temp;
        recv(fd, &temp, sizeof(temp), 0);
        games_search.push(temp);
    }
    recv(fd, &total_no_results, sizeof(total_no_results), 0);
    unsigned int counter = 0;
    results_count = min((unsigned int)10, total_no_results);
    while (!games_search.empty())
    {
        GameDbInfo temp = games_search.front();
        games_search.pop();
        string temp_text = "";
        temp_text += "MID: " + string(temp.game_id) + "     " + string(temp.white_player) + " vs ";
        temp_text += string(temp.black_player) + "     Played on: " + string(temp.time);
        results_text[counter].SetString(temp_text.c_str());
        results_text[counter].Center(results[counter].GetPosition(), results[counter].GetSize());
        counter++;
    }
}

void SocialScreen::DefaultUserSearch()
{
    unsigned int client_command;
    unsigned int server_response;
    client_command = Commands::Search;
    send(fd, &client_command, sizeof(client_command), 0);
    client_command = SearchCommands::GetUsers;
    send(fd, &client_command, sizeof(client_command), 0);
    while (true)
    {
        recv(fd, &server_response, sizeof(server_response), 0);
        if (server_response == SearchCommands::LastElement)
            break;
        UserDbInfo temp;
        recv(fd, &temp, sizeof(temp), 0);
        users_search.push(temp);
    }
    recv(fd, &total_no_results, sizeof(total_no_results), 0);
    unsigned int counter = 0;
    results_count = min((unsigned int)10, total_no_results);
    while (!users_search.empty())
    {
        UserDbInfo temp = users_search.front();
        users_search.pop();
        string temp_text = "";
        temp_text += "ID: " + string(temp.user_id) + "     Username: " + string(temp.username);
        results_text[counter].SetString(temp_text.c_str());
        results_text[counter].Center(results[counter].GetPosition(), results[counter].GetSize());
        counter++;
    }
}

void SocialScreen::DefaultFriendSearch()
{
    unsigned int client_command;
    unsigned int server_response;
    client_command = Commands::Search;
    send(fd, &client_command, sizeof(client_command), 0);
    client_command = SearchCommands::GetFriends;
    send(fd, &client_command, sizeof(client_command), 0);
    while (true)
    {
        recv(fd, &server_response, sizeof(server_response), 0);
        if (server_response == SearchCommands::LastElement)
            break;
        char temp[20];
        recv(fd, &temp, sizeof(temp), 0);
        friends_queue.push(temp);
    }
    recv(fd, &friends_count, sizeof(friends_count), 0);
    unsigned int counter = 0;
    friends_count = min((unsigned int)10, friends_count);
    while (!friends_queue.empty())
    {
        string temp = friends_queue.front();
        friends_queue.pop();
        friends_text[counter].SetString(temp.c_str());
        friends_text[counter].Center(friends[counter].GetPosition(), friends[counter].GetSize());
        counter++;
    }
}

void SocialScreen::GameSearchById(const string &_id)
{
    unsigned int client_command;
    unsigned int server_response;
    client_command = Commands::Search;
    send(fd, &client_command, sizeof(client_command), 0);
    client_command = SearchCommands::GetGamesById;
    send(fd, &client_command, sizeof(client_command), 0);
    char id[10];
    strcpy(id, _id.c_str());
    send(fd, id, sizeof(id), 0);
    while (true)
    {
        recv(fd, &server_response, sizeof(server_response), 0);
        if (server_response == SearchCommands::LastElement)
            break;
        GameDbInfo temp;
        recv(fd, &temp, sizeof(temp), 0);
        games_search.push(temp);
    }
    recv(fd, &total_no_results, sizeof(total_no_results), 0);
    unsigned int counter = 0;
    results_count = min((unsigned int)10, total_no_results);
    while (!games_search.empty())
    {
        GameDbInfo temp = games_search.front();
        games_search.pop();
        string temp_text = "";
        temp_text += "MID: " + string(temp.game_id) + "     " + string(temp.white_player) + " vs ";
        temp_text += string(temp.black_player) + "     Played on: " + string(temp.time);
        results_text[counter].SetString(temp_text.c_str());
        results_text[counter].Center(results[counter].GetPosition(), results[counter].GetSize());
        counter++;
    }
}

void SocialScreen::UserSearchById(const string &_id)
{
    unsigned int client_command;
    unsigned int server_response;
    client_command = Commands::Search;
    send(fd, &client_command, sizeof(client_command), 0);
    client_command = SearchCommands::GetUsersById;
    send(fd, &client_command, sizeof(client_command), 0);
    char id[10];
    strcpy(id, _id.c_str());
    send(fd, id, sizeof(id), 0);
    while (true)
    {
        recv(fd, &server_response, sizeof(server_response), 0);
        if (server_response == SearchCommands::LastElement)
            break;
        UserDbInfo temp;
        recv(fd, &temp, sizeof(temp), 0);
        users_search.push(temp);
    }
    recv(fd, &total_no_results, sizeof(total_no_results), 0);
    unsigned int counter = 0;
    results_count = min((unsigned int)10, total_no_results);
    while (!users_search.empty())
    {
        UserDbInfo temp = users_search.front();
        users_search.pop();
        string temp_text = "";
        temp_text += "ID: " + string(temp.user_id) + "     Username: " + string(temp.username);
        results_text[counter].SetString(temp_text.c_str());
        results_text[counter].Center(results[counter].GetPosition(), results[counter].GetSize());
        counter++;
    }
}

SocialScreen::SocialScreen()
{
    current_page = 0;
    results_count = 0;
    page_count = 0;
    search_type_counter = 1;
    search_type_counter++;

    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    search_background.SetSize(Vector2f((WINDOW_WIDTH - 50) / 10 * 7,
                                       (WINDOW_HEIGHT - 40)));
    search_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    search_background.SetPosition(Vector2f(20, 20));
    search_background.SetCornerSize(10);

    social_background.SetSize(Vector2f((WINDOW_WIDTH - 50) / 10 * 3,
                                       (WINDOW_HEIGHT - 40)));
    social_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    social_background.SetPosition(Vector2f(search_background.GetEndPosition().x + 10, 20));
    social_background.SetCornerSize(10);

    back_button.SetSize(Vector2f(100, 45));
    back_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    back_button.SetPosition(social_background.GetPosition() + Vector2f(10, 10));
    back_button.SetCornerSize(10);

    back_button_shade.SetSize(Vector2f(100, 50));
    back_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    back_button_shade.SetPosition(back_button.GetPosition());
    back_button_shade.SetCornerSize(10);

    back_button_text.SetString("Menu");
    back_button_text.Center(back_button.GetPosition(), back_button.GetSize());

    search_type_button.SetSize(Vector2f(100, 45));
    search_type_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    search_type_button.SetPosition(search_background.GetPosition() + Vector2f(10, 10));
    search_type_button.SetCornerSize(10);

    search_type_button_shade.SetSize(Vector2f(100, 50));
    search_type_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    search_type_button_shade.SetPosition(search_type_button.GetPosition());
    search_type_button_shade.SetCornerSize(10);

    search_type_button_text.SetString("Game");
    search_type_button_text.Center(search_type_button.GetPosition(), search_type_button.GetSize());

    search_button.SetSize(Vector2f(100, 45));
    search_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    search_button.SetPosition(Vector2f(search_background.GetEndPosition().x - 110,
                                       search_background.GetPosition().x + 10));
    search_button.SetCornerSize(10);

    search_button_shade.SetSize(Vector2f(100, 50));
    search_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    search_button_shade.SetPosition(search_button.GetPosition());
    search_button_shade.SetCornerSize(10);

    search_button_text.SetString("Search");
    search_button_text.Center(search_button.GetPosition(), search_button.GetSize());

    search_box_background.SetSize(Vector2f(search_background.GetSize().x - 40 - 200, 50));
    search_box_background.SetFillColor(THIRD_BACKGROUND_COLOR);
    search_box_background.SetPosition(search_type_button.GetEndPosition() + Vector2f(10, -45));
    search_box_background.SetCornerSize(20);

    search_box.SetString("Search");
    search_box.SetIBFillColor(Color::Black);
    search_box.SetPosition(search_box_background.GetPosition() + Vector2f(10, 10));
    search_box.SetFillColor(TEXT_FONT_COLOR);
    search_box.SetType(InputBox::Type::Search);

    page[0].SetSize(Vector2f(50, 45));
    page[0].SetFillColor(BUTTON_BACKGROUND_COLOR);
    page[0].SetPosition(Vector2f(search_background.GetSize().x / 2 - 25 + 20,
                                 search_background.GetEndPosition().y - 10 - 50));
    page[0].SetCornerSize(10);

    page_shade[0].SetSize(Vector2f(50, 50));
    page_shade[0].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    page_shade[0].SetPosition(page[0].GetPosition());
    page_shade[0].SetCornerSize(10);

    for (size_t i = 1; i < 1; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            if (j == 0)
                page[j].SetPosition(Vector2f(20 + search_background.GetSize().x / 2 - (i + 1) * 5 - (i + 1) * 25,
                                             page[0].GetPosition().y));
            else
                page[j].SetPosition(Vector2f(page[j - 1].GetEndPosition().x + 10,
                                             page[j - 1].GetPosition().y));
            page_shade[j].SetPosition(page[j].GetPosition());
        }
        page[i].SetSize(Vector2f(50, 45));
        page[i].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        page[i].SetCornerSize(10);

        page_shade[i].SetSize(Vector2f(50, 50));
        page_shade[i].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
        page_shade[i].SetCornerSize(10);

        page_count++;
    }
    for (size_t i = 0; i < 5; i++)
    {
        page_number[i].SetString(to_string(i + 1).c_str());
        page_number[i].Center(page[i].GetPosition(), page[i].GetSize());
    }
    page_count++;
    previous_page.SetSize(Vector2f(50, 45));
    previous_page.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    previous_page.SetPosition(Vector2f(page[0].GetPosition().x - 10 - 50,
                                       page[0].GetPosition().y));
    previous_page.SetCornerSize(10);

    previous_page_shade.SetSize(Vector2f(50, 50));
    previous_page_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    previous_page_shade.SetPosition(previous_page.GetPosition());
    previous_page_shade.SetCornerSize(10);

    previous_text.SetString("<");
    previous_text.Center(previous_page.GetPosition(), previous_page.GetSize());

    next_page.SetSize(Vector2f(50, 45));
    next_page.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    next_page.SetPosition(Vector2f(page[page_count - 1].GetEndPosition().x + 10,
                                   page[page_count - 1].GetPosition().y));
    next_page.SetCornerSize(10);

    next_page_shade.SetSize(Vector2f(50, 50));
    next_page_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    next_page_shade.SetPosition(next_page.GetPosition());
    next_page_shade.SetCornerSize(10);

    next_text.SetString(">");
    next_text.Center(next_page.GetPosition(), next_page.GetSize());

    float result_padding = page_shade[0].GetPosition().y - back_button.GetEndPosition().y;
    result_padding = result_padding - 90 - 10 * 50;
    result_padding /= 2;

    results[0].SetSize(Vector2f(search_background.GetSize().x - 20, 45));
    results[0].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    results[0].SetPosition(Vector2f(search_background.GetPosition().x + 10,
                                    search_type_button_shade.GetEndPosition().y + result_padding));
    results[0].SetCornerSize(10);

    results_shade[0].SetSize(Vector2f(search_background.GetSize().x - 20, 50));
    results_shade[0].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    results_shade[0].SetPosition(results[0].GetPosition());
    results_shade[0].SetCornerSize(10);
    results_count++;
    for (size_t i = 1; i < 10; i++)
    {
        results[i].SetSize(Vector2f(search_background.GetSize().x - 20, 45));
        results[i].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        results[i].SetPosition(Vector2f(results[i - 1].GetPosition().x,
                                        results[i - 1].GetEndPosition().y + 15));
        results[i].SetCornerSize(10);

        results_shade[i].SetSize(Vector2f(search_background.GetSize().x - 20, 50));
        results_shade[i].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
        results_shade[i].SetPosition(results[i].GetPosition());
        results_shade[i].SetCornerSize(10);

        results_count++;
    }

    friends_count = 0;

    friends[0].SetSize(Vector2f(social_background.GetSize().x - 20, 45));
    friends[0].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    friends[0].SetPosition(Vector2f(social_background.GetPosition().x + 10,
                                    back_button.GetEndPosition().y + result_padding));
    friends[0].SetCornerSize(10);

    friends_shade[0].SetSize(Vector2f(social_background.GetSize().x - 20, 50));
    friends_shade[0].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    friends_shade[0].SetPosition(friends[0].GetPosition());
    friends_shade[0].SetCornerSize(10);
    friends_count++;
    for (size_t i = 1; i < 10; i++)
    {
        friends[i].SetSize(Vector2f(social_background.GetSize().x - 20, 45));
        friends[i].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        friends[i].SetPosition(Vector2f(friends[i - 1].GetPosition().x,
                                        friends[i - 1].GetEndPosition().y + 15));
        friends[i].SetCornerSize(10);

        friends_shade[i].SetSize(Vector2f(social_background.GetSize().x - 20, 50));
        friends_shade[i].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
        friends_shade[i].SetPosition(friends[i].GetPosition());
        friends_shade[i].SetCornerSize(10);

        friends_count++;
    }
}

void SocialScreen::UpdateOnFileDescriptor(int fd)
{
    this->fd = fd;
    DefaultGameSearch();
    DefaultFriendSearch();
}

void SocialScreen::Animate(RenderWindow &window)
{
    Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));
    back_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    search_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    search_type_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    previous_page.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    next_page.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    for (unsigned int i = 0; i < results_count; i++)
        results[i].Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    for (unsigned int i = 0; i < page_count; i++)
    {
        if (i == current_page)
            page[i].Animate(mouse_pos, BUTTON_BACKGROUND_COLOR);
        else
            page[i].Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    }
    for (unsigned int i = 0; i < friends_count; i++)
        friends[i].Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
}

void SocialScreen::UpdateOnKeyPressed(unsigned int code)
{
    if (search_box.GetSelected())
    {
        search_box.UpdateText(code, 29);
        search_box.SetPosition(search_box_background.GetPosition() + Vector2f(10, 10));
    }
}

void SocialScreen::UpdateOnClick(Vector2f mouse_pos)
{
    DefaultFriendSearch();
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (search_type_button.Contains(mouse_pos))
        {
            if (search_type_counter % 2 == 0)
            {
                search_type_button_text.SetString("User");
                DefaultUserSearch();
            }
            else
            {
                search_type_button_text.SetString("Game");
                DefaultGameSearch();
            }
            search_type_counter++;
            search_type_counter %= 2;
            search_type_button_text.Center(search_type_button.GetPosition(), search_type_button.GetSize());
        }
        if (search_box_background.Contains(mouse_pos))
            search_box.Select();
        else
            search_box.DeSelect();
        for (unsigned int i = 0; i < page_count; i++)
        {
            if (page[i].Contains(mouse_pos))
            {
                page[current_page].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
                page_shade[current_page].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
                current_page = i;
                page[current_page].SetFillColor(BUTTON_BACKGROUND_COLOR);
                page_shade[current_page].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
            }
        }
        if (next_page.Contains(mouse_pos))
        {
            page[current_page].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
            page_shade[current_page].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
            if (current_page < page_count - 1)
                current_page++;
            page[current_page].SetFillColor(BUTTON_BACKGROUND_COLOR);
            page_shade[current_page].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
        }
        if (previous_page.Contains(mouse_pos))
        {
            page[current_page].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
            page_shade[current_page].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
            if (current_page > 0)
                current_page--;
            page[current_page].SetFillColor(BUTTON_BACKGROUND_COLOR);
            page_shade[current_page].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
        }
    }
}

void SocialScreen::Restart()
{
    current_page = 0;
    results_count = 0;
    page_count = 0;
    search_type_counter = 1;
    search_type_counter++;

    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    search_background.SetSize(Vector2f((WINDOW_WIDTH - 50) / 10 * 7,
                                       (WINDOW_HEIGHT - 40)));
    search_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    search_background.SetPosition(Vector2f(20, 20));
    search_background.SetCornerSize(10);

    social_background.SetSize(Vector2f((WINDOW_WIDTH - 50) / 10 * 3,
                                       (WINDOW_HEIGHT - 40)));
    social_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    social_background.SetPosition(Vector2f(search_background.GetEndPosition().x + 10, 20));
    social_background.SetCornerSize(10);

    back_button.SetSize(Vector2f(100, 45));
    back_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    back_button.SetPosition(social_background.GetPosition() + Vector2f(10, 10));
    back_button.SetCornerSize(10);

    back_button_shade.SetSize(Vector2f(100, 50));
    back_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    back_button_shade.SetPosition(back_button.GetPosition());
    back_button_shade.SetCornerSize(10);

    back_button_text.SetString("Menu");
    back_button_text.Center(back_button.GetPosition(), back_button.GetSize());

    search_type_button.SetSize(Vector2f(100, 45));
    search_type_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    search_type_button.SetPosition(search_background.GetPosition() + Vector2f(10, 10));
    search_type_button.SetCornerSize(10);

    search_type_button_shade.SetSize(Vector2f(100, 50));
    search_type_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    search_type_button_shade.SetPosition(search_type_button.GetPosition());
    search_type_button_shade.SetCornerSize(10);

    search_type_button_text.SetString("Game");
    search_type_button_text.Center(search_type_button.GetPosition(), search_type_button.GetSize());

    search_button.SetSize(Vector2f(100, 45));
    search_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    search_button.SetPosition(Vector2f(search_background.GetEndPosition().x - 110,
                                       search_background.GetPosition().x + 10));
    search_button.SetCornerSize(10);

    search_button_shade.SetSize(Vector2f(100, 50));
    search_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    search_button_shade.SetPosition(search_button.GetPosition());
    search_button_shade.SetCornerSize(10);

    search_button_text.SetString("Search");
    search_button_text.Center(search_button.GetPosition(), search_button.GetSize());

    search_box_background.SetSize(Vector2f(search_background.GetSize().x - 40 - 200, 50));
    search_box_background.SetFillColor(THIRD_BACKGROUND_COLOR);
    search_box_background.SetPosition(search_type_button.GetEndPosition() + Vector2f(10, -45));
    search_box_background.SetCornerSize(20);

    search_box.SetString("Search");
    search_box.SetIBFillColor(Color::Black);
    search_box.SetPosition(search_box_background.GetPosition() + Vector2f(10, 10));
    search_box.SetFillColor(TEXT_FONT_COLOR);
    search_box.SetType(InputBox::Type::Search);

    page[0].SetSize(Vector2f(50, 45));
    page[0].SetFillColor(BUTTON_BACKGROUND_COLOR);
    page[0].SetPosition(Vector2f(search_background.GetSize().x / 2 - 25 + 20,
                                 search_background.GetEndPosition().y - 10 - 50));
    page[0].SetCornerSize(10);

    page_shade[0].SetSize(Vector2f(50, 50));
    page_shade[0].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    page_shade[0].SetPosition(page[0].GetPosition());
    page_shade[0].SetCornerSize(10);

    for (size_t i = 1; i < 1; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            if (j == 0)
                page[j].SetPosition(Vector2f(20 + search_background.GetSize().x / 2 - (i + 1) * 5 - (i + 1) * 25,
                                             page[0].GetPosition().y));
            else
                page[j].SetPosition(Vector2f(page[j - 1].GetEndPosition().x + 10,
                                             page[j - 1].GetPosition().y));
            page_shade[j].SetPosition(page[j].GetPosition());
        }
        page[i].SetSize(Vector2f(50, 45));
        page[i].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        page[i].SetCornerSize(10);

        page_shade[i].SetSize(Vector2f(50, 50));
        page_shade[i].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
        page_shade[i].SetCornerSize(10);

        page_count++;
    }
    for (size_t i = 0; i < 5; i++)
    {
        page_number[i].SetString(to_string(i + 1).c_str());
        page_number[i].Center(page[i].GetPosition(), page[i].GetSize());
    }
    page_count++;
    previous_page.SetSize(Vector2f(50, 45));
    previous_page.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    previous_page.SetPosition(Vector2f(page[0].GetPosition().x - 10 - 50,
                                       page[0].GetPosition().y));
    previous_page.SetCornerSize(10);

    previous_page_shade.SetSize(Vector2f(50, 50));
    previous_page_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    previous_page_shade.SetPosition(previous_page.GetPosition());
    previous_page_shade.SetCornerSize(10);

    previous_text.SetString("<");
    previous_text.Center(previous_page.GetPosition(), previous_page.GetSize());

    next_page.SetSize(Vector2f(50, 45));
    next_page.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    next_page.SetPosition(Vector2f(page[page_count - 1].GetEndPosition().x + 10,
                                   page[page_count - 1].GetPosition().y));
    next_page.SetCornerSize(10);

    next_page_shade.SetSize(Vector2f(50, 50));
    next_page_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    next_page_shade.SetPosition(next_page.GetPosition());
    next_page_shade.SetCornerSize(10);

    next_text.SetString(">");
    next_text.Center(next_page.GetPosition(), next_page.GetSize());

    float result_padding = page_shade[0].GetPosition().y - back_button.GetEndPosition().y;
    result_padding = result_padding - 90 - 10 * 50;
    result_padding /= 2;

    results[0].SetSize(Vector2f(search_background.GetSize().x - 20, 45));
    results[0].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    results[0].SetPosition(Vector2f(search_background.GetPosition().x + 10,
                                    search_type_button_shade.GetEndPosition().y + result_padding));
    results[0].SetCornerSize(10);

    results_shade[0].SetSize(Vector2f(search_background.GetSize().x - 20, 50));
    results_shade[0].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    results_shade[0].SetPosition(results[0].GetPosition());
    results_shade[0].SetCornerSize(10);
    results_count++;
    for (size_t i = 1; i < 10; i++)
    {
        results[i].SetSize(Vector2f(search_background.GetSize().x - 20, 45));
        results[i].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        results[i].SetPosition(Vector2f(results[i - 1].GetPosition().x,
                                        results[i - 1].GetEndPosition().y + 15));
        results[i].SetCornerSize(10);

        results_shade[i].SetSize(Vector2f(search_background.GetSize().x - 20, 50));
        results_shade[i].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
        results_shade[i].SetPosition(results[i].GetPosition());
        results_shade[i].SetCornerSize(10);

        results_count++;
    }

    friends_count = 0;

    friends[0].SetSize(Vector2f(social_background.GetSize().x - 20, 45));
    friends[0].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    friends[0].SetPosition(Vector2f(social_background.GetPosition().x + 10,
                                    back_button.GetEndPosition().y + result_padding));
    friends[0].SetCornerSize(10);

    friends_shade[0].SetSize(Vector2f(social_background.GetSize().x - 20, 50));
    friends_shade[0].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    friends_shade[0].SetPosition(friends[0].GetPosition());
    friends_shade[0].SetCornerSize(10);
    friends_count++;
    for (size_t i = 1; i < 10; i++)
    {
        friends[i].SetSize(Vector2f(social_background.GetSize().x - 20, 45));
        friends[i].SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
        friends[i].SetPosition(Vector2f(friends[i - 1].GetPosition().x,
                                        friends[i - 1].GetEndPosition().y + 15));
        friends[i].SetCornerSize(10);

        friends_shade[i].SetSize(Vector2f(social_background.GetSize().x - 20, 50));
        friends_shade[i].SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
        friends_shade[i].SetPosition(friends[i].GetPosition());
        friends_shade[i].SetCornerSize(10);

        friends_count++;
    }
}

unsigned int SocialScreen::LogicOnClick(Vector2f mouse_pos)
{
    unsigned int client_command;
    if (back_button.Contains(mouse_pos))
        return ScreenCommand::MenuS;
    if (search_button.Contains(mouse_pos))
    {
        string temp_search = search_box.GetString();
        if (StartsWith(temp_search, "id"))
        {
            string temp_id = "";
            for (size_t i = 0; i < temp_search.size(); i++)
                if (temp_search[i] >= '0' && temp_search[i] <= '9')
                    temp_id.push_back(temp_search[i]);
            if (search_type_counter % 2 == 0)
                GameSearchById(temp_id);
            else
                UserSearchById(temp_id);
        }
    }
    for (unsigned int i = 0; i < results_count; i++)
    {
        if (results[i].Contains(mouse_pos))
        {
            if (search_type_counter % 2 == 0)
            {
                string temp_id = "";
                string temp_text = results_text[i].GetString();
                for (size_t i = 0; i < temp_text.size(); i++)
                {
                    if (temp_text[i] >= '0' && temp_text[i] <= '9')
                        temp_id.push_back(temp_text[i]);
                    else if (temp_id != "")
                        break;
                }
                client_command = Commands::Search;
                send(fd, &client_command, sizeof(client_command), 0);
                client_command = SearchCommands::GetGame;
                send(fd, &client_command, sizeof(client_command), 0);
                char id[10];
                strcpy(id, temp_id.c_str());
                send(fd, id, sizeof(id), 0);
                return AnalysisS;
            }
            else
            {
                string temp_id = "";
                string temp_text = results_text[i].GetString();
                for (size_t i = 0; i < temp_text.size(); i++)
                {
                    if (temp_text[i] >= '0' && temp_text[i] <= '9')
                        temp_id.push_back(temp_text[i]);
                    else if (temp_id != "")
                        break;
                }
                client_command = Commands::Search;
                send(fd, &client_command, sizeof(client_command), 0);
                client_command = SearchCommands::SendFriendRequest;
                send(fd, &client_command, sizeof(client_command), 0);
                char id[10];
                strcpy(id, temp_id.c_str());
                send(fd, id, sizeof(id), 0);
            }
        }
    }
    for (unsigned int i = 0; i < friends_count; i++)
    {
        if (friends[i].Contains(mouse_pos))
        {
            unsigned int client_command = Commands::Play;
            send(fd, &client_command, sizeof(client_command), 0);
            return ScreenCommand::PlayS;
        }
    }
    return ScreenCommand::NoneS;
}

void SocialScreen::Draw(RenderWindow &window)
{
    window.draw(screen_background);
    search_background.Draw(window);
    social_background.Draw(window);

    back_button_shade.Draw(window);
    back_button.Draw(window);
    back_button_text.Draw(window);

    search_type_button_shade.Draw(window);
    search_type_button.Draw(window);
    search_type_button_text.Draw(window);

    search_button_shade.Draw(window);
    search_button.Draw(window);
    search_button_text.Draw(window);

    search_box_background.Draw(window);
    search_box.Draw(window);

    for (unsigned int i = 0; i < results_count; i++)
    {
        results_shade[i].Draw(window);
        results[i].Draw(window);
        results_text[i].Draw(window);
    }

    for (unsigned int i = 0; i < page_count; i++)
    {
        page_shade[i].Draw(window);
        page[i].Draw(window);
        page_number[i].Draw(window);
    }

    for (unsigned int i = 0; i < friends_count; i++)
    {
        friends_shade[i].Draw(window);
        friends[i].Draw(window);
        friends_text[i].Draw(window);
    }
    previous_page_shade.Draw(window);
    previous_page.Draw(window);
    previous_text.Draw(window);

    next_page_shade.Draw(window);
    next_page.Draw(window);
    next_text.Draw(window);
}
