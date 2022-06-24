#pragma once

#include <vector>
#include <string>
#include <ncurses.h>
#include "common.hpp"

class Window
{
private:
    WINDOW *win;
    int _winStartY;
    int _winStartX;
    int _winHeight;
    int _winActualHeight;
    int _winWidth;
    int _winCursorY;
    int _winCursorX;
    bool isBox;
    bool isLogging;
    bool isWinPrintable;
    Window *otherLogger;
    int32_t createWindow(void);

    // calcSpecific members
    int _winCursorX_input;

public:
    Window(int height, int width, int starty, int startx);
    Window(int starty, int startx, std::vector<std::string> &vec,
                   bool drawBox, Window &loggerWindow);
    void printStr(std::string s);
    void clearRow(int row);
    void printChar(const char ch, int row);
    void printStr(std::string s, int row);
    
};