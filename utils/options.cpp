#include "common.h"

using namespace std;

unordered_map<string, string> commandList;
unordered_map<string, string> commandsByPrefix;
string matches[MAX_OPTIONS_VISIBLE];

int matchCount = 0;
char numpadChars[10] = { '0', '1', '2', '3', '4', '6', '7', '8', '9', '/' };

// SORTING
#pragma region

// ------------------------------------------------------------------------------
// below code from https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/
// ------------------------------------------------------------------------------

// Comparator function to sort pairs
// according to second value
bool cmp(pair<string, int>& a,
	pair<string, int>& b)
{
	return a.second < b.second;
}

// Function to sort the map according
// to value in a (key-value) pairs
vector<pair<string, int>> sort(map<string, int>& M) {

	// Declare vector of pairs
	vector<pair<string, int> > A;

	// Copy key-value pair from Map
	// to vector of pairs
	for (auto& it : M) {
		A.push_back(it);
	}

	// Sort using comparator function
	sort(A.begin(), A.end(), cmp);

	return A;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------


#pragma endregion

void readCommandFile() {
	string commandsFileText;
	string line;
	string command;
	string action;
	smatch tabMatch;
	regex tab("\\t");

	ifstream commandsFile ("private/commands.txt");
	if (commandsFile.is_open()) {
		while (getline(commandsFile, line)) {
			regex_search(line, tabMatch, tab);
			command = line.substr(0, tabMatch.position());
			action = line.substr(tabMatch.position() + 1);
			commandList[command] = action;
		}
		commandsFile.close();
	}
	
}

void readHistory() {
	string historyFileText;
	string line;
	string prefix;
	string command;
	smatch tabMatch;
	regex tab("\\t");

	ifstream historyFile("private/history.txt");
	if (historyFile.is_open()) {
		while (getline(historyFile, line)) {
			regex_search(line, tabMatch, tab);
			prefix = line.substr(0, tabMatch.position());
			command = line.substr(tabMatch.position() + 1);
			commandsByPrefix[prefix] = command;
		}
		historyFile.close();
	}

}

void writeHistory() {
	ofstream historyFile("private/history.txt");
	if (historyFile.is_open()) {
		for (auto iter = commandsByPrefix.begin(); iter != commandsByPrefix.end(); iter++) {
			historyFile << iter->first << "\t" << iter->second << "\n";
		}
		historyFile.close();
	}
}

void setCommandPrefix(string prefix, string command) {
	commandsByPrefix[prefix] = command;
}

string* showOptions(int selectedOption) {
	int numOpts = min(MAX_OPTIONS_VISIBLE, getNumMatches());
	string prompt = PROMPT;
	int promptSize = prompt.size();
	for (int i = 0; i < numOpts; i++) {
		string addl = "   ";
		if (i < 10) addl[0] = numpadChars[i];
		cout << "\n";
		setColor(OPT_FG, OPT_BG, OPT_DC);
		for (int j = 0; j < promptSize - 3; j++) cout << " ";
		if (i == selectedOption) setColor(OPT_SEL_FG, OPT_SEL_BG, OPT_SEL_DC);
		cout << addl << matches[i];
	}
	return matches;
}

bool findMatches(string userText, bool countLetters) {
	matchCount = 0;

	map<string, int> commandScores;

	for (auto cmd : commandList) {
		commandScores[cmd.first] = getScore(makeRegexSafe(userText), cmd.first, countLetters);
	}

	auto sortedCmds = sort(commandScores);

	int topScore = 0;

	for (auto iter = sortedCmds.rbegin(); iter != sortedCmds.rend(); iter++) {
		topScore = max(topScore, iter->second);
		if (iter->second == 0) break;
		matches[matchCount++] = iter->first;
		if (matchCount >= MAX_OPTIONS_VISIBLE) break;
	}

	if (matchCount <= 1 && countLetters == false) {
		bool realMatches = matchCount > 0 && topScore > FIRST_LETTER_BONUS;

		findMatches(userText, true);

		// Top result could be a "fake" match.
		return realMatches;
	}

	return userText.length() <= 5 || topScore > FIRST_LETTER_BONUS;

}

int getScore(string userText, string command, bool countLetters) {

	if (commandsByPrefix.count(userText) == 1 && commandsByPrefix[userText] == command) return INT_MAX;

	int score = (userText.length() > 0 && tolower(userText[0]) == tolower(command[0])) ? FIRST_LETTER_BONUS : 0;

	int groupSize = min(MAX_COMPARE_GSIZE, userText.length());
	int querySize = MAX_QUERY_LENGTH;

	if (countLetters) {
		groupSize = min(2, userText.length());
		querySize = 10;
	}

	int possibleStart =  userText.length() - querySize - 1;

	int start = possibleStart > 0 ? possibleStart : 0;

	for (unsigned int i = start; i < userText.length() - groupSize + 1; i++) {
		string search = userText.substr(i, groupSize);
		regex searchRegex(search, regex::ECMAScript | regex::icase);
		int numMatches = distance(sregex_iterator(command.begin(), command.end(), searchRegex), sregex_iterator());
		score += numMatches * groupSize;
	}

	return score;
}

int getNumMatches() {
	return matchCount;
}

string makeRegexSafe(string originalSearch) {
	regex specialChars("([\\.\\+\\*\\?\\^\\$\\(\\)\\[\\]\\{\\}\\|\\\\])");
	return regex_replace(originalSearch, specialChars, "");
}

string getAction(string command) {
	if (commandList.count(command) > 0) {
		return getSysCommand(commandList[command]);
	}
	return getSysCommand("goo:" + command);
}

string getSysCommand(string command) {
	if (command.substr(0, 4) == "goo:") return "start msedge \"https://www.google.com/search?q=" + url_encode(command.substr(4)) + "\" --new-window";
	if (command.substr(0, 4) == "web:") return "start msedge " + command.substr(4) + " --new-window"; 
	return command.substr(4);
}