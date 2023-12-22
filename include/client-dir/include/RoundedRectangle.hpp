#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

class RoundedRectangle{
    Vector2f pos;
    Vector2f size;
    float rounded_size;
    ConvexShape base;
    CircleShape circles[4];
public:
    RoundedRectangle();

    bool Contains(Vector2f pos);

    void SetSize(Vector2f size);
    void SetCornerSize(unsigned int size);
    void SetCornerResolution(unsigned int resolution);
    void SetFillColor(Color color);
    void SetPosition(Vector2f pos);

    Vector2f GetSize();
    Vector2f GetPosition();
    Color GetFillColor();

    void Draw(RenderWindow &window);
};