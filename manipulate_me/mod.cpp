#include <curses.h>
#include <vector>
#include "../handle_display/mod.cpp"

class ManipulateMe : public HandleDisplay
{
    private:
        class RoomSize {
        public:
            int x;
            int y;
            int diagonal_x;
            int diagonal_y;
        };
        int my_x;
        int my_y;
        int my_previous_x;
        int my_previous_y;
        char under_foot;
        char symbol;
        void erase_floor(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            if (arr[at_y][at_x] == '+') {
                // 上
                if (at_y - 1 >= 0) {
                    if (arr[at_y - 1][at_x] == '.') {
                        erase_rooms_floor(arr, at_y - 1, at_x, maxlines, maxcols);
                    }
                }
                // 下
                if (at_y + 1 < maxlines - 1) {
                    if (arr[at_y + 1][at_x] == '.') {
                        erase_rooms_floor(arr, at_y + 1, at_x, maxlines, maxcols);
                    }
                }
                // 右
                if (at_x - 1 >= 0) {
                    if (arr[at_y][at_x - 1] == '.') {
                        erase_rooms_floor(arr, at_y, at_x - 1, maxlines, maxcols);
                    }
                }
                // 左
                if (at_x + 1 < maxcols - 1) {
                    if (arr[at_y][at_x + 1] == '.') {
                        erase_rooms_floor(arr, at_y, at_x + 1, maxlines, maxcols);
                    }
                }
            }

        }

        void erase_rooms_floor(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            if (arr[at_y][at_x] == '.') {
                RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y,  at_x,  maxlines, maxcols);
                for (int i = room_size.y + 1; i < room_size.diagonal_y; i++) {
                    for (int j = room_size.x + 1; j < room_size.diagonal_x; j++) {
                            mvaddch(i, j, ' ');
                            refresh();
                    }
                }
            }
        }

        void display_rooms_wall(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            if (arr[at_y][at_x] == '.') {
                RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y,  at_x,  maxlines, maxcols);
                for (int i = room_size.x; i < room_size.diagonal_x + 1; i++) {
                        mvaddch(room_size.y, i, arr[room_size.y][i]);
                        refresh();
                        mvaddch(room_size.diagonal_y, i, arr[room_size.diagonal_y][i]);
                        refresh();
                }
                for (int i = room_size.y; i < room_size.diagonal_y + 1; i++) {
                    mvaddch(i, room_size.x, arr[i][room_size.x]);
                    refresh();
                    mvaddch(i, room_size.diagonal_x, arr[i][room_size.diagonal_x]);
                    refresh();
                }
            }
        }

        //bool is_enemy_position(std::vector<std::tuple<int, int> > enemy_positions, int emulated_y, int emulated_x) {
        bool is_enemy_position(std::vector<ManipulateMe> enemy_objects, int emulated_y, int emulated_x) {
            for (auto &enemy_object: enemy_objects) {
                if (enemy_object.my_y == emulated_y && enemy_object.my_x == emulated_x) {
                    return true;
                }
            }
            return false;
        }

    public:
        // デフォルトコンストラクタ // Vectorにこのクラスのオブジェクトを入れる必要
        ManipulateMe() :
            my_y(0), my_x(0), symbol('@'), under_foot('.') {}

        // コンストラクタ
        ManipulateMe(int y, int x, char  symbol0, char under_foot0) :
            my_y(y), my_x(x), symbol(symbol0), under_foot(under_foot0),
            my_previous_y(y),  my_previous_x(x) {}

        // コピーコンストラクタ // Vectorにこのクラスのオブジェクトを入れるのに必要
        ManipulateMe(const ManipulateMe &manipulate_me) :
            my_y(manipulate_me.my_y), my_x(manipulate_me.my_x), symbol(manipulate_me.symbol),
            under_foot(manipulate_me.under_foot) {}

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
        RoomSize compute_size_of_room_that_you_in(std::vector<std::vector<char> > arr, int y, int x, int maxlines, int maxcols) {
            RoomSize room_size;
            for (int yi = y; yi >= 0; yi--) {
                room_size.y = yi;
                if (arr[yi][x] == '-' || arr[yi][x] == '+') {
                    break;
                }
            }
            for (int yi = y; yi < maxlines; yi++) {
                room_size.diagonal_y = yi;
                if (arr[yi][x] == '-' || arr[yi][x] == '+') {
                    break;
                }
            }
            for (int xi = x; xi >= 0; xi--) {
                room_size.x = xi;
                if (arr[y][xi] == '|' || arr[y][xi] == '+') {
                    break;
                }
            }
            for (int xi = x; xi < maxcols; xi++) {
                room_size.diagonal_x = xi;
                if (arr[y][xi] == '|' || arr[y][xi] == '+') {
                    break;
                }
            }
            return room_size;
        }


//        void display_room(std::vector<std::vector<char>> arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<std::tuple<int, int>> enemy_positions)
        void display_room(std::vector<std::vector<char>> arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<ManipulateMe> enemy_objects)
        {
            RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y, at_x, maxlines, maxcols);
            for (int i = room_size.y; i < room_size.diagonal_y + 1; i++)
            {
                for (int j = room_size.x; j < room_size.diagonal_x + 1; j++)
                {
                    if (is_enemy_position(enemy_objects, i, j))
                    {
                        mvaddch(i, j, 'A');
                        refresh();
                    }
                    else
                    {
                        mvaddch(i, j, arr[i][j]);
                        refresh();
                    }
                }
            }
        }

        void emulate(char ch, int maxlines, int maxcols, std::vector<std::vector<char> > arr) {
            //HandleDisplay handle_display;
            switch (ch) {
                case 'a': // 左
                    if (0 < my_x - 1 && on_land(arr, my_y, my_x - 1))
                    {
                        if (arr[my_y][my_x] == '+' && arr[my_y][my_x - 1] == '#') {
                            //handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                            erase_floor(arr, my_y, my_x, maxlines, maxcols);
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
                            //handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                            erase_floor(arr, my_y, my_x, maxlines, maxcols);
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
                            //handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                            erase_floor(arr, my_y, my_x, maxlines, maxcols);
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
                            //handle_display.erase_floor(arr, my_y, my_x, maxlines, maxcols);
                            erase_floor(arr, my_y, my_x, maxlines, maxcols);
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
            if (arr[at_y][at_x] == '+') {
                // 上
                if (at_y - 1 >= 0 && arr[at_y - 1][at_x] == '.') {
                    display_room(arr, at_y - 1, at_x, maxlines, maxcols, enemy_objects);
                }
                // 下
                else if (at_y + 1 < maxlines - 1 && arr[at_y + 1][at_x] == '.') {
                    display_room(arr, at_y + 1, at_x, maxlines, maxcols, enemy_objects);
                }
                // 右
                else if (at_x - 1 >= 0 && arr[at_y][at_x - 1] == '.') {
                    display_room(arr, at_y, at_x - 1, maxlines, maxcols, enemy_objects);
                }
                // 左
                else if (at_x + 1 < maxcols - 1 && arr[at_y][at_x + 1] == '.') {
                    display_room(arr, at_y, at_x + 1, maxlines, maxcols, enemy_objects);
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
