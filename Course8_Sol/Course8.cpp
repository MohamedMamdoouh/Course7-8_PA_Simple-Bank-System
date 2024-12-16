#include <bits/stdc++.h>
using namespace std;

const string Users = "Users.txt";

struct stLoginData
{
    string Username = "";
    string Password = "";
    short Permission = 0;
};

vector<string> SplitString(string OriginalString, string Delimeter)
{
    vector<string> vString;
    short pos = 0;
    string sWord = "";

    while ((pos = OriginalString.find(Delimeter)) != string::npos)
    {
        sWord = OriginalString.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        OriginalString.erase(0, pos + Delimeter.length()); // move to next word
    }

    if (OriginalString != "")
    {
        vString.push_back(OriginalString); // add last word of the string
    }

    return vString;
}

stLoginData ConvertLineToLoginRecord(string Line, string Delimeter = "#//#")
{
    stLoginData Data;
    vector<string> vLoginData;
    vLoginData = SplitString(Line, Delimeter);

    Data.Username = vLoginData[0];
    Data.Password = vLoginData[1];
    Data.Permission = stoi(vLoginData[2]);

    return Data;
}

bool IsValidLoginData(string Username, string Password, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stLoginData Data;

        while (getline(MyFile, Line))
        {
            Data = ConvertLineToLoginRecord(Line);
            if (Data.Username == Username && Data.Password == Password)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }
    return false;
}

// enum enMainMenuOptions
// {
//     eListClients = 1,
//     eAddNewClient = 2,
//     eDeleteClient = 3,
//     eUpdateClient = 4,
//     eFindClient = 5,
//     eExit = 6
// };

short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

// void PerfromMainMenuOption(enMainMenuOptions MainMenuOption)
// {
//     switch (MainMenuOption)
//     {
//     case enMainMenuOptions::eListClients:
//         ShowAllClientsScreen();
//         ShowMainMenu();
//         break;

//     case enMainMenuOptions::eAddNewClient:
//         ShowAddNewClientsScreen();
//         ShowMainMenu();
//         break;

//     case enMainMenuOptions::eDeleteClient:
//         ShowDeleteClientScreen();
//         ShowMainMenu();
//         break;

//     case enMainMenuOptions::eUpdateClient:
//         ShowUpdateClientScreen();
//         ShowMainMenu();
//         break;

//     case enMainMenuOptions::eFindClient:
//         ShowFindClientScreen();
//         ShowMainMenu();
//         break;

//     case enMainMenuOptions::eExit:
//         ShowEndScreen();
//         break;
//     }
// }

void ShowMainMenu()
{
    cout << "===========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "===========================================\n";

    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "===========================================\n";

    // PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

stLoginData ReadLoginData()
{
    stLoginData Data;
    Data.Permission = 0;

    cout << "Enter your username\n";
    cin >> Data.Username;

    cout << "Enter your password\n";
    cin >> Data.Password;

    return Data;
}

void ShowLoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";

    stLoginData Data = ReadLoginData();

    while (!IsValidLoginData(Data.Username, Data.Password, Users))
    {
        cout << "Invalid username or password, try again\n";
        Data = ReadLoginData();
    }

    ShowMainMenu();
}

int main()
{
    
}