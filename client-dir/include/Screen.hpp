#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include "Constants.hpp"

#include "ClientData.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "RoundedRectangle.hpp"
#include "CustomUtility.hpp"
#include "InputBox.hpp"

using namespace sf;

class Screen
{
public:
    virtual void UpdateOnFileDescriptor(int fd) = 0;

    virtual void Animate(RenderWindow &window) = 0;

    virtual void UpdateOnKeyPressed(unsigned int code) = 0;
    virtual void UpdateOnClick(Vector2f mouse_pos) = 0;
    virtual void Restart() = 0;

    virtual unsigned int LogicOnClick(Vector2f mouse_pos) = 0;

    virtual void Draw(RenderWindow &window) = 0;
};