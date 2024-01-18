#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include "Constants.hpp"

#include "Screen.hpp"

#include "ClientData.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "RoundedRectangle.hpp"
#include "CustomUtility.hpp"
#include "InputBox.hpp"

using namespace sf;

class SocialScreen : public Screen
{
    queue<GameDbInfo> games_search;
    queue<UserDbInfo> users_search;
    queue<string> friends_queue;
    unsigned int total_no_results;
    int fd;
    RectangleShape screen_background;

    RoundedRectangle search_background;
    RoundedRectangle social_background;

    Button back_button;
    RoundedRectangle back_button_shade;
    TextBox back_button_text;

    Button search_type_button;
    RoundedRectangle search_type_button_shade;
    TextBox search_type_button_text;

    Button search_button;
    RoundedRectangle search_button_shade;
    TextBox search_button_text;

    RoundedRectangle search_box_background;
    InputBox search_box;

    unsigned int results_count;
    unsigned int page_count;
    unsigned int current_page;
    unsigned int search_type_counter;

    Button results[10];
    RoundedRectangle results_shade[10];
    TextBox results_text[10];

    Button page[5];
    RoundedRectangle page_shade[5];
    TextBox page_number[5];

    Button previous_page;
    RoundedRectangle previous_page_shade;
    TextBox previous_text;

    Button next_page;
    RoundedRectangle next_page_shade;
    TextBox next_text;

    unsigned int friends_count;

    Button friends[10];
    RoundedRectangle friends_shade[10];
    TextBox friends_text[10];

    void DefaultGameSearch();
    void DefaultUserSearch();
    void DefaultFriendSearch();

    void GameSearchById(const string &_id);
    void UserSearchById(const string &_id);

public:
    SocialScreen();

    void UpdateOnFileDescriptor(int fd);

    void Animate(RenderWindow &window);

    void UpdateOnKeyPressed(unsigned int code);
    void UpdateOnClick(Vector2f mouse_pos);
    void Restart();

    unsigned int LogicOnClick(Vector2f mouse_pos);

    void Draw(RenderWindow &window);
};