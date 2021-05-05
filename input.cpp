#include "common.h"

using namespace std;

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

			switch (c) {
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
	return command;
}