#include "ClientDlg.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	Client dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_CLIENT), NULL, (DLGPROC)Client::DlgProc);
}