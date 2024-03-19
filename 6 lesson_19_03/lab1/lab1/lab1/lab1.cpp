#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;



string findKey(const string& jsonString, const string& fieldName) {

	string pathMarker = "\"" + fieldName + "\":";


	int  startInd = jsonString.find(pathMarker);
	if (startInd != string::npos) {

		int  sPos = jsonString.find_first_not_of(" \t\r\n", startInd + pathMarker.length());
		if (sPos != string::npos) {

			int  ePos = jsonString.find_first_of(",}", sPos);
			if (ePos != string::npos) {

				return jsonString.substr(sPos, ePos - sPos);
			}
		}
	}

	return "";
}


int main()
{
	setlocale(0, "ru");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	string date;
	string codeVal;
	cin >> date;
	cin >> codeVal;

	string srcURL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?valcode=" + codeVal + "&date=" + date + "&json";

	string destFile = "file.txt";

	if (S_OK == URLDownloadToFileA(NULL, srcURL.c_str(), destFile.c_str(), 0, NULL))
	{
		cout << "Saved to " << destFile << "\n";
	}
	else
	{
		cout << "Fail!\n";
	}
	ifstream file(destFile);
	if (file.is_open()) {
		string jsonString;
		string line;
		while (getline(file, line)) {
			jsonString += line;
		}
		file.close();


		string rate = findKey(jsonString, "rate");
		string text = findKey(jsonString, "txt");
		string cc = findKey(jsonString, "cc");
		string exchangedate = findKey(jsonString, "exchangedate");


		cout << "Rate: " << rate << endl;
		cout << "Text: " << text << endl;
		cout << "Currency Code: " << cc << endl;
		cout << "Exchange Date: " << exchangedate << endl;

		ofstream out("file.txt", ios::app);
		if (out.is_open()) {
			out << "--------------------------------------" << endl;
			out << "Rate: " << rate << endl;
			out << "Text: " << text << endl;
			out << "Currency Code: " << cc << endl;
			out << "Exchange Date: " << exchangedate << endl;
			out << "--------------------------------------" << endl;
			out.close();
			cout << "Data written to file.txt" << endl;
		}
		else {
			cout << "Unable to open file.txt" << endl;
		}
	}
	else {
		cout << "Unable to open file!" << endl;
	}

}