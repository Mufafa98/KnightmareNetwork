#include <iostream>
#include <SFML/Graphics.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include "ClientData.hpp"

#include "Screen.hpp"

#include "AuthScreen.hpp"
#include "Constants.hpp"

#define PORT 8080
#define ADDRESS "127.0.0.1"

using namespace std;
using namespace sf;

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    unsigned int client_command;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("[Client]: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ADDRESS, &server_addr.sin_addr) <= 0)
    {
        perror("[Client]: Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[Client]: Connection failed");
        exit(EXIT_FAILURE);
    }

    // to do signal handeling

    unsigned int current_screen_id = 0;
    vector<Screen *> screen;

    screen.push_back(new AuthScreen());
    screen.push_back(new AuthScreen());
    for (Screen *s : screen)
        s->SetFileDescriptor(client_socket);
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, Style::Close | Style::Titlebar);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        Event window_event;
        while (window.pollEvent(window_event))
        {
            switch (window_event.type)
            {
            case Event::Closed:
            {
                window.close();
                client_command = Commands::Exit;
                send(client_socket, &client_command, sizeof(client_command), 0);
                close(client_socket);
                break;
            }
            case Event::MouseButtonPressed:
            {
                Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));
                screen[current_screen_id]->UpdateOnClick(mouse_pos);
                unsigned int response = screen[current_screen_id]->LogicOnClick(mouse_pos);
                if (response == LogicCodes::NextScreen)
                {
                    current_screen_id++;
                    current_screen_id %= NO_OF_SCREENS;
                }
                break;
            }
            case Event::TextEntered:
            {
                screen[current_screen_id]->UpdateOnKeyPressed(window_event.text.unicode);
                break;
            }
            default:
                break;
            }
        }

        screen[current_screen_id]->Animate(window);

        window.clear();
        screen[current_screen_id]->Draw(window);
        window.display();
    }
    close(client_socket);
    return 0;
}