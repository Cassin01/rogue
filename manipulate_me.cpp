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

        void emulate(char ch, int maxlines, int maxcols) {
            switch (ch) {
                case 'h': // 左
                    if (0 < my_x - 1)
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_x = my_x - 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                    }
                    break;
                case 'j': // 下
                    if (maxlines > my_y + 1)
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_y = my_y + 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                    }
                    break;
                case 'k': // 上
                    if (0 < my_y - 1)
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_y = my_y - 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                    }
                    break;
                case 'l': // 右
                    if (maxcols > my_x + 1)
                    {
                        mvaddch(my_y, my_x, ' ');
                        my_x = my_x + 1;
                        mvaddch(my_y, my_x, '@');
                        refresh();
                    }
                    break;
                default:
                    break;
            }
        }
};