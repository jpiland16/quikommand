#include "utils/common.h"

using namespace std;

int winColumns = 30; // Dynamically updated later
int usableColumns = 16; // Dynamically updated later

int main() {
	checkWinSize();
	readFile();
	string command = getCommand();
	clear();
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);

	if (command != "") {
		cout << "Command was ";
		setColor(CMD_FG, CMD_BG, DC_UNDERLINE);
		string action = getAction(command);
		const char* a = action.c_str();

		cout << command + "\n\n--> " + action + "\n\n";
		setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);

		system(a);
	} else {
		cout << "No command given.\n";
	}

	return 0;
}

void checkWinSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	winColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	string prompt = PROMPT;
	usableColumns = winColumns - prompt.length();
}

void showPrompt() {
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);
	cout << PROMPT;
	setColor(CMD_FG, CMD_BG, CMD_DC);
}

void clear() {
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);
	system("cls");
}

void showCommand(string command) {
	int pos = 0;
	int row = 0;
	string prompt = PROMPT;
	while (pos < command.length()) {
		if (row > 0) {
			cout << "\n";
			for (int i = 0; i < prompt.length(); i++) cout << " ";
		}
		cout << command.substr(pos, usableColumns);
		pos += usableColumns;
		row++;
	}
}

void moveCursor(int cursorPos) {
	int row = 0;
	string prompt = PROMPT;
	while (cursorPos > usableColumns) {
		cursorPos -= usableColumns;
		row++;
	}
	int col = prompt.length() + cursorPos + 1;
	cout << ANSI_ESC + to_string(row + 1) + ";" + to_string(col) + "H";
}

void setColor(string fgColor, string bgColor, string displayCode) {
	cout << "\033[0m";
	cout << ANSI_ESC + fgColor + bgColor + displayCode + "m";
}