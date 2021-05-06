#include "utils/common.h"

using namespace std;

int winColumns = 30; // Dynamically updated later
int usableColumns = 16; // Dynamically updated later
int winRows = 17; // Dynamically updated later

int main() {
	checkWinSize();
	readCommandFile();
	readHistory();
	string command = getCommand();
	clear();
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);

	if (command != "") {
		if (VERBOSE_OUTPUT) cout << "Command was ";
		if (VERBOSE_OUTPUT) setColor(CMD_FG, CMD_BG, DC_UNDERLINE);
		string action = getAction(command);
		const char* a = action.c_str();

		if (VERBOSE_OUTPUT) cout << command + "\n\n--> " + action + "\n\n";
		if (VERBOSE_OUTPUT) setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);

		writeHistory();
		system(a);
	} else {
		if (VERBOSE_OUTPUT) cout << "No command given.\n";
	}
	return 0;
}

void checkWinSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	winColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	winRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

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

void stat(string info) {
	if (STAT_ON) {
		setColor(STAT_FG, STAT_BG, STAT_DC);
		int row = winRows - 1;
		int col = 1;
		cout << ANSI_ESC + to_string(row) + ";" + to_string(col) + "H";
		for (int i = 0; i < winColumns; i++) cout << "_";
		cout << ANSI_ESC + to_string(row + 1) + ";" + to_string(col) + "H";
		cout << info;
	}
}