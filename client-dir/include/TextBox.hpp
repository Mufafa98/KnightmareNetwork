#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class TextBox{
protected:
    Font font;
    Text text;
    float text_height;
public:
    TextBox();

    void SetCharacterSize(unsigned int size);
    void SetString(const char *buffer);
    void SetPosition(Vector2f pos);
    void SetFillColor(Color color);
    void Center(Vector2f start_pos, Vector2f size);

    void Draw(RenderWindow &window);
};