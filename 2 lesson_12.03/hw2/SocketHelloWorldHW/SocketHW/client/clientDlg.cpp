#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#include "clientDlg.h"
#pragma comment(lib, "Ws2_32.lib") 

using namespace std;

Client* Client::ptr = nullptr;

const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
sockaddr_in addr;

void Client::ClientSocket() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(20000);
    connect(_socket, (SOCKADDR*)&addr, sizeof(addr));

    char buf[MAXSTRLEN], text[MAXSTRLEN];
    int i = recv(_socket, buf, MAXSTRLEN, 0);
    buf[i] = '\0';

    SendMessageA(ptr->hListInfo, LB_ADDSTRING, 0, (LPARAM)buf);
    SendMessageA(ptr->hListInfo, LB_ADDSTRING, 0, (LPARAM)"   ");

    GetWindowTextA(Client::ptr->hEditSend, text, strlen(text));
    send(_socket, text, strlen(text), 0);
    system("pause");
}

Client::Client(void) {
    ptr = this;
}

Client::~Client(void) {
}

void Client::Cls_OnClose(HWND hwnd) {
    EndDialog(hwnd, IDCANCEL);
}

BOOL Client::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
    hBtnEnd = GetDlgItem(hwnd, IDC_BTN_OFF_CLIENT);
    hBtnSend = GetDlgItem(hwnd, IDC_BTN_MESSAGE_TO_SERVER);
    hListInfo = GetDlgItem(hwnd, IDC_LIST_MESSAGE_FROM_SERVER);
    hEditSend = GetDlgItem(hwnd, IDC_EDIT_MESSAGE_TO_SERVER);

    return TRUE;
}


void Client::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    if(id == IDC_BTN_MESSAGE_TO_SERVER) {
        ClientSocket();
    }
    else if (id == IDC_BTN_OFF_CLIENT) {
        closesocket(_socket);
        WSACleanup();
    }
}

BOOL CALLBACK Client::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}