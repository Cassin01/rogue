#include <vector>

class HandleDisplay {
    private:
        std::vector<std::vector<char> > displayed_data;
        std::vector<std::vector<char> > init_displayed_data(int maxlines, int maxcols) {
            displayed_data.resize(maxlines);
            for (int i = 0; i < maxlines; i++) {
                displayed_data[i].resize(maxcols);
            }
            for (int i = 0; i < maxlines; i++) {
                for (int j = 0; j < maxcols; j++) {
                    displayed_data[i][j] = ' ';
                }
            }
            return displayed_data;
        }

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
                if (arr[yi][x] != '.') {
                    break;
                }
            }
            for (int yi = y; yi < maxlines; yi++) {
                room_size.diagonal_y = yi;
                if (arr[yi][x] != '.') {
                    break;
                }
            }
            for (int xi = x; xi >= 0; xi--) {
                room_size.x = xi;
                if (arr[y][xi] != '.') {
                    break;
                }
            }
            for (int xi = x; xi < maxcols; xi++) {
                room_size.diagonal_x = xi;
                if (arr[y][xi] != '.') {
                    break;
                }
            }
            return room_size;
        }
    public:
        HandleDisplay(int maxlines, int maxcols) {
            displayed_data = init_displayed_data(maxlines, maxcols);
        }

        void add_a_room_to_displayed_data(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y,  at_x,  maxlines, maxcols);
            for (int i = room_size.y + 1; i < room_size.diagonal_y; i++) {
                for (int j = room_size.x + 1; j < room_size.diagonal_x; j++) {
                    displayed_data[i][j] = '.';
                }
            }
        }
        std::vector<std::vector<char> > get_displayed_data() {
            return displayed_data;
        }
};