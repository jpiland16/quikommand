#include "common.h"

using namespace std;

unordered_map<string, string> commandList;
string matches[MAX_OPTIONS_VISIBLE];

int matchCount = 0;

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

void readFile() {
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

string* showOptions(int selectedOption) {
	int numOpts = min(MAX_OPTIONS_VISIBLE, getNumMatches());
	for (int i = 0; i < numOpts; i++) {
		cout << "\n";
		if (i == selectedOption) setColor(OPT_SEL_FG, OPT_SEL_BG, OPT_SEL_DC);
		else setColor(OPT_FG, OPT_BG, OPT_DC);
		cout << matches[i];
	}
	return matches;
}

void findMatches(string userText) {
	matchCount = 0;

	map<string, int> commandScores;

	for (auto cmd : commandList) {
		commandScores[cmd.first] = getScore(userText, cmd.first);
	}

	auto sortedCmds = sort(commandScores);

	for (auto iter = sortedCmds.rbegin(); iter != sortedCmds.rend(); iter++) {
		if (iter->second == 0) break;
		matches[matchCount++] = iter->first;
	}

}

int getScore(string userText, string command) {
	int score = 0;

	for (int groupSize = 1; groupSize <= min(MAX_COMPARE_GSIZE, userText.length()); groupSize++) {
		for (int i = 0; i < userText.length() - groupSize + 1; i++) {
			string search = userText.substr(i, groupSize);
			regex searchRegex(search, regex::ECMAScript | regex::icase);
			int numMatches = distance(sregex_iterator(command.begin(), command.end(), searchRegex), sregex_iterator());
			score += numMatches * groupSize;
		}
	}

	return score;
}

int getNumMatches() {
	return matchCount;
}

string getAction(string command) {
	if (commandList.count(command) > 0) return getSysCommand(commandList[command]);
	return getSysCommand("goo:" + command);
}

string getSysCommand(string command) {
	if (command.substr(0, 4) == "goo:") return "start msedge \"https://www.google.com/search?q=" + command.substr(4) + "\" --new-window";
	if (command.substr(0, 4) == "web:") return "start msedge " + command.substr(4) + " --new-window"; 
	return command.substr(4);
}