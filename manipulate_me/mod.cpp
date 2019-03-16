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
    public:
        ManipulateMe(std::vector<std::vector<char> > arr, int x, int y) {
            my_x = x;
            my_y = y;
            under_foot = arr[my_y][my_x];
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
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, '@');
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
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, '@');
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
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, '@');
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
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        erase_me();
                        return;
                    } else {
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        return;
                    }
                    break;
                default:
                    mvaddch(my_y, my_x, '@');
                    refresh();
                    return;
                    break;
            }
        }

    std::vector<std::vector<char> > operation_in_one_turn(std::vector<std::vector<char> > arr, int maxlines, int maxcols, char ch) {
        if (arr[my_y][my_x] == '+') {
            display(arr, my_y, my_x, maxlines, maxcols);
        }
        update_me(arr);
        emulate(ch, maxlines, maxcols, arr);

        if (arr[my_y][my_x] == '#') {
            serch_and_display(arr, my_y,  my_x, maxlines, maxcols);
        }

        return arr;
    }

    void draw_me(void) {
        mvaddch(my_y, my_x, '@');
        refresh();
    }
};
