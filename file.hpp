#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>

namespace File {

  static std::string str_read(std::string path, bool first_line=true) {
    std::ifstream file;

    file.open(path, std::ifstream::in);

    char current_char;
    std::string concat = "";

    while (file.get(current_char)) {
      if (first_line == true && current_char == '\n') {break;}
      concat += current_char;
    }

    file.close();

    return concat;
  }

}
#endif
