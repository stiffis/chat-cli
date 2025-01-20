
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>

// Función para inicializar ncurses
void init_ncurses() {
    setlocale(LC_ALL, ""); // Habilitar soporte para UTF-8
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

// Función para cargar historial de mensajes de un archivo
std::vector<std::string> load_chat_history(const std::string& filename) {
    std::vector<std::string> history;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            history.push_back(line);
        }
        file.close();
    }
    return history;
}

// Función para guardar historial de mensajes en un archivo
void save_chat_history(const std::string& filename, const std::vector<std::string>& history) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& line : history) {
            file << line << "\n";
        }
        file.close();
    }
}

// Función para dibujar la interfaz
void draw_interface(WINDOW* contacts_win, WINDOW* chat_win, WINDOW* status_win,
                    const std::vector<std::string>& contacts,
                    const std::vector<std::string>& chat_history,
                    const std::string& current_message,
                    int selected_contact, bool in_chat) {
    // Dibujar contactos
    werase(contacts_win);
    box(contacts_win, 0, 0);
    for (size_t i = 0; i < contacts.size(); ++i) {
        if (i == selected_contact) {
            wattron(contacts_win, A_REVERSE); // Resalta el contacto seleccionado
        }
        mvwprintw(contacts_win, i + 1, 1, "%s", contacts[i].c_str());
        if (i == selected_contact) {
            wattroff(contacts_win, A_REVERSE);
        }
    }
    wrefresh(contacts_win);

    // Dibujar ventana de chat
    werase(chat_win);
    box(chat_win, 0, 0);
    mvwprintw(chat_win, 1, 1, "Chat with %s", contacts[selected_contact].c_str());

    // Mostrar historial de mensajes
    int row = 3;
    for (const auto& msg : chat_history) {
        mvwprintw(chat_win, row++, 1, "%s", msg.c_str());
    }

    if (in_chat) {
        mvwprintw(chat_win, row + 1, 1, "Type your message: %s", current_message.c_str());
    } else {
        mvwprintw(chat_win, row + 1, 1, "Press Enter to chat.");
    }

    wrefresh(chat_win);

    // Dibujar barra de estado
    werase(status_win);
    if (in_chat) {
        mvwprintw(status_win, 0, 1, "Status: Typing a message (Press Esc to go back)");
    } else {
        mvwprintw(status_win, 0, 1, "Status: Selecting a contact (Press Enter to chat)");
    }
    wrefresh(status_win);
}

int main() {
    // Inicializa ncurses
    init_ncurses();

    // Obtener dimensiones de la terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Dimensiones de las ventanas
    int contacts_width = cols / 4; // 25% de la pantalla para contactos
    int chat_width = cols - contacts_width;

    // Crear ventanas
    WINDOW* contacts_win = newwin(rows - 1, contacts_width, 0, 0);   // Ventana de contactos
    WINDOW* chat_win = newwin(rows - 1, chat_width, 0, contacts_width); // Ventana de chat
    WINDOW* status_win = newwin(1, cols, rows - 1, 0);               // Barra de estado

    // Lista de contactos
    std::vector<std::string> contacts = {"Alice", "Bob", "Charlie", "Diana", "Eve"};
    int selected_contact = 0;

    // Historial de mensajes y mensaje actual
    std::vector<std::string> chat_history = load_chat_history(contacts[selected_contact] + ".txt");
    std::string current_message;

    // Bandera para saber si estamos en el modo de chat
    bool in_chat = false;

    // Dibujar interfaz inicial
    draw_interface(contacts_win, chat_win, status_win, contacts, chat_history, current_message, selected_contact, in_chat);

    // Manejar entrada del usuario
    int ch;
    while ((ch = getch()) != 'q' || in_chat) {
        if (!in_chat) {
            // Modo selección de contacto
            switch (ch) {
                case 'j': // Mover hacia abajo en la lista de contactos
                    if (selected_contact < static_cast<int>(contacts.size()) - 1) {
                        selected_contact++;
                        chat_history = load_chat_history(contacts[selected_contact] + ".txt");
                    }
                    break;
                case 'k': // Mover hacia arriba en la lista de contactos
                    if (selected_contact > 0) {
                        selected_contact--;
                        chat_history = load_chat_history(contacts[selected_contact] + ".txt");
                    }
                    break;
                case '\n': // Entrar al modo de chat
                    in_chat = true;
                    current_message.clear(); // Limpia el mensaje actual
                    break;
            }
        } else {
            // Modo escritura de mensajes
            if (ch == 27) { // Salir del modo de chat (Esc)
                in_chat = false;
                save_chat_history(contacts[selected_contact] + ".txt", chat_history);
            } else if (ch == '\n') { // Enviar mensaje
                chat_history.push_back("You: " + current_message);
                current_message.clear(); // Limpia el mensaje
            } else if (ch == KEY_BACKSPACE || ch == 127) { // Borrar carácter
                if (!current_message.empty()) {
                    current_message.pop_back();
                }
            } else if (ch >= 32 && ch <= 126) { // Agregar carácter (caracteres imprimibles)
                current_message.push_back(ch);
            }
        }
        // Redibujar interfaz
        draw_interface(contacts_win, chat_win, status_win, contacts, chat_history, current_message, selected_contact, in_chat);
    }

    // Guardar historial del último contacto antes de salir
    save_chat_history(contacts[selected_contact] + ".txt", chat_history);

    // Finalizar ncurses
    delwin(contacts_win);
    delwin(chat_win);
    delwin(status_win);
    endwin();

    return 0;
}
