# quikommand
A command-line utility for speeding up your workflow (currently for Windows only)

## The basics
Start typing, and quikommand will attempt to predict what you would like to do. You can use the arrow keys to select an option, then press ENTER to execute the corresponding command. The target command will appear in the status bar. 

Alternatively, each of the search results (up to 10) has a corresponding key on the NUMPAD. Press CTRL and this key simultaneuosly to execute that command immediately, without having to use the arrow keys.

## How it could be useful
This program could be paired with another script which watches for a certain keystroke, and then launches quikommand. I've provided a sample Python script which listens for a double-tap of the CTRL key. You can double-tap the ESC key to prompt a "Close program?" dialog, or use the system tray icon to close the listener.

In order to use this script, you will need to replace the string `<PATH_TO_QUIKOMMAND_FOLDER>` on line 52 of keywatcherV2.py with the path to the folder containing `quikommand.exe`.

## Compiling `quikommand.exe`
Navigate to the folder containing `quikommand.cpp` in a VS Developer Command Prompt, and execute the following command:

```
cl quikommand.cpp utils/*.cpp
```

## The `commands.txt` file
The file `commands.txt` should be located in a subfolder called `private` (where `private` is in the same folder as `quikommand.exe`.) The syntax is as follows: a user-friendly command name (such as `my favorite website`) followed by a TAB, then the command (for example, `web:https://github.com`). The command must contain one of three possible 4-character prefixes:
 - `goo:` - for a search on google.com
 - `web:` - to launch the web browser (in this case, `msedge` - see code)
 - `sys:` - to execute a system command


 ### A note on matching
 If a match is close enough, then the topmost option will be preselected. Otherwise, pressing ENTER will default to searching the web with the given command. 