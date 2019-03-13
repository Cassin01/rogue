#include <vector>

// Rand
#include <stdlib.h>

class GenerateMap {
  public:
    std::vector<std::vector<char> > split_space(std::vector<std::vector<char> > arr, int maxlines, int maxcols, int minimum_room_size, int y_start, int y_end, int x_start, int x_end) {
      int r = std::rand() % 2;

      // 終了条件
      if (r == 0) {
        int split_point = y_start + std::rand() % (y_end - y_start);
        if (y_end - split_point <= minimum_room_size || split_point - y_start <= minimum_room_size) {
          arr = add_room(arr, y_start, y_end, x_start, x_end, maxlines, maxcols);
          return arr;
        } else {
          arr = fill_horizontal(arr, split_point, x_start, x_end);
          arr = split_space(arr, maxlines, maxcols, minimum_room_size, split_point + 1, y_end, x_start, x_end);
          arr = split_space(arr, maxlines, maxcols, minimum_room_size, y_start, split_point - 1, x_start, x_end);
          return arr;
        }
      } else {
        int split_point = x_start + std::rand() % (x_end - x_start);
        if (x_end - split_point <= minimum_room_size || split_point - x_start <= minimum_room_size) {
          arr = add_room(arr, y_start, y_end, x_start, x_end, maxlines, maxcols);
          return arr;
        } else {
          arr = fill_vertical(arr, split_point, y_start, y_end);
          arr = split_space(arr, maxlines, maxcols, minimum_room_size, y_start, y_end, split_point + 1, x_end);
          arr = split_space(arr, maxlines, maxcols, minimum_room_size, y_start, y_end, x_start, split_point - 1);
          return arr;
        }
      }
    }



    std::vector<std::vector<char> > make_p_hash(std::vector<std::vector<char> > arr, int maxlines, int maxcols) {
      for (int i = 0; i < maxlines - 1; i++) {
        for (int j = 0; j < maxcols - 1; j++) {
          if (arr[i][j] == 'p') {
            arr[i][j] = '#';
          }
        }
      }
      return arr;
    }



    std::vector<std::vector<char> > remove_unnecessary_root(std::vector<std::vector<char> > arr, int maxlines, int maxcols) {
        // 列
        for (int row_number = 0; row_number < maxcols; row_number++) {
          if (arr[0][row_number] == '#') {
            arr = remove_unnecessary_root_column(arr, maxlines, row_number);
          }
      }

      for (int row_number = 0; row_number < maxcols; row_number++) {
          if (arr[maxlines - 1][row_number] == '#') {
            arr = remove_unnecessary_root_column_from_down(arr, maxlines, row_number);
          }
        }

        // 行
        for (int column_number = 0; column_number < maxlines; column_number++) {
          if (arr[column_number][0] == '#') {
            arr = remove_unnecessary_root_row(arr, maxcols, column_number);
          }
        }

        for (int column_number = 0; column_number < maxlines; column_number++)
        {
          if (arr[column_number][maxcols - 1] == '#')
          {
            arr = remove_unnecessary_root_row_from_right(arr, maxcols, column_number);
          }
        }
        return arr;
    }
  private:
    // 横埋め
    std::vector<std::vector<char> > fill_horizontal(std::vector<std::vector<char> > arr, int y, int x_start, int x_end) {
      for (int i = x_start; i <= x_end; i++) {
          arr[y][i] = '#';
      }
      return arr;
    }

    // 縦埋め
    std::vector<std::vector<char> > fill_vertical(std::vector<std::vector<char> > arr, int x, int y_start, int y_end) {
      for (int i = y_start; i <= y_end; i++) {
          arr[i][x] = '#';
      }
      return arr;
    }

