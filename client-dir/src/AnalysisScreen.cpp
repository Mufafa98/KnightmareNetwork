#include "AnalysisScreen.hpp"

AnalysisScreen::AnalysisScreen()
{
    current_move = 0;
    play = false;

    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    moves_bg.SetSize(Vector2f(WINDOW_WIDTH - 800 - 20, (WINDOW_HEIGHT - 30) / 10 * 9));
    moves_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    moves_bg.SetPosition(Vector2f(810, 10));
    moves_bg.SetCornerSize(10);

    moves_text.SetPosition(moves_bg.GetPosition() + Vector2f(90, 30));

    move_highlighter.SetSize(Vector2f(WINDOW_WIDTH - 800 - 20 - 60, 30));
    move_highlighter.SetFillColor(THIRD_BACKGROUND_COLOR);
    move_highlighter.SetPosition(Vector2f(moves_bg.GetPosition().x + 30, moves_text.GetPosition().y));
    move_highlighter.SetCornerSize(10);

    back_button.SetSize(Vector2f(moves_bg.GetSize().x, (WINDOW_HEIGHT - 30) / 10 * 1 - 5));
    back_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    back_button.SetPosition(Vector2f(810, moves_bg.GetEndPosition().y + 10));
    back_button.SetCornerSize(10);

    back_button_shade.SetSize(Vector2f(moves_bg.GetSize().x, (WINDOW_HEIGHT - 30) / 10 * 1));
    back_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    back_button_shade.SetPosition(Vector2f(810, moves_bg.GetEndPosition().y + 10));
    back_button_shade.SetCornerSize(10);

    back_button_text.SetString("Back");
    back_button_text.SetCharacterSize(40);
    back_button_text.Center(back_button.GetPosition(), back_button.GetSize());

    float button_width = (moves_bg.GetSize().x - 50) / 4;

    previous_button.SetSize(Vector2f(button_width, 45));
    previous_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    previous_button.SetPosition(Vector2f(moves_bg.GetPosition().x + 10, moves_bg.GetEndPosition().y - 10 - 50));
    previous_button.SetCornerSize(10);

    previous_button_shade.SetSize(Vector2f(button_width, 50));
    previous_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    previous_button_shade.SetPosition(previous_button.GetPosition());
    previous_button_shade.SetCornerSize(10);

    previous_button_text.SetString("<");
    previous_button_text.SetCharacterSize(30);
    previous_button_text.Center(previous_button.GetPosition(), previous_button.GetSize());

    play_button.SetSize(Vector2f(button_width, 45));
    play_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    play_button.SetPosition(previous_button.GetPosition() + Vector2f(10 + button_width, 0));
    play_button.SetCornerSize(10);

    play_button_shade.SetSize(Vector2f(button_width, 50));
    play_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    play_button_shade.SetPosition(play_button.GetPosition());
    play_button_shade.SetCornerSize(10);

    play_button_text.SetString(">>");
    play_button_text.SetCharacterSize(30);
    play_button_text.Center(play_button.GetPosition(), play_button.GetSize());

    pause_button.SetSize(Vector2f(button_width, 45));
    pause_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    pause_button.SetPosition(play_button.GetPosition() + Vector2f(10 + button_width, 0));
    pause_button.SetCornerSize(10);

    pause_button_shade.SetSize(Vector2f(button_width, 50));
    pause_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    pause_button_shade.SetPosition(pause_button.GetPosition());
    pause_button_shade.SetCornerSize(10);

    pause_button_text.SetString("| |");
    pause_button_text.SetCharacterSize(30);
    pause_button_text.Center(pause_button.GetPosition(), pause_button.GetSize());

    next_button.SetSize(Vector2f(button_width, 45));
    next_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    next_button.SetPosition(pause_button.GetPosition() + Vector2f(10 + button_width, 0));
    next_button.SetCornerSize(10);

    next_button_shade.SetSize(Vector2f(button_width, 50));
    next_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    next_button_shade.SetPosition(next_button.GetPosition());
    next_button_shade.SetCornerSize(10);

    next_button_text.SetString(">");
    next_button_text.SetCharacterSize(30);
    next_button_text.Center(next_button.GetPosition(), next_button.GetSize());
}

AnalysisScreen::~AnalysisScreen()
{
    delete analysis_board;
}

