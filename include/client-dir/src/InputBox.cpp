#include "InputBox.hpp"

void InputBox::SetBgSize(Vector2f size)
{
    bg_size = size;
}

void InputBox::SetBgPosition(Vector2f pos)
{
    bg_pos = pos;
}

void InputBox::SetType(unsigned short type)
{
    this->type = type;
}

string InputBox::GetBuffer()
{
    return buffer;
}

void InputBox::UpdateText(unsigned int code)
{
    if (modified == false)
        text.setString("");
    string current_text = text.getString();
    switch (code)
    {
    case 8: // backspace
        if (current_text.size() > 0)
        {
            current_text.erase(current_text.length() - 1);
            buffer.erase(buffer.length() - 1);
        }
        break;

    default:
        if (current_text.size() > 13)
            break;
        if ((isalnum(code) || ispunct(code)) && !isspace(code))
        {
            if (type == InputBox::Type::Text)
            {
                current_text += code;
                buffer = current_text;
            }
            else if (type == InputBox::Type::Password)
            {
                current_text += '*';
                if (modified == false)
                    buffer = "";
                buffer += code;
            }
            modified = true;
        }
        break;
    }
    text.setString(current_text);
    Center(bg_pos, bg_size);
}

void InputBox::Select()
{
    selected = true;
}

void InputBox::DeSelect()
{
    selected = false;
    if (modified == false)
    {
        if (type == InputBox::Type::Text)
            text.setString("User");
        else if (type == InputBox::Type::Password)
            text.setString("Password");
        Center(bg_pos, bg_size);
    }
}

bool InputBox::GetSelected()
{
    return selected;
}
