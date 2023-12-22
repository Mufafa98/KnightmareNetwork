#include "LoginLogic.hpp"

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