    std::vector<std::vector<char> > fill_till_borderline_horizontal(std::vector<std::vector<char> > arr, int draw_point, int x_start, bool up, int maxcols) {
      int i = x_start;

      if (up) {
        i++;
        while (i >= 0 && i < maxcols - 1) {
          if (arr[draw_point][i] == '#') {
            arr[draw_point][i] = 'p';
            return arr;
          } else if (arr[draw_point][i] == 'p') {
            return arr;
          } else {
            arr[draw_point][i] = 'p';
            i++;
          }
        }
        for (int j = i - 1; j > x_start; j--) {
          arr[draw_point][j] = ' ';
        }
        return arr;
      } else {
        i--;
        while (i >= 0 && i < maxcols - 1) {
          if (arr[draw_point][i] == '#') {
            arr[draw_point][i] = 'p';
            return arr;
          } else if (arr[draw_point][i] == 'p') {
            return arr;
          } else {
            arr[draw_point][i] = 'p';
            i--;
          }
        }
        for (int j = i + 1; j < x_start; j++) {
          arr[draw_point][j] = ' ';
        }
        return arr;
      }
    }

    std::vector<std::vector<char> > fill_till_borderline_vertical(std::vector<std::vector<char> > arr, int draw_point, int y_start, bool up, int maxlines) {
      int i = y_start;
      if (up) {
        i++;
        while (i >= 0 && i < maxlines - 1) {
          if (arr[i][draw_point] == '#') {
            arr[i][draw_point] = 'p';
            return arr;
          } else if (arr[i][draw_point] == 'p') {
            return arr;
          } else {
            arr[i][draw_point] = 'p';
            i++;
          }
        }
        for (int j = i - 1; j > y_start; j--) {
          arr[j][draw_point] = ' ';
        }
        return arr;
      } else {
        i--;
        while (i >= 0 && i < maxlines - 1) {
          if (arr[i][draw_point] == '#') {
            arr[i][draw_point] = 'p';
            return arr;
          } else if (arr[i][draw_point] == 'p') {
            return arr;
          } else {
            arr[i][draw_point] = 'p';
            i--;
          }
        }
        for (int j = i + 1; j < y_start; j++) {
          arr[j][draw_point] = ' ';
        }
        return arr;
      }
    }

    std::vector<std::vector<char> > add_pass_to_border_line(std::vector<std::vector<char> > arr, int y_draw_start, int y_draw_end, int x_draw_start, int x_draw_end, int maxlines, int maxcols) {
      int draw_point;
      // 上
      draw_point = x_draw_start + std::rand() % (x_draw_end - x_draw_start + 1);
      arr = fill_till_borderline_vertical(arr, draw_point, y_draw_start, false, maxlines);
      // 下
      draw_point = x_draw_start + std::rand() % (x_draw_end - x_draw_start + 1);
      arr = fill_till_borderline_vertical(arr, draw_point, y_draw_end, true, maxlines);
      // 左
      draw_point = y_draw_start + std::rand() % (y_draw_end - y_draw_start + 1);
      arr = fill_till_borderline_horizontal(arr, draw_point, x_draw_start, false, maxcols);
      // 右
      draw_point = y_draw_start + std::rand() % (y_draw_end - y_draw_start + 1);
      arr = fill_till_borderline_horizontal(arr, draw_point, x_draw_end, true, maxcols);
      return arr;
    }

    std::vector<std::vector<char> > add_room(std::vector<std::vector<char> > arr, int y_start, int y_end, int x_start, int x_end, int maxlines, int maxcols) {
      int minimun_room_width = 2;
      int y_draw_start = (y_start + 1) + std::rand() % ((y_end - 1) - (y_start + 1) - (minimun_room_width - 1));
      int x_draw_start = (x_start + 1) + std::rand() % ((x_end - 1) - (x_start + 1) - (minimun_room_width - 1));

      int y_draw_end = y_draw_start + std::rand() % ((y_end - 1) - y_draw_start) + (minimun_room_width - 1);
      int x_draw_end = x_draw_start + std::rand() % ((x_end - 1) - x_draw_start) + (minimun_room_width - 1);
      for (int i = y_draw_start; i <= y_draw_end; i++) {
        for (int j = x_draw_start; j <= x_draw_end; j++) {
          arr[i][j] = '.';
        }
      }
      return add_pass_to_border_line(arr, y_draw_start, y_draw_end, x_draw_start, x_draw_end, maxlines, maxcols);
    }

