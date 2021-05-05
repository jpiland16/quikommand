#include "common.h"

using namespace std;

string options[4] = { "Option 1", "Option 2", "another option", "pick this one" };

int main() {
	string command = getCommand();
	clear();
	setColor(PROMPT_FG, PROMPT_BG, PROMPT_DC);
	cout << "Command was ";
	setColor(OPT_FG, OPT_BG, DC_UNDERLINE);
	cout << command + "\n\n";
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

void showOptions(int selectedOption) {
	int numOpts = getOptLength();
	for (int i = 0; i < numOpts; i++) {
		cout << "\n";
		if (i == selectedOption) setColor(OPT_SEL_FG, OPT_SEL_BG, OPT_SEL_DC);
		else setColor(OPT_FG, OPT_BG, OPT_DC);
		cout << options[i];
	}
}

int getOptLength() {
	return min(sizeof(options) / sizeof(options[0]), MAX_OPTIONS_VISIBLE);
}