#include "utils/common.h"

using namespace std;

int winColumns = 30; // Dynamically updated later

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

void moveCursor(int cursorPos) {
	int row = 0;
	string prompt = PROMPT;
	while (cursorPos + prompt.length() > winColumns) {
		cursorPos -= winColumns;
		row++;
	}
	int col = prompt.length() + cursorPos + 1;
	cout << ANSI_ESC + to_string(row + 1) + ";" + to_string(col) + "H";
}

void setColor(string fgColor, string bgColor, string displayCode) {
	cout << "\033[0m";
	cout << ANSI_ESC + fgColor + bgColor + displayCode + "m";
}