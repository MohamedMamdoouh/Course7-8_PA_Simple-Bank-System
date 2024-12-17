
#include <bits/stdc++.h>
using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct stUserData
{
    string Username = "";
    string Password = "";
    
    short Permission = 0;
    bool MarkForDelete = false;
};

stUserData UserData; // Global Variable

void ShowMainMenu();
void ShowMainUsersMenu();
void Logout();

struct stClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
};

enum enMainUsersMenuPermissions
{
    eAll = -1,
    enListUsers = 1,
    enAddUser = 2,
    enDeleteUser = 4,
    enUpdateUserInfo = 8,
    enFindUser = 16,
};

void ShowAccessDeniedMessage()
{
    cout << "Access Denied, please contact your admin! \n";
}

short ReadPermissionToSet()
{
    short Permission = 0;
    char Option = 'n';

    cout << "Do you want to give full access? y/n \n";
    cin >> Option;

    if (Option == 'y' || Option == 'Y')
        return -1;

    cout << "Add User? y/n\n";
    cin >> Option;
    if (Option == 'y' || Option == 'Y')
        Permission += enMainUsersMenuPermissions::enAddUser;

    cout << "Show users list? y/n\n";
    cin >> Option;
    if (Option == 'y' || Option == 'Y')
        Permission += enMainUsersMenuPermissions::enListUsers;

    cout << "Delete user? y/n\n";
    cin >> Option;
    if (Option == 'y' || Option == 'Y')
        Permission += enMainUsersMenuPermissions::enDeleteUser;

    cout << "Update user? y/n\n";
    cin >> Option;
    if (Option == 'y' || Option == 'Y')
        Permission += enMainUsersMenuPermissions::enUpdateUserInfo;

    cout << "Find user? y/n\n";
    cin >> Option;
    if (Option == 'y' || Option == 'Y')
        Permission += enMainUsersMenuPermissions::enFindUser;

    return Permission;
}

bool CheckAccessPermission(enMainUsersMenuPermissions Permission)
{
    if (UserData.Permission == enMainUsersMenuPermissions::eAll)
        return true;

    if ((Permission & UserData.Permission) == Permission)
        return true;

    else
        return false;
}

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

stClient ConvertLineToClientRecord(string Line, string Delimeter = "#//#")
{
    stClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Delimeter);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertClientRecordToLine(stClient Client, string Delimeter = "#//#")
{
    string ClientRecord = "";
    ClientRecord += Client.AccountNumber + Delimeter;
    ClientRecord += Client.PinCode + Delimeter;
    ClientRecord += Client.Name + Delimeter;
    ClientRecord += Client.Phone + Delimeter;
    ClientRecord += to_string(Client.AccountBalance);

    return ClientRecord;
}

bool FindClientByAccountNumber(string AccountNumber, string FileName)
{
    // vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToClientRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            // vClients.push_back(Client);
        }

        MyFile.close();
    }
    return false;
}

stClient ReadNewClient()
{
    stClient Client;
    cout << "Enter Account Number?\n";

    // Usage of ws will extract all the whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (FindClientByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "Client with [" << Client.AccountNumber << "] already exists, Enter another Account Number\n";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToClientRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void ShowAllClientsScreen()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t Client List (" << vClients.size() << ") Client(s)\n";
    cout << "--------------------------------------------------------------\n";
    cout << "\t Account Number \t \t PinCode \t \t Name \t \t Phone \t \t Balance\n";

    for (stClient &Client : vClients)
    {
        cout << "\t " << Client.AccountNumber << " \t \t \t \t " << Client.PinCode;
        cout << "  \t \t \t" << Client.Name << " \t \t " << Client.Phone << " \t \t " << Client.AccountBalance << endl;
    }
    cout << "----------------------------------------------------------------\n";
}

void PrintClientCard(stClient Client)
{
    cout << "The following are the client details:\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code       : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "----------------------------------------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClients, stClient &Client)
{
    // And update 'Client' with the found one
    for (stClient &C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

stClient ChangeClientRecordByAccountNumber(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

void SaveClientsDataToFile(string FileName, vector<stClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClient &C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                // we only write records that are not marked for delete
                DataLine = ConvertClientRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }
}

void AddDataLineToFile(string FileName, string DataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

void AddNewClient()
{
    stClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n";
        AddNewClient();
        cout << "Client Added Successfully, do you want to add more clients? Y/N?\n";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{
    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Are you sure you want delete this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\nClient Deleted Successfully.";
            return true;
        }
    }

    else
    {
        cout << "Client with Account Number (" << AccountNumber << ") is Not Found!\n";
        return false;
    }

    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{

    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "Are you sure you want update this client? y/n ? \n";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (stClient &C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecordByAccountNumber(AccountNumber);
                    break;
                }
            }

            // Client = ChangeClientRecordByAccountNumber(AccountNumber);
            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "Client Updated Successfully.";
            return true;
        }
    }

    else
    {
        cout << "Client with Account Number (" << AccountNumber << ") is Not Found!\n";
        return false;
    }
    return false;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber?\n";

    cin >> AccountNumber;
    return AccountNumber;
}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    stClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);

    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is not found!\n";
}

