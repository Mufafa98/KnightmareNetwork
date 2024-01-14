#pragma once
#include <SFML/Graphics.hpp>
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200
#define WINDOW_TITLE "KnightmareNetwork"

#define BOARD_WIDTH 800
#define BOARD_HEIGHT 800
#define SQUARE_SIZE BOARD_WIDTH / 8
#define SELECTED_SQ_COLOR sf::Color(124, 252, 0, 50)
#define LEGAL_MOVE sf::Color(0, 255, 255, 50)
#define ATTACKED sf::Color(255, 0, 0, 50)
#define PROMOTE_BG sf::Color(239, 225, 213)

#define NO_OF_SCREENS 3

#define SCREEN_BACKGROUND_COLOR sf::Color(162, 155, 144)
#define SECOND_BACKGROUND_COLOR sf::Color(64, 61, 57)
#define THIRD_BACKGROUND_COLOR sf::Color(37, 36, 34)
#define TEXT_BACKGROUND_COLOR THIRD_BACKGROUND_COLOR
#define BUTTON_BACKGROUND_COLOR sf::Color(235, 94, 40)
#define BUTTON_BACKGROUND_COLOR_V2 sf::Color(46, 45, 43)
#define TEXT_FONT_COLOR sf::Color(255, 252, 242)
