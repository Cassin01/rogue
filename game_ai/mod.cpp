// game_ai/mod.cpp

// Curses
#include <curses.h>

// Random
#include <cstdlib>

// Others
#include <vector>
#include <tuple>

class GameAI {
  private:
  public:
    std::vector<std::tuple<int, int>> generate_positions(std::vector<std::vector<char> > arr, int maxlines, int maxcols, std::tuple<int, int> my_position) {
      int generate_times = 8;
      std::vector<std::tuple<int, int>> positions;

      for (int i = 0; i < generate_times; i++) {
        positions = define_spawn_position(positions, arr, maxlines, maxcols, my_position);
      }
      return positions;
    }

    bool search_reserved(std::vector<std::tuple<int, int>> positions, std::tuple<int, int> reserving_position) {
      for (auto& position: positions) {
        if (std::get<0>(position) == std::get<0>(reserving_position)
        && std::get<1>(position) == std::get<1>(reserving_position)) {
          return false;
        }
      }
      return true;
    }

    std::vector<std::tuple<int, int> > define_spawn_position(std::vector<std::tuple<int, int> > positions, std::vector<std::vector<char> > arr, int maxlines, int maxcols, std::tuple<int, int> my_position) {
      int try_times = 20;
      for (int i = 0; i < try_times; i++) {
        int y = std::rand() % maxlines;
        int x = std::rand() % maxcols;
        if ((arr[y][x] == '.' || arr[y][x] == '#') &&
            (std::get<0>(my_position) != y && std::get<1>(my_position) != x) &&
            search_reserved(positions, my_position)) {
          std::tuple<int, int> position = std::make_tuple(y, x);
          positions.push_back(position);
          return positions;
        }
      }
      return positions;
    }


    void draw_enemy(std::vector<std::tuple<int, int>> positions) {
      int i = 0;
      for (auto &position: positions) {
        mvaddch(std::get<0>(position), std::get<1>(position), 'A');
        refresh();
        i++;
      }
    }

};