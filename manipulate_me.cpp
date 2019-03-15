#include <curses.h>
#include <vector>

class ManipulateMe
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

        bool emulate(char ch, int maxlines, int maxcols, std::vector<std::vector<char> > arr) {
            switch (ch) {
                case 'a': // 左
                    if (0 < my_x - 1 && on_land(arr, my_y, my_x - 1))
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_x = my_x - 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        return true;
                    } else {
                        return false;
                    }
                    break;
                case 's': // 下
                    if (maxlines > my_y + 1 && on_land(arr, my_y + 1, my_x))
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_y = my_y + 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        return true;
                    } else {
                        return false;
                    }
                    break;
                case 'w': // 上
                    if (0 < my_y - 1 && on_land(arr, my_y - 1, my_x))
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_y = my_y - 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        return true;
                    } else {
                        return false;
                    }
                    break;
                case 'd': // 右
                    if (maxcols > my_x + 1 && on_land(arr, my_y, my_x + 1))
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_x = my_x + 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                        return true;
                    } else {
                        return false;
                    }
                    break;
                default:
                    return false;
                    break;
            }
        }
};