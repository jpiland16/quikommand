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

#define MAX_COMPARE_GSIZE 5

using namespace std;

void readFile();
string getCommand();
void clear();
void moveCursor(int);
void setColor(string, string, string);
void showPrompt();
string* showOptions(int);
void findMatches(string);
int getNumMatches();
int getScore(string, string);
string getAction(string);
string getSysCommand(string);