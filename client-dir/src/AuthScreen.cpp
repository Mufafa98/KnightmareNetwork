#include "AuthScreen.hpp"

AuthScreen::AuthScreen()
{
    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    float start_ui_y = WINDOW_HEIGHT / 2 - (200 + 120 + 160) / 2;
    float padding = 60;
    float button_width = WINDOW_WIDTH - 800 - 2 * padding;

    second_background.SetSize(Vector2f(WINDOW_WIDTH - 800, WINDOW_HEIGHT - 200));
    second_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    second_background.SetPosition(Vector2f(400, 100));
    second_background.SetCornerSize(20);

    user_background.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 60));
    user_background.SetFillColor(TEXT_BACKGROUND_COLOR);
    user_background.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, start_ui_y + 30));
    user_background.SetCornerSize(10);

    password_background.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 60));
    password_background.SetFillColor(TEXT_BACKGROUND_COLOR);
    password_background.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, user_background.GetPosition().y + user_background.GetSize().y + 30));
    password_background.SetCornerSize(10);

    login_button.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 75));
    login_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    login_button.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, password_background.GetPosition().y + password_background.GetSize().y + 80));
    login_button.SetCornerSize(10);

    register_button.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 75));
    register_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    register_button.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, login_button.GetPosition().y + login_button.GetSize().y + 30));
    register_button.SetCornerSize(10);

    login_button_shade.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 80));
    login_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    login_button_shade.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, password_background.GetPosition().y + password_background.GetSize().y + 80));
    login_button_shade.SetCornerSize(10);

    register_button_shade.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 80));
    register_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    register_button_shade.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, login_button.GetPosition().y + login_button.GetSize().y + 30));
    register_button_shade.SetCornerSize(10);

    user_text.SetString("User");
    user_text.Center(user_background.GetPosition(), user_background.GetSize());
    user_text.SetFillColor(TEXT_FONT_COLOR);
    user_text.SetBgSize(user_background.GetSize());
    user_text.SetBgPosition(user_background.GetPosition());
    user_text.SetType(InputBox::Type::Text);

    password_text.SetString("Password");
    password_text.Center(password_background.GetPosition(), password_background.GetSize());
    password_text.SetFillColor(TEXT_FONT_COLOR);
    password_text.SetBgSize(password_background.GetSize());
    password_text.SetBgPosition(password_background.GetPosition());
    password_text.SetType(InputBox::Type::Password);

    login_text.SetString("Login");
    login_text.SetCharacterSize(40);
    login_text.Center(login_button.GetPosition(), login_button.GetSize());

    register_text.SetString("Register");
    register_text.SetCharacterSize(40);
    register_text.Center(register_button.GetPosition(), register_button.GetSize());

    error_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    error_background.SetCornerSize(10);
    error_background.SetPosition(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

    error_text.SetString("");
    error_text.SetCharacterSize(20);

    error_appeared = false;
}

void AuthScreen::UpdateOnFileDescriptor(int fd)
{
    this->fd = fd;
}

void AuthScreen::Animate(RenderWindow &window)
{
    Vector2f mouse_pos = window.mapPixelToCoords(Mouse::getPosition(window));
    if (login_button.Contains(mouse_pos))
        login_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.9));
    else
        login_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    if (register_button.Contains(mouse_pos))
        register_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.9));
    else
        register_button.SetFillColor(BUTTON_BACKGROUND_COLOR);

    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (login_button.Contains(mouse_pos))
            login_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.8));
        else
            login_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
        if (register_button.Contains(mouse_pos))
            register_button.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.8));
        else
            register_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    }
    if (error_appeared)
    {
        if (error_background.GetPosition().x > WINDOW_WIDTH - 10 - error_background.GetSize().x)
        {
            error_background.SetPosition(error_background.GetPosition() - Vector2f(4.5, 0));
            error_text.Center(error_background.GetPosition(), error_background.GetSize());
        }
    }
    else
    {
        if (error_background.GetPosition().x < WINDOW_WIDTH)
        {
            error_background.SetPosition(error_background.GetPosition() + Vector2f(4.5, 0));
            error_text.Center(error_background.GetPosition(), error_background.GetSize());
        }
    }
}

