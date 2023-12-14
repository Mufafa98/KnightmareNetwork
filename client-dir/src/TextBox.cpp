#include "TextBox.hpp"

TextBox::TextBox()
{
    font.loadFromFile("./client-dir/assets/coolvetica rg.otf");
    text.setFont(font);
    text.setCharacterSize(24);
    text.setString("Q");
    text.setFillColor(Color::White);
    text_height = text.getGlobalBounds().getSize().y;
}


void TextBox::SetCharacterSize(unsigned int size)
{
    string temp = text.getString();
    text.setString("Q");
    text.setCharacterSize(size);
    text_height = text.getGlobalBounds().getSize().y;
    text.setString(temp);
}

void TextBox::SetString(const char *buffer)
{
    text.setString(buffer);
}

void TextBox::SetPosition(Vector2f pos)
{
    text.setPosition(pos);
}

void TextBox::SetFillColor(Color color)
{
    text.setFillColor(color);
}

void TextBox::Center(Vector2f start_pos, Vector2f size)
{
    float mid_width = start_pos.x + size.x / 2.0;
    float mid_height = start_pos.y + size.y / 2.0 - 3;
    text.setPosition(Vector2f(mid_width - text.getGlobalBounds().width / 2, mid_height - text_height / 2));
}

void TextBox::Draw(RenderWindow &window)
{
    window.draw(text);
}
