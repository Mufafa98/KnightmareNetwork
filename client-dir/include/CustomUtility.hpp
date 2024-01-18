#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

Color DarkenColor(const Color color, const float amount);
bool StartsWith(const string &str, const string &prefix);