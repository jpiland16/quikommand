#include "common.h"

using namespace std;

int numpadControls[10] = { CTRL_NUMPAD_ZERO, CTRL_NUMPAD_ONE, CTRL_NUMPAD_TWO, CTRL_NUMPAD_THREE, CTRL_NUMPAD_FOUR, CTRL_NUMPAD_SIX, CTRL_NUMPAD_SEVEN, CTRL_NUMPAD_EIGHT, CTRL_NUMPAD_NINE, CTRL_NUMPAD_SLASH };

string getCommand() {
	cout << PROMPT;

	string command = "";
	string* options = {};

	int cursorPos = 0;
	bool terminateEntry = false;

	int selectedOption = -1;

	string keys = "";

	while (!terminateEntry) {

		if (_kbhit()) {

			bool userSelecting = false;

			char c = _getch();
			char n;
			int i;
			string commandUpToCursor;
			smatch lastWord;

			keys += to_string(int(c)) + ",";

			switch (c) {
			case BACKSPACE:
				if (cursorPos > 0) {
					command = command.substr(0, size_t(cursorPos) - 1) + command.substr(cursorPos);
					cursorPos--;
				}
				break;
			case ENTER:
				terminateEntry = true;
				userSelecting = true;
				break;
			case NAVIGATION:

				n = _getch();
				keys += to_string(int(n)) + ",";

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
					userSelecting = true;
					break;
				case DOWN:
					selectedOption = min(selectedOption + 1, getNumMatches() - 1);
					userSelecting = true;
					break;
				case LEFT:
					cursorPos = max(0, cursorPos - 1);
					break;
				case RIGHT:
					cursorPos = min(cursorPos + 1, int(command.length()));
					break;
				case DEL:
					if (cursorPos < int(command.length())) {
						command = command.substr(0, cursorPos) + command.substr(size_t(cursorPos) + 1, command.length() - cursorPos - 1);
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
					
				}
				break;

			case CTRL_NUMPAD:

				n = _getch();
				i = 0;
				for (int ctrlId : numpadControls) {
					if (ctrlId == n) {
						selectedOption = i;
						terminateEntry = true;
						userSelecting = true;
						break;
					}
					i++;
				}
				break;

			case CTRL_BACKSPACE:

				commandUpToCursor = command.substr(0, cursorPos);
				regex_search(commandUpToCursor, lastWord, regex(REGEX_LAST_WORD));

				command = command.substr(0, lastWord.position());
				cursorPos = command.length();

				break;

			case ESCAPE:
				return "";

			default:
				if (int(c) >= 32) {
					command = command.substr(0, cursorPos) + c + command.substr(cursorPos);
					cursorPos++;
					selectedOption = 0;
				}
			}

			if (!userSelecting) {
				bool hasRealMatches = findMatches(command, false);
				if (!hasRealMatches) selectedOption = -1;
			}
			
			if (command.length() == 0) selectedOption = -1;

			clear();
			showPrompt();
			showCommand(command);
			options = showOptions(selectedOption);

			//stat("Character pressed was " + to_string(int(c)));

			moveCursor(cursorPos);

		}
	}
	return selectedOption >= 0 && getNumMatches() > 0 ? options[selectedOption] : command;
}