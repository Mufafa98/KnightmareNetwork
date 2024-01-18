#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>

#include <stack>

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

class AnalysisScreen : public Screen
{
    int fd;
    RectangleShape screen_background;

    Board *analysis_board;

    RoundedRectangle moves_bg;
    TextBox moves_text;
    RoundedRectangle move_highlighter;

    vector<game_info> moves;
    stack<string> previous_poss;
    unsigned int current_move;

    Button back_button;
    RoundedRectangle back_button_shade;
    TextBox back_button_text;

    Button previous_button;
    RoundedRectangle previous_button_shade;
    TextBox previous_button_text;

    Button play_button;
    RoundedRectangle play_button_shade;
    TextBox play_button_text;
    Clock play_clock;
    bool play;

    Button pause_button;
    RoundedRectangle pause_button_shade;
    TextBox pause_button_text;

    Button next_button;
    RoundedRectangle next_button_shade;
    TextBox next_button_text;

public:
    AnalysisScreen();
    ~AnalysisScreen();

    void UpdateOnFileDescriptor(int fd);

    void Animate(RenderWindow &window);

    void UpdateOnKeyPressed(unsigned int code);
    void UpdateOnClick(Vector2f mouse_pos);
    void Restart();

    unsigned int LogicOnClick(Vector2f mouse_pos);

    void Draw(RenderWindow &window);
};