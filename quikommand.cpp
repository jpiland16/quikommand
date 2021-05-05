#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <regex>
#include "keys.h"
#include "colors.h"

#define PROMPT "quikommand ~$ "
#define ANSI_ESC "\033["
#define MAX_OPTIONS_VISIBLE 10

#define PROMPT_FG FG_LIGHTGREEN
#define PROMPT_BG BG_BLACK
#define PROMPT_DC DC_NONE

#define CMD_FG PROMPT_FG
#define CMD_BG PROMPT_BG
#define CMD_DC DC_UNDERLINE

#define OPT_FG FG_DARKGRAY
#define OPT_BG PROMPT_BG
#define OPT_DC DC_NONE

#define OPT_SEL_FG OPT_FG
#define OPT_SEL_BG OPT_BG
#define OPT_SEL_DC DC_INVERT

#define REGEX_LAST_WORD "\\s[^\\s]+\\s?$"
#define REGEX_NEXT_WORD "^[^\\s]*\\s?"

using namespace std;

string options[4] = { "Option 1", "Option 2", "another option", "pick this one" };

string getCommand();
void clear();
void moveCursor(int);
void setColor(string, string, string);
void showPrompt();
void showOptions(int selectedOption);
int getOptLength();

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

string getCommand() {
	showPrompt();

	string command = "";
	int cursorPos = 0;
	bool terminateEntry = false;

	int selectedOption = -1;

	while (!terminateEntry) {

		if (_kbhit()) {
			clear();
			char c = _getch();
			char n;
			string commandUpToCursor;
			smatch lastWord;

			switch(c) {
				case BACKSPACE:
					if (cursorPos > 0) {
						command = command.substr(0, cursorPos - 1) + command.substr(cursorPos);
						cursorPos--;
					}
					break;
				case ENTER:
					terminateEntry = true;
					break;
				case NAVIGATION:

					n = _getch();
					switch (n) {
						case HOME:
						case PG_UP:
							cursorPos = 0;
							break;
						case END:
						case PG_DN:
							cursorPos = command.length();
							break;
						case UP:
							selectedOption = max(selectedOption - 1, -1);
							break;
						case DOWN:
							selectedOption = min(selectedOption + 1, getOptLength());
							break;
						case LEFT:
							cursorPos = max(0, cursorPos - 1);
							break;
						case RIGHT:
							cursorPos = min(cursorPos + 1, command.length());
							break;
						case DELETE:
							if (cursorPos < command.length()) {
								command = command.substr(0, cursorPos) + command.substr(cursorPos + 1, command.length() - cursorPos - 1);
							}
							break;
						case CTRL_LEFT:
							commandUpToCursor = command.substr(0, cursorPos);
							regex_search(commandUpToCursor, lastWord, regex(REGEX_LAST_WORD));
							cursorPos = max(lastWord.position(), 0);
							break;
						case CTRL_RIGHT:
							commandUpToCursor = command.substr(cursorPos);
							regex_search(commandUpToCursor, lastWord, regex(REGEX_NEXT_WORD));
							cursorPos += max(lastWord.position() + lastWord.length(), 0);
							break;
						default:
							cout << "Naviagtion control for " << int(n) << " not set!";

					}
					break;

				case CTRL_BACKSPACE:
					
					commandUpToCursor = command.substr(0, cursorPos);
					regex_search(commandUpToCursor, lastWord, regex(REGEX_LAST_WORD));

					command = command.substr(0, lastWord.position());
					cursorPos = command.length();

					break;

				default:
					command = command.substr(0, cursorPos) + c + command.substr(cursorPos);
					cursorPos++;
					selectedOption = -1;
			}

			showPrompt();
			cout << command;
			showOptions(selectedOption);

			moveCursor(cursorPos);

		}
	}
	return selectedOption >= 0 ? options[selectedOption] : command;
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