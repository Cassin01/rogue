#include <vector>
#include <curses.h>
#include <functional>

class HandleDisplay {
    private:
        class RoomSize {
            public:
                int x;
                int y;
                int diagonal_x;
                int diagonal_y;
        };

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

        void serch(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<std::vector<bool> > visited) {
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
        void display_room(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            if (arr[at_y][at_x] == '.') {
                RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y,  at_x,  maxlines, maxcols);
                for (int i = room_size.y; i < room_size.diagonal_y + 1; i++) {
                    for (int j = room_size.x; j < room_size.diagonal_x + 1; j++) {
                            mvaddch(i, j, arr[i][j]);
                            refresh();
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

        void display(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            if (arr[at_y][at_x] == '+') {
                // 上
                if (at_y - 1 >= 0) {
                    if (arr[at_y - 1][at_x] == '.') {
                        display_room(arr, at_y - 1, at_x, maxlines, maxcols);
                    }
                }
                // 下
                if (at_y + 1 < maxlines - 1) {
                    if (arr[at_y + 1][at_x] == '.') {
                        display_room(arr, at_y + 1, at_x, maxlines, maxcols);
                    }
                }
                // 右
                if (at_x - 1 >= 0) {
                    if (arr[at_y][at_x - 1] == '.') {
                        display_room(arr, at_y, at_x - 1, maxlines, maxcols);
                    }
                }
                // 左
                if (at_x + 1 < maxcols - 1) {
                    if (arr[at_y][at_x + 1] == '.') {
                        display_room(arr, at_y, at_x + 1, maxlines, maxcols);
                    }
                }
            }
        }
};
