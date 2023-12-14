#pragma once
#include <SFML/Graphics.hpp>
#include "ClientData.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "RoundedRectangle.hpp"
#include "CustomUtility.hpp"
#include "InputBox.hpp"

using namespace sf;

class AuthScreen{
    RectangleShape screen_background;
    RoundedRectangle second_background;

    RoundedRectangle user_background;
    RoundedRectangle password_background;

    Button login_button;
    RoundedRectangle login_button_shade;
    Button register_button;
    RoundedRectangle register_button_shade;

    InputBox user_text;
    InputBox password_text;
    TextBox login_text;
    TextBox register_text;
public:
    AuthScreen();

    void Animate(RenderWindow &window);

    void UpdateOnKeyPressed(unsigned int code);
    void UpdateOnClick(Vector2f mouse_pos);

    void Draw(RenderWindow &window);
};