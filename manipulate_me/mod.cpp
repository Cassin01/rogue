#include <curses.h>
#include <vector>
#include <functional>
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
        char under_foot;
        char symbol;

        static void erase_floor(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
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

        static void draw_char(int y, int x, char ch) {
            mvaddch(y, x, ch);
            refresh();
        }

        static void do_nothing(int, int , char) {};
        static void erase_floor_main(int y, int x, int new_y, int new_x, int maxlines, int maxcols, std::vector<std::vector<char> > arr) {
            if (arr[y][x] == '+' && arr[new_y][new_x] == '#') {
                erase_floor(arr, y, x, maxlines, maxcols);
            }
        }
        static void do_nothing_to_floor(int, int, int, int, int, int, std::vector<std::vector<char> >) {};

        static void erase_rooms_floor(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) {
            if (arr[at_y][at_x] == '.') {
                RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y,  at_x,  maxlines, maxcols);
                for (int i = room_size.y + 1; i < room_size.diagonal_y; i++) {
                    for (int j = room_size.x + 1; j < room_size.diagonal_x; j++) {
                        draw_char(i, j, ' ');
                    }
                }
            }
        }

        void display_rooms_wall(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols) const {
            if (arr[at_y][at_x] == '.') {
                RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y,  at_x,  maxlines, maxcols);
                for (int i = room_size.x; i < room_size.diagonal_x + 1; i++) {
                        draw_char(room_size.y, i, arr[room_size.y][i]);
                        draw_char(room_size.diagonal_y, i, arr[room_size.diagonal_y][i]);
                }
                for (int i = room_size.y; i < room_size.diagonal_y + 1; i++) {
                    draw_char(i, room_size.x, arr[i][room_size.x]);
                    draw_char(i, room_size.diagonal_x, arr[i][room_size.diagonal_x]);
                }
            }
        }

        static bool is_enemy(const std::vector<ManipulateMe> enemy_objects, const int y, const int x) {
            for (auto enemy_object: enemy_objects) {
                if (enemy_object.my_y == y && enemy_object.my_x == x) {
                    draw_char(enemy_object.my_y, enemy_object.my_x, enemy_object.symbol);
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
            my_y(y), my_x(x), symbol(symbol0), under_foot(under_foot0) {}

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

        static bool on_land(std::vector<std::vector<char> > arr, int y, int x) {
            if (arr[y][x] == '#' || arr[y][x] == '.' || arr[y][x] == '+') {
                return true;
            } else {
                return false;
            }
        }


        std::tuple<int, int> up_date(
            int my_y, int my_x, std::vector<std::vector<char> > arr,
            int new_y, int new_x, int maxlines, int maxcols,
            std::function<void(int, int, char)> fn_for_draw,
            std::function<void(int, int, int, int, int, int,
            std::vector<std::vector<char> >)> erase_f) const {
            if (new_y > 0 &&  new_y < maxlines && new_x > 0 && new_x < maxcols && on_land(arr, new_y, new_x)) {
                erase_f(my_y, my_x, new_y, new_x, maxlines, maxcols, arr);
                fn_for_draw(my_y, my_x, arr[my_y][my_x]);
                fn_for_draw(new_y, new_x, symbol);
                return {new_y, new_x};
            } else {
                fn_for_draw(my_y, my_x, symbol);
                return {my_y, my_x};
            }
        }

        static RoomSize compute_size_of_room_that_you_in(std::vector<std::vector<char> > arr, int y, int x, int maxlines, int maxcols) {
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

        static void display_room_main(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<ManipulateMe> enemy_objects) {
            RoomSize room_size = compute_size_of_room_that_you_in(arr, at_y, at_x, maxlines, maxcols);
            display_room(room_size, arr, maxlines, maxcols);
            display_enemys_in_room(room_size, enemy_objects);
        }

        static void display_room(RoomSize room_size, std::vector<std::vector<char> > arr, int maxlines, int maxcols) {
            for (int i = room_size.y; i < room_size.diagonal_y + 1; i++)
            {
                for (int j = room_size.x; j < room_size.diagonal_x + 1; j++)
                {
                    mvaddch(i, j, arr[i][j]);
                    refresh();
                }
            }
        }

        static void display_enemys_in_room(RoomSize room_size, std::vector<ManipulateMe> enemy_objects) {
            for (auto enemy_object: enemy_objects) {
                if (enemy_object.my_y >= room_size.y &&
                    enemy_object.my_y <= room_size.diagonal_y &&
                    enemy_object.my_x >= room_size.x &&
                    enemy_object.my_x <= room_size.diagonal_x ) {
                    mvaddch(enemy_object.my_y, enemy_object.my_x, enemy_object.symbol);
                    refresh();
                }
            }
        }

        void emulate(char ch, int maxlines, int maxcols, std::vector<std::vector<char> > arr, std::vector<ManipulateMe> npcs) {
            switch (ch) {
                case 'h': // 左
                    if (!duplicate_npcs_and_player(npcs, my_y, my_x - 1)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y, my_x - 1, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    }
                    break;
                case 'j': // 下
                    if (!duplicate_npcs_and_player(npcs, my_y + 1, my_x)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y + 1, my_x, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                case 'k': // 上
                    if (!duplicate_npcs_and_player(npcs, my_y - 1, my_x)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y - 1, my_x, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                case 'l': // 右
                    if (!duplicate_npcs_and_player(npcs, my_y, my_x + 1)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y, my_x + 1, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                case 'n': // 右下
                    if (!duplicate_npcs_and_player(npcs, my_y + 1, my_x + 1)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y + 1, my_x + 1, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                case 'b': // 左下
                    if (!duplicate_npcs_and_player(npcs, my_y + 1, my_x - 1)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y + 1, my_x - 1, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                case 'u': // 右上
                    if (!duplicate_npcs_and_player(npcs, my_y - 1, my_x + 1)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y - 1, my_x + 1, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                case 'y': // 左上
                    if (!duplicate_npcs_and_player(npcs, my_y - 1, my_x - 1)) {
                        std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y - 1, my_x - 1, maxlines, maxcols, ManipulateMe::draw_char, ManipulateMe::erase_floor_main);
                    break;
                    }
                default:
                    draw_char(my_y, my_x, symbol);
                    break;
            }
        }

        void display(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<ManipulateMe> enemy_objects) const {
            if (arr[at_y][at_x] == '+') {
                // 上
                if (at_y - 1 >= 0 && arr[at_y - 1][at_x] == '.') {
                    display_room_main(arr, at_y - 1, at_x, maxlines, maxcols, enemy_objects);
                }
                // 下
                else if (at_y + 1 < maxlines - 1 && arr[at_y + 1][at_x] == '.') {
                    display_room_main(arr, at_y + 1, at_x, maxlines, maxcols, enemy_objects);
                }
                // 右
                else if (at_x - 1 >= 0 && arr[at_y][at_x - 1] == '.') {
                    display_room_main(arr, at_y, at_x - 1, maxlines, maxcols, enemy_objects);
                }
                // 左
                else if (at_x + 1 < maxcols - 1 && arr[at_y][at_x + 1] == '.') {
                    display_room_main(arr, at_y, at_x + 1, maxlines, maxcols, enemy_objects);
                }
            }
        }

        static void serch_and_display(std::vector<std::vector<char> > arr, int at_y, int at_x, int maxlines, int maxcols, std::vector<ManipulateMe> enemy_objects)  {

            // 上
            if (at_y - 1 >= 0 && arr[at_y - 1][at_x] == '#') {
                draw_char(at_y - 1, at_x, arr[at_y - 1][at_x]);
                is_enemy(enemy_objects, at_y - 1, at_x);
            }
            // 下
            if (at_y + 1 < maxlines - 1 && arr[at_y + 1][at_x] == '#') {
                draw_char(at_y + 1, at_x, arr[at_y + 1][at_x]);
                is_enemy(enemy_objects, at_y + 1, at_x);
            }
            // 右
            if (at_x - 1 >= 0 && arr[at_y][at_x - 1] == '#') {
                draw_char(at_y, at_x - 1, arr[at_y][at_x - 1]);
                is_enemy(enemy_objects, at_y, at_x - 1);

            }
            // 左
            if (at_x + 1 < maxcols - 1 && arr[at_y][at_x + 1] == '#') {
                draw_char(at_y, at_x + 1, arr[at_y][at_x + 1]);
                is_enemy(enemy_objects, at_y, at_x + 1);
            }
        }

        static void serch_and_erase_npc(const std::vector<std::vector<char> > arr, const int at_y, const int at_x, const int maxlines, const int maxcols, const std::vector<ManipulateMe> enemy_objects) {
            // 上
            if (at_y - 1 >= 0 && arr[at_y - 1][at_x] == '#')
            {
                draw_char(at_y - 1, at_x, arr[at_y - 1][at_x]);
            }
            // 下
            if (at_y + 1 < maxlines - 1 && arr[at_y + 1][at_x] == '#')
            {
                draw_char(at_y + 1, at_x, arr[at_y + 1][at_x]);
            }
            // 右
            if (at_x - 1 >= 0 && arr[at_y][at_x - 1] == '#')
            {
                draw_char(at_y, at_x - 1, arr[at_y][at_x - 1]);
            }
            // 左
            if (at_x + 1 < maxcols - 1 && arr[at_y][at_x + 1] == '#')
            {
                draw_char(at_y, at_x + 1, arr[at_y][at_x + 1]);
            }
        }


        std::vector<ManipulateMe> operation_in_one_turn(std::vector<std::vector<char> > arr, int maxlines, int maxcols, char ch, std::vector<ManipulateMe> enemy_objects) {
            if (arr[my_y][my_x] == '+') {
                display(arr, my_y, my_x, maxlines, maxcols, enemy_objects);
            } else if (arr[my_y][my_x] == '.') {
                display_room_main(arr, my_y, my_x, maxlines, maxcols, enemy_objects);
            } else if (arr[my_y][my_x] == '#') {
                serch_and_erase_npc(arr, my_y,  my_x, maxlines, maxcols, enemy_objects);
            }

            // ユーザーからのコマンドを受け取り駒を進める
            emulate(ch, maxlines, maxcols, arr, enemy_objects);

            if (arr[my_y][my_x] == '#') {
                // プレイヤーの周囲一マスに進める場所があれば表示する
                serch_and_display(arr, my_y,  my_x, maxlines, maxcols, enemy_objects);
            }
            return enemy_objects;
        }

        void draw_me(void) const {
            mvaddch(my_y, my_x, symbol);
            refresh();
        }

        int npc_find_player(int opponent_y, int opponent_x)
        const {
            if (opponent_y < my_y) {
                if (opponent_x < my_x) {
                    // 左上
                    return 7;
                }
                if (opponent_x == my_x) {
                    // 上
                    return 2;
                } else {
                    // 右上
                    return 6;
                }
            } else if (opponent_y == my_y) {
                if (opponent_x < my_x) {
                    // 左
                    return 0;
                }
                if (opponent_x == my_x) {
                    // 真ん中
                    return 8;
                } else {
                    // 右
                    return 3;
                }
            } else {
                if (opponent_x < my_x) {
                    // 左下
                    return 5;
                }
                if (opponent_x == my_x) {
                    // 下
                    return 1;
                } else {
                    // 右下
                    return 4;
                }
            }
        }

        bool duplicate(int y0, int x0, int y1, int x1) const {
            if (y0 == y1 && x0 == x1) {
                return true;
            } else {
                return false;
            }
        }

        static bool duplicate_npcs_and_player(const std::vector<ManipulateMe> npcs, const int y, const int x) {
            for (auto npc: npcs) {
                if (npc.my_x == x && npc.my_y == y) {
                    return true;
                }
            }
            return false;
        }

        void operation_in_one_turn_for_enemy(std::vector<std::vector<char>> arr, int maxlines, int maxcols, int direction, int player_y, int player_x, std::vector<ManipulateMe> npcs)
    {
        switch (direction) {
            case 0: // 左
                if (!duplicate(my_y, my_x - 1, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y, my_x - 1)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y, my_x - 1, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 1: // 下
                if (!duplicate(my_y + 1, my_x, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y + 1, my_x)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y + 1, my_x, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 2: // 上
                if (!duplicate(my_y - 1, my_x, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y - 1, my_x)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y - 1, my_x, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 3: // 右
                if (!duplicate(my_y, my_x + 1, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y, my_x + 1)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y, my_x + 1, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 4: // 右下
                if (!duplicate(my_y + 1, my_x + 1, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y + 1, my_x + 1)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y + 1, my_x + 1, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 5: // 左下
                if (!duplicate(my_y + 1, my_x - 1, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y + 1, my_x - 1)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y + 1, my_x - 1, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 6: // 右上
                if (!duplicate(my_y - 1, my_x + 1, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y - 1, my_x + 1)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y - 1, my_x + 1, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            case 7: // 左上
                if (!duplicate(my_y - 1, my_x - 1, player_y, player_x) && !duplicate_npcs_and_player(npcs, my_y - 1, my_x - 1)) {
                    std::tie(my_y, my_x) = up_date(my_y, my_x, arr, my_y - 1, my_x - 1, maxlines, maxcols, ManipulateMe::do_nothing, ManipulateMe::do_nothing_to_floor);
                }
                break;
            default: // 動かない
                break;
        }
        return;
    }

    std::vector<ManipulateMe> operation_in_one_turn_for_enemys(std::vector<std::vector<char> > arr, int maxlines, int maxcols, std::vector<ManipulateMe> enemys) {
        for (auto &enemy: enemys) {
            //enemy.operation_in_one_turn_for_enemy(arr, maxlines, maxcols, std::rand() % 9);
            enemy.operation_in_one_turn_for_enemy(arr, maxlines, maxcols, enemy.npc_find_player(my_y, my_x), my_y, my_x, enemys);
        }
        return enemys;
    }
};
