#include <iostream>


using namespace std;

class I {
  private:
    int j = 0;
  public:
  void unko(int i) const {
    j = i;
  }
    
};

int main(void) {
  I is;
  int i = 10;
  is.unko(i);
}

