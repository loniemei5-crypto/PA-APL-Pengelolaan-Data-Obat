#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

json LoadJSON(string filename)
{
    ifstream i(filename);
    json j;
    i >> j;
    return j;
}

void SaveJSON(string filename, json j)
{
    ofstream o(filename);
    o << j.dump(4) << endl;
}

void setColor(int color)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        color);
}

void registerUser(json &users)
{
    string username, password;
    setColor(9);
    cout << "\n===== REGISTER =====\n";
    setColor(7);

    setColor(3);
    cout << "Enter username: ";
    setColor(7);
    cin >> username;

    for (auto &u : users["users"])
    {
        if (u["username"] == username)
        {
            cout << "Username telah digunakan!\n";
            return;
        }
    }

    cout << "Enter password: ";
    cin >> password;

    json u;

    u["username"] = username;
    u["password"] = password;

    // default role register = kasir
    u["role"] = "kasir";

    users["users"].push_back(u);

    void saveJSON("users.json", users);

    cout << "Registrasi berhasil!\n";
}

int main()
{
    cout << "Hello, World!" << endl;
    return 0;
}