void AnalysisScreen::UpdateOnFileDescriptor(int fd)
{
    this->fd = fd;
    size_t size;
    recv(fd, &size, sizeof(size), 0);
    string game;
    char c;
    for (size_t i = 0; i < size; i++)
    {
        recv(fd, &c, sizeof(c), 0);
        game.push_back(c);
    }
    bool start_pos_ended = false;
    unsigned int counter = 0;
    unsigned int char_counter = 0;
    string starting_pos;
    string temp_pos = "";
    string moves_text_buffer = "";
    for (size_t i = 0; i < game.size(); i++)
    {
        if (game[i] == '&')
        {
            start_pos_ended = true;
            continue;
        }
        if (!start_pos_ended)
            starting_pos.push_back(game[i]);
        else
        {
            if (char_counter % 5 == 0 && char_counter != 0)
            {
                moves_text_buffer += temp_pos;
                if (counter % 2 == 0)
                {
                    moves_text_buffer += "\t\t\t";
                }
                else
                {
                    moves_text_buffer += "\n";
                }
                game_info temp;
                temp.first_pos = Vector2i(temp_pos[1] - 'a', 8 - (temp_pos[2] - '0'));
                temp.second_pos = Vector2i(temp_pos[3] - 'a', 8 - (temp_pos[4] - '0'));
                moves.push_back(temp);
                temp_pos = "";
                counter++;
            }
            temp_pos.push_back(game[i]);
            char_counter++;
        }
    }
    moves_text_buffer += temp_pos + "\n";
    moves_text.SetString(moves_text_buffer.c_str());
    analysis_board = new Board(starting_pos, Vector2f(0, 0));
}

void AnalysisScreen::Animate(RenderWindow &window)
{
    Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));

    back_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR);
    previous_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    next_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    play_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    pause_button.Animate(mouse_pos, BUTTON_BACKGROUND_COLOR_V2);
    if (play && play_clock.getElapsedTime().asMilliseconds() >= 400 && current_move < moves.size())
    {
        previous_poss.push(analysis_board->BoardToFEN());
        analysis_board->MovePiece(moves[current_move].first_pos, moves[current_move].second_pos);
        move_highlighter.SetPosition(Vector2f(moves_bg.GetPosition().x + 30,
                                              moves_text.GetPosition().y + (current_move / 2) * move_highlighter.GetSize().y));

        current_move++;
        play_clock.restart();
    }
    if (current_move >= moves.size())
        play = false;
}

void AnalysisScreen::UpdateOnKeyPressed(unsigned int code)
{
}

void AnalysisScreen::UpdateOnClick(Vector2f mouse_pos)
{
    if (next_button.Contains(mouse_pos) && current_move < moves.size())
    {
        previous_poss.push(analysis_board->BoardToFEN());
        analysis_board->MovePiece(moves[current_move].first_pos, moves[current_move].second_pos);
        move_highlighter.SetPosition(Vector2f(moves_bg.GetPosition().x + 30,
                                              moves_text.GetPosition().y + (current_move / 2) * move_highlighter.GetSize().y));
        current_move++;
    }
    if (previous_button.Contains(mouse_pos) && current_move > 0)
    {
        analysis_board->PlaySound(moves[current_move].second_pos, moves[current_move].first_pos);
        analysis_board->FENToBoard(previous_poss.top());

        previous_poss.pop();
        current_move--;
        move_highlighter.SetPosition(Vector2f(moves_bg.GetPosition().x + 30,
                                              moves_text.GetPosition().y + (current_move / 2) * move_highlighter.GetSize().y));
    }
    if (pause_button.Contains(mouse_pos))
    {
        play = false;
    }
    if (play_button.Contains(mouse_pos))
    {
        play = true;
    }
}

