#include <ncurses.h>

int main () {
    initscr();//inicializa la pantalla
    keypad(stdscr, TRUE);//activa teclas especiales como las f-n, home, esc, etc...
    curs_set(1);//1 para mostrar el cursor, 0 para ocultarlo
    mvprintw(0,0,"Hola Mundo");// Funcion para imprimir en la ubicacion 0,0
    refresh();//refrescar la pantalla
    char ch;
    ch=getch();//entrada
    while ((ch)!='q') {
        ch=getch();
    }
    //mientras la entrada sea distinta de 'q', se queda en bucle y hace lo q hace y si no
    // termina la instancia de la ventana;
    endwin();
    return 0;
}
