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
    strcpy(info.username, username);
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

void BoardCommandLogic(ClientInfo &info, Engine &engine, GameInfo &game)
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
        if (engine.MakeMoveIfLegal(game.first_pos, game.second_pos, 0))
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

// void BoardCommandMatchLogic(ClientInfo &info, Engine &engine, GameInfo &game)
// {
//     unsigned int client_command;
//     unsigned int server_response = BoardCommands::None;
//     if (game.color != engine.GetTurn())
//         server_response = BoardCommands::NotYourTurn;
//     else
//         server_response = BoardCommands::YourTurn;
//     send(info.socket, &server_response, sizeof(server_response), 0);
//     recv(info.socket, &client_command, sizeof(client_command), 0);
//     if (client_command == BoardCommands::FirstPos)
//     {
//         int x, y;
//         recv(info.socket, &x, sizeof(x), 0);
//         recv(info.socket, &y, sizeof(y), 0);
//         game.first_pos = Vector2i(x, y);
//         if (engine.SelectSquare(game.first_pos))
//         {
//             server_response = BoardCommands::SelectSquare;
//             send(info.socket, &server_response, sizeof(server_response), 0);
//             bool selected_sq[8][8];
//             engine.HighlightLegalMoves(Vector2i(x, y));
//             engine.GetHighlightedArray(selected_sq);
//             send(info.socket, &selected_sq, sizeof(selected_sq), 0);
//         }
//         else
//         {
//             server_response = BoardCommands::None;
//             send(info.socket, &server_response, sizeof(server_response), 0);
//         }
//     }
//     else if (client_command == BoardCommands::SecondPos)
//     {
//         int x, y;
//         recv(info.socket, &x, sizeof(x), 0);
//         recv(info.socket, &y, sizeof(y), 0);
//         game.second_pos = Vector2i(x, y);
//         unsigned int server_response;
//         if (engine.MakeMoveIfLegal(game.first_pos, game.second_pos, 0))
//             server_response = BoardCommands::MovePiece;
//         else
//             server_response = BoardCommands::None;
//         send(info.socket, &server_response, sizeof(server_response), 0);
//         if (server_response == BoardCommands::MovePiece)
//         {
//             if (engine.PawnNeedsPromotion())
//             {
//                 server_response = BoardCommands::NeedPromotion;
//                 send(info.socket, &server_response, sizeof(server_response), 0);
//             }
//             else
//             {
//                 server_response = BoardCommands::None;
//                 send(info.socket, &server_response, sizeof(server_response), 0);
//             }
//         }
//     }
//     else if (client_command == BoardCommands::PromotePawn)
//     {
//         cout << "recived promote pawn\n";
//         unsigned int piece_id;
//         recv(info.socket, &piece_id, sizeof(piece_id), 0);
//         engine.PromotePawn(piece_id);
//         unsigned int server_response = BoardCommands::None;
//         send(info.socket, &server_response, sizeof(server_response), 0);
//     }
// }

stack<int> waiting_list;
mutex waiting_list_mutex;

