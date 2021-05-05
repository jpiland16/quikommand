#include "common.h"

using namespace std;

int main() {
	readFile();
	string command = getCommand();
	clear();
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);
	cout << "Command was ";
	setColor(OPT_FG, OPT_BG, DC_UNDERLINE);
	cout << command + "\n--> " + getAction(command) + "\n\n";
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);
	return 0;
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
	int col = prompt.length() + cursorPos + 1;
	cout << ANSI_ESC + to_string(row) + ";" + to_string(col) + "f";
}

void setColor(string fgColor, string bgColor, string displayCode) {
	cout << "\033[0m";
	cout << ANSI_ESC + fgColor + bgColor + displayCode + "m";
}