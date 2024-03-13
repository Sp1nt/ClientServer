#pragma once
#include "header.h"

class Server
{
public:
	Server(void);
	~Server(void);

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Server* ptr;

	void ServerSocket();
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hListInfo, hEditSend, hBtnSend, hBtnEnd, hEditInfo;
};