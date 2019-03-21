#include <vector>
#include <curses.h>
#include <functional>

class HandleDisplay {
    private:
        static void serch(const std::vector<std::vector<char> > arr,const int at_y, const int at_x, const int maxlines, const int maxcols, std::vector<std::vector<bool> > visited) {
            if ((arr[at_y][at_x] == '#' || arr[at_y][at_x] == '+') && visited[at_y][at_x] == false) {
                visited[at_y][at_x] = true;
                mvaddch(at_y, at_x, arr[at_y][at_x]);
                refresh();

                // 上
                if (at_y - 1 >= 0) {
                    serch(arr, at_y - 1, at_x, maxlines, maxcols, visited);
                }
                // 下
                if (at_y + 1 < maxlines - 1) {
                    serch(arr, at_y + 1, at_x, maxlines, maxcols, visited);
                }
                // 右
                if (at_x - 1 >= 0) {
                    serch(arr, at_y, at_x - 1, maxlines, maxcols, visited);
                }
                // 左
                if (at_x + 1 < maxcols - 1) {
                    serch(arr, at_y, at_x + 1, maxlines, maxcols, visited);
                }
                return;
            } else {
                return;
            }
        }
    public:
        static void serch_and_display(const std::vector<std::vector<char> > arr, const int at_y, int at_x, const int maxlines, const int maxcols) {
            // 上
            if (at_y - 1 >= 0 && arr[at_y - 1][at_x] == '#') {
                mvaddch(at_y - 1, at_x, arr[at_y - 1][at_x]);
                refresh();
            }
            // 下
            if (at_y + 1 < maxlines - 1 && arr[at_y + 1][at_x] == '#') {
                mvaddch(at_y + 1, at_x, arr[at_y + 1][at_x]);
                refresh();
            }
            // 右
            if (at_x - 1 >= 0 && arr[at_y][at_x - 1] == '#') {
                mvaddch(at_y, at_x - 1, arr[at_y][at_x - 1]);
                refresh();
            }
            // 左
            if (at_x + 1 < maxcols - 1 && arr[at_y][at_x + 1] == '#') {
                mvaddch(at_y, at_x + 1, arr[at_y][at_x + 1]);
                refresh();
            }

        }
};
