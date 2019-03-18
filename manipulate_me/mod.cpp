#include <curses.h>
#include <vector>
#include "../handle_display/mod.cpp"

class ManipulateMe : public HandleDisplay
{
    private:
        int my_x;
        int my_y;
        int my_previous_x;
        int my_previous_y;
        char under_foot;
        char symbol;
    public:
        ManipulateMe(int x, int y, char symbol0, char under_foot0) {
            symbol = symbol0;
            my_x = x;
            my_y = y;
            under_foot = under_foot0;
            my_previous_x = my_x;
            my_previous_y = my_y;
        }

        int get_my_x() {
            return my_x;
        }

        int get_my_y() {
            return my_y;
        }

        void erase_me()
        {
            mvaddch(my_previous_y, my_previous_x, under_foot);
            refresh();
        }

        void update_me(std::vector<std::vector<char> > arr)  {
            under_foot = arr[my_y][my_x];
            my_previous_y = my_y;
            my_previous_x = my_x;
        }

        void spawn_at_room(std::vector<std::vector<char> > arr, int maxlines, int maxcols) {
            for (int i = 0; i < maxlines; i++) {
                for (int j = 0; j < maxcols; j++) {
                    if (arr[i][j] == '.') {
                        my_y = i;
                        my_x = j;
                        return;
                    }
                }
            }
        }

        bool on_land(std::vector<std::vector<char> > arr, int y, int x) {
            if (arr[y][x] == '#' || arr[y][x] == '.' || arr[y][x] == '+') {
                return true;
            } else {
                return false;
            }
        }

        void display_enemys_in_room(std::tuple<int, int, int, int> room_size, std::vector<ManipulateMe> enemy_objects) {
            for (auto &enemy_object: enemy_objects) {
                int enemy_y = enemy_object.my_y;
                int enemy_x = enemy_object.my_x;
                if (std::get<0>(room_size) < enemy_y && enemy_y < std::get<2>(room_size) &&
                    std::get<1>(room_size) < enemy_x && enemy_x < std::get<3>(room_size)) {
                        mvaddch(enemy_y, enemy_x, enemy_object.symbol);
                        refresh();
                }
            }
        }

        void emulate(char ch, int maxlines, int maxcols, std::vector<std::vector<char> > arr) {
            HandleDisplay handle_display;
            switch (ch) {
                case 'a': // 左
                    if (0 < my_x - 1 && on_land(arr, my_y, my_x - 1))
                    {
                        if (arr[my_y][my_x] == '+' && arr[my_y][my_x - 1] == '#') {
                            handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                        }
                        mvaddch(my_y, my_x, ' ');
                        my_x = my_x - 1;
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        return;
                    }
                    break;
                case 's': // 下
                    if (maxlines > my_y + 1 && on_land(arr, my_y + 1, my_x))
                    {
                        if (arr[my_y][my_x] == '+' && arr[my_y + 1][my_x] == '#') {
                            handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                        }
                        mvaddch(my_y, my_x, ' ');
                        my_y = my_y + 1;
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        return;
                    }
                    break;
                case 'w': // 上
                    if (0 < my_y - 1 && on_land(arr, my_y - 1, my_x))
                    {
                        if (arr[my_y][my_x] == '+' && arr[my_y - 1][my_x] == '#') {
                            handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                        }
                        mvaddch(my_y, my_x, ' ');
                        my_y = my_y - 1;
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        return;
                    }
                    break;
                case 'd': // 右
                    if (maxcols > my_x + 1 && on_land(arr, my_y, my_x + 1))
                    {
                        if (arr[my_y][my_x] == '+' && arr[my_y][my_x + 1] == '#') {
                            handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                        }
                        mvaddch(my_y, my_x, ' ');
                        my_x = my_x + 1;
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, symbol);
                        refresh();
                        return;
                    }
                    break;
                default:
                    mvaddch(my_y, my_x, symbol);
                    refresh();
                    return;
                    break;
            }
        }

        void display(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<ManipulateMe> enemy_objects) {
            std::vector<std::tuple<int, int>> enemy_positions;
            for (auto &enemy_object: enemy_objects) {
                enemy_positions.push_back(std::make_tuple(enemy_object.my_x, enemy_object.my_y));
            }

            if (arr[at_y][at_x] == '+') {
                // 上
                if (at_y - 1 >= 0 && arr[at_y - 1][at_x] == '.') {
                    display_room(arr, at_y - 1, at_x, maxlines, maxcols, enemy_positions);
                }
                // 下
                else if (at_y + 1 < maxlines - 1 && arr[at_y + 1][at_x] == '.') {
                    display_room(arr, at_y + 1, at_x, maxlines, maxcols, enemy_positions);
                }
                // 右
                else if (at_x - 1 >= 0 && arr[at_y][at_x - 1] == '.') {
                    display_room(arr, at_y, at_x - 1, maxlines, maxcols, enemy_positions);
                }
                // 左
                else if (at_x + 1 < maxcols - 1 && arr[at_y][at_x + 1] == '.') {
                    display_room(arr, at_y, at_x + 1, maxlines, maxcols, enemy_positions);
                }
            }
        }


    //std::vector<std::vector<char> > operation_in_one_turn(std::vector<std::vector<char> > arr, int maxlines, int maxcols, char ch) {
    std::vector<ManipulateMe> operation_in_one_turn(std::vector<std::vector<char> > arr, int maxlines, int maxcols, char ch, std::vector<ManipulateMe> enemy_objects) {
        if (arr[my_y][my_x] == '+') {
            display(arr, my_y, my_x, maxlines, maxcols, enemy_objects);
        }
        // 自分がいる場所を'いた場所'に記録する
        update_me(arr);

        // ユーザーからのコマンドを受け取り駒を進める
        emulate(ch, maxlines, maxcols, arr);

        if (arr[my_y][my_x] == '#') {
            // プレイヤーの周囲一マスに進める場所があれば表示する
            serch_and_display(arr, my_y,  my_x, maxlines, maxcols);
        }
        return enemy_objects;
    }

    void draw_me(void) {
        mvaddch(my_y, my_x, symbol);
        refresh();
    }
};
