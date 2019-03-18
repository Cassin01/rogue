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
#include <string>

#include "map.cpp"
#include "manipulate_me/mod.cpp"
#include "game_ai/mod.cpp"

// 文章ファイル読み込み
#include "read_text.cpp"

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

int is_num(char ch) {
    switch (ch)
    {
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    default:
        return 10;
    }
}

void game_main_stream(int windows_maxlines, int windows_maxcols) {
    int text_lines = 10;
    int minimun_space_size = 6;

    ReadText read_text;
    int maxlines = windows_maxlines - text_lines;
    int maxcols  = windows_maxcols;
    mvaddstr(maxlines + 1, 0, read_text.read("./texts/cp01/02.txt").c_str());

    GenerateMap generate_map;

    std::vector<std::vector<char> > arr = init_arr(maxlines, maxcols);

    arr = generate_map.split_space(arr, maxlines, maxcols, minimun_space_size, 0, maxlines - 1, 0, maxcols - 1);
    arr = generate_map.remove_unnecessary_root(arr, maxlines, maxcols);
    arr = generate_map.make_p_hash(arr, maxlines, maxcols);

    ManipulateMe manipulate_me(0, 0, '@', arr[0][0]);
    manipulate_me.spawn_at_room(arr, maxlines, maxcols);


    // define position that enemys spawn
    GameAI game_ai;
    std::vector<std::tuple<int, int> > enemy_positions =
        game_ai.generate_positions(arr, maxlines, maxcols,
            std::make_tuple(manipulate_me.get_my_y(), manipulate_me.get_my_x()), 8);

    // for debug
    mvaddstr(0, 0, std::to_string(enemy_positions.size()).c_str());
    refresh();
    for (auto &i: enemy_positions) {
        mvaddch(std::get<0>(i), std::get<1>(i), 'B');
        refresh();
    }

    // Generate enemy objects
    std::vector<ManipulateMe> enemy_objects;
    for (auto &enemy_position: enemy_positions) {
        ManipulateMe enemy_object(std::get<0>(enemy_position), std::get<1>(enemy_position), 'A', arr[std::get<0>(enemy_position)][std::get<1>(enemy_position)]);
        enemy_objects.push_back(enemy_object);
    }

    // Start draw
    manipulate_me.display_room(arr, manipulate_me.get_my_y(), manipulate_me.get_my_x(), maxlines, maxcols, enemy_positions);

    // add '@'
    manipulate_me.draw_me();

    while (true)
    {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        } else if (0 < is_num(ch) && is_num(ch) < 10) {
            int ch_in = getch();
            for (int i = 0; i < is_num(ch); i++) {
                manipulate_me.operation_in_one_turn(arr, maxlines, maxcols, ch_in, enemy_objects);
            }
        } else {
            manipulate_me.operation_in_one_turn(arr, maxlines, maxcols, ch, enemy_objects);
        }
    }
}

void prologue(void) {
    ReadText read_text;
    printw(read_text.read("./texts/prologue/01-1.txt").c_str());
    int ch = getch();
    clear();
    printw(read_text.read("./texts/prologue/01-2.txt").c_str());
    ch = getch();
    clear();
    printw(read_text.read("./texts/prologue/01-3.txt").c_str());
    ch = getch();
    clear();
}


int main(void) {
    // Generate seed
    std::srand(std::time(nullptr));

    // 2バイト文字有効化
    setlocale(LC_ALL, "");

    // initialize curses
    init_curses();

    // カーソルを非表示
    curs_set(0);

    // initialize triangle
    int maxlines = LINES - 1;
    int maxcols = COLS - 1;

    prologue();
    game_main_stream(maxlines, maxcols);


    determinate_curses(maxlines);
}
