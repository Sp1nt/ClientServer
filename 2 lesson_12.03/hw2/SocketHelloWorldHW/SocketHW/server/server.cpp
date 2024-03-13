#include "serverDlg.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	Server dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVER), NULL, (DLGPROC)Server::DlgProc);
}