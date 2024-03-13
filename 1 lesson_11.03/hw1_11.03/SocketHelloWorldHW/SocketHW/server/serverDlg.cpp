#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "serverDlg.h"
using namespace std;


Server* Server::ptr = nullptr;

const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
SOCKET acceptSocket;
sockaddr_in addr;

void Server::ServerSocket() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(20000);
    bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
    listen(_socket, 1);
    acceptSocket = accept(_socket, NULL, NULL);

    char buf[MAXSTRLEN];
    GetWindowTextA(Server::ptr->hEditSend, buf, MAXSTRLEN);
    send(acceptSocket, buf, strlen(buf) + 1, 0);
    int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
    buf[i] = '\0';

    SendMessageA(ptr->hListInfo, LB_ADDSTRING, 0, (LPARAM)buf);
    SendMessageA(ptr->hListInfo, LB_ADDSTRING, 0, (LPARAM)"   ");

    WSACleanup();
    system("pause");

}
Server::Server(void)
{
    ptr = this;
}

Server::~Server(void)
{

}

void Server::Cls_OnClose(HWND hwnd)
{
    EndDialog(hwnd, IDCANCEL);
}

BOOL Server::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
    hBtnEnd = GetDlgItem(hwnd, IDC_BTN_OFF_SERVER);
    hBtnSend = GetDlgItem(hwnd, IDC_BTN_MESSAGE_TO_CLIENT);
    hListInfo = GetDlgItem(hwnd, IDC_LIST_MESSAGE_FROM_CLIENT);
    hEditSend = GetDlgItem(hwnd, IDC_EDIT_MESSAGE_TO_CLIENT);
   
    return TRUE;
}



void Server::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    if (id == IDC_BTN_MESSAGE_TO_CLIENT) {
        ServerSocket();
    }
    else if (id == IDC_BTN_OFF_SERVER) {
        closesocket(acceptSocket);
        closesocket(_socket);
        WSACleanup();
    }
}




BOOL CALLBACK Server::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
};