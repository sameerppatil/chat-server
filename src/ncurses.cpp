#include "ncurses.hpp"
using namespace std;


void ncursesInit(void)
{
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    refresh();
}

Window::Window(int height, int width, int starty, int startx)
{
    _winStartY = starty;
    _winStartX = startx;
    _winHeight = height;
    _winWidth = width;
    isBox = true;
    createWindow();
    assert(win != nullptr);
    isWinPrintable = true;
}

Window::Window(int starty, int startx, vector<string> &vec,
                bool drawBox, Window &loggerWindow)
{
    _winStartY = starty;
    _winStartX = startx;
    isBox = drawBox;

    _winHeight = vec.size() + 2;
    _winWidth = vec[0].size() + 2;
    _winCursorY = 0;
    _winCursorX = 0;
    _winCursorX_input = 1;
    createWindow();

    otherLogger = &loggerWindow;
    // increment cursor
    attron(A_BOLD | A_BLINK);
    for (int i = 0; i < vec.size(); i++)
    {
        mvwprintw(win, _winCursorY++, _winCursorX, vec[i].c_str());
    }
    wrefresh(win);
    attroff(A_BOLD | A_BLINK);
}

// TODO use variable string list
void Window::printStr(string s)
{
    if (isWinPrintable)
    {
        if (abs(_winCursorY - _winStartY) > _winHeight)
        {
            wclear(win);
            if (isBox)
            {
                box(win, 0, 0);
            }
            wrefresh(win);
            _winCursorY = _winStartY + 1;
        }
        string newS = to_string(_winCursorY) + ", " + to_string(_winCursorX) + ", " + to_string(_winHeight) + " -> " + s;
        mvwprintw(win, _winCursorY++, _winCursorX, newS.c_str());
        wrefresh(win);
    }
}

void Window::clearRow(int row)
{
    string msg = string((_winWidth - 2), ' ');
    otherLogger->printStr("clearRow() Y is " + to_string(row) + ", x: " + to_string(1));
    mvwprintw(win, row, 1, msg.c_str());
    wrefresh(win);
    _winCursorX = 1;
    if (row == 1)
        _winCursorX_input = 1;
}

void Window::printChar(const char ch, int row)
{
    if (_winCursorX > (_winWidth - 2))
    {
        clearRow(row);
    }
    if (_winCursorX_input > (_winWidth - 2))
    {
        clearRow(row);
    }
    otherLogger->printStr("y is " + to_string(row) + ", x is " + to_string(_winCursorX) + ", width: " + to_string(_winWidth) + ", input cur: " + to_string(_winCursorX) + ", row: " + to_string(row));
    string msg(1, ch);
    if (row == 1)
        mvwprintw(win, row, _winCursorX_input++, msg.c_str());
    else
        mvwprintw(win, row, _winCursorX++, msg.c_str());
    wrefresh(win);
}

void Window::printStr(string s, int row)
{
    if (_winCursorX_input > (_winWidth - 2))
    {
        clearRow(row);
    }
    if (row == 1)
    {
        mvwprintw(win, row, _winCursorX_input, s.c_str());
        _winCursorX_input += (s.size());
    }
    else
        mvwprintw(win, row, _winCursorX++, s.c_str());
    wrefresh(win);
}


int32_t Window::createWindow(void)
{
    int32_t ret = -1;
    if (isBox && _winHeight <= 2)
    {
        return ret;
    }
    win = newwin(_winHeight, _winWidth, _winStartY, _winStartX);
    if (isBox)
    {
        box(win, 0, 0);
        _winActualHeight = _winHeight;
        _winHeight -= 2;
        _winCursorX += 1;
    }
    wrefresh(win);
    _winCursorY += 1;

    ret = 0;
    return ret;
}