void AuthScreen::UpdateOnKeyPressed(unsigned int code)
{
    if (user_text.GetSelected())
    {
        user_text.UpdateText(code, 13);
        user_text.Center(user_text.GetBgPosition(), user_text.GetBgSize());
    }
    if (password_text.GetSelected())
    {
        password_text.UpdateText(code, 13);
        password_text.Center(password_text.GetBgPosition(), password_text.GetBgSize());
    }
}

void AuthScreen::UpdateOnClick(Vector2f mouse_pos)
{
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        if (user_background.Contains(mouse_pos))
            user_text.Select();
        else
        {
            user_text.DeSelect();
            user_text.Center(user_text.GetBgPosition(), user_text.GetBgSize());
        }
        if (password_background.Contains(mouse_pos))
            password_text.Select();
        else
        {
            password_text.DeSelect();
            password_text.Center(password_text.GetBgPosition(), password_text.GetBgSize());
        }
        if (error_background.Contains(mouse_pos))
            error_appeared = false;
    }
}

void AuthScreen::Restart()
{
    screen_background.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    screen_background.setFillColor(SCREEN_BACKGROUND_COLOR);

    float start_ui_y = WINDOW_HEIGHT / 2 - (200 + 120 + 160) / 2;
    float padding = 60;
    float button_width = WINDOW_WIDTH - 800 - 2 * padding;

    second_background.SetSize(Vector2f(WINDOW_WIDTH - 800, WINDOW_HEIGHT - 200));
    second_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    second_background.SetPosition(Vector2f(400, 100));
    second_background.SetCornerSize(20);

    user_background.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 60));
    user_background.SetFillColor(TEXT_BACKGROUND_COLOR);
    user_background.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, start_ui_y + 30));
    user_background.SetCornerSize(10);

    password_background.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 60));
    password_background.SetFillColor(TEXT_BACKGROUND_COLOR);
    password_background.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, user_background.GetPosition().y + user_background.GetSize().y + 30));
    password_background.SetCornerSize(10);

    login_button.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 75));
    login_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    login_button.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, password_background.GetPosition().y + password_background.GetSize().y + 80));
    login_button.SetCornerSize(10);

    register_button.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 75));
    register_button.SetFillColor(BUTTON_BACKGROUND_COLOR);
    register_button.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, login_button.GetPosition().y + login_button.GetSize().y + 30));
    register_button.SetCornerSize(10);

    login_button_shade.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 80));
    login_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    login_button_shade.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, password_background.GetPosition().y + password_background.GetSize().y + 80));
    login_button_shade.SetCornerSize(10);

    register_button_shade.SetSize(Vector2f(second_background.GetSize().x - 2 * padding, 80));
    register_button_shade.SetFillColor(DarkenColor(BUTTON_BACKGROUND_COLOR, 0.7));
    register_button_shade.SetPosition(Vector2f(WINDOW_WIDTH / 2 - button_width / 2, login_button.GetPosition().y + login_button.GetSize().y + 30));
    register_button_shade.SetCornerSize(10);

    user_text.SetString("User");
    user_text.Center(user_background.GetPosition(), user_background.GetSize());
    user_text.SetFillColor(TEXT_FONT_COLOR);
    user_text.SetBgSize(user_background.GetSize());
    user_text.SetBgPosition(user_background.GetPosition());
    user_text.SetType(InputBox::Type::Text);

    password_text.SetString("Password");
    password_text.Center(password_background.GetPosition(), password_background.GetSize());
    password_text.SetFillColor(TEXT_FONT_COLOR);
    password_text.SetBgSize(password_background.GetSize());
    password_text.SetBgPosition(password_background.GetPosition());
    password_text.SetType(InputBox::Type::Password);

    login_text.SetString("Login");
    login_text.SetCharacterSize(40);
    login_text.Center(login_button.GetPosition(), login_button.GetSize());

    register_text.SetString("Register");
    register_text.SetCharacterSize(40);
    register_text.Center(register_button.GetPosition(), register_button.GetSize());

    error_background.SetFillColor(SECOND_BACKGROUND_COLOR);
    error_background.SetCornerSize(10);
    error_background.SetPosition(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

    error_text.SetString("");
    error_text.SetCharacterSize(20);

    error_appeared = false;
}

