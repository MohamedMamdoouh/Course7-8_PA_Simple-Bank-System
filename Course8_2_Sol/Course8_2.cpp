#include <bits/stdc++.h>
using namespace std;

const string ClientsFileName = "C:/Users/Mohamed Mamdouh/Documents/MyGitHub/Course8_PA_Simple-Bank-System/Clients.txt";
const string UsersFileName = "C:/Users/Mohamed Mamdouh/Documents/MyGitHub/Course8_PA_Simple-Bank-System/Users.txt";

struct stClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
};

stClient CurrentClient; // Global Variable

struct stUserData
{
    string Username = "";
    string Password = "";
    short Permission = 0;
    bool MarkForDelete = false;
};

stUserData CurrentUser; // Global Variable

void ShowQuickWithdrawScreen();
void ShowMainMenu();

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

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient> &vClients)
{
    char Answer = 'n';

    cout << "Are you sure you want perfrom this transaction?y/n?\n";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {

        for (stClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "Done Successfully. New balance is: " << C.AccountBalance  << endl;
                return true;
            }
        }
        return false;
    }

    return false;
}

short ReadQuickWithdrawOption()
{
    short QuickWithdrawOption = 0;
    cout << "Enter Withdraw option\n";
    cin >> QuickWithdrawOption;

    while (QuickWithdrawOption < 1 || QuickWithdrawOption > 9)
    {
        cout << "Invalid option, try again\n";
        cin >> QuickWithdrawOption;
    }

    return QuickWithdrawOption;
}

short GetQuickWithDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
        break;

    case 2:
        return 50;
        break;

    case 3:
        return 100;
        break;

    case 4:
        return 200;
        break;

    case 5:
        return 400;
        break;

    case 6:
        return 600;
        break;

    case 7:
        return 800;
        break;

    case 8:
        return 1000;
        break;

    default:
        return 0;
        break;
    }
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
    if (QuickWithDrawOption == 9) // Exit
        return;

    short WithdrawBalance = GetQuickWithDrawAmount(QuickWithDrawOption);

    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice\n";
        ShowQuickWithdrawScreen();
        return; // Important, to avoid going behind!
    }

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawBalance;
}

void ShowQuickWithdrawScreen()
{
    cout << "===========================================\n";
    cout << "\tQuick Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

enum enMainMenuOption
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eLogout = 5
};

short ReadMainMenuOption()
{
    short Option = 0;
    cout << "Enter your option\n";
    cin >> Option;

    while (Option < 1 || Option > 5)
    {
        cout << "Invalid option, try again\n";
        cin >> Option;
    }
    return Option;
}

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

string ReadAccountNumber()
{
    string AccountNumber;
    cout << "Enter Account Number\n";
    cin >> AccountNumber;

    return AccountNumber;
}

string ReadPinCode()
{
    string PinCode;
    cout << "Enter your PinCode\n";
    cin >> PinCode;

    return PinCode;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient &Client)
{
    // And update 'Client' with the found one
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
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

void LoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";

    string AccountNumber = ReadAccountNumber();
    string PinCode = ReadPinCode();
    stClient Client;

    while (!FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, Client))
    {
        cout << "Invalid Account Number or PinCode, try again\n";
        AccountNumber = ReadAccountNumber();
        PinCode = ReadPinCode();
    }

    CurrentClient = Client;
    PrintClientCard(Client);
    ShowMainMenu();
}

void ShowCheckBalanceScreen()
{
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
}

double ReadDepositAmount()
{
    double DepositAmount = 0;

    cout << "Enter Amount to Deposit\n";
    cin >> DepositAmount;

    while (DepositAmount < 0)
    {
        cout << "Please enter a positive number only\n";
        cin >> DepositAmount;
    }

    return DepositAmount;
}

void PerfromDepositOption()
{
    double DepositAmount = ReadDepositAmount();

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
    PerfromDepositOption();
}

short ReadWithdrawAmount()
{
    short WithdrawAmount = 0;

    cout << "Enter Withdraw Amount which is multiple of 5 only\n";
    cin >> WithdrawAmount;

    while (WithdrawAmount % 5 != 0)
    {
        cout << "Only amount of 5's multiple is allowed\n";
        cin >> WithdrawAmount;
    }

    return WithdrawAmount;
}

void PerfromNormalWithdrawOption()
{
    short WithdrawAmount = ReadWithdrawAmount();

    while (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance" << endl;
        cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
        short WithdrawAmount = ReadWithdrawAmount();
    }

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowNormalWithDrawScreen()
{
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    PerfromNormalWithdrawOption();
}

void PerfromMainMenuOption(enMainMenuOption eOption)
{
    switch (eOption)
    {
    case enMainMenuOption::eCheckBalance:
        ShowCheckBalanceScreen();
        ShowMainMenu();
        break;

    case enMainMenuOption::eDeposit:
        ShowDepositScreen();
        ShowMainMenu();
        break;

    case enMainMenuOption::eLogout:
        LoginScreen();
        break;

    case enMainMenuOption::eNormalWithdraw:
        ShowNormalWithDrawScreen();
        ShowMainMenu();
        break;

    case enMainMenuOption::eQuickWithdraw:
        ShowQuickWithdrawScreen();
        ShowMainMenu();
        break;

    default:
        break;
    }
}

void ShowMainMenu()
{
    cout << "===========================================\n";
    cout << "\t\tATM Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw\n";
    cout << "\t[2] Normal Withdraw\n";
    cout << "\t[3] Deposit\n";
    cout << "\t[4] Check Balance\n";
    cout << "\t[5] Logout\n";
    cout << "===========================================\n";

    PerfromMainMenuOption((enMainMenuOption)ReadMainMenuOption());
}

int main()
{
    LoginScreen();
    return 0;
}