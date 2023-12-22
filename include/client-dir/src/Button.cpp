#include "Button.hpp"

Button::Button()
{
    background.SetSize(Vector2f(WINDOW_WIDTH - 800, WINDOW_HEIGHT - 200));
    start_pos = background.GetPosition();
    end_pos = background.GetPosition() + background.GetSize();
}

bool Button::Contains(Vector2f pos)
{
    return background.Contains(pos);
}

void Button::SetFillColor(Color color)
{
    background.SetFillColor(color);
}

void Button::SetPosition(Vector2f pos)
{
    background.SetPosition(pos);
    start_pos = background.GetPosition();
    end_pos = background.GetPosition() + background.GetSize();
}

void Button::SetSize(Vector2f size)
{
    background.SetSize(size);
    start_pos = background.GetPosition();
    end_pos = background.GetPosition() + background.GetSize();
}

void Button::SetCornerSize(unsigned int size)
{
    background.SetCornerSize(size);
}

Color Button::GetFillColor()
{
    return background.GetFillColor();
}

Vector2f Button::GetPosition()
{
    return background.GetPosition();
}

Vector2f Button::GetSize()
{
    return background.GetSize();
}

void Button::Draw(RenderWindow &window)
{
    background.Draw(window);
}