unsigned int AuthScreen::LogicOnClick(Vector2f mouse_pos)
{
    if (Mouse::isButtonPressed(Mouse::Button::Left))
    {
        unsigned int client_command;
        unsigned int server_response;
        char user[20];
        char pass[20];
        strcpy(user, user_text.GetBuffer().c_str());
        strcpy(pass, password_text.GetBuffer().c_str());
        if (login_button.Contains(mouse_pos))
        {
            client_command = Commands::Login;
            send(fd, &client_command, sizeof(client_command), 0);
            send(fd, user, sizeof(user), 0);
            send(fd, pass, sizeof(pass), 0);
            recv(fd, &server_response, sizeof(server_response), 0);
            switch (server_response)
            {
            case LogicCodes::Success:
            {
                return ScreenCommand::MenuS;
                break;
            }
            case LogicCodes::InvalidLoginInfo:
            {
                error_text.SetString("Incorect username or password");
                error_background.SetSize(Vector2f(error_text.GetSize().x + 30, error_text.GetSize().y + 30));
                error_background.SetPosition(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - 10 - error_background.GetSize().y));
                error_appeared = true;
                break;
            }
            case LogicCodes::VoidInput:
            {
                error_text.SetString("Please insert an username and a password");
                error_background.SetSize(Vector2f(error_text.GetSize().x + 30, error_text.GetSize().y + 30));
                error_background.SetPosition(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - 10 - error_background.GetSize().y));
                error_appeared = true;
                break;
            }
            default:
                break;
            }
        }
        if (register_button.Contains(mouse_pos))
        {
            client_command = Commands::Register;
            send(fd, &client_command, sizeof(client_command), 0);
            send(fd, user, sizeof(user), 0);
            send(fd, pass, sizeof(pass), 0);
            recv(fd, &server_response, sizeof(server_response), 0);
            switch (server_response)
            {
            case LogicCodes::Success:
            {
                return ScreenCommand::MenuS;
                break;
            }
            case LogicCodes::InvalidRegInfo:
            {
                error_text.SetString("User already registred");
                error_background.SetSize(Vector2f(error_text.GetSize().x + 30, error_text.GetSize().y + 30));
                error_background.SetPosition(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - 10 - error_background.GetSize().y));
                error_appeared = true;
                break;
            }
            case LogicCodes::VoidInput:
            {
                error_text.SetString("Please insert an username and a password");
                error_background.SetSize(Vector2f(error_text.GetSize().x + 30, error_text.GetSize().y + 30));
                error_background.SetPosition(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - 10 - error_background.GetSize().y));
                error_appeared = true;
                break;
            }
            default:
                break;
            }
        }
    }
    return ScreenCommand::NoneS;
}

void AuthScreen::Draw(RenderWindow &window)
{
    window.draw(screen_background);
    second_background.Draw(window);
    user_background.Draw(window);
    password_background.Draw(window);

    login_button_shade.Draw(window);
    register_button_shade.Draw(window);

    login_button.Draw(window);
    register_button.Draw(window);

    user_text.Draw(window);
    password_text.Draw(window);
    login_text.Draw(window);
    register_text.Draw(window);

    error_background.Draw(window);
    error_text.Draw(window);
}
