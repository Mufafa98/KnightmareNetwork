#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "TextBox.hpp"

using namespace sf;
using namespace std;

class InputBox : public TextBox
{
    bool modified = false;
    bool selected = false;
    unsigned short type;
    Vector2f bg_size, bg_pos;
    string buffer;

public:
    enum Type
    {
        Text,
        Password,
        Message,
        Search
    };
    void SetBgSize(Vector2f size);
    void SetBgPosition(Vector2f pos);
    void SetIBFillColor(Color color);

    void SetType(unsigned short type);

    string GetBuffer();
    Vector2f GetBgSize();
    Vector2f GetBgPosition();
    void UpdateText(unsigned int code, unsigned int limit);
    void Reset();

    void Select();
    void DeSelect();

    bool GetSelected();
};