unsigned int PlayLogic(ClientInfo &info, Engine &engine, GameInfo &game)
{
    unique_lock<mutex> lock(waiting_list_mutex);
    ClientInfo opponent_info;
    bool game_saved = false;

    if (waiting_list.empty())
    {
        int local_socket[2];
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, local_socket) == -1)
            cerr << "Error creating socket pair " << info.perm_id << '\n';
        waiting_list.push(local_socket[0]);
        lock.unlock();
        cout << "waiting for opponent\n";
        info.thread_socket = local_socket[1];

        unsigned int internal_command;
        recv(info.thread_socket, &internal_command, sizeof(internal_command), 0);
        game.color = !internal_command;
        recv(info.thread_socket, &opponent_info, sizeof(opponent_info), 0);
        send(info.thread_socket, &info, sizeof(info), 0);
    }
    else
    {
        info.thread_socket = waiting_list.top();
        waiting_list.pop();
        lock.unlock();

        cout << "opponent found\n";
        unsigned int internal_command = generateIntRandom(info.seed, 0, 1);
        game.color = internal_command;
        send(info.thread_socket, &internal_command, sizeof(internal_command), 0);
        send(info.thread_socket, &info, sizeof(info), 0);
        recv(info.thread_socket, &opponent_info, sizeof(opponent_info), 0);
    }
    unsigned int client_command;
    unsigned int thread_command;
    unsigned int server_response = InternalLogic::OpponentFound;
    send(info.socket, &server_response, sizeof(server_response), 0);
    server_response = !game.color;
    send(info.socket, &server_response, sizeof(server_response), 0);
    send(info.socket, opponent_info.username, sizeof(opponent_info.username), 0);

    engine.Restart();

    while (true)
    {
        int flags = fcntl(info.thread_socket, F_GETFL, 0);
        fcntl(info.thread_socket, F_SETFL, flags | O_NONBLOCK);
        flags = fcntl(info.socket, F_GETFL, 0);
        fcntl(info.socket, F_SETFL, flags | O_NONBLOCK);
        if (recv(info.thread_socket, &thread_command, sizeof(thread_command), 0) > 0)
        {
            flags = fcntl(info.thread_socket, F_GETFL, 0);
            fcntl(info.thread_socket, F_SETFL, flags & ~O_NONBLOCK);
            if (thread_command == BoardCommands::MovePiece)
            {
                int x1, x2, y1, y2;
                recv(info.thread_socket, &x1, sizeof(x1), 0);
                recv(info.thread_socket, &y1, sizeof(y1), 0);
                recv(info.thread_socket, &x2, sizeof(x2), 0);
                recv(info.thread_socket, &y2, sizeof(y2), 0);
                unsigned int server_response;
                if (engine.MakeMoveIfLegal(Vector2i(x1, y1), Vector2i(x2, y2), 3))
                    server_response = BoardCommands::MovePieceAnimate;
                else
                    server_response = BoardCommands::None;
                send(info.socket, &server_response, sizeof(server_response), 0);
                if (server_response == BoardCommands::MovePieceAnimate)
                {
                    send(info.socket, &x1, sizeof(x1), 0);
                    send(info.socket, &y1, sizeof(y1), 0);
                    send(info.socket, &x2, sizeof(x2), 0);
                    send(info.socket, &y2, sizeof(y2), 0);
                }
                server_response = BoardCommands::None;
                send(info.socket, &server_response, sizeof(server_response), 0);
            }
            else if (thread_command == BoardCommands::PromotePawn)
            {
                unsigned int piece_id;
                unsigned int server_response = BoardCommands::PromotePawnAnimate;
                recv(info.thread_socket, &piece_id, sizeof(piece_id), 0);
                engine.PromotePawn(piece_id);
                send(info.socket, &server_response, sizeof(server_response), 0);
                send(info.socket, &piece_id, sizeof(piece_id), 0);
            }
            else if (thread_command == BoardCommands::GameOver)
            {
                send(info.socket, &thread_command, sizeof(thread_command), 0);
            }
            else if (thread_command == BoardCommands::GameSaved)
            {
                game_saved = true;
            }
            else if (thread_command == Commands::Chat)
            {
                char message[130];
                recv(info.thread_socket, message, sizeof(message), 0);
                server_response = thread_command;
                send(info.socket, &server_response, sizeof(server_response), 0);
                send(info.socket, message, sizeof(message), 0);
            }
        }
        if (recv(info.socket, &client_command, sizeof(client_command), 0) > 0)
        {
            flags = fcntl(info.socket, F_GETFL, 0);
            fcntl(info.socket, F_SETFL, flags & ~O_NONBLOCK);
            if (client_command == Commands::BoardCommandMatch)
            {
                if (game.color != engine.GetTurn())
                    server_response = BoardCommands::NotYourTurn;
                else
                    server_response = BoardCommands::YourTurn;
                // server_response = BoardCommands::YourTurn;
                send(info.socket, &server_response, sizeof(server_response), 0);
                if (server_response == BoardCommands::YourTurn || engine.PawnNeedsPromotion())
                {
                    recv(info.socket, &client_command, sizeof(client_command), 0);
                    if (client_command == BoardCommands::FirstPos)
                    {
                        int x, y;
                        recv(info.socket, &x, sizeof(x), 0);
                        recv(info.socket, &y, sizeof(y), 0);
                        game.first_pos = Vector2i(x, y);

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
                        if (engine.MakeMoveIfLegal(game.first_pos, game.second_pos, 0))
                            server_response = BoardCommands::MovePiece;
                        else
                            server_response = BoardCommands::UnableToMovePiece;
                        send(info.socket, &server_response, sizeof(server_response), 0);
                        if (server_response == BoardCommands::MovePiece)
                        {
                            int x1 = game.first_pos.x;
                            int y1 = game.first_pos.y;
                            int x2 = game.second_pos.x;
                            int y2 = game.second_pos.y;
                            send(info.socket, &x1, sizeof(x1), 0);
                            send(info.socket, &y1, sizeof(y1), 0);
                            send(info.socket, &x2, sizeof(x2), 0);
                            send(info.socket, &y2, sizeof(y2), 0);
                            send(info.thread_socket, &server_response, sizeof(server_response), 0);
                            send(info.thread_socket, &x1, sizeof(x1), 0);
                            send(info.thread_socket, &y1, sizeof(y1), 0);
                            send(info.thread_socket, &x2, sizeof(x2), 0);
                            send(info.thread_socket, &y2, sizeof(y2), 0);
                            if (engine.PawnNeedsPromotion())
                            {
                                server_response = BoardCommands::NeedPromotion;
                                send(info.socket, &server_response, sizeof(server_response), 0);
                            }
                            else
                            {
                                engine.CheckEndConditions();
                                if (!engine.GameRunning())
                                {
                                    server_response = BoardCommands::GameOver;
                                    send(info.thread_socket, &server_response, sizeof(server_response), 0);
                                }
                                else
                                {
                                    server_response = BoardCommands::None;
                                }
                                send(info.socket, &server_response, sizeof(server_response), 0);
                            }
                        }
                        else if (server_response == BoardCommands::UnableToMovePiece)
                        {
                            int x, y;
                            recv(info.socket, &x, sizeof(x), 0);
                            recv(info.socket, &y, sizeof(y), 0);
                            game.first_pos = Vector2i(x, y);

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
                    }
                    else if (client_command == BoardCommands::PromotePawn)
                    {
                        unsigned int piece_id;
                        recv(info.socket, &piece_id, sizeof(piece_id), 0);
                        engine.PromotePawn(piece_id);
                        send(info.thread_socket, &client_command, sizeof(client_command), 0);
                        send(info.thread_socket, &piece_id, sizeof(piece_id), 0);
                    }
                }
            }
            else if (client_command == Commands::Exit)
            {
                return Commands::Exit;
            }
            else if (client_command == Commands::Chat)
            {
                char message[130];
                recv(info.socket, message, sizeof(message), 0);
                string temp = "[";
                temp += info.username;
                temp += "]: ";
                temp += message;
                strcpy(message, temp.c_str());
                send(info.socket, message, sizeof(message), 0);
                server_response = client_command;
                send(info.thread_socket, &server_response, sizeof(server_response), 0);
                send(info.thread_socket, message, sizeof(message), 0);
            }
            else if (client_command == Commands::SaveMatch)
            {
                engine.CheckEndConditions();
                string game_poss = START_CHESS_POS;
                game_poss += "&";
                game_poss += engine.GetPlayedMoves();
                server_response = BoardCommands::GameOver;
                send(info.thread_socket, &server_response, sizeof(server_response), 0);
                auto current_tyme_point = chrono::system_clock::now();
                time_t current_time = chrono::system_clock::to_time_t(current_tyme_point);
                tm *local_time = localtime(&current_time);
                string time = "";
                time += to_string(local_time->tm_mday) + "/" + to_string(local_time->tm_mon + 1);
                time += "/" + to_string(local_time->tm_year + 1900) + " ";
                time += to_string(local_time->tm_hour) + ":" + to_string(local_time->tm_min);
                time += ":" + to_string(local_time->tm_sec);
                string querry = "INSERT INTO games (white_player, black_player, game, time) VALUES (";
                if (game.color == 0)
                {
                    querry += to_string(info.perm_id);
                    querry += ", ";
                    querry += to_string(opponent_info.perm_id);
                    querry += ", '";
                }
                else
                {
                    querry += to_string(opponent_info.perm_id);
                    querry += ", ";
                    querry += to_string(info.perm_id);
                    querry += ", '";
                }
                querry += game_poss;
                querry += "', '";
                querry += time;
                querry += "');";
                if (game_saved)
                    return -1;
                else
                {
                    info.db->InsertData(querry.c_str());
                    game_saved = true;
                    server_response = BoardCommands::GameSaved;
                    send(info.thread_socket, &server_response, sizeof(server_response), 0);
                    return -1;
                }
            }
        }
    }
    return -1;
}
