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

#include "Board.hpp"

using namespace sf;

class HomeScreen : public Screen
{
    int fd;
    unsigned short click_count;
    unsigned int turn;
    bool need_to_promote;

    game_info game;

    RectangleShape screen_background;
    RoundedRectangle profile_background;
    RoundedRectangle profile_picture;

    RoundedRectangle id_background;
    RoundedRectangle elo_background;

    RoundedRectangle menu_background;
    RoundedRectangle analysis_menu_bg;

    Button play_button;
    RoundedRectangle play_button_shade;
    Button search_button;
    RoundedRectangle search_button_shade;
    Button options_button;
    RoundedRectangle options_button_shade;
    Button exit_button;
    RoundedRectangle exit_button_shade;

    TextBox username;
    TextBox id_text;
    TextBox stat_text;

    TextBox play_text;
    TextBox search_text;
    TextBox options_text;
    TextBox exit_text;

    Board *analysis_board;

public:
    HomeScreen();
    ~HomeScreen();

    void UpdateOnFileDescriptor(int fd);

    void Animate(RenderWindow &window);

    void UpdateOnKeyPressed(unsigned int code);
    void UpdateOnClick(Vector2f mouse_pos);

    unsigned int LogicOnClick(Vector2f mouse_pos);

    void Draw(RenderWindow &window);
};