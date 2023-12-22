#include "RoundedRectangle.hpp"

RoundedRectangle::RoundedRectangle()
{
    pos.x = 0;
    pos.y = 0;
    rounded_size = 100;
    size.x = 400;
    size.y = 400;
    base.setPointCount(8);

    base.setPoint(0, Vector2f(100, 0));
    base.setPoint(1, Vector2f(300, 0));
    base.setPoint(2, Vector2f(400, 100));
    base.setPoint(3, Vector2f(400, 300));
    base.setPoint(4, Vector2f(300, 400));
    base.setPoint(5, Vector2f(100, 400));
    base.setPoint(6, Vector2f(0, 300));
    base.setPoint(7, Vector2f(0, 100));

    for(int i = 0; i < 4; i++)
    {
        circles[i].setPointCount(50);
        circles[i].setRadius(rounded_size);
        switch (i)
        {
            case 0:
                circles[i].setPosition(Vector2f(0,0));
                break;
            case 1:
                circles[i].setPosition(Vector2f(200,0));
                break;
            case 2:
                circles[i].setPosition(Vector2f(0,200));
                break;
            case 3:
                circles[i].setPosition(Vector2f(200,200));
                break;
            default:
                break;
        }
    }

}

bool RoundedRectangle::Contains(Vector2f pos)
{
    return pos.x >= this->pos.x && pos.x <= this->pos.x + size.x && pos.y >= this->pos.y && pos.y <= this->pos.y + size.y;
}

void RoundedRectangle::SetSize(Vector2f size)
{
    this->size = size;
    base.setPoint(0, Vector2f(pos.x + rounded_size         , pos.y));
    base.setPoint(1, Vector2f(pos.x + size.x - rounded_size, pos.y));
    base.setPoint(2, Vector2f(pos.x + size.x               , pos.y + rounded_size));
    base.setPoint(3, Vector2f(pos.x + size.x               , pos.y + size.y - rounded_size));
    base.setPoint(4, Vector2f(pos.x + size.x - rounded_size, pos.y + size.y));
    base.setPoint(5, Vector2f(pos.x + rounded_size         , pos.y + size.y));
    base.setPoint(6, Vector2f(pos.x                        , pos.y + size.y - rounded_size));
    base.setPoint(7, Vector2f(pos.x                        , pos.y + rounded_size));

    for(int i = 0; i < 4; i++)
    {
        circles[i].setPointCount(50);
        circles[i].setRadius(rounded_size);
        switch (i)
        {
            case 0:
                circles[i].setPosition(Vector2f(pos.x, pos.y));
                break;
            case 1:
                circles[i].setPosition(Vector2f(pos.x + size.x - 2 * rounded_size, pos.y));
                break;
            case 2:
                circles[i].setPosition(Vector2f(pos.x, pos.y + size.y - 2 * rounded_size));
                break;
            case 3:
                circles[i].setPosition(Vector2f(pos.x + size.x - 2 * rounded_size, pos.y + size.y - 2 * rounded_size));
                break;
            default:
                break;
        }
    }

}

void RoundedRectangle::SetCornerSize(unsigned int size)
{
    rounded_size = size;
    SetSize(this->size);
}

void RoundedRectangle::SetCornerResolution(unsigned int resolution)
{
    for(int i = 0; i < 4; i++)
        circles[i].setPointCount(resolution);
}

void RoundedRectangle::SetFillColor(Color color)
{
    base.setFillColor(color);
    for(int i = 0; i < 4; i++)
        circles[i].setFillColor(color);
}

void RoundedRectangle::SetPosition(Vector2f pos)
{
    this->pos = pos;
    SetSize(this->size);
}

Vector2f RoundedRectangle::GetSize()
{
    return size;
}

Vector2f RoundedRectangle::GetPosition()
{
    return pos;
}

Color RoundedRectangle::GetFillColor()
{
    return base.getFillColor();
}

void RoundedRectangle::Draw(RenderWindow &window)
{
    window.draw(base);
    for(int i = 0; i < 4; i++)
        window.draw(circles[i]);
}