    std::vector<std::vector<char> > remove_unnecessary_root_column_from_down(std::vector<std::vector<char> > arr, int maxlines, int row_number) {
      bool erase_flag = true;
      for (int i = maxlines - 1; i >= 0; i--) {
        if (arr[i][row_number] == 'p') {
          erase_flag = false;
        } else if (arr[i][row_number] == '.') {
          return arr;
        } else if (arr[i][row_number] == ' '){
          for (int j = i + 2; j < maxlines; j++) {
            if (arr[j][row_number] == 'p') {
              return arr;
            }
            arr[j][row_number] = ' ';
          }
          return arr;
        } else {
          if (erase_flag) {
            arr[i][row_number] = ' ';
          }
        }
      }
      for (int i = 0; i < maxlines; i++) {
        if (arr[i][row_number] == 'p') {
          return arr;
        } else {
          arr[i][row_number] = ' ';
        }
      }
      return arr;
    }

    std::vector<std::vector<char> > remove_unnecessary_root_column(std::vector<std::vector<char> > arr, int maxlines, int row_number) {
      bool erase_flag = true;
      for (int i = 0; i < maxlines; i++) {
        if (arr[i][row_number] == 'p') {
          erase_flag = false;
        } else if (arr[i][row_number] == '.') {
          return arr;
        } else if (arr[i][row_number] == ' '){
          for (int j = i - 2; j >= 0; j--) {
            if (arr[j][row_number] == 'p') {
              return arr;
            }
            arr[j][row_number] = ' ';
          }
          return arr;
        } else {
          if (erase_flag) {
            arr[i][row_number] = ' ';
          }
        }
      }
      for (int i = maxlines - 1; i >= 0; i--) {
        if (arr[i][row_number] == 'p') {
          return arr;
        } else {
          arr[i][row_number] = ' ';
        }
      }
      return arr;
    }

    std::vector<std::vector<char> > remove_unnecessary_root_row(std::vector<std::vector<char> > arr, int maxcols, int column_number) {
      bool erase_flag = true;
      for (int i = 0; i < maxcols; i++) {
        if (arr[column_number][i] == 'p') {
          erase_flag = false;
        } else if (arr[column_number][i] == '.') {
          return arr;
        } else if (arr[column_number][i] == ' '){
          for (int j = i - 2; j >= 0; j--) {
            if (arr[column_number][j] == 'p') {
              return arr;
            }
            arr[column_number][j] = ' ';
          }
          return arr;
        } else {
          if (erase_flag) {
            arr[column_number][i] = ' ';
          }
        }
      }
      for (int i = maxcols - 1; i >= 0; i--) {
        if (arr[column_number][i] == 'p') {
          return arr;
        } else {
          arr[column_number][i] = ' ';
        }
      }
      return arr;
    }

    std::vector<std::vector<char> > remove_unnecessary_root_row_from_right(std::vector<std::vector<char> > arr, int maxcols, int column_number) {
      bool erase_flag = true;
      for (int i = maxcols - 1; i >= 0; i--) {
        if (arr[column_number][i] == 'p') {
          erase_flag = false;
        } else if (arr[column_number][i] == '.') {
          return arr;
        } else if (arr[column_number][i] == ' '){
          for (int j = i + 2; j < maxcols; j++) {
            if (arr[column_number][j] == 'p') {
              return arr;
            }
            arr[column_number][j] = ' ';
          }
          return arr;
        } else {
          if (erase_flag) {
            arr[column_number][i] = ' ';
          }
        }
      }
      for (int i = 0; i < maxcols; i++) {
        if (arr[column_number][i] == 'p') {
          return arr;
        } else {
          arr[column_number][i] = ' ';
        }
      }
      return arr;
    }
};