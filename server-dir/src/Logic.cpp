#include "Logic.hpp"

void LoginLogic(ClientInfo &info)
{
    unsigned int server_response;

    char user[20];
    char pass[20];
    recv(info.socket, user, sizeof(user), 0);
    recv(info.socket, pass, sizeof(pass), 0);
    cout << "Recived Login command : User - " << user << " Password - " << pass << '\n';
    if (strcmp(user, "") == 0 || strcmp(pass, "") == 0)
    {
        server_response = LogicCodes::VoidInput;
    }
    else
    {
        string login_query = "SELECT CASE WHEN COUNT(*) > 0 THEN MIN(id) ELSE -1 END AS user_id FROM users WHERE username = '";
        login_query += user;
        login_query += "' AND password = '";
        login_query += pass;
        login_query += "';";
        unsigned int querry_result;
        info.db->SelectData(login_query.c_str(), CheckLoginInfoInDb, querry_result);
        if (querry_result != DBCodes::NotFound)
        {
            server_response = LogicCodes::Success;
            info.perm_id = querry_result;
        }
        else
            server_response = LogicCodes::InvalidLoginInfo;
    }
    send(info.socket, &server_response, sizeof(server_response), 0);
}

int CheckLoginInfoInDb(void *data, int argc, char **argv, char **colNames)
{
    unsigned int *result = static_cast<unsigned int *>(data);
    unsigned int temp_result;
    if (atoi(argv[0]) == -1)
        temp_result = DBCodes::NotFound;
    else
        temp_result = atoi(argv[0]);
    *result = temp_result;
    return 0;
}

void RegisterLogic(ClientInfo &info)
{
    unsigned int server_response;

    char user[20];
    char pass[20];
    recv(info.socket, user, sizeof(user), 0);
    recv(info.socket, pass, sizeof(pass), 0);
    cout << "Recived Register command : User - " << user << " Password - " << pass << '\n';
    if (strcmp(user, "") == 0 || strcmp(pass, "") == 0)
    {
        server_response = LogicCodes::VoidInput;
    }
    else
    {
        string login_query = "SELECT CASE WHEN COUNT(*) > 0 THEN MIN(id) ELSE -1 END AS user_id FROM users WHERE username = '";
        login_query += user;
        login_query += "';";
        unsigned int querry_result;
        info.db->SelectData(login_query.c_str(), CheckRegisterInfoInDb, querry_result);
        if (querry_result == DBCodes::AlreadyInDb)
        {
            server_response = LogicCodes::InvalidRegInfo;
        }
        else
        {
            string insert_querry = "INSERT INTO users (username, password) VALUES ('";
            insert_querry += user;
            insert_querry += "', '";
            insert_querry += pass;
            insert_querry += "');";
            info.db->InsertData(insert_querry.c_str());
            server_response = LogicCodes::Success;

            login_query = "SELECT CASE WHEN COUNT(*) > 0 THEN MIN(id) ELSE -1 END AS user_id FROM users WHERE username = '";
            login_query += user;
            login_query += "' AND password = '";
            login_query += pass;
            login_query += "';";
            info.db->SelectData(login_query.c_str(), CheckLoginInfoInDb, querry_result);
            info.perm_id = querry_result;
        }
    }
    send(info.socket, &server_response, sizeof(server_response), 0);
}

int CheckRegisterInfoInDb(void *data, int argc, char **argv, char **colNames)
{
    unsigned int *result = static_cast<unsigned int *>(data);
    unsigned int temp_result;
    if (atoi(argv[0]) != -1)
        temp_result = DBCodes::AlreadyInDb;
    else
        temp_result = atoi(argv[0]);
    *result = temp_result;
    return 0;
}

void ReqUserData(ClientInfo &info)
{
    user_info server_response;
    cout << "Recived ReqUserData command from: " << info.perm_id << '\n';
    server_response.id = info.perm_id;

    char username[20];
    string query = "SELECT USERNAME FROM USERS WHERE ID = " + to_string(info.perm_id);
    query += ";";
    info.db->SelectData(query.c_str(), CheckDataReq, username);

    strcpy(server_response.username, username);
    send(info.socket, &server_response, sizeof(server_response), 0);
}
int CheckDataReq(void *data, int argc, char **argv, char **colNames)
{
    char result[20];
    strcpy(result, argv[0]);
    char *result_ptr = static_cast<char *>(data);
    strcpy(result_ptr, result);
    return 0;
}

void BoardCommandLogic(ClientInfo &info, Engine &engine, game_info &game)
{
    cout << "Recived BoardCommand command from: " << info.perm_id << '\n';
    unsigned int client_command;
    recv(info.socket, &client_command, sizeof(client_command), 0);
    if (client_command == BoardCommands::FirstPos)
    {
        int x, y;
        recv(info.socket, &x, sizeof(x), 0);
        recv(info.socket, &y, sizeof(y), 0);
        game.first_pos = Vector2i(x, y);
        unsigned int server_response;

        if (engine.SelectSquare(game.first_pos))
        {
            server_response = BoardCommands::SelectSquare;
            send(info.socket, &server_response, sizeof(server_response), 0);
            bool selected_sq[8][8];
            engine.HighlightLegalMoves(Vector2i(x, y));
            engine.GetHighlightedArray(selected_sq);
            send(info.socket, &selected_sq, sizeof(selected_sq), 0);
        }
        else
        {
            server_response = BoardCommands::None;
            send(info.socket, &server_response, sizeof(server_response), 0);
        }
    }
    else if (client_command == BoardCommands::SecondPos)
    {
        int x, y;
        recv(info.socket, &x, sizeof(x), 0);
        recv(info.socket, &y, sizeof(y), 0);
        game.second_pos = Vector2i(x, y);
        unsigned int server_response;
        if (engine.MakeMove(game.first_pos, game.second_pos))
            server_response = BoardCommands::MovePiece;
        else
            server_response = BoardCommands::None;
        send(info.socket, &server_response, sizeof(server_response), 0);
        if (server_response == BoardCommands::MovePiece)
        {
            if (engine.PawnNeedsPromotion())
            {
                server_response = BoardCommands::NeedPromotion;
                send(info.socket, &server_response, sizeof(server_response), 0);
            }
            else
            {
                server_response = BoardCommands::None;
                send(info.socket, &server_response, sizeof(server_response), 0);
            }
        }
    }
    else if (client_command == BoardCommands::PromotePawn)
    {
        cout << "recived promote pawn\n";
        unsigned int piece_id;
        recv(info.socket, &piece_id, sizeof(piece_id), 0);
        engine.PromotePawn(piece_id);
        unsigned int server_response = BoardCommands::None;
        send(info.socket, &server_response, sizeof(server_response), 0);
    }
}
