#include <fstream>
#include <iostream>
#include <string>
#include <iterator>

class ReadText {
  public:
  std::string read(std::string text_name)
  {
      std::ifstream ifs(text_name);
      if (ifs.fail())
      {
          std::cerr << "失敗" << std::endl;
          std::string err;
          return err;
      }

      // ほとんどさっきと同じですが、こうもかけます。こっちのほうがわかりやすい
      std::istreambuf_iterator<char> it(ifs);
      std::istreambuf_iterator<char> last;
      std::string str(it, last);
      return str;
  }
};
