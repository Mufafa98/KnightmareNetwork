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

class PlayScreen : public Screen
{
    bool opponent_found;

    int fd;
    unsigned short click_count;
    unsigned int turn;
    bool need_to_promote;
    bool game_ended;

    game_info game;

    RectangleShape screen_background;

    Clock waiting_animation;
    RoundedRectangle waiting_bg;
    TextBox waiting_text;

    RoundedRectangle chat_bg;
    CircleShape opponent_color;
    TextBox opponent_username;
    string chat_buffer;
    TextBox chat_text;
    RoundedRectangle message_bg;
    InputBox message_text;

    Button send_message;
    RoundedRectangle send_message_shade;
    TextBox send_message_text;

    Button back_to_menu;
    RoundedRectangle back_to_menu_shade;
    TextBox back_to_menu_text;

    RoundedRectangle board_possitions;

    Board *board;

public:
    PlayScreen();
    ~PlayScreen();

    void UpdateOnFileDescriptor(int fd);

    void Animate(RenderWindow &window);

    void UpdateOnKeyPressed(unsigned int code);
    void UpdateOnClick(Vector2f mouse_pos);
    void Restart();

    unsigned int LogicOnClick(Vector2f mouse_pos);

    void Draw(RenderWindow &window);
};