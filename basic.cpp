#include <ncurses.h>

int main () {
    initscr();
    printw("Hello World !!!");
    refresh(); //don´t forget this
    getch();
    endwin();
    return 0;
}
