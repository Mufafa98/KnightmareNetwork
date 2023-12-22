#include "CustomUtility.hpp"

Color DarkenColor(const Color color, const float amount)
{
    return Color(color.r * amount, color.g * amount, color.b * amount);
}

