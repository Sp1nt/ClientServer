#pragma once
#include "header.h"

class Client
{
public:
	Client(void);
	~Client(void);

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Client* ptr;

	void ClientSocket();
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hListInfo, hEditSend, hBtnSend, hBtnEnd, hEditInfo;
};