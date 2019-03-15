// Curses
#include <curses.h>
#include <stdlib.h>
#include <locale.h>

// Random
#include <ctime>
#include <cstdlib>

// Others
#include <vector>
#include <iostream>

#include "map.cpp"
#include "manipulate_me.cpp"
//#include "handle_display.cpp"


void init_curses() {
    // Start curses mode
    initscr();

    // CTRL-Z, CTRL-C が呼ばれた時にプログラムを強制終了させる。
    cbreak();

    // Don't echo() while we getch
    noecho();

    // The screen is cleared completely on the next call
    clear();
}

std::vector<std::vector<char> > init_arr(int maxlines, int maxcols)
{
    std::vector<std::vector<char> > arr;
    arr.resize(maxlines);
    for (int i = 0; i < maxlines; i++)
    {
        arr[i].resize(maxcols);
    }
    for (int i = 0; i < maxlines; i++)
    {
        for (int j = 0; j < maxcols; j++)
        {
            arr[i][j] = ' ';
        }
    }
    return arr;
}

void write(std::vector<std::vector<char> > arr) {
    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < arr[i].size(); j++) {
            mvaddch(i, j, arr[i][j]);
            refresh();
        }
    }
}
void determinate_curses(int maxlines)
{
    mvaddstr(maxlines, 0, "Press any key to quit");

    getch();

    endwin();

    exit(0);
}

void arr_debug(std::vector<std::vector<char> > arr) {
    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < arr[i].size(); j++) {
            std::cout << arr[i][j] << std::endl;
        }
        std::cout << std::endl;
    }

}

int main(void) {
    // Generate seed
    std::srand(std::time(nullptr));

    // initialize curses
    init_curses();

    // カーソルを非表示
    curs_set(0);

    // initialize triangle
    int maxlines = LINES - 1;
    int maxcols = COLS - 1;

    GenerateMap generate_map;

    std::vector<std::vector<char> > arr = init_arr(maxlines, maxcols);

    arr = generate_map.split_space(arr, maxlines, maxcols, 6, 0, maxlines - 1, 0, maxcols - 1);
    arr = generate_map.remove_unnecessary_root(arr, maxlines, maxcols);
    arr = generate_map.make_p_hash(arr, maxlines, maxcols);

    //write(arr);

    ManipulateMe manipulate_me(arr, 0, 0);
    manipulate_me.spawn_at_room(arr, maxlines, maxcols);

    //HandleDisplay handle_display;
    //handle_display.display_room(arr, manipulate_me.get_my_y(), manipulate_me.get_my_x(), maxlines, maxcols);
    manipulate_me.display_room(arr, manipulate_me.get_my_y(), manipulate_me.get_my_x(), maxlines, maxcols);
    //write(arr);

    // add '@'
    mvaddch(manipulate_me.get_my_y(), manipulate_me.get_my_x(), '@');
    refresh();

    while (true)
    {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        } else {
            if (arr[manipulate_me.get_my_y()][manipulate_me.get_my_x()] == '+') {
                //handle_display.display(arr, manipulate_me.get_my_y(), manipulate_me.get_my_x(), maxlines, maxcols);
                manipulate_me.display(arr, manipulate_me.get_my_y(), manipulate_me.get_my_x(), maxlines, maxcols);
            }
            manipulate_me.update_me(arr);
            manipulate_me.emulate(ch, maxlines, maxcols, arr);

            if (arr[manipulate_me.get_my_y()][manipulate_me.get_my_x()] == '#') {
                manipulate_me.serch_and_display(arr, manipulate_me.get_my_y(),  manipulate_me.get_my_x(), maxlines, maxcols);
            }
        }
    }
    determinate_curses(maxlines);
}
