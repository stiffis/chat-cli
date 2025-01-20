#include <ncurses.h>

int main () {
    initscr(); //
    raw();// use for disable line bufferig
    noecho();//echo is see what is typing the user.
    keypad(stdscr,TRUE);//enables funtion keys like F-n, arroy keys etc.
    printw("Hello World !!!");//print a text on window, but with a position(y-x).
    refresh(); //don´t forget this, because, when we make changes on our window, actually that changes are in a imaginary windows, here is where refresh() function enters, it  upload that changes to our real window, but whit a "merge"
    getch();
    endwin();
    return 0;
}