void AnalysisScreen::Restart()
{
    current_move = 0;
    moves.clear();
    play = false;

    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    moves_bg.SetSize(Vector2f(WINDOW_WIDTH - 800 - 20, (WINDOW_HEIGHT - 30) / 10 * 9));
    moves_bg.SetFillColor(SECOND_BACKGROUND_COLOR);
    moves_bg.SetPosition(Vector2f(810, 10));
    moves_bg.SetCornerSize(10);

    moves_text.SetPosition(moves_bg.GetPosition() + Vector2f(90, 30));

    move_highlighter.SetSize(Vector2f(WINDOW_WIDTH - 800 - 20 - 60, 30));
    move_highlighter.SetFillColor(THIRD_BACKGROUND_COLOR);
    move_highlighter.SetPosition(Vector2f(moves_bg.GetPosition().x + 30, moves_text.GetPosition().y));
    move_highlighter.SetCornerSize(10);

    back_button.SetSize(Vector2f(moves_bg.GetSize().x, (WINDOW_HEIGHT - 30) / 10 * 1 - 5));
    back_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    back_button.SetPosition(Vector2f(810, moves_bg.GetEndPosition().y + 10));
    back_button.SetCornerSize(10);

    back_button_shade.SetSize(Vector2f(moves_bg.GetSize().x, (WINDOW_HEIGHT - 30) / 10 * 1));
    back_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    back_button_shade.SetPosition(Vector2f(810, moves_bg.GetEndPosition().y + 10));
    back_button_shade.SetCornerSize(10);

    back_button_text.SetString("Back");
    back_button_text.SetCharacterSize(40);
    back_button_text.Center(back_button.GetPosition(), back_button.GetSize());

    float button_width = (moves_bg.GetSize().x - 50) / 4;

    previous_button.SetSize(Vector2f(button_width, 45));
    previous_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    previous_button.SetPosition(Vector2f(moves_bg.GetPosition().x + 10, moves_bg.GetEndPosition().y - 10 - 50));
    previous_button.SetCornerSize(10);

    previous_button_shade.SetSize(Vector2f(button_width, 50));
    previous_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    previous_button_shade.SetPosition(previous_button.GetPosition());
    previous_button_shade.SetCornerSize(10);

    previous_button_text.SetString("<");
    previous_button_text.SetCharacterSize(30);
    previous_button_text.Center(previous_button.GetPosition(), previous_button.GetSize());

    play_button.SetSize(Vector2f(button_width, 45));
    play_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    play_button.SetPosition(previous_button.GetPosition() + Vector2f(10 + button_width, 0));
    play_button.SetCornerSize(10);

    play_button_shade.SetSize(Vector2f(button_width, 50));
    play_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    play_button_shade.SetPosition(play_button.GetPosition());
    play_button_shade.SetCornerSize(10);

    play_button_text.SetString(">>");
    play_button_text.SetCharacterSize(30);
    play_button_text.Center(play_button.GetPosition(), play_button.GetSize());

    pause_button.SetSize(Vector2f(button_width, 45));
    pause_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    pause_button.SetPosition(play_button.GetPosition() + Vector2f(10 + button_width, 0));
    pause_button.SetCornerSize(10);

    pause_button_shade.SetSize(Vector2f(button_width, 50));
    pause_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    pause_button_shade.SetPosition(pause_button.GetPosition());
    pause_button_shade.SetCornerSize(10);

    pause_button_text.SetString("| |");
    pause_button_text.SetCharacterSize(30);
    pause_button_text.Center(pause_button.GetPosition(), pause_button.GetSize());

    next_button.SetSize(Vector2f(button_width, 45));
    next_button.SetFillColor(BUTTON_BACKGROUND_COLOR_V2);
    next_button.SetPosition(pause_button.GetPosition() + Vector2f(10 + button_width, 0));
    next_button.SetCornerSize(10);

    next_button_shade.SetSize(Vector2f(button_width, 50));
    next_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR_V2, 0.7));
    next_button_shade.SetPosition(next_button.GetPosition());
    next_button_shade.SetCornerSize(10);

    next_button_text.SetString(">");
    next_button_text.SetCharacterSize(30);
    next_button_text.Center(next_button.GetPosition(), next_button.GetSize());
}

unsigned int AnalysisScreen::LogicOnClick(Vector2f mouse_pos)
{
    if (back_button.Contains(mouse_pos))
        return ScreenCommand::SocialS;
    return ScreenCommand::NoneS;
}

void AnalysisScreen::Draw(RenderWindow &window)
{
    window.draw(screen_background);

    moves_bg.Draw(window);
    move_highlighter.Draw(window);
    moves_text.Draw(window);

    back_button_shade.Draw(window);
    back_button.Draw(window);
    back_button_text.Draw(window);

    previous_button_shade.Draw(window);
    previous_button.Draw(window);
    previous_button_text.Draw(window);

    play_button_shade.Draw(window);
    play_button.Draw(window);
    play_button_text.Draw(window);

    pause_button_shade.Draw(window);
    pause_button.Draw(window);
    pause_button_text.Draw(window);

    next_button_shade.Draw(window);
    next_button.Draw(window);
    next_button_text.Draw(window);

    analysis_board->Draw(window);
}
