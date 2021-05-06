#pragma once

#include <iostream>
#include <unordered_map>
#include <map>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <regex>
#include <fstream>
#include "keys.h"
#include "colors.h"

#define PROMPT "quikommand <=> "
#define ANSI_ESC "\033["
#define MAX_OPTIONS_VISIBLE 10

#define PROMPT_FG FG_LIGHTGREEN
#define PROMPT_BG BG_BLACK
#define PROMPT_DC DC_NONE

#define CMD_FG FG_LIGHTCYAN
#define CMD_BG PROMPT_BG
#define CMD_DC DC_NONE

#define OPT_FG FG_DARKGRAY
#define OPT_BG PROMPT_BG
#define OPT_DC DC_NONE

#define OPT_SEL_FG OPT_FG
#define OPT_SEL_BG OPT_BG
#define OPT_SEL_DC DC_INVERT

#define STAT_FG FG_LIGHTPINK
#define STAT_BG PROMPT_BG
#define STAT_DC DC_NONE

#define STAT_ON true

#define REGEX_LAST_WORD "\\s[^\\s]+\\s?$"
#define REGEX_NEXT_WORD "^[^\\s]*\\s?"

#define MAX_COMPARE_GSIZE 5
#define MAX_QUERY_LENGTH 20
#define FIRST_LETTER_BONUS 2

#define VERBOSE_OUTPUT false

using namespace std;

void checkWinSize();
void readFile();
string getCommand();
void clear();
void moveCursor(int);
void setColor(string, string, string);
void showPrompt();
void showCommand(string);
string* showOptions(int);
bool findMatches(string, bool);
int getNumMatches();
int getScore(string, string, bool);
string makeRegexSafe(string);
string getAction(string);
string getSysCommand(string);
void stat(string);