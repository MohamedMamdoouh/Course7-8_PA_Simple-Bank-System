#include <bits/stdc++.h>
using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenu();

struct stClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
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

stClient ConvertLineToRecord(string Line, string Delimeter = "#//#")
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

string ConvertRecordToLine(stClient Client, string Delimeter = "#//#")
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
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
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
            Client = ConvertLineToRecord(Line);
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
                DataLine = ConvertRecordToLine(C);
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
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
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

            cout << "\n\nClient Deleted Successfully.";
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

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends";
    cout << "\n-----------------------------------\n";
}

enum enMainMenuOptions
{
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eExit = 6
};

short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
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

    case enMainMenuOptions::eExit:
        ShowEndScreen();
        break;
    }
}

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

    PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

int main()

{
    ShowMainMenu();
    return 0;
}
