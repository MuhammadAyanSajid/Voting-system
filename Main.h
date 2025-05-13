#ifndef MAIN_H
#define MAIN_H 

#include "Authorization.h" 

void start();

void typingAnimation(const string text) {
	cout << "\n\n\n\n\t\t\t\t";
	for (char c : text) {
		cout << c;
		Sleep(80); 
	}
	Sleep(500);
	cout << string(text.length(), '\b') << string(text.length(), ' ') << string(text.length(), '\b');
	Sleep(100);
}

void start() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 14); 

	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
	cfi.dwFontSize.Y = 30;
	cfi.dwFontSize.X = 15; 
	cfi.FontWeight = FW_BOLD;
	wcscpy_s(cfi.FaceName, LF_FACESIZE, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	string filesToCheck[] = {
		"admin_credentials.txt", "admin_data.txt",
		"user_credentials.txt", "user_data.txt",
		APPROVAL_FILE 
	};

	for (const string& filename : filesToCheck) {
		ofstream checkFile(filename, ios::app); 
		checkFile.close();
	}


	Sleep(500); 
	typingAnimation("WELCOME TO ONLINE VOTING SYSTEM!");

	cfi.dwFontSize.Y = 18;
	cfi.dwFontSize.X = 0;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	populateApprovalFile();

	mainPanel();
}

#endif