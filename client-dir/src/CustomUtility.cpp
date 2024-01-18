#include "CustomUtility.hpp"

Color DarkenColor(const Color color, const float amount)
{
    return Color(color.r * amount, color.g * amount, color.b * amount);
}

bool StartsWith(const string &str, const string &prefix)
{
    return str.compare(0, prefix.length(), prefix) == 0;
}
