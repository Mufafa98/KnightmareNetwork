#include "TextBox.hpp"

TextBox::TextBox()
{
    font.loadFromFile("./assets/coolvetica rg.otf");
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

Vector2f TextBox::GetSize()
{
    return Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height);
}

Vector2f TextBox::GetPosition()
{
    return text.getPosition();
}

string TextBox::GetString()
{
    return text.getString();
}

void TextBox::Center(Vector2f start_pos, Vector2f size)
{
    float mid_width = start_pos.x + size.x / 2.0;
    float mid_height = start_pos.y + size.y / 2.0 - 3;
    text.setPosition(Vector2f(mid_width - text.getGlobalBounds().width / 2, mid_height - text_height / 2));
}
void TextBox::CenterV(Vector2f start_pos, Vector2f size)
{
    float mid_height = start_pos.y + size.y / 2.0 - 3;
    text.setPosition(Vector2f(start_pos.x, mid_height - text_height / 2.0));
}

unsigned int TextBox::FitWidth(const float &start, const float &end)
{
    const float max_width = end - start - 10;
    unsigned int nl_count = 0;
    string temp = "";
    string text_string = text.getString();
    text.setString(temp);
    for (size_t i = 0; i < text_string.size(); i++)
    {
        temp += text_string[i];
        text.setString(temp);
        if (text.getGlobalBounds().getSize().x >= max_width)
        {
            temp.pop_back();
            temp += "\n";
            nl_count++;
            temp += text_string[i];
        }
        if (text_string[i] == '\n')
            nl_count++;
    }
    return nl_count;
}

void TextBox::Draw(RenderWindow &window)
{
    window.draw(text);
}
