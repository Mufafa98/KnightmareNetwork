#include <iostream>
#include <SFML/Graphics.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include "ClientData.hpp"
#include "AuthScreen.hpp"
#include "Constants.hpp"

#define PORT 8080
#define ADDRESS "127.0.0.1"


using namespace std;
using namespace sf;

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    unsigned int client_command;
    unsigned int server_response;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[Client]: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ADDRESS, &server_addr.sin_addr) <= 0) {
        perror("[Client]: Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[Client]: Connection failed");
        exit(EXIT_FAILURE);
    }


    AuthScreen screen;
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, Style::Close | Style::Titlebar);
    while(window.isOpen())
    {
        Event window_event;
        while(window.pollEvent(window_event))
        {
            switch(window_event.type)
            {
                case Event::Closed:
                {
                    window.close();
                    client_command = Commands::Exit;
                    write(client_socket, &client_command, sizeof(client_command));
                    close(client_socket);
                    break;
                }
                case Event::MouseButtonPressed:
                {
                    Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));
                    screen.UpdateOnClick(mouse_pos);
                    break;
                }
                case Event::TextEntered:
                {
                    screen.UpdateOnKeyPressed(window_event.text.unicode);
                    break;
                }
                default:
                    break;
            }
        }

        screen.Animate(window);

        window.clear();
        screen.Draw(window);
        window.display();
    }
    close(client_socket);
    return 0;
}