enum enMainMenuOptions
{
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eManageUsers = 6,
    eLogout = 7
};

short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eListClients:
        ShowAllClientsScreen();
        ShowMainMenu();
        break;

    case enMainMenuOptions::eAddNewClient:
        ShowAddNewClientsScreen();
        ShowMainMenu();
        break;

    case enMainMenuOptions::eDeleteClient:
        ShowDeleteClientScreen();
        ShowMainMenu();
        break;

    case enMainMenuOptions::eUpdateClient:
        ShowUpdateClientScreen();
        ShowMainMenu();
        break;

    case enMainMenuOptions::eFindClient:
        ShowFindClientScreen();
        ShowMainMenu();
        break;

    case enMainMenuOptions::eManageUsers:
        ShowMainUsersMenu();
        break;

    case enMainMenuOptions::eLogout:
        Logout();
        break;

    default:
        break;
    }
}

void ShowMainMenu()
{
    cout << "===========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "===========================================\n";

    cout << "\t[1] Show Client List\n";
    cout << "\t[2] Add New Client\n";
    cout << "\t[3] Delete Client\n";
    cout << "\t[4] Update Client Info\n";
    cout << "\t[5] Find Client\n";
    cout << "\t[6] Manage Users\n";
    cout << "\t[7] Logout\n";
    cout << "===========================================\n";

    PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

///////////////////////////////////

stUserData ConvertLineToUserRecord(string Line, string Delimeter = "#//#")
{
    stUserData User;
    vector<string> vUsers;
    vUsers = SplitString(Line, Delimeter);

    User.Username = vUsers[0];
    User.Password = vUsers[1];
    User.Permission = stoi(vUsers[2]);

    return User;
}

vector<stUserData> LoadUsersDataFromFile(string FileName)
{
    vector<stUserData> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stUserData User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToUserRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

void ShowAllUsersScreen()
{
    if (!CheckAccessPermission(enMainUsersMenuPermissions::eAll))
    {
        ShowAccessDeniedMessage();
        ShowMainMenu();
        return;
    }

    vector<stUserData> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t Users List (" << vUsers.size() << ") User(s)\n";
    cout << "--------------------------------------------------------------\n";
    cout << "\t Username \t \t Password \t \t Permission\n";

    for (stUserData &User : vUsers)
    {
        cout << User.Username << "\t \t " << User.Password << "\t \t " << User.Permission << endl;
    }
    cout << "----------------------------------------------------------------\n";
}

string ConvertUserRecordToLine(stUserData User, string Delimeter = "#//#")
{
    string UserRecord = "";
    UserRecord += User.Username + Delimeter;
    UserRecord += User.Password + Delimeter;
    UserRecord += to_string(User.Permission);

    return UserRecord;
}

bool FindUserByUsername(string Username, string FileName)
{
    // First Way -> search in file
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stUserData User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToUserRecord(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }
    return false;
}

bool FindUserByUsername(string Username, vector<stUserData> vUsers, stUserData &User)
{
    // Second Way -> search in vector
    // It updates 'Client' with the found one
    for (stUserData &D : vUsers)
    {
        if (D.Username == Username)
        {
            User = D;
            return true;
        }
    }
    return false;
}

stUserData ReadUserData()
{
    stUserData User;
    cout << "Enter Username\n";
    cin >> User.Username;

    cout << "Enter Password\n";
    cin >> User.Password;

    while (FindUserByUsername(User.Username, UsersFileName))
    {
        cout << "Username [" << User.Username << "] already exists! try again\n";
        cout << "Enter username\n";
        cin >> User.Username;
    }

    User.Permission = ReadPermissionToSet();
    return User;
}

void AddNewUser()
{
    stUserData User;
    User = ReadUserData();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
    if (!CheckAccessPermission(enMainUsersMenuPermissions::enAddUser))
    {
        ShowAccessDeniedMessage();
        ShowMainMenu();
        return;
    }

    char AddMore = 'Y';
    do
    {
        cout << "Adding New User:\n";
        AddNewUser();
        cout << "User Added Successfully, do you want to add more Users? Y/N?\n";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

short ReadMainUsersMenu()
{
    cout << "Choose what do you want to do? [1 to 6]?\n";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

enum enMainUsersMenuOption
{
    eListUsers = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUserInfo = 4,
    eFindUser = 5,
    eMainMenu = 6
};

string ReadUsername()
{
    string Username;
    cout << "Enter Username\n";
    cin >> Username;

    return Username;
}

void PrintUserCard(stUserData User)
{
    cout << "The following are the User details:\n";
    cout << "Username   : " << User.Username << endl;
    cout << "Password   : " << User.Password << endl;
    cout << "Permission : " << User.Permission << endl;
    cout << "----------------------------------------------------------------\n";
}

void ShowFindUserScreen()
{

    if (!CheckAccessPermission(enMainUsersMenuPermissions::enFindUser))
    {
        ShowAccessDeniedMessage();
        ShowMainMenu();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<stUserData> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUserData User;
    string Username = ReadUsername();

    if (FindUserByUsername(Username, vUsers, User))
        PrintUserCard(User);

    else
        cout << "\nUsername [" << Username << "] is not found!\n";
}

bool MarkUserForDeleteByUsername(string Username, vector<stUserData> &vUsers)
{
    for (stUserData &D : vUsers)
    {
        if (D.Username == Username)
        {
            D.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

void SaveUsersDataToFile(string FileName, vector<stUserData> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stUserData &D : vUsers)
        {
            if (D.MarkForDelete == false)
            {
                // we only write records that are NOT marked for delete
                DataLine = ConvertUserRecordToLine(D);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }
}

bool DeleteUserByUsername(string Username, vector<stUserData> &vUsers)
{
    stUserData User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        if (Username == "admin")
        {
            cout << "Cannot delete admin user\n";
            return false;
        }

        cout << "Are you sure you want delete this user? y/n ?\n";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "User Deleted Successfully.";
            return true;
        }
    }

    else
    {
        cout << "Username [" << Username << "] is Not Found!\n";
        return false;
    }

    return false;
}

void ShowDeleteUserScreen()
{
    if (!CheckAccessPermission(enMainUsersMenuPermissions::enDeleteUser))
    {
        ShowAccessDeniedMessage();
        ShowMainMenu();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector<stUserData> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUsername();
    DeleteUserByUsername(Username, vUsers);
}

stUserData ChangeUserRecordByUsername(string Username)
{
    stUserData User;
    User.Username = Username;

    cout << "\nEnter Password?\n";
    getline(cin >> ws, User.Password);

    cout << "Enter Permission?\n";
    cin >> User.Permission;

    return User;
}

bool UpdateUserByUsername(string Username, vector<stUserData> &vUsers)
{
    stUserData User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "Are you sure you want update this User? y/n ?\n";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUserData &D : vUsers)
            {
                if (D.Username == Username)
                {
                    D = ChangeUserRecordByUsername(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "User Updated Successfully.";
            return true;
        }
    }

    else
    {
        cout << "Username [" << Username << "] is Not Found!\n";
        return false;
    }
    return false;
}

void ShowUpdateUserScreen()
{
    if (!CheckAccessPermission(enMainUsersMenuPermissions::enUpdateUserInfo))
    {
        ShowAccessDeniedMessage();
        ShowMainMenu();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<stUserData> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUsername();
    UpdateUserByUsername(Username, vUsers);
}

void PerfromMainUsersMenuOption(enMainUsersMenuOption eMainUsersMenuOption)
{
    switch (eMainUsersMenuOption)
    {
    case enMainUsersMenuOption::eListUsers:
        ShowAllUsersScreen();
        ShowMainUsersMenu();
        break;

    case enMainUsersMenuOption::eAddNewUser:
        AddNewUsers();
        ShowMainUsersMenu();
        break;

    case enMainUsersMenuOption::eFindUser:
        ShowFindUserScreen();
        ShowMainUsersMenu();
        break;

    case enMainUsersMenuOption::eDeleteUser:
        ShowDeleteUserScreen();
        ShowMainUsersMenu();
        break;

    case enMainUsersMenuOption::eUpdateUserInfo:
        ShowUpdateUserScreen();
        ShowMainUsersMenu();
        break;

    case enMainUsersMenuOption::eMainMenu:
        ShowMainMenu();
        break;

    default:
        break;
    }
}

void ShowMainUsersMenu(stUserData Data)
{
    cout << "===========================================\n";
    cout << "\n\t\tMain Users Menu Screen\n";
    cout << "===========================================\n";

    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "===========================================\n";

    PerfromMainUsersMenuOption((enMainUsersMenuOption)ReadMainUsersMenu());
}

bool IsValidLoginData(string Username, string Password, string FileName, stUserData &Data)
{
    // We added 'stUserData &Data' parameter to update 'Data' with the found one
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stUserData User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToUserRecord(Line);
            if (User.Username == Username && User.Password == Password)
            {
                Data = User;
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }
    return false;
}

string ReadPassword()
{
    string Password;
    cout << "Enter your password\n";
    cin >> Password;

    return Password;
}

void LoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";

    string Username = ReadUsername();
    string Password = ReadPassword();

    while (!IsValidLoginData(Username, Password, UsersFileName, UserData))
    {
        cout << "Invalid username or password, try again\n";
        Username = ReadUsername();
        Password = ReadPassword();
    }

    ShowMainMenu();
}

void Logout()
{
    LoginScreen();
}

int main()
{
    LoginScreen();
    return 0;
}
