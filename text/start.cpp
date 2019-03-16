#include <ncurses.h>
#include <locale.h>

#include "read_text.cpp"

int main() {
  // 2バイト文字有効化
  setlocale(LC_ALL,"");

  int ch;

  // Start curses mode
  initscr();

  // Line buffering disabled
  raw();

  // We get F1, F2 etc..
  keypad(stdscr, TRUE);

  // Don't echo() while we do getch
  noecho();

  cbreak();  // CTRL-Z, CTRL-C が呼ばれた時にプログラムを強制終了させる。
             // なおこれをraw(); に書き直した時は無視される

  ReadText read_text;


  int i = 0;
  while (true) {
    if (i == 0) {
      printw(read_text.read("02.txt").c_str());
    } else {
      clear();
      printw(read_text.read("01.txt").c_str());
    }
    ch = getch(); // If raw() hadn't been called
  // we have to press enter before it
  // gets to the program

  // Without keypad enabled this will
  // not get to us either
  // // Without noecho() some ugly escape
  // charachters might have been printed
  // on screen

    if(ch == KEY_F(1)) {
      printw("F1 Key pressed");
    } else if (ch == 'q') {
      break;
    } else if (ch == 10) {
      i++;
    }
  }


  // Print it on to the real screen
  refresh();

  // Wait for user input
  getch();

  // End curses mode;
  endwin();

  return 0;
}
