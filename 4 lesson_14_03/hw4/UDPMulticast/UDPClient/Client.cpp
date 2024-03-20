#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 4096

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT "8888"

SOCKET _socket;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

string temp;
int color;

DWORD WINAPI Sender(void* param)
{
    char msg[DEFAULT_BUFLEN];

    while (true)
    {
        cout << "You: ";
        cin.getline(msg, DEFAULT_BUFLEN);

        char fullInfo[DEFAULT_BUFLEN * 2];
        sprintf_s(fullInfo, "%d|%s|%s", color, temp.c_str(), msg);
        send(_socket, fullInfo, strlen(fullInfo), 0);
    }
}

void ReceiveChatHistory() {
    char response[DEFAULT_BUFLEN];

    int result = recv(_socket, response, DEFAULT_BUFLEN, 0);
    if (!result) {
        return;
    }
    response[result] = '\0';

    cout << response << endl;
}

DWORD WINAPI Receiver(void* param)
{
    while (true) {
        char response[DEFAULT_BUFLEN];
        int result = recv(_socket, response, DEFAULT_BUFLEN, 0);
        if (result > 0) {
            response[result] = '\0';

            int msgColor;
            char sender[DEFAULT_BUFLEN];
            char message[DEFAULT_BUFLEN];

            sscanf_s(response, "%d|%[^|]|%[^\n]", &msgColor, sender, DEFAULT_BUFLEN, message, DEFAULT_BUFLEN);

            SetConsoleTextAttribute(hConsole, msgColor);
            cout << sender << ": " << message << endl;
            SetConsoleTextAttribute(hConsole, 7); // Reset text color to default
        }
    }
}

BOOL ExitHandler(DWORD whatHappening)
{
    switch (whatHappening)
    {
    case CTRL_C_EVENT: // closing console by ctrl + c
    case CTRL_BREAK_EVENT: // ctrl + break
    case CTRL_CLOSE_EVENT: // closing the console window by X button
        return(TRUE);
        break;
    default:
        return FALSE;
    }
}

int main()
{
    system("title Client");

    // initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // resolve server address and port
    addrinfo* result = nullptr;
    iResult = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 2;
    }

    addrinfo* ptr = nullptr;
    // try to connect to the address until success
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // create client socket to connect to server
        _socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 3;
        }

        // connect to server
        iResult = connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (_socket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 5;
    }

    cout << "Enter your nick: ";
    getline(cin, temp);

    cout << "Enter color (0-15): ";
    cin >> color;

    CreateThread(0, 0, Sender, 0, 0, 0);
    ReceiveChatHistory();
    CreateThread(0, 0, Receiver, 0, 0, 0);

    Sleep(INFINITE);
}
