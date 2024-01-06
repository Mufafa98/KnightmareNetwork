#pragma once
#include <SFML/Graphics.hpp>
#include "ClientData.hpp"
#include "RoundedRectangle.hpp"

using namespace sf;

class Button
{
    Vector2f start_pos, end_pos;
    RoundedRectangle background;

public:
    Button();

    bool Contains(Vector2f pos);

    void SetFillColor(Color color);
    void SetPosition(Vector2f pos);
    void SetSize(Vector2f size);
    void SetCornerSize(unsigned int size);

    Color GetFillColor();
    Vector2f GetPosition();
    Vector2f GetSize();
    void Draw(RenderWindow &window